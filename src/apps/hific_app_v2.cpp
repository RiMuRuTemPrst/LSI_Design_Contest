#include <fcntl.h>
#include <gst/gst.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <chrono>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "Hific_full.cpp"
#include "../core/Core.h"

#define GPIO_DATA_OFFSET 0x00
#define GPIO_GLOBAL_IRQ 0x11C
#define GPIO_IRQ_CONTROL 0x128
#define GPIO_IRQ_STATUS 0x120
#define GPIO_MAP_SIZE 0x10000

static std::string exec(const char* cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

#include <fstream>
#include <iostream>
#include <string>

static void GetMonitorResolution(int& w, int& h)
{
    w = 1366;
    h = 768;

    std::ifstream file("/sys/class/drm/card0-DP-1/modes");
    std::string res;

    if (file && std::getline(file, res)) {

        size_t pos = res.find('x');

        if (pos != std::string::npos) {

            w = std::stoi(res.substr(0, pos));
            h = std::stoi(res.substr(pos + 1));

            std::cout << "[INFO] DP Resolution: "
                      << w << "x" << h << std::endl;

            return;
        }
    }

    std::cout << "[WARN] Cannot detect monitor resolution. Using default: "
              << w << "x" << h << std::endl;
}

int main(int argc, char *argv[]) {
    // Initialize GStreamer
    gst_init(&argc, &argv);
    // Get monitor resolution
    GstElement *display_pipeline = NULL;

    std::cout << "==================================================" << std::endl;
    std::cout << "                      HiFiC App                   " << std::endl;
    std::cout << "==================================================" << std::endl;

    // -------- DISPLAY CONFIGURATION --------
    int screen_w, screen_h;
    GetMonitorResolution(screen_w, screen_h);

    int panel_w = screen_w / 2;
    int display_size = std::min(panel_w, screen_h);  // Crop 1:1
    int ypos = (screen_h - display_size) / 2;        // Center vertically

    int left_x = 0;
    int right_x = panel_w;
    // ---------------------------------------

    int crop_size = 256;
    Shape io_shape(1, crop_size, crop_size, 3);
    int total_elements = crop_size * crop_size * 3;
    
    // Allocate memory buffers for model input and output 
    std::vector<_Float16> input_vec(total_elements);
    std::vector<_Float16> output_vec(total_elements);
    TensorMem<_Float16> model_input(input_vec.data(), io_shape, false);
    TensorMem<_Float16> model_output(output_vec.data(), io_shape, false);

    // 1. Initialize UIO device
    int fd = open("/dev/uio0", O_RDWR);
    if (fd < 0) {
        perror("[ERROR] Cannot open /dev/uio0. Check permissions or hardware.");
        return -1;
    }

    uint8_t *gpio_ptr = (uint8_t *)mmap(NULL, GPIO_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    // Enable interrupts
    *((unsigned *)(gpio_ptr + GPIO_GLOBAL_IRQ)) = 0x80000000;
    *((unsigned *)(gpio_ptr + GPIO_IRQ_CONTROL)) = 1;
    
    int reenable = 1;
    write(fd, &reenable, sizeof(int));
    
    std::cout << "=> [INIT] Opening Camera via GStreamer..." << std::endl;
    std::string gst_in = "v4l2src device=/dev/video0 ! videoconvert ! appsink drop=true max-buffers=1";
    cv::VideoCapture cap(gst_in, cv::CAP_GSTREAMER);
    if (!cap.isOpened()) {
        std::cerr << "[ERROR] Cannot open camera! Exit." << std::endl;
        return -1;
    }

    std::cout << "[SYSTEM] Ready! Press Button 0 to capture image and run HiFiC model." << std::endl;

    struct pollfd fds = {
        .fd = fd,
        .events = POLLIN,
    };

    unsigned int last_value = 0; 

    while (1) {
        // Wait for hardware interrupt
        if (poll(&fds, 1, -1) >= 1) {
            read(fd, &reenable, sizeof(int));

            // Simple debounce delay (50 ms)
            usleep(50000);

            unsigned int current_value = *((unsigned *)(gpio_ptr + GPIO_DATA_OFFSET)) & 0x0F;
            
            if (current_value != 0 && current_value != last_value) {
                if (current_value == 0x01) {
                    std::cout << "\n--------------------------------------------------" << std::endl;
                    std::cout << "=> [TRIGGER] Button 0 pressed. Starting pipeline..." << std::endl;
                    auto pipeline_start = std::chrono::high_resolution_clock::now();
                    long timestamp = time(NULL);

                    // 1. CAPTURE IMAGE
                    std::cout << "=> [STEP 1] Capturing frame..." << std::endl;
                    cv::Mat raw_frame;
                    cap.grab(); 
                    cap.retrieve(raw_frame);
                    if (raw_frame.empty()) {
                        std::cerr << "[ERROR] Failed to capture frame!" << std::endl;
                    } else {
                        std::cout << "=> [STEP 2] Cropping & Resizing to 256x256..." << std::endl;
                        int square_size = std::min(raw_frame.cols, raw_frame.rows);
                        int start_x = (raw_frame.cols - square_size) / 2;
                        int start_y = (raw_frame.rows - square_size) / 2;

                        cv::Rect roi(start_x, start_y, square_size, square_size);
                        cv::Mat square_frame = raw_frame(roi).clone();

                        cv::Mat resized_frame;
                        cv::resize(square_frame, resized_frame, cv::Size(crop_size, crop_size));

                        char in_filename[128];
                        snprintf(in_filename, sizeof(in_filename), "images/input_%ld.png", timestamp);
                        cv::imwrite(in_filename, resized_frame);
                        std::cout << "   -> Saved input image as: " << in_filename << std::endl;

                        // -------------------------------------------------------------
                        // Display input image on the left side of the screen
                        std::cout << "=> [DISPLAY] Displaying INPUT image on the left side..." << std::endl;
                        if (display_pipeline != NULL) {
                            gst_element_set_state(display_pipeline, GST_STATE_NULL);
                            gst_object_unref(display_pipeline);
                            display_pipeline = NULL;
                        }// Clear any existing GStreamer pipelines
                        char cmd_left[2048];
                        snprintf(cmd_left, sizeof(cmd_left),
                            "compositor name=comp "
                            "sink_0::xpos=%d sink_0::ypos=%d ! "
                            "videoconvert ! \"video/x-raw, width=%d, height=%d, format=NV12\" ! "
                            "kmssink bus-id=\"fd4a0000.display\" sync=false fullscreen-overlay=true "
                            "filesrc location=%s ! pngdec ! imagefreeze ! videoconvert ! videoscale ! "
                            "\"video/x-raw, width=%d, height=%d\" ! comp.sink_0",
                            left_x, ypos,
                            screen_w, screen_h,
                            in_filename,
                            display_size, display_size);
                        display_pipeline = gst_parse_launch(cmd_left, NULL);
                        if (display_pipeline) {
                            gst_element_set_state(display_pipeline, GST_STATE_PLAYING);
                        } else {
                            std::cerr << "=> [ERROR] Failed to launch GStreamer pipeline!" << std::endl;
                        }
                        // -------------------------------------------------------------

                        // 2. PREPROCESSING
                        std::cout << "=> [STEP 3] Preprocessing (BGR->RGB->Normalize)..." << std::endl;
                        cv::Mat rgb_frame;
                        cv::cvtColor(resized_frame, rgb_frame, cv::COLOR_BGR2RGB);
                        rgb_frame.convertTo(rgb_frame, CV_32FC3, 1.0 / 255.0);

                        float* f_ptr = (float*)rgb_frame.data;
                        for (int i = 0; i < total_elements; ++i) {
                            input_vec[i] = (_Float16)f_ptr[i];
                        }

                        // 3. INFERENCE
                        std::cout << "=> [STEP 4] Running HiFiC Model (Please wait)..." << std::endl;
                        auto inf_start = std::chrono::high_resolution_clock::now();
                        Hific(model_input, model_output);
                        auto inf_end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double> inf_elapsed = inf_end - inf_start;
                        std::cout << "   -> Inference finished in: " << inf_elapsed.count() << " seconds." << std::endl;

                        // 4. POSTPROCESSING
                        std::cout << "=> [STEP 5] Postprocessing (Float->Uint8->RGB->BGR)..." << std::endl;
                        cv::Mat output_img(crop_size, crop_size, CV_32FC3);
                        float* out_ptr = (float*)output_img.data;
                        for (int i = 0; i < total_elements; ++i) {
                            out_ptr[i] = (float)output_vec[i];
                        }
                        output_img.convertTo(output_img, CV_8UC3, 255.0);

                        cv::Mat final_bgr;
                        cv::cvtColor(output_img, final_bgr, cv::COLOR_RGB2BGR);
                        
                        char out_filename[128];
                        snprintf(out_filename, sizeof(out_filename), "images/output_%ld.png", timestamp);
                        cv::imwrite(out_filename, final_bgr);
                        std::cout << "   -> Saved output image as: " << out_filename << std::endl;

                        // Display output on the right side
                        std::cout << "=> [DISPLAY] Displaying both INPUT and OUTPUT on the screen..." << std::endl;
                        if (display_pipeline != NULL) {
                            gst_element_set_state(display_pipeline, GST_STATE_NULL);
                            gst_object_unref(display_pipeline);
                            display_pipeline = NULL;
                        } // Clear any existing GStreamer pipelines
                        char cmd_both[2048];
                        snprintf(cmd_both, sizeof(cmd_both),
                            "compositor name=comp "
                            "sink_0::xpos=%d sink_0::ypos=%d "
                            "sink_1::xpos=%d sink_1::ypos=%d ! "
                            "videoconvert ! \"video/x-raw, width=%d, height=%d, format=NV12\" ! "
                            "kmssink bus-id=\"fd4a0000.display\" sync=false fullscreen-overlay=true "
                            "filesrc location=%s ! pngdec ! imagefreeze ! videoconvert ! videoscale ! "
                            "\"video/x-raw, width=%d, height=%d\" ! comp.sink_0 "
                            "filesrc location=%s ! pngdec ! imagefreeze ! videoconvert ! videoscale ! "
                            "\"video/x-raw, width=%d, height=%d\" ! comp.sink_1",
                            left_x, ypos,
                            right_x, ypos,
                            screen_w, screen_h,
                            in_filename,
                            display_size, display_size,
                            out_filename,
                            display_size, display_size);
                        display_pipeline = gst_parse_launch(cmd_both, NULL);
                        if (display_pipeline) {
                            gst_element_set_state(display_pipeline, GST_STATE_PLAYING);
                        } else {
                            std::cerr << "=> [ERROR] Failed to launch GStreamer pipeline!" << std::endl;
                        }
                        // -------------------------------------------------------------

                        auto pipeline_end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double> total_elapsed = pipeline_end - pipeline_start;
                        std::cout << "=> [DONE] Total time: " << total_elapsed.count() << " seconds." << std::endl;
                    }
                }
            }

            last_value = current_value;

            // Clear interrupt status and re-enable UIO interrupt
            *((unsigned *)(gpio_ptr + GPIO_IRQ_STATUS)) = 1;
            write(fd, &reenable, sizeof(int));
        }
    }

    cap.release();
    munmap(gpio_ptr, GPIO_MAP_SIZE);
    close(fd);
    return 0;
}