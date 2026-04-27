# Technical Context for AI Agents (HiFiC FPGA Project)

## Project Overview
Dự án triển khai mô hình HiFiC lên FPGA ZCU104.
Kiến trúc Generator (Decoder) được triển khai thành **một IP hoàn chỉnh duy nhất** (`full_generator_top`) bao gồm: **Fusion Core** (CBI + 9×ResBlock + GlobalAdd) và **UpConv Core** (4×UCB).

## Full Generator Architecture (reference — từ GAN.jpg)
```
hyper-latent (16×16×960)
    │
    ▼ [conv_block_init / CBI] ✅ DONE
  Norm → Pad → Conv 3×3 → Norm
    │
    ▼ [9× res_block] ✅ DONE
  Pad→Conv3×3→Norm→ReLU→Pad→Conv3×3→Norm→Add(SC)
    │
    ▼ [GlobalAdd — skip từ CBI] ✅ DONE
    │
    ▼ [4× up_conv_block] ✅ DONE
  UCB_0: 16×16×960  → 32×32×480   (ConvTranspose 3×3 → Norm → ReLU)
  UCB_1: 32×32×480  → 64×64×240
  UCB_2: 64×64×240  → 128×128×120
  UCB_3: 128×128×120 → 256×256×60
    │
    ▼ [conv_block_out] ⏳ TODO
  Pad → Conv 7×7 → Clip
    │
    ▼ Output (256×256×3)
```

## Implemented IP: full_generator_top
Bao gồm Fusion Core + UpConv Core. Output hiện tại là `256×256×60` (trước conv_block_out).

**AXI Ports:** `gmem_x` (X inout), `gmem_wf` (W_fusion), `gmem_pf` (P_fusion),
`gmem_wu` (W_upconv), `gmem_pu` (P_upconv), `gmem_y` (Y inout — UCB_3 output, 256×256×60)

### Source Files
| File | Mô tả |
| :--- | :--- |
| `fusion_core/gen/Hls_Layers_Fusion.tpp` | Universal_Engine_Kernel, GlobalAdd_Kernel |
| `upconv_core/gen/Hls_Layers_UpConv.tpp` | UpConv_Fused_Row |
| `full_generator/gen/generator_top.cpp` | Top-level unified IP |
| `fusion_core/gen/fusion_core_top.cpp` | Standalone fusion IP (deploy riêng nếu cần) |
| `upconv_core/gen/upconv_core_top.cpp` | Standalone upconv IP (deploy riêng nếu cần) |

## Synthesis Results — ZCU104 (xczu7ev-ffvc1156-2-e, 300 MHz)

### Resource Utilization (full_generator_top)
| Resource | Used | Available | % |
| :--- | :--- | :--- | :--- |
| BRAM_18K | 416 | 624 | **66%** |
| DSP | 776 | 1728 | **44%** |
| LUT | 86,544 | 230,400 | **37%** |
| FF | 97,755 | 460,800 | **21%** |
| URAM | 80 | 96 | **83%** |
| **Fmax** | **308 MHz** | 300 MHz | **✅ Pass** |

### On-chip Memory Layout
| Buffer | Size | Type | Mục đích |
| :--- | :--- | :--- | :--- |
| `skip_buf` | 16×16×60 words | URAM (16 blocks) | ResBlock skip connection |
| `global_buf` | 16×16×60 words | URAM (16 blocks) | CBI→GlobalAdd skip |
| `x_buf` (upconv) | 2×128×60 words | URAM (16 blocks) | Ping-pong sliding window |
| `row_acc` | 256×480 half | URAM (32 blocks) | UpConv output accumulator |
| `x_buf` (fusion) | 4×W×60 words | **BRAM** | Fusion IFM sliding window |

## Latency Estimates (@300 MHz)

### Fusion Core (từ full_generator synthesis — tham số cố định)
| Block | Cycles (min) | Latency (min) |
| :--- | :--- | :--- |
| CBI (1 lần gọi UEK) | 10.6M | **35.4 ms** |
| 9×ResBlock (L1+L2) | 191M | **637 ms** |
| GlobalAdd | 15,507 | **52 µs** |
| **Fusion tổng** | **202M** | **~673 ms** |

### UpConv Core (từ per-mode standalone synthesis)
| Mode | Config | Latency min | Latency max |
| :--- | :--- | :--- | :--- |
| UCB_0 | 16×16×960 → 32×32×480 | **40 ms** | 366 ms |
| UCB_1 | 32×32×480 → 64×64×240 | **40 ms** | 582 ms |
| UCB_2 | 64×64×240 → 128×128×120 | **57 ms** | 2.21 sec |
| UCB_3 | 128×128×120 → 256×256×60 | **133 ms** | 3.35 sec |

> **Ghi chú:** min/max của UCB rộng vì `ho` (output row index) quyết định số kernel
> position hợp lệ (1 hay 2) → computation thay đổi theo từng row. Min là boundary rows,
> max là interior rows. Thực tế sẽ ở giữa (trung bình ~1.5× computation per row vs min).

## Key Technical Decisions

### Tại sao x_buf của Fusion Core phải là BRAM (không phải URAM)
`x_buf[4][W][60]` trong `Universal_Engine_Kernel` dùng `ARRAY_PARTITION complete dim=2`
→ 16 banks song song → nếu để URAM: 64 blocks (vượt 96/96 limit). Phải để BRAM.
Ngược lại, `skip_buf` và `global_buf` (lớn hơn, persistent) mới để URAM.

### Tại sao HLS tạo 1 instance thay vì 2
Nếu pass `NULL` cho G_IN/BE_IN trong RB calls → HLS thấy connectivity khác → tạo 2
hardware instances → BRAM overflow. Fix: pass `P_fusion` (dummy) cho tất cả calls →
HLS thấy identical port connections → 1 shared instance cho CBI + 9×RB.
Tương tự cho `run_upconv_block`: pass cả X lẫn Y, chọn input bằng `mode` bên trong.

### Rotating Accumulator (II=1 cho MAC FP16)
FP16 adder có latency 3-4 cycles. Dùng `psum[PEs][4]` xoay vòng với `acc_idx = ciw & 3`
→ distance giữa RAW dependencies = 4 ≥ latency → `#pragma HLS DEPENDENCE false` hợp lệ
→ II=1 cho CI_LOOP.

## Known Issues / Workarounds
| Issue | Root Cause | Workaround / Fix |
| :--- | :--- | :--- |
| `x_buf` fusion phải là BRAM, không phải URAM | `ARRAY_PARTITION complete dim=2` → 16 banks → URAM cần 64 blocks, vượt limit 96 | Bind `x_buf` vào BRAM; chỉ `skip_buf`/`global_buf` mới để URAM |
| HLS tạo 2 instances của `Universal_Engine_Kernel` | Pass `NULL` vs non-NULL cho G_IN/BE_IN → HLS thấy connectivity khác → 2 hardware instances → BRAM overflow | Pass `P_fusion` (dummy, non-NULL) cho tất cả calls → identical port connections → 1 shared instance |
| HLS tạo 2 instances của `run_upconv_block` | UCB_0 đọc X, UCB_1-3 đọc Y → different input ports → 2 instances | Pass cả X và Y vào function, chọn input bằng `mode` bên trong → 1 shared instance |
| Duplicate symbol khi include cả 2 `.tpp` | `half_to_bits`, `bits_to_half`, `my_sqrt_f` defined ở cả `Hls_Layers_Fusion.tpp` và `Hls_Layers_UpConv.tpp` | Guard `#ifndef HLS_HALF_HELPERS_DEFINED` quanh các helper functions |

## Data Classification
- **Weights/Params**: `assets/test_data/model_params/` (Trọng số Generator bắt đầu bằng `Gen_...`)
- **Input/Output**: `io_params/` và `layer_test_vectors/`
