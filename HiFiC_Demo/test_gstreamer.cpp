#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <poll.h>
#include <time.h>

// Libraries for auto-scaling
#include <array>
#include <memory>
#include <stdexcept>
#include <string>

// Your OpenCV library
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

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

int main(void) {
    int reenable = 1;
    unsigned int last_value = 0; 

    int screen_w, screen_h;
    GetMonitorResolution(screen_w, screen_h);

    int panel_w = screen_w / 2;
    int display_size = std::min(panel_w, screen_h);  // Crop 1:1
    int ypos = (screen_h - display_size) / 2; 

    int left_x = 0;
    int right_x = panel_w;
    // -------------------------------------------------------
    
    // 1. Initialize UIO device
    int fd = open("/dev/uio0", O_RDWR);
    if (fd < 0) {
        perror("Error: Cannot open /dev/uio0");
        return -1;
    }

    // Map GPIO registers to user space
    uint8_t *gpio_ptr = (uint8_t *)mmap(NULL, GPIO_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    // Enable global interrupt
    *((unsigned *)(gpio_ptr + GPIO_GLOBAL_IRQ)) = 0x80000000;

    // Enable channel interrupt
    *((unsigned *)(gpio_ptr + GPIO_IRQ_CONTROL)) = 1;

    // Enable interrupt from UIO side
    write(fd, &reenable, sizeof(int));

    std::cout << "System ready! Press Button 0 (Bit 0) to capture and test AI pipeline." << std::endl;

    struct pollfd fds = {
        .fd = fd,
        .events = POLLIN,
    };

    // Your GStreamer pipeline
    std::string gst_in = "v4l2src device=/dev/video0 num-buffers=1 ! videoconvert ! appsink";

    // 2. Interrupt waiting loop
    while (1) {
        if (poll(&fds, 1, -1) >= 1) {

            // Read interrupt event
            read(fd, &reenable, sizeof(int));

            usleep(50000); 

            // Read GPIO button state (lower 4 bits)
            unsigned int current_value = *((unsigned *)(gpio_ptr + GPIO_DATA_OFFSET)) & 0x0F;
            
            // If any button is pressed
            if (current_value != 0 && current_value != last_value) {
                
                // Button 0 (0x01) triggers image capture
                if (current_value == 0x01) {
                    std::cout << "\n=============================================" << std::endl;
                    std::cout << "=> Button 0x01 pressed. Capturing image..." << std::endl;

                    // Open camera via GStreamer
                    cv::VideoCapture cap(gst_in, cv::CAP_GSTREAMER);

                    if (!cap.isOpened()) {
                        std::cerr << "=> ERROR: Cannot open camera!" << std::endl;
                    } else {
                        cv::Mat raw_frame;
                        cap >> raw_frame; // Capture one frame

                        if (raw_frame.empty()) {
                            std::cerr << "=> ERROR: Failed to capture frame!" << std::endl;
                        } else {
                            // Crop to 1:1 square from center
                            int square_size = std::min(raw_frame.cols, raw_frame.rows);
                            int start_x = (raw_frame.cols - square_size) / 2;
                            int start_y = (raw_frame.rows - square_size) / 2;

                            cv::Rect roi(start_x, start_y, square_size, square_size);
                            cv::Mat square_frame = raw_frame(roi).clone();

                            // Resize to 256x256
                            cv::Mat resized_frame;
                            cv::resize(square_frame, resized_frame, cv::Size(256, 256));

                            // Generate filenames with timestamp
                            long current_time = time(NULL);
                            char in_filename[64];
                            char out_filename[64];
                            snprintf(in_filename, sizeof(in_filename), "input_256x256_%ld.png", current_time);
                            snprintf(out_filename, sizeof(out_filename), "output_256x256_%ld.png", current_time);

                            // Save INPUT
                            cv::imwrite(in_filename, resized_frame);
                            // Save OUTPUT (Mocking AI result by saving the exact same image)
                            cv::imwrite(out_filename, resized_frame); 
                            std::cout << "=> SUCCESS! Images saved to disk." << std::endl;

                            // -------------------------------------------------------------
                            // Stage 1: Display INPUT on the left side of the screen using GStreamer
                            // -------------------------------------------------------------
                            std::cout << "=> Displaying INPUT on the left side..." << std::endl;
                            
                            // Clear any existing GStreamer pipelines
                            system("killall -9 gst-launch-1.0 2>/dev/null");

                            char cmd_left_only[1024];
                            // CẬP NHẬT GIAI ĐOẠN 1: Tham số hóa lệnh
                            snprintf(cmd_left_only, sizeof(cmd_left_only),
                                "gst-launch-1.0 compositor name=comp sink_0::xpos=%d sink_0::ypos=%d ! "
                                "videoconvert ! \"video/x-raw, width=%d, height=%d, format=NV12\" ! "
                                "kmssink bus-id=\"fd4a0000.display\" sync=false fullscreen-overlay=true "
                                "filesrc location=%s ! pngdec ! imagefreeze ! videoconvert ! videoscale ! "
                                "\"video/x-raw, width=%d, height=%d\" ! comp.sink_0 > /dev/null 2>&1 &",
                                left_x, ypos, 
                                screen_w, screen_h, 
                                in_filename, 
                                display_size, display_size);
                            system(cmd_left_only);


                            // -------------------------------------------------------------
                            // Stage 2: Simulate running the AI model (replace with actual inference call)
                            // -------------------------------------------------------------
                            std::cout << "=> Running AI Model (simulating for 5 minutes)..." << std::endl;
                            // Here you would call your actual AI inference function instead of sleep
                            sleep(300); // Sleep for 5 minutes


                            // -------------------------------------------------------------
                            // Stage 3: Display both INPUT (left) and OUTPUT (right)
                            // -------------------------------------------------------------
                            std::cout << "=> AI Done! Displaying OUTPUT on the right side..." << std::endl;
                            
                            // Clear any existing GStreamer pipelines
                            system("killall -9 gst-launch-1.0 2>/dev/null");

                            char cmd_both[2048];
                            // CẬP NHẬT GIAI ĐOẠN 3: Tham số hóa lệnh
                            snprintf(cmd_both, sizeof(cmd_both),
                                "gst-launch-1.0 compositor name=comp sink_0::xpos=%d sink_0::ypos=%d sink_1::xpos=%d sink_1::ypos=%d ! "
                                "videoconvert ! \"video/x-raw, width=%d, height=%d, format=NV12\" ! "
                                "kmssink bus-id=\"fd4a0000.display\" sync=false fullscreen-overlay=true "
                                "filesrc location=%s ! pngdec ! imagefreeze ! videoconvert ! videoscale ! \"video/x-raw, width=%d, height=%d\" ! comp.sink_0 "
                                "filesrc location=%s ! pngdec ! imagefreeze ! videoconvert ! videoscale ! \"video/x-raw, width=%d, height=%d\" ! comp.sink_1 > /dev/null 2>&1 &",
                                left_x, ypos, right_x, ypos,
                                screen_w, screen_h,
                                in_filename, display_size, display_size,
                                out_filename, display_size, display_size);
                            system(cmd_both);
                            
                            std::cout << "=============================================\n" << std::endl;
                        }

                        // Release camera for next button press
                        cap.release();
                    }

                } else {
                    // Other buttons pressed
                    std::cout << "\n=> Other button pressed: 0x"
                              << std::hex << current_value << std::dec << std::endl;
                }
            }

            current_value = *((unsigned *)(gpio_ptr + GPIO_DATA_OFFSET)) & 0x0F;
            last_value = current_value;

            // Clear interrupt status and re-enable UIO interrupt
            *((unsigned *)(gpio_ptr + GPIO_IRQ_STATUS)) = 1;
            write(fd, &reenable, sizeof(int));
        }
    }

    munmap(gpio_ptr, GPIO_MAP_SIZE);
    close(fd);
    return 0;
}