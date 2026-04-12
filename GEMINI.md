# Technical Context for AI Agents (HiFiC FPGA Project)

## Project Overview
Dự án triển khai mô hình HiFiC lên FPGA ZCU104. 
Kiến trúc mô hình chia làm 3 phần chính: **Encoder**, **Hyperprior**, và **Generator (Decoder)**.

## Data Classification (Crucial)
AI Agent cần chú ý sự khác biệt giữa các bộ tham số để gọi tệp tin chính xác:

### 1. Weights/Params (`assets/test_data/`)
- **`model_params/`**: Chứa trọng số bắt đầu bằng `Gen_...`. Đây là các tham số cho khối **Generator**.
- **`model_params_2/`**: Chứa trọng số bắt đầu bằng `Enc_...` (cho **Encoder**) và `Hyp_...` (cho **Hyperprior**).

### 2. Input/Output Vectors
- **`io_params_2/` (Main System Test)**:
    - `main_input_image.txt`: Input gốc của toàn mạng.
    - `main_output_gold.txt`: Output chuẩn của toàn mạng.
    - `main_output_result.txt`: Output sinh ra bởi code C++ (để so sánh).
- **`io_params/` (Module Test)**: Chứa các vector test cho riêng khối Generator.
- **`layer_test_vectors/`**: Chứa In/Out trung gian của từng Layer lẻ để debug cuốn chiếu.

## Implementation Strategy
Phát triển theo kiểu **"Cuốn chiếu"**: Implement từng Layer HLS -> Debug với `layer_test_vectors` -> Tích hợp vào `Hific_full.cpp`.

Quy chuẩn cấu trúc HLS Block (ví dụ: `src/hls/resblock_top/`):
- `gen/`: Chứa mã HLS tổng hợp (`.cpp`, `.h`) và thư viện Tensor `Core.h`.
- `non_gen/`: Chứa mô hình tham chiếu (Golden) và các tiện ích in/out.
- `test.cpp`: Testbench chính.
- `hls_config.cfg`: File cấu hình cho `vitis-run`. Luôn sử dụng `vitis-run --mode hls --config hls_config.cfg --work_dir prj --csim` để chạy mô phỏng.

## Key Technical Details
- **Data Type**: `_Float16` (Half-precision). Trong C++ Simulation, có thể dùng `float` thông qua cờ `#ifndef __SYNTHESIS__`.
- **Memory**: Sử dụng **Memory Arena** (`ARENA_1`, `ARENA_2`) 20MB mỗi vùng.
- **Paths**: Luôn dùng Linux forward slashes (`/`). Từ `src/apps/`, đường dẫn dữ liệu thường là `../../assets/test_data/...`.
- **SIMD**: Mô phỏng phần mềm cần cờ `-mavx -mavx2 -mf16c -mfma` để hỗ trợ `<immintrin.h>`.

## Generator Architecture
1. `conv_block_init` -> 2. `res_block0-8` -> 3. `Add (Global Skip)` -> 4. `up_conv_block1-4` -> 5. `conv_block_out` -> `Clip`.
Chi tiết sizing nằm trong `docs/GAN_Layer_Sizing_Details.csv`.
