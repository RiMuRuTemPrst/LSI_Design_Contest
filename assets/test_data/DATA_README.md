# Test Data Directory — HiFiC Generator FPGA

Tất cả dữ liệu test được generate từ PyTorch HiFiC model với 1 ảnh test cố định.
Epsilon = 0.001, kiểu dữ liệu float16 (half precision).

---

## Cấu trúc thư mục

```
assets/test_data/
├── io_params/          ← IO tensors của Generator (dùng trong CSIM)
├── io_params_2/        ← IO tensors từ test case KHÁC (ảnh khác) — KHÔNG dùng cho CSIM
├── model_params/       ← Weights của Generator (Gen_*) — dùng trong mọi CSIM
├── model_params_2/     ← Weights của Encoder (Enc_*) — không dùng trong Generator CSIM
└── layer_test_vectors/ ← Test vectors cho các block riêng lẻ (ResBlock standalone)
```

---

## io_params/ — Tensor IO của Generator

Tất cả file trong `io_params/` thuộc **cùng 1 test case** (cùng ảnh input).
Pipeline: `Gen_input` → CBI → 9×ResBlock → GlobalAdd → 4×UCB → Conv77 → `Gen_final_output`

### Pipeline flow và file tương ứng

| File | Shape | Mô tả |
|------|-------|--------|
| `Gen_input.txt` | 16×16×220 = 56,320 | **Generator input** — hyper-latent đã reconstruct. CBI input. |
| `Gen_cbi_output.txt` | 16×16×960 = 245,760 | CBI output (Norm→Conv3×3→Norm). ResBlock input. Cũng là global skip cho GlobalAdd. |
| `Gen_rb0_output.txt` | 16×16×960 = 245,760 | ResBlock-0 output |
| `Gen_rb1_output.txt` | 16×16×960 | ResBlock-1 output |
| `Gen_rb2_output.txt` | 16×16×960 | ResBlock-2 output |
| `Gen_rb3_output.txt` | 16×16×960 | ResBlock-3 output |
| `Gen_rb4_output.txt` | 16×16×960 | ResBlock-4 output |
| `Gen_rb5_output.txt` | 16×16×960 | ResBlock-5 output |
| `Gen_rb6_output.txt` | 16×16×960 | ResBlock-6 output |
| `Gen_rb7_output.txt` | 16×16×960 | ResBlock-7 output |
| `Gen_rb8_output.txt` | 16×16×960 | ResBlock-8 output |
| `Gen_global_add_output.txt` | 16×16×960 = 245,760 | GlobalAdd output (rb8 + CBI skip). UCB_0 input. |
| `Gen_ucb1_pretranspose.txt` | 32×32×480 = 491,520 | UCB1 ConvTranspose output trước Norm+ReLU (intermediate, không dùng trong CSIM) |
| `Gen_ucb1_output.txt` | 32×32×480 = 491,520 | UCB1 (=UCB_0) output sau ConvTranspose+Norm+ReLU. UCB_1 input. |
| `Gen_ucb2_output.txt` | 64×64×240 = 983,040 | UCB2 (=UCB_1) output. UCB_2 input. |
| `Gen_ucb3_output.txt` | 128×128×120 = 1,966,080 | UCB3 (=UCB_2) output. UCB_3 input. |
| `Gen_ucb4_output.txt` | 256×256×60 = 3,932,160 | UCB4 (=UCB_3) output. Conv77 input. |
| `Gen_conv77_output.txt` | 256×256×3 = 196,608 | Conv77 output trước Clip (256×256×3) |
| `Gen_final_output.txt` | 256×256×3 = 196,608 | **Generator output** — ảnh sau Clip(0,1) |

> **Chú ý về ucbX naming**: file dùng 1-indexed (ucb1..ucb4) để match với model_params/Gen_ucbX_weight.txt.
> HLS dùng 0-indexed (MODE_UCB_0..MODE_UCB_3). Mapping: ucb1↔UCB_0, ucb2↔UCB_1, ucb3↔UCB_2, ucb4↔UCB_3.

### test_* files trong io_params/

Các file có prefix `test_` (e.g. `test_Gen_cbi_output.txt`, `test_Gen_rb0_output.txt`, ...) là output từ reference C++ implementation (`generator.cpp` trong Downloads) — **không phải golden chuẩn**, không dùng trong CSIM.

---

## model_params/ — Model Weights

Naming convention: `Gen_<block>_<type>.txt`

| Pattern | Ví dụ | Mô tả |
|---------|-------|--------|
| `Gen_cbi_cbi0_gamma/beta.txt` | — | CBI input normalization params (220 values) |
| `Gen_cbi_cbi2_weight.txt` | — | CBI conv weight [960, 3, 3, 220] = 1,900,800 values |
| `Gen_cbi_cbi2_bias.txt` | — | CBI conv bias (960 values) |
| `Gen_cbi_cbi3_gamma/beta.txt` | — | CBI output normalization params (960 values) |
| `Gen_rb0_weight_1/2.txt` | — | ResBlock-0 conv1/conv2 weight [960, 3, 3, 960] = 8,294,400 values |
| `Gen_rb0_bias_1/2.txt` | — | ResBlock-0 bias (960 values) |
| `Gen_rb0_gamma/beta_1/2.txt` | — | ResBlock-0 norm params (960 values) |
| `Gen_rb1_*` .. `Gen_rb8_*` | — | Tương tự cho ResBlock-1..8 |
| `Gen_ucb1_weight.txt` | — | UCB_0 ConvTranspose weight [480, 3, 3, 960] = 4,147,200 values |
| `Gen_ucb1_bias/gamma/beta.txt` | — | UCB_0 params (480 values) |
| `Gen_ucb2_*` .. `Gen_ucb4_*` | — | Tương tự cho UCB_1..3 |
| `Gen_cbo_weight.txt` | — | Conv77 weight [3, 7, 7, 60] = 8,820 values |
| `Gen_cbo_bias.txt` | — | Conv77 bias (3 values) |

> Weight layout: tất cả weight đã được **pre-transpose sang [CO, KH, KW, CI]** (NHWC convention).

---

## io_params_2/ — Test Case Khác (ảnh khác)

**KHÔNG dùng cho CSIM hiện tại.** Đây là IO tensors từ một ảnh test khác.

| File đáng chú ý | Mô tả |
|----------------|--------|
| `main_input_image.txt` | Input image RGB |
| `main_output_gold.txt` | Generator input (16×16×220) cho test case này |
| `Gen_cbi_cbi0_ReduceMean_1.txt` | ≈ main_output_gold.txt (cùng data, khác precision) |
| `Cb1..Cb5_Relu_output_0_numbers.txt` | Encoder outputs |
| `Hyp_*` | Hyperprior network outputs |

---

## Lịch sử điều tra (để tránh lặp lại)

### Bug đã phát hiện — CBI CSIM dùng sai input file
- **Vấn đề**: testbench ban đầu load `io_params_2/main_output_gold.txt` làm CBI input
- **Nguyên nhân**: file `io_params/Gen_input.txt` không tồn tại trong dataset, team member khác không save lại
- **Phát hiện**: `io_params_2/main_output_gold.txt` từ ảnh khác → max_err=1.52 (sai hoàn toàn)
- **Fix**: dùng `io_params/Gen_input.txt` (đã rename từ `Gen_cbi_cbi0_ReduceMean_1.txt`)
- **Xác nhận**: Python sim max_err=0.00756 ✓, HLS CSIM chờ kết quả

### CSIM Results (2026-05-14)
| Block | Kết quả | max_err | rmse |
|-------|---------|---------|------|
| UpConv UCB_0 | **PASS** | 0.00781 | 0.000591 |
| UpConv UCB_1 | **PASS** | 0.01367 | 0.001061 |
| UpConv UCB_2 | **PASS** | 0.02148 | 0.002284 |
| UpConv UCB_3 | **PASS** | 0.01563 | 0.001405 |
| Conv77 | **PASS** | 0.488 (TOL=1.0) | 0.162 |
| Fusion CBI/RB0/GA | chờ CSIM | — | — |
