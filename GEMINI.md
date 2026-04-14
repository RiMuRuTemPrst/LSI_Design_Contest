# Technical Context for AI Agents (HiFiC FPGA Project)

## Project Overview
Dự án triển khai mô hình HiFiC lên FPGA ZCU104. 
Kiến trúc mô hình chia làm 3 phần chính: **Encoder**, **Hyperprior**, và **Generator (Decoder)**. Hiện tại tập trung chính vào khối **Generator**.

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

## Implementation Strategy (Standardized)
Phát triển theo kiểu **"Cuốn chiếu"**: Implement từng Layer HLS -> Debug với `layer_test_vectors` -> Tích hợp vào `Hific_full.cpp`.

### Quy chuẩn cấu trúc HLS Block (Ví dụ: `src/hls/resblock_top/`):
- `gen/`: Chứa mã HLS tổng hợp.
    - `Hls_Layers.tpp`: Chứa logic các layer "nguyên tử" (Conv, CNorm, Act...) và các hàm kernel lõi (ví dụ: `ResidualBlock_Kernel`).
    - `Core.h`: Khai báo kiểu dữ liệu (`data_256_t`), cấu trúc `TensorMem` và include các file `.tpp`.
    - `[ip_name]_top.cpp`: File top-level chứa interface AXI, gọi đến `_Kernel` trong `.tpp`.
- `non_gen/`: Chứa mô hình tham chiếu (Golden) và các tiện ích in/out.
- `test.cpp`: Testbench chính.
- `hls_config.cfg`: File cấu hình cho `vitis-run`. Luôn sử dụng `vitis-run --mode hls --config hls_config.cfg --work_dir prj --csim` để chạy mô phỏng.

## Key Technical Details
- **Data Type**: `_Float16` (Half-precision). Trong C++ Simulation, có thể dùng `float` thông qua cờ `#ifndef __SYNTHESIS__`.
- **Memory**: Sử dụng **Memory Arena** (`ARENA_1`, `ARENA_2`) 20MB mỗi vùng.
- **Reflect Padding**: Tích hợp trực tiếp vào logic load buffer (`init_rows`) và tính toán chỉ số, không tách thành IP riêng.
- **Paths**: Luôn dùng đường dẫn tương đối (Relative paths) trong code HLS để đảm bảo tính di động.
- **SIMD**: Mô phỏng phần mềm cần cờ `-mavx -mavx2 -mf16c -mfma` để hỗ trợ `<immintrin.h>`.

## Generator Architecture
1. `conv_block_init` -> 2. `res_block0-8` -> 3. `Add (Global Skip)` -> 4. `up_conv_block1-4` -> 5. `conv_block_out` -> `Clip`.
Chi tiết sizing nằm trong `docs/GAN_Layer_Sizing_Details.csv`.

### Independent IP Cores
Ngoài các khối lớn (`resblock_top`, `convtranspose`, `Conv77`), Generator còn được xây dựng từ các IP độc lập nhỏ hơn (nguyên tử) để tăng tính tái sử dụng và ghép nối linh hoạt trên Block Design:
- **`Conv3x3`**: Tích chập 3x3 với On-the-fly Reflect Padding tích hợp.
- **`ChannelNorm`**: Chuẩn hóa kênh (Tính Mean, Variance theo không gian).
- **`Relu`**: Vectorized ReLU activation (16 phần tử FP16/cycle).
- **`Add`**: Element-wise addition cho Skip Connections.
Tất cả các IP này đều sử dụng bus `data_256_t` (256-bit AXI) để đảm bảo băng thông và tính tương thích.
