# LSI Design Contest 2026 - HiFiC FPGA Optimization

Dự án tối ưu hóa mô hình **High-Fidelity Generative Image Compression (HiFiC)** cho FPGA (ZCU104).

## 📁 Cấu trúc Dữ liệu (Assets)
Dữ liệu được chia thành 2 bộ chính để phục vụ test từng phần hoặc toàn bộ hệ thống:

- **Bộ 1 (Generator Focus)**:
    - `assets/test_data/model_params/`: Trọng số của khối **Generator (Decoder)**.
    - `assets/test_data/io_params/`: Vector vào/ra trung gian để test độc lập Decoder.
- **Bộ 2 (End-to-End Focus)**:
    - `assets/test_data/model_params_2/`: Trọng số của khối **Encoder** và **Hyperprior**.
    - `assets/test_data/io_params_2/`: Dữ liệu test toàn hệ thống.
        - `main_input_image.txt`: Ảnh đầu vào gốc (256x256x3).
        - `main_output_gold.txt`: Kết quả chuẩn (Gold) để so sánh.

## 🚀 Quick Start
### 1. Mô phỏng End-to-End (Software)
Chạy mã nguồn C++ thuần cho toàn bộ hệ thống bằng `g++`. Yêu cầu hỗ trợ AVX/F16C để chạy các hàm intrinsic được tối ưu hóa.
```bash
g++ -Isrc/core -mavx -mavx2 -mf16c -mfma src/apps/Hific_full.cpp -o hific_full
./hific_full
```
Kết quả sẽ xuất ra `assets/test_data/io_params_2/main_output_result.txt`.

### 2. Mô phỏng HLS Block (C-Simulation)
Các khối phần cứng (ví dụ: `resblock_top`) được cấu trúc với `hls_config.cfg`. Chạy mô phỏng bằng công cụ Vitis HLS 2024.2+:
```bash
cd src/hls/resblock_top
vitis-run --mode hls --config hls_config.cfg --work_dir prj --csim
```

---
*Dự án phục vụ cuộc thi thiết kế LSI 2026.*
