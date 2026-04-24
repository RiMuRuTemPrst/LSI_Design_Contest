# Technical Context for AI Agents (HiFiC FPGA Project)

## Project Overview
Dự án triển khai mô hình HiFiC lên FPGA ZCU104. 
Kiến trúc mô hình chia làm 3 phần chính: **Encoder**, **Hyperprior**, và **Generator (Decoder)**. Hiện tại tập trung tối ưu hóa khối **Generator** bằng kiến trúc **Fusion Core**.

## Data Classification
- **Weights/Params (`assets/test_data/model_params/`)**: Trọng số Generator bắt đầu bằng `Gen_...`.
- **Input/Output Vectors**: Nằm trong `io_params/` và `layer_test_vectors/`.

## Implementation Strategy (Standardized)
Phát triển theo kiểu **"Cuốn chiếu"** và **"Hợp nhất tài nguyên"**:
- **Hls_Layers_Fusion.tpp**: Thư viện primitives hợp nhất (CBI + Residual Block).
- **Kernel Standard**: Tất cả hàm xử lý lõi dùng hậu tố `_Kernel`.
- **Layer Fusion**: Gộp nhiều lớp tính toán vào một IP để triệt tiêu việc ghi/đọc DDR trung gian.

## Generator Architecture: Fusion Core (`src/hls/fusion_core/`)
Thay vì sử dụng nhiều IP rời rạc, Generator được vận hành bởi một **"Fusion Core"** duy nhất, xử lý 10 blocks đầu tiên (1 CBI + 9 ResBlocks) bằng cách tái sử dụng 100% tài nguyên vật lý.

### 1. Thông số Kỹ thuật đạt được (ZCU104):
| Thông số | Giá trị | Trạng thái |
| :--- | :--- | :--- |
| **Fmax** | **411 MHz** | Vượt mức 300MHz yêu cầu |
| **Initiation Interval (II)** | **1** | Đạt tối ưu tuyệt đối cho vòng lặp MAC và Stats |
| **BRAM Utilization** | **263 (42%)** | Cực kỳ tiết kiệm nhờ burst_length=64 |
| **LUT Utilization** | **55,274 (24%)** | Tăng nhẹ để tích hợp thêm logic CBI |

### 2. Chế độ hoạt động (Modes):
Điều khiển qua AXI-Lite để IP "biến hình" linh hoạt:
- **MODE_CBI (0)**: `Norm-on-Load -> Conv 3x3 -> Norm -> Bypass`.
- **MODE_RB_L1 (1)**: `Plain Load -> Conv 3x3 -> Norm -> ReLU`.
- **MODE_RB_L2 (2)**: `Plain Load -> Conv 3x3 -> Norm -> Add (Skip)`.

### 3. Các bài học tối ưu hóa quan trọng:
- **Partial Sum Pattern**: Sử dụng mảng `partial_sum[14]` cho các phép tích lũy số thực trong CBI để phá vỡ sự phụ thuộc vòng lặp, đạt II=1.
- **AXI Burst Length**: Cố định `max_read/write_burst_length=64` để tránh việc HLS tự động chèn quá nhiều bộ đệm BRAM dư thừa.
- **Resource Sharing**: Hợp nhất toàn bộ cụm PE MAC và bộ đệm dòng (`x_buf`) vào một kernel duy nhất thay vì chia nhiều kernel độc lập.
- **CSIM Memory**: Sử dụng cấp phát động (`new`) cho các bộ đệm tĩnh lớn trong môi trường mô phỏng để tránh lỗi `double free` hoặc `invalid pointer` khi gọi IP nhiều lần.

## Key Technical Details
- **Data Type**: `half` (_Float16) cho lõi tính toán, `float` cho các bước tính Mean/Var trung gian để đảm bảo độ chính xác.
- **Memory**: Ưu tiên **URAM** cho bộ đệm `skip_buf` (Persistent) để giải phóng BRAM.
- **SIMD**: Xử lý 16 phần tử FP16 mỗi chu kỳ clock trên bus 256-bit.
