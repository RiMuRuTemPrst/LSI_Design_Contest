# Technical Context for AI Agents (HiFiC FPGA Project)

## Project Overview
Dự án triển khai mô hình HiFiC lên FPGA ZCU104.
Kiến trúc Generator (Decoder) được triển khai thành **1 IP duy nhất**: `full_generator_top` (Fusion Core + UpConv Core + Conv77).

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
    ▼ [conv_block_out] ✅ DONE (integrated into full_generator_top — Stage 3)
  Pad → Conv 7×7 (SIMD_DEPTH=8, NUM_WIN_PEs=8)
    │
    ▼ Output (256×256×3)
```

## Implemented IPs

### full_generator_top ✅ UPDATED (3 stages)
Bao gồm Fusion Core + UpConv Core + Conv77. Output cuối là `256×256×3` (Z port).

**AXI Ports:**
- `gmem_x` (X inout — latent 16×16×960, 256-bit words)
- `gmem_wf` (W_fusion), `gmem_pf` (P_fusion)
- `gmem_wu` (W_upconv), `gmem_pu` (P_upconv)
- `gmem_y` (Y inout — UCB_3 output 256×256×60, đọc bởi Conv77)
- `gmem_wc` (W_conv77 — 588 words), `gmem_bc` (B_conv77 — 1 word)
- `gmem_z` (Z write — 65536 words, 1 pixel/word, bits[47:0]=RGB half)

### conv77_core_top (standalone — reference/backup)
Standalone IP cũ cho conv_block_out: 256×256×60 → 256×256×3 (flat loop, 8 URAM).

### Source Files
| File | Mô tả |
| :--- | :--- |
| `fusion_core/gen/Hls_Layers_Fusion.tpp` | Universal_Engine_Kernel, GlobalAdd_Kernel |
| `upconv_core/gen/Hls_Layers_UpConv.tpp` | UpConv_Fused_Row |
| `Conv77/gen/Hls_Layers_Conv77.tpp` | Conv77_Kernel — SIMD+PE (8×8), data_256_t/half |
| `Conv77/gen/Hard_op_3.cpp` | Standalone HLS wrapper cho Conv77 csim |
| `full_generator/gen/generator_top.cpp` | Top-level unified IP (fusion + upconv + conv77) |
| `conv77_core/gen/Hls_Layers_Conv77.tpp` | Conv77_Core — flat loop, 196 iters (reference) |
| `conv77_core/gen/conv77_core_top.cpp` | Standalone AXI wrapper cho conv77_core |
| `fusion_core/gen/fusion_core_top.cpp` | Standalone fusion IP |
| `upconv_core/gen/upconv_core_top.cpp` | Standalone upconv IP |

## CSIM Results — Real-Data Verification (2026-05-14) ✅ ALL PASS

| Block | Config | max_err | rmse | mismatch |
| :--- | :--- | :--- | :--- | :--- |
| CBI | 16×16×220→960 | 0.01953 | 0.00169 | 0/245,760 |
| ResBlock-0 | 16×16×960→960 | 0.01953 | 0.000978 | 0/245,760 |
| GlobalAdd | 16×16×960→960 | 0.02344 | 0.00178 | 0/245,760 |
| UCB_0 | 16×16×960→32×32×480 | 0.00781 | 0.000591 | 0/491,520 |
| UCB_1 | 32×32×480→64×64×240 | 0.01367 | 0.001061 | 0/983,040 |
| UCB_2 | 64×64×240→128×128×120 | 0.02148 | 0.002284 | 0/1,966,080 |
| UCB_3 | 128×128×120→256×256×60 | 0.01563 | 0.001405 | 0/3,932,160 |
| Conv77 | 256×256×60→3 | 0.488 (TOL=1.0) | 0.162 | 0/196,608 |

> Input data: real model weights + io_params/ golden tensors. Xem `assets/test_data/DATA_README.md` để biết file mapping.
> CBI input file: `io_params/Gen_input.txt` (= Gen_cbi_cbi0_ReduceMean_1.txt đã đổi tên).

## Synthesis Results — ZCU104 (xczu7ev-ffvc1156-2-e, 300 MHz)

### Resource Utilization (full_generator_top — Fusion + UpConv + Conv77) ✅ UPDATED (post Clip+WI+N_TILES fix)
| Resource | Used | Available | % |
| :--- | :--- | :--- | :--- |
| BRAM_18K | 545 | 624 | **87%** |
| DSP | 1224 | 1728 | **70%** |
| LUT | 160,811 | 230,400 | **69%** |
| FF | 144,322 | 460,800 | **31%** |
| URAM | 80 | 96 | **83%** |
| **Fmax** | **308.74 MHz** | 300 MHz | **✅ Pass** |

> Cập nhật 2026-05-13: Re-synthesis sau WI_LOOP fix, N_TILES fix, và Clip(0,1) thêm vào Conv77 output.
> LUT tăng từ 123K→160K chủ yếu do Conv77 Clip logic trên ap_fixed<32,16> (xem per-block breakdown bên dưới).

### Per-Block Resource Breakdown (full_generator_top)
| Block | BRAM_18K | DSP | FF | LUT | URAM |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Universal_Engine_Kernel (Fusion) | 152 | 174 | 33,999 | 32,102 | 16 |
| UpConv_Fused_Row (UpConv) | 144 | 567 | 51,414 | 41,676 | 32 |
| Conv77_Kernel (Conv77 + Clip) | 56 | 448 | 41,832 | 69,010 | 0 |
| Other / overhead | 193 | 35 | 17,077 | 18,023 | 32 |
| **Total** | **545** | **1224** | **144,322** | **160,811** | **80** |

### Resource Utilization (full_generator_top — cũ, Fusion + UpConv only, reference)
| Resource | Used | Available | % |
| :--- | :--- | :--- | :--- |
| BRAM_18K | 416 | 624 | 66% |
| DSP | 776 | 1728 | 44% |
| LUT | 86,544 | 230,400 | 37% |
| FF | 97,755 | 460,800 | 21% |
| URAM | 80 | 96 | 83% |
| **Fmax** | **308 MHz** | 300 MHz | ✅ Pass |

### Resource Utilization (conv77_core_top — standalone flat-loop, reference)
| Resource | Used | Available | % |
| :--- | :--- | :--- | :--- |
| BRAM_18K | 198 | 624 | 31% |
| DSP | 192 | 1728 | 11% |
| LUT | 25,838 | 230,400 | 11% |
| FF | 38,243 | 460,800 | 8% |
| URAM | 8 | 96 | 8% |
| **Fmax** | **~308 MHz** | 300 MHz | ✅ Pass (CP=2.554 ns) |

> conv77_core CSIM: max_err=0.00586, rmse=0.000886, 0/196608 mismatch — **PASS**

### On-chip Memory Layout
| Buffer | Size | Type | Mục đích |
| :--- | :--- | :--- | :--- |
| `skip_buf` | 16×16×60 words | URAM (16 blocks) | ResBlock skip connection |
| `global_buf` | 16×16×60 words | URAM (16 blocks) | CBI→GlobalAdd skip |
| `x_buf` (upconv) | 2×128×60 words | URAM (16 blocks) | Ping-pong sliding window |
| `row_acc` | 256×480 half | URAM (32 blocks) | UpConv output accumulator |
| `x_buf` (fusion) | 4×W×60 words | **BRAM** | Fusion IFM sliding window |
| `x_buffer` (conv77) | 7×14×64 fp16_t | BRAM | Conv77 SIMD sliding window |
| `w_buffer` (conv77) | 3×7×7×64 fp16_t | BRAM | Conv77 weight on-chip |

> Conv77 SIMD+PE design dùng 0 URAM (vs 8 URAM của flat-loop design). Integrated URAM budget: **80/96 (83%)** ✅

## Latency Estimates (@300 MHz)

### Fusion Core (từ full_generator synthesis)
| Block | Cycles (min) | Latency (min) |
| :--- | :--- | :--- |
| CBI (1 lần gọi UEK) | 10.6M | **35.4 ms** |
| 9×ResBlock (L1+L2) | 191M | **637 ms** |
| GlobalAdd | 15,507 | **52 µs** |
| **Fusion tổng** | **202M** | **~673 ms** |

### UpConv Core (từ verified standalone bench synthesis)
| Mode | Config | Latency min | Latency max | Timing (Est) |
| :--- | :--- | :--- | :--- | :--- |
| UCB_0 | 16×16×960 → 32×32×480 | **39.96 ms** | 202 ms | 2.947 ns |
| UCB_1 | 32×32×480 → 64×64×240 | **39.55 ms** | 309 ms | 2.993 ns |
| UCB_2 | 64×64×240 → 128×128×120 | **63.24 ms** | 546 ms | 2.433 ns |
| UCB_3 | 128×128×120 → 256×256×60 | **132 ms** | 1.75 s | 2.904 ns |

> Kết quả từ ucbX_bench (inlined body + exact tripcounts). Khoảng min/max rộng do `KH_LOOP` (1-2 iters) và `KW_LOOP` (2-3 iters) phụ thuộc vào row index `ho`.
> Cập nhật 2026-05-13: Verified latency range sau khi inline core function và fix tripcounts.
> UCB_2 re-synthesis sau fix N_TILES bug (16→15): min tăng nhẹ do HLS scheduling, max giảm 52% (1.13s→546ms), timing cải thiện (2.719→2.433ns).

### Conv77 (SIMD+PE, từ full_generator integrated synthesis) ✅ UPDATED
| Block | Cycles | Latency @ 300 MHz |
| :--- | :--- | :--- |
| Conv77_Kernel (256×256×60→3) | 54.5M | **~182 ms** |

> SIMD+PE design: 8×8 = 448 MACs/cycle, 0 URAM, dùng BRAM cho x_buffer và w_buffer.
> Conv77 CSIM (data_256_t interface, post-clip golden `output_numbers.txt`): max_err=0.488, rmse=0.162, 0/196608 mismatch (TOL=1.0) — **PASS**

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

### Conv77 tích hợp vào full_generator: dùng Conv77_Kernel<> không phải HW_Conv7x7
`HW_Conv7x7` có AXI pragmas riêng (bundle=gmem_X/W/B/Z) → conflict với full_generator_top.
Phải gọi `Conv77_Kernel<8,8,1,60,3,256,256,7,7,256,256>()` trực tiếp, ports khai báo ở top-level.

## Known Issues / Workarounds
| Issue | Root Cause | Workaround / Fix |
| :--- | :--- | :--- |
| `x_buf` fusion phải là BRAM, không phải URAM | `ARRAY_PARTITION complete dim=2` → 16 banks → URAM cần 64 blocks, vượt limit 96 | Bind `x_buf` vào BRAM; chỉ `skip_buf`/`global_buf` mới để URAM |
| HLS tạo 2 instances của `Universal_Engine_Kernel` | Pass `NULL` vs non-NULL cho G_IN/BE_IN → HLS thấy connectivity khác → 2 hardware instances → BRAM overflow | Pass `P_fusion` (dummy, non-NULL) cho tất cả calls → identical port connections → 1 shared instance |
| HLS tạo 2 instances của `run_upconv_block` | UCB_0 đọc X, UCB_1-3 đọc Y → different input ports → 2 instances | Pass cả X và Y vào function, chọn input bằng `mode` bên trong → 1 shared instance |
| Duplicate symbol khi include cả 2 `.tpp` | `half_to_bits`, `bits_to_half`, `my_sqrt_f` defined ở cả `Hls_Layers_Fusion.tpp` và `Hls_Layers_UpConv.tpp` | Guard `#ifndef HLS_HALF_HELPERS_DEFINED` quanh các helper functions |
| Conv77 latency 1.365 sec (49 × CI_LOOP pipeline startup) | CI_LOOP iteration latency = 91 cycles (L_MAC tree depth) × 49 kernel positions = lãng phí fill/drain overhead | Flat loop 196 iterations: `psum[co][k_ci]` write-once → no RAW, II=1 không cần DEPENDENCE; REDUCE depth-4 rotating acc → 0.120 sec (**11× speedup**) |
| Conv77 line_buf 16 URAM → integration 96/96 = 100% | `ARRAY_PARTITION complete dim=3` trên line_buf tạo 4 banks × 4 URAM wide = 16 URAM; 80+16=96/96 không fit | Bỏ dim=3 partition (flat loop chỉ cần 1 read/iter) → 8 URAM; integration: 80+8=88/96 (92%) ✅ |
| Conv77 CSIM vitis_hls fail: "Cannot open: io_params/..." | vitis_hls csim chạy từ `Conv77_HLS/solution/csim/build/`, copy `-tb` files flat (không giữ subdir) | test.cpp dùng tên file phẳng (không prefix `io_params/` hay `model_params/`); TOL=1.0 cho ap_fixed<16,8> |
| Conv77 Clip(0,1) thêm ~36K LUT (69K total vs 32K before) | Clip trên `fp32acc_t = ap_fixed<32,16>` bên trong pipelined unrolled loop → HLS tạo 32-bit comparator logic cho mỗi PE output | Chấp nhận: tổng LUT=160,811 (69%) vẫn trong limit; Clip bắt buộc theo HiFiC model spec |

## Data Classification
- **Weights/Params**: `assets/test_data/model_params/` (Trọng số Generator bắt đầu bằng `Gen_...`)
- **Input/Output**: `io_params/` và `layer_test_vectors/`
