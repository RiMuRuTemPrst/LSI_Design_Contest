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
- **Mã nguồn**: `src/`.
- **Mô phỏng End-to-End**: Chạy `src/apps/Hific_full.cpp`. Kết quả sẽ xuất ra `assets/test_data/io_params_2/main_output_result.txt`.

---
*Dự án phục vụ cuộc thi thiết kế LSI 2026.*
