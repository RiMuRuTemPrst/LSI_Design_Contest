# HiFiC GAN Accelerator on ZCU104 - Live Demo Guide

This repository contains the hardware-accelerated HiFiC (High-Fidelity Image Compression) application running on the Xilinx Zynq UltraScale+ ZCU104. The application uses a hardware-software co-design approach to perform real-time image capture, AI inference (compression & reconstruction), and DisplayPort visualization.

---

## 1. Hardware Setup Checklist

Before powering on the board, ensure the following connections are firmly established:
- [ ] **Camera**: Connect a USB 3.0 Web Camera to the USB port on the ZCU104.
- [ ] **Display**: Connect a monitor to the **DisplayPort (DP)** connector on the board.
- [ ] **SD Card**: Insert the SD card containing the custom PetaLinux boot images (`BOOT.BIN`, `image.ub`, `boot.scr`, and rootfs).
- [ ] **UART/Terminal**: Connect the Micro-USB cable for serial terminal access (Baudrate: 115200).

---

## 2. Quick Start: Compiling the App

Boot up the ZCU104 board and log in to the PetaLinux terminal.

### Step 2.1: Connect via Picocom (From Host PC)

Open a terminal on your Host PC and connect to the board's serial console using `picocom`. (The device is usually `/dev/ttyUSB1`).
```bash
picocom -b 115200 /dev/ttyUSB1
```
### Step 2.2: Compile the Application (On ZCU104) (Optional)

```bash
# Navigate to the directory containing the project source code on the board:
cd HiFiC/
# Compile the application with OpenCV and GStreamer libraries
g++ hific_app.cpp -o hific_app -O3 -mcpu=cortex-a53 - `pkg-config --cflags --libs opencv4 gstreamer-1.0`
```

## 3. Running the Interactive Demo

### Step 3.1: Launch the application

```bash
./hific_app
```

*Expected console output:*

> `[INFO] DP Monitor detected: 1366x768`
> `=> [INIT] Opening Camera via GStreamer...`
> `[SYSTEM] Ready! Press Button 0 to capture image and run HiFiC model.`

### Step 3.3: Trigger the Pipeline

Locate **Push Button SW14** (Button 0) on the ZCU104 board. **Press it once.**

1. **Capture**: The camera takes a snapshot.
2. **First Display (Left Screen)**: The DisplayPort monitor instantly shows the **Original Input Image** (cropped to 256x256) on the left half.
3. **AI Inference**: The FPGA accelerator computes the ResBlocks.
4. **Final Display (Full Screen)**: The monitor refreshes to display the **Original Input (Left)** side-by-side with the **Reconstructed Output (Right)**.

*To exit the C++ application, press `Ctrl + C` in the terminal.*

---

## 4. Manual Display via GStreamer (For Debug/Showcase)

If you need to manually display specific generated images on the DisplayPort monitor, use the following terminal commands.


**Show a SINGLE image on the left side:**

```bash
gst-launch-1.0 compositor name=comp sink_0::xpos=0 sink_0::ypos=42 ! \
videoconvert ! "video/x-raw, width=1366, height=768, format=NV12" ! \
kmssink bus-id="fd4a0000.display" sync=false fullscreen-overlay=true \
filesrc location=images/input_123456789.png ! pngdec ! imagefreeze ! videoconvert ! videoscale ! \
"video/x-raw, width=683, height=683" ! comp.sink_0

```

**Show TWO images SIDE-BY-SIDE (Input vs Output):**

```bash
gst-launch-1.0 compositor name=comp sink_0::xpos=0 sink_0::ypos=42 sink_1::xpos=683 sink_1::ypos=42 ! \
videoconvert ! "video/x-raw, width=1366, height=768, format=NV12" ! \
kmssink bus-id="fd4a0000.display" sync=false fullscreen-overlay=true \
filesrc location=images/input_123456789.png ! pngdec ! imagefreeze ! videoconvert ! videoscale ! "video/x-raw, width=683, height=683" ! comp.sink_0 \
filesrc location=images/output_123456789.png ! pngdec ! imagefreeze ! videoconvert ! videoscale ! "video/x-raw, width=683, height=683" ! comp.sink_1

```

*(Replace `input_123456789.png` and `output_123456789.png` with your actual filenames).*

---

## 5. Troubleshooting the Demo

**Nothing happens when the button is pressed:**
Make sure you are pressing the correct button (**SW14**). Verify that the UIO driver is loaded properly in the system (`ls /dev/uio*`).
**Error: "Cannot open camera!"**
The camera is busy or not detected. Run `fuser -k /dev/video0` and restart the app. Check the USB connection.
**DisplayPort not detected:**
Check DRM devices:
```bash
ls /sys/class/drm
```
Expected output
```bash
card0
card0-DP-1
renderD128
```
Check connection status:
```bash
cat /sys/class/drm/card0-DP-1/status
```
Expected:
```bash
connected
```
**Cannot detect monitor resolution**

If the application prints:
```bash
[WARN] Cannot detect monitor resolution
```
verify:
```bash
cat /sys/class/drm/card0-DP-1/modes
```

## Notes

The application automatically reads the monitor resolution from:
```bash
/sys/class/drm/card0-DP-1/modes
```
If detection fails, the default resolution is:
```bash
1366x768
```