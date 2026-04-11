# LSI Design Contest - High-Fidelity Generative Image Compression (HiFiC)

Dự án tối ưu hóa mô hình HiFiC cho FPGA (ZCU104) phục vụ cuộc thi thiết kế LSI.

## Cấu trúc thư mục (New Structure)

- `src/`: Mã nguồn chính của dự án.
    - `src/core/`: Thư viện lõi (C++ templates) cho tensor operations, conv, v.v.
    - `src/apps/`: Các ứng dụng thực thi (encoder, generator, demo).
    - `src/hls/`: Các dự án/IP Cores cho Vitis HLS (Conv77, GAN, Adder, v.v.).
- `tests/`: Các kịch bản kiểm thử (unit test và integration test).
- `sim/`: Môi trường mô phỏng C++ standalone.
- `assets/`: Tài sản tĩnh (Model weights, test vectors, cấu hình board).
- `docs/`: Tài liệu hướng dẫn và các bài báo khảo sát.
- `labs/`: Các bài thực hành/lab nền tảng về HLS.
- `scripts/`: Các script hỗ trợ (Python) để kiểm tra độ chính xác (RMSE, compare).

## Link Drive Model ONNX 

https://drive.google.com/drive/folders/1rIFSeccSnSQJDVDc3nhCQjibEOhMbgoR
