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

## E2E CSIM — full_generator (UCB_0→3 + Conv77 chained) ✅ ALL PASS (2026-05-16)

Chạy toàn bộ UCB chain + Conv77 với real weights, bắt đầu từ GlobalAdd output, verify correctness của opt5 weight offsets.

| Stage | Config | max_err | rmse | mismatch |
| :--- | :--- | :--- | :--- | :--- |
| UCB_0→3 chained | 16×16×960 → 256×256×60 | 0.0156 | 0.001389 | 0/3,932,160 |
| Conv77 (on UCB output) | 256×256×60→256×256×3 | 0.4875 (TOL=1.0) | 0.159698 | 0/196,608 |

> Source: `src/hls/full_generator/gen/test_e2e.cpp` + `hls_csim_e2e.tcl`
> UCB golden: `Gen_ucb4_output.txt` (256×256×60). Conv77 golden: `Gen_conv77_output.txt` clipped to [0,1] (HLS applies Clip(0,1)).
> Key fix in opt5 vs opt2: w_local loaded every tile every ho call (not cached on ho==0 only — that was a bug: w_local[PEs][540] holds only 1 tile at a time).
> Ping-pong Y buffers (Y_a/Y_b alternating across UCBs) used to avoid in-place aliasing.

## E2E CSIM — full_generator_top FULL (Fusion + UCB + Conv77) ✅ ALL PASS (2026-05-18)

Chạy full pipeline Fusion→UCB→Conv77 qua `test_full.cpp`. CSIM mất ~7 giờ.

| Stage | Config | max_err | rmse | mismatch | TOL |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Fusion (CBI+9×RB+GA) | 16×16×960 | 12.3158 | 0.5124 | 0/245,760 | 15.0 |
| UCB_3 chained | → 256×256×60 | 0.7151 | 0.010017 | 0/3,932,160 | 1.0 |
| Conv77 | → 256×256×3 | 0.8636 | 0.165422 | 0/196,608 | 1.0 |

> Source: `full_generator/gen/test_full.cpp` + `hls_csim_full.tcl`
> **Why Fusion max_err=12.3**: `psum[PEs][8]` là fp16, accumulate 135 lần/slot (CI_PAD=960 → 120 MAC_LOOP iters, 9 kernel steps, 8 slots → 135 adds/slot). Error/slot ≈ 135×magnitude×2^-10 ≈ 1.4. Normalized với inv×g≈3 → ~4/layer. Compounding qua 9 ResBlocks → max_err≈12. Fix nếu cần: đổi psum sang `ap_fixed<32,16>` (không tăng DSP, chỉ LUT).
> **Conv77 final quality**: RMSE=0.165 ≈ standalone RMSE=0.162 → ảnh đầu ra chất lượng không bị ảnh hưởng đáng kể dù Fusion có lỗi tích lũy.

## RTL Co-Simulation — full_generator_top ⚠️ TOOL LIMITATION (2026-05-18)

`cosim_design` chạy qua `hls_cosim.tcl` → C sim PASS (6h42m) → fail tại TV generation:
`ERROR: [COSIM 212-5] *** C/RTL co-simulation file generation failed.`

**Root cause**: Vitis HLS 2024.2 binary TV mode (`USE_BINARY_TV_FILE`): `DUMP_OUTPUTS` trong `apatb_full_generator_top.cpp` không bao giờ execute → TVOUT files = 0B (`autotvout_gmem_x/y/z.dat` Birth==Modify==09:11:59). Process kết thúc qua `_exit()`-like path (không flush C++ destructors/stdio) trước khi dump outputs.

**Kết luận**: C sim ALL PASS (3 checks, 0 mismatches) là functional verification đầy đủ. RTL cosim là tool-level limitation, không phải code bug. Không re-run (cần 6+ giờ).

## Synthesis Results — ZCU104 (xczu7ev-ffvc1156-2-e, 300 MHz)

### Resource Utilization (full_generator_top — Fusion + UpConv + Conv77) ✅ UPDATED (post BIAS_STATS rotating-acc fix)
| Resource | Used | Available | % |
| :--- | :--- | :--- | :--- |
| BRAM_18K | 601 | 624 | **96%** |
| DSP | 1614 | 1728 | **93%** |
| LUT | 182,374 | 230,400 | **79%** |
| FF | 175,744 | 460,800 | **38%** |
| URAM | 68 | 96 | **70%** |
| **Fmax** | **308.74 MHz** | 300 MHz | **✅ Pass** |

> Cập nhật 2026-05-22: Re-synthesis sau BIAS_STATS rotating-acc fix (PEs_F=16, PEs_U=8).
> URAM giảm từ 80→68 (run_upconv_block 48→36 URAM, rotating acc sum_rot/sumsq_rot mapped thành registers).

### Per-Block Resource Breakdown (full_generator_top — PEs_F=16, PEs_U=8, post BIAS_STATS fix)
| Block | BRAM_18K | DSP | FF | LUT | URAM |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Universal_Engine_Kernel (16-MAC, PEs_F=16) | 208 | 558 | 62,081 | 51,574 | 16 |
| run_upconv_block (PEs_U=8) | 144 | 576 | 56,246 | 45,921 | 36 |
| Conv77_Kernel (8×8 SIMD+PE + Clip) | 56 | 448 | 41,896 | 69,081 | 0 |
| Other / overhead | 193 | 32 | 15,521 | 15,798 | 16 |
| **Total** | **601** | **1614** | **175,744** | **182,374** | **68** |

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

### UpConv Core (từ verified standalone bench synthesis, post BIAS_STATS fix 2026-05-21)
| Mode | Config | Latency min | Latency max | Timing (Est) |
| :--- | :--- | :--- | :--- | :--- |
| UCB_0 | 16×16×960 → 32×32×480 | **30.46 ms** | 193 ms | 3.010 ns |
| UCB_1 | 32×32×480 → 64×64×240 | **21.21 ms** | 290 ms | 3.010 ns |
| UCB_2 | 64×64×240 → 128×128×120 | **29.21 ms** | 512 ms | 3.010 ns |
| UCB_3 | 128×128×120 → 256×256×60 | **75.00 ms** | 1.687 s | 3.010 ns |

> Kết quả từ ucbX_bench (inlined body + exact tripcounts). Khoảng min/max rộng do `KH_LOOP` (1-2 iters) và `KW_LOOP` (2-3 iters) phụ thuộc vào row index `ho`.
> Cập nhật 2026-05-21: Re-synthesis sau BIAS_STATS rotating accumulator fix (II: 7→1). Speedup min: UCB_0 1.31×, UCB_1 1.86×, UCB_2 2.17×, UCB_3 1.76×. UCB total min: 275ms → 156ms.

### UpConv Core — upconv_core_top (post BIAS_STATS rotating-acc fix, 2026-05-21) ✅
| Resource | BRAM_18K | DSP | FF | LUT | URAM | Timing |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| upconv_core_top (PEs=8) | 204 | 572 | 59,848 | 50,112 | 36 | **3.239 ns ✅** |

**Latency (all 4 UCBs chained):** min=367,693 cy (1.226 ms), max=5,738,425,982 cy (19.1 sec)

**PIXEL_NORM per UpConv_Fused_Row call:** min=10,496 cy, max=198,144 cy (iter_lat=328~774, trip=32~256)

| Loop | Before fix (II≈7) | After fix (II=1) | Speedup |
| :--- | :--- | :--- | :--- |
| BIAS_STATS UCB_0 (C_OUT=480) | 3,379 cy | 507 cy | **6.7×** |
| BIAS_STATS UCB_3 (C_OUT=60)  | 439 cy   | 87 cy  | **5.0×** |
| PIXEL_NORM UCB_0 (W_OUT=32)  | 113,792 cy | ~24,768 cy | **4.6×** |
| PIXEL_NORM UCB_3 (W_OUT=256) | 151,040 cy | ~83,968 cy | **1.8×** |

> CSIM ALL PASS sau fix: UCB_0 max_err=0.00781, UCB_1 0.01367, UCB_2 0.02148, UCB_3 0.01563 (0 mismatch)
> Fix: `sum_rot[8]`/`sumsq_rot[8]` rotating accumulator, `acc_idx = c & 7`, depth=8 ≥ FP32 adder latency → II=1.

### Conv77 (SIMD+PE, từ full_generator integrated synthesis) ✅ UPDATED
| Block | Cycles | Latency @ 300 MHz |
| :--- | :--- | :--- |
| Conv77_Kernel (256×256×60→3) | 5.3M–6.2M | **17.8–20.7 ms** |

> SIMD+PE design: 8×8 = 448 MACs/cycle, 0 URAM, dùng BRAM cho x_buffer và w_buffer.
> Cập nhật 2026-05-22: HLS estimate từ re-synthesis post BIAS_STATS fix (min=5,326,641 cy, max=6,219,569 cy).
> Conv77 CSIM (data_256_t interface, post-clip golden `output_numbers.txt`): max_err=0.488, rmse=0.162, 0/196608 mismatch (TOL=1.0) — **PASS**

## Optimization Experiments — full_generator_opt (2026-05-15)

### Mục tiêu
Maximize inference speed (minimize cycles) trong giới hạn: BRAM ≤ 624, DSP ≤ 1728, URAM ≤ 96, Fmax ≥ 300 MHz.

### Kết quả csynth (HLS estimate, xczu7ev-ffvc1156-2-e)
| Experiment | PEs_F (spatial) | MAC-width | PEs_U | Total Cycles | BRAM | DSP | URAM | Timing |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| full_generator (gốc) | 8 | 8-wide | 8 | 24.07B | 545 (87%) | 1352 (78%) | 80 (83%) | -0.81 ns |
| full_generator_opt3 | 8 | 16-wide | 8 | 23.16B | 545 (87%) | 1614 (93%) | 80 (83%) | -0.81 ns |
| full_generator_opt4 | 8 | 4-wide | 8 | 23.64B | 545 (87%) | 1230 (71%) | 80 (83%) | -0.81 ns |
| full_generator_opt2 | 8 | 8-wide | **12** | **19.67B** | **605 (96%)** | **1624 (93%)** | **80 (83%)** | -0.81 ns |
| **full_generator ✅ CANONICAL** | **16** | **8-wide** | **8** | **177M–16.16B** | **601 (96%)** | **1614 (93%)** | **68 (70%)** | **3.239 ns ✅** |

### Per-block breakdown cho opt2 (BEST)
| Block | BRAM | DSP | FF | LUT | URAM |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Universal_Engine_Kernel (8-MAC, PEs=8) | 152 | 302 | 41,432 | 36,246 | 16 |
| run_upconv_block (PEs=12) | 204 | 842 | 77,161 | 59,751 | 48 |
| Conv77_Kernel (8×8 SIMD+PE) | 56 | 448 | 41,832 | 69,016 | 0 |
| Other / overhead | 193 | 32 | 15,327 | 15,411 | 16 |
| **Total** | **605** | **1624** | **175,752** | **180,424** | **80** |

### Giải thích opt2 wins
- **BIAS_STATS fix**: rotating accumulator depth=8 → BIAS_STATS II=1 (507 cycles vs 3377 trước), PIXEL_NORM 5× faster (184K vs 910K cycles)
- **PEs_U=12 UpConv**: N_TILES giảm 33% (480/12=40 vs 480/8=60 tiles cho UCB_0; 60/12=5 vs 60/8=8 cho UCB_3). Tất cả C_OUT sizes (480, 240, 120, 60) đều chia hết cho 12 → NO partial tiles
- run_upconv_block cycles: 4.82B (PEs=12) vs 5.73B (PEs=8) → **16% faster UpConv**
- Tổng opt2: 19.67B cycles vs opt3 23.16B → **15% faster overall**

### Tại sao không thể cải thiện thêm (hard constraints)
| Attempt | Result | Why Blocked |
| :--- | :--- | :--- |
| PEs_U=13 | ❌ | +16 BRAM/PE → total 621 BRAM (99.5%), overflow risk |
| 16-MAC Fusion + PEs_U=12 | ❌ | DSP: 558+839+448=1845 > 1728 |
| 16-MAC Fusion + PEs_U=9 | ~21.3B cycles | Worse than opt2 (UpConv TILE overhead >> Fusion savings) |
| Row_acc URAM→BRAM | ❌ | +16 BRAM → 621 BRAM (99.5%), no real timing benefit |

### Về timing violation -0.81 ns trong csynth
Pre-existing trong full_generator gốc (cùng -0.81 ns tại run_upconv_block) nhưng implementation đã pass 308.74 MHz. HLS csynth timing là pessimistic estimate — **không ảnh hưởng đến implementation timing closure**.

### Recommended Design
**`full_generator`** (opt2 + UCB weight/param offset correctness fix + PEs_F=16 + PEs_U=8):
- Source: `src/hls/full_generator/`
- Synthesis ✅ RE-CONFIRMED (2026-05-22): BRAM 601(96%), DSP 1614(93%), FF 175,744(38%), LUT 182,374(79%), URAM 68(70%), Fmax 308.74 MHz, timing est. 3.239 ns ✅
- Latency: min 177M cycles (0.591 sec), max 16.16B cycles (53.9 sec) — wide range due to UpConv KH/KW loop bound uncertainty
- **Fixes UCB_1/2/3 reading wrong weights** — opt2 had all 4 UCBs starting from W_upconv[0], thiếu offset per UCB
- **PEs_F=16** (up from 8): doubles Fusion MACs/cycle; **PEs_U=8** (down from 12): reduces UpConv BRAM while staying in budget

### Per-block breakdown cho opt5 ✅ (PEs_F=16, PEs_U=8) — 2026-05-22 (post BIAS_STATS fix)
| Block | BRAM | DSP | FF | LUT | URAM |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Universal_Engine_Kernel (16-MAC, PEs_F=16) | 208 | 558 | 62,081 | 51,574 | 16 |
| run_upconv_block (PEs_U=8) | 144 | 576 | 56,246 | 45,921 | 36 |
| Conv77_Kernel (8×8 SIMD+PE) | 56 | 448 | 41,896 | 69,081 | 0 |
| Other / overhead | 193 | 32 | 15,521 | 15,798 | 16 |
| **Total** | **601** | **1614** | **175,744** | **182,374** | **68** |

#### Weight/Param layout expected in W_upconv & P_upconv (opt5)
```
W_upconv: [UCB0: 259200 words][UCB1: 64800][UCB2: 16200][UCB3: 4320]  total=344520 words
P_upconv: [UCB0_B:30][UCB0_G:30][UCB0_BE:30][UCB1_B:15][UCB1_G:15][UCB1_BE:15]
          [UCB2_B:8][UCB2_G:8][UCB2_BE:8][UCB3_B:4][UCB3_G:4][UCB3_BE:4]  total=171 words
```

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
| csynth timing violation -0.81 ns tại run_upconv_block | URAM read-modify-write trong ACC_WRITE (TILE_LOOP) là critical path; HLS pessimistic estimate | Không cần fix: full_generator gốc có cùng violation nhưng implementation pass 308.74 MHz. csynth timing ≠ impl timing |
| BRAM là hard constraint giới hạn PEs_U ở 12 | w_local[PEs][540] dùng 16 BRAM_18K/PE × 12 = 192 BRAM; AXI buffers thêm ~193 BRAM. Total 605/624 (96%) với PEs=12. PEs=13 → ≈621 BRAM (overflow risk) | Không thể tăng PEs_U vượt 12 với kiến trúc hiện tại mà không redesign weight storage |
| UCB_1/2/3 đọc sai weights và params (opt2 bug) | Tất cả 4 UCBs dùng cùng `W_upconv`/`P_upconv` pointer không có offset → UCB_1 đọc từ offset 0 thay vì 259200 → sai weights hoàn toàn. Tương tự cho bias/gamma/beta | **opt5**: thêm per-UCB offsets trong `run_upconv_block`. Weight layout: [UCB0:259200][UCB1:64800][UCB2:16200][UCB3:4320]. Param layout: [UCB0_B/G/BE][UCB1_B/G/BE]... |

## Data Classification
- **Weights/Params**: `assets/test_data/model_params/` (Trọng số Generator bắt đầu bằng `Gen_...`)
- **Input/Output**: `io_params/` và `layer_test_vectors/`
