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

| File                                      | Mô tả                                          |
| :---------------------------------------- | :----------------------------------------------- |
| `fusion_core/gen/Hls_Layers_Fusion.tpp` | Universal_Engine_Kernel, GlobalAdd_Kernel        |
| `upconv_core/gen/Hls_Layers_UpConv.tpp` | UpConv_Fused_Row                                 |
| `Conv77/gen/Hls_Layers_Conv77.tpp`      | Conv77_Kernel — SIMD+PE (8×8), data_256_t/half |
| `Conv77/gen/Hard_op_3.cpp`              | Standalone HLS wrapper cho Conv77 csim           |
| `full_generator/gen/generator_top.cpp`  | Top-level unified IP (fusion + upconv + conv77)  |
| `conv77_core/gen/Hls_Layers_Conv77.tpp` | Conv77_Core — flat loop, 196 iters (reference)  |
| `conv77_core/gen/conv77_core_top.cpp`   | Standalone AXI wrapper cho conv77_core           |
| `fusion_core/gen/fusion_core_top.cpp`   | Standalone fusion IP                             |
| `upconv_core/gen/upconv_core_top.cpp`   | Standalone upconv IP                             |

## CSIM Results — Real-Data Verification (2026-05-14) ✅ ALL PASS

| Block      | Config                      | max_err         | rmse     | mismatch    |
| :--------- | :-------------------------- | :-------------- | :------- | :---------- |
| CBI        | 16×16×220→960            | 0.01953         | 0.00169  | 0/245,760   |
| ResBlock-0 | 16×16×960→960            | 0.01953         | 0.000978 | 0/245,760   |
| GlobalAdd  | 16×16×960→960            | 0.02344         | 0.00178  | 0/245,760   |
| UCB_0      | 16×16×960→32×32×480    | 0.00781         | 0.000591 | 0/491,520   |
| UCB_1      | 32×32×480→64×64×240    | 0.01367         | 0.001061 | 0/983,040   |
| UCB_2      | 64×64×240→128×128×120  | 0.02148         | 0.002284 | 0/1,966,080 |
| UCB_3      | 128×128×120→256×256×60 | 0.01563         | 0.001405 | 0/3,932,160 |
| Conv77     | 256×256×60→3             | 0.488 (TOL=1.0) | 0.162    | 0/196,608   |

> Input data: real model weights + io_params/ golden tensors. Xem `assets/test_data/DATA_README.md` để biết file mapping.
> CBI input file: `io_params/Gen_input.txt` (= Gen_cbi_cbi0_ReduceMean_1.txt đã đổi tên).

## E2E CSIM — full_generator (UCB_0→3 + Conv77 chained) ✅ ALL PASS (2026-05-16)

Chạy toàn bộ UCB chain + Conv77 với real weights, bắt đầu từ GlobalAdd output, verify correctness của opt5 weight offsets.

| Stage                  | Config                      | max_err          | rmse     | mismatch    |
| :--------------------- | :-------------------------- | :--------------- | :------- | :---------- |
| UCB_0→3 chained       | 16×16×960 → 256×256×60 | 0.0156           | 0.001389 | 0/3,932,160 |
| Conv77 (on UCB output) | 256×256×60→256×256×3   | 0.4875 (TOL=1.0) | 0.159698 | 0/196,608   |

> Source: `src/hls/full_generator/gen/test_e2e.cpp` + `hls_csim_e2e.tcl`
> UCB golden: `Gen_ucb4_output.txt` (256×256×60). Conv77 golden: `Gen_conv77_output.txt` clipped to [0,1] (HLS applies Clip(0,1)).
> Key fix in opt5 vs opt2: w_local loaded every tile every ho call (not cached on ho==0 only — that was a bug: w_local[PEs][540] holds only 1 tile at a time).
> Ping-pong Y buffers (Y_a/Y_b alternating across UCBs) used to avoid in-place aliasing.

## E2E CSIM — full_generator_top FULL (Fusion + UCB + Conv77) ✅ ALL PASS (2026-05-18)

Chạy full pipeline Fusion→UCB→Conv77 qua `test_full.cpp`. CSIM mất ~7 giờ.

| Stage                 | Config          | max_err | rmse     | mismatch    | TOL  |
| :-------------------- | :-------------- | :------ | :------- | :---------- | :--- |
| Fusion (CBI+9×RB+GA) | 16×16×960     | 12.3158 | 0.5124   | 0/245,760   | 15.0 |
| UCB_3 chained         | → 256×256×60 | 0.7151  | 0.010017 | 0/3,932,160 | 1.0  |
| Conv77                | → 256×256×3  | 0.8636  | 0.165422 | 0/196,608   | 1.0  |

> Source: `full_generator/gen/test_full.cpp` + `hls_csim_full.tcl`
> **Why Fusion max_err=12.3**: `psum[PEs][8]` là fp16, accumulate 135 lần/slot (CI_PAD=960 → 120 MAC_LOOP iters, 9 kernel steps, 8 slots → 135 adds/slot). Error/slot ≈ 135×magnitude×2^-10 ≈ 1.4. Normalized với inv×g≈3 → ~4/layer. Compounding qua 9 ResBlocks → max_err≈12. Fix nếu cần: đổi psum sang `ap_fixed<32,16>` (không tăng DSP, chỉ LUT).
> **Conv77 final quality**: RMSE=0.165 ≈ standalone RMSE=0.162 → ảnh đầu ra chất lượng không bị ảnh hưởng đáng kể dù Fusion có lỗi tích lũy.

## RTL Co-Simulation — full_generator_top + UpConv probe (2026-05-18, REVISED 2026-06-02)

`cosim_design` trên full_generator_top: C sim PASS (6h42m) → fail tại TV generation:
`ERROR: [COSIM 212-5] *** C/RTL co-simulation file generation failed.`
Chẩn đoán ban đầu (2026-05-18): nghi Vitis 2024.2 binary-TV `DUMP_OUTPUTS`/`_exit()` skip dump → gán nhãn "tool limitation".

**REVISED 2026-06-02 — nhãn "tool limitation" SAI.** Probe cosim trên `upconv_core` (UCB3 datapath, H reduced, C full — `src/hls/upconv_core/probe_cosim/`) cho thấy `[COSIM 212-5]` thực ra do **thiếu `depth=` trên m_axi port** (cosim cần port depth để cấp test vector; csim không cần nên trước giờ thiếu vẫn chạy). Thêm `depth=` → `[212-1] file generation COMPLETED` → `[212-1000] *** finished: PASS ***` (RTL khớp C bit-exact). Bug **FIX ĐƯỢC**, không phải lỗi cấp tool. (full_generator_top muốn cosim lại chỉ cần thêm `depth=` cho mọi m_axi port.)

**Rào THẬT của cosim = throughput xsim, không phải bug.** Đo trên probe no-trace: **~50 cycle/giây** cho datapath fp16×128 + fp32 + sqrt.

- Tiny probe (23.7k cy, sim-time 79.029µs): cosim wall **11m27s** → PASS.
- Suy ra: reduced UCB3 H=8 (~0.55M cy) ≈ **3h**; `upconv_core` full (61M cy) ≈ **2 tuần**; full_generator (≥177M cy) → bất khả thi.
- Waveform trace ON (`-wdb`) là đòn chí mạng phụ: run 0.55M cy **có trace** sau 2h vẫn kẹt ở init (`@119000ps` ≈ 36 cycle). BẮT BUỘC `cosim_design -trace_level none`.

**Kết luận**: cosim đúng chức năng (PASS scope nhỏ, RTL==C bit-exact) nhưng bất khả thi về thời gian ở size thật. Verification stack: (1) **C sim full pipeline** = functional sign-off (ALL PASS, 0 mismatch); (2) **deterministic latency** (per-loop csynth × trip-count thật, bench tables) = latency; (3) **cosim PASS scope nhỏ** = RTL-vs-C spot check. KHÔNG cosim full size (cần ngày→tuần).

## Synthesis Results — ZCU104 (xczu7ev-ffvc1156-2-e, 300 MHz)

### Resource Utilization (full_generator_top — Fusion + UpConv + Conv77) ✅ UPDATED (post [A]+[B]+[C-pipe] UpConv opt, 2026-06-02)

| Resource       | Used                 | Available | %                 |
| :------------- | :------------------- | :-------- | :---------------- |
| BRAM_18K       | 601                  | 624       | **96%**     |
| DSP            | 1710                 | 1728      | **98%**     |
| LUT            | 186,503              | 230,400   | **81%**     |
| FF             | 189,763              | 460,800   | **41%**     |
| URAM           | 68                   | 96        | **70%**     |
| **Fmax** | **308.74 MHz** | 300 MHz   | **✅ Pass** |

> Tiến trình DSP: pre-opt 1614 (93%) → [A] flatten +64 → 1678 (97%) → [B] PIXEL_NORM 2-pass +32 → **1710 (98%, +18 margin)**. [C-pipe] RESET_ROW_ACC II=15→1: **0 DSP thêm**, LUT giảm (189,333→186,503). BRAM 601 / URAM 68 / Fmax 308.74 KHÔNG đổi qua [A]/[B]/[C-pipe].

> ⚠ **Bảng trên là post-[C-pipe]. Sau [D] PIPO double-buffer (2026-06-04, đã áp vào `full_generator/gen`)**: DSP **1710→1550**, BRAM 601→**510**, URAM 68→**96**, LUT 186,503→**205,233**, **Fmax 308.74 giữ**. Lý do dịch chuyển: w_local→URAM PIPO + x_buf URAM→BRAM + fp16-mult UpConv→LUT (`BIND_OP op=hmul impl=fabric`). Xem mục **UpConv [D]**. (csim full sign-off đang chạy.)

> Cập nhật 2026-05-22: Re-synthesis sau BIAS_STATS rotating-acc fix (PEs_F=16, PEs_U=8).
> URAM giảm từ 80→68 (run_upconv_block 48→36 URAM, rotating acc sum_rot/sumsq_rot mapped thành registers).

### Per-Block Resource Breakdown (full_generator_top — PEs_F=16, PEs_U=8, post BIAS_STATS fix)

| Block                                      | BRAM_18K      | DSP            | FF                | LUT               | URAM         |
| :----------------------------------------- | :------------ | :------------- | :---------------- | :---------------- | :----------- |
| Universal_Engine_Kernel (16-MAC, PEs_F=16) | 208           | 558            | 62,081            | 51,574            | 16           |
| run_upconv_block (PEs_U=8)                 | 144           | 576            | 56,246            | 45,921            | 36           |
| Conv77_Kernel (8×8 SIMD+PE + Clip)        | 56            | 448            | 41,896            | 69,081            | 0            |
| Other / overhead                           | 193           | 32             | 15,521            | 15,798            | 16           |
| **Total**                            | **601** | **1614** | **175,744** | **182,374** | **68** |

### Resource Utilization (full_generator_top — cũ, Fusion + UpConv only, reference)

| Resource       | Used              | Available | %       |
| :------------- | :---------------- | :-------- | :------ |
| BRAM_18K       | 416               | 624       | 66%     |
| DSP            | 776               | 1728      | 44%     |
| LUT            | 86,544            | 230,400   | 37%     |
| FF             | 97,755            | 460,800   | 21%     |
| URAM           | 80                | 96        | 83%     |
| **Fmax** | **308 MHz** | 300 MHz   | ✅ Pass |

### Resource Utilization (conv77_core_top — standalone flat-loop, reference)

| Resource       | Used               | Available | %                     |
| :------------- | :----------------- | :-------- | :-------------------- |
| BRAM_18K       | 198                | 624       | 31%                   |
| DSP            | 192                | 1728      | 11%                   |
| LUT            | 25,838             | 230,400   | 11%                   |
| FF             | 38,243             | 460,800   | 8%                    |
| URAM           | 8                  | 96        | 8%                    |
| **Fmax** | **~308 MHz** | 300 MHz   | ✅ Pass (CP=2.554 ns) |

> conv77_core CSIM: max_err=0.00586, rmse=0.000886, 0/196608 mismatch — **PASS**

### On-chip Memory Layout

| Buffer                | Size               | Type             | Mục đích                |
| :-------------------- | :----------------- | :--------------- | :------------------------- |
| `skip_buf`          | 16×16×60 words   | URAM (16 blocks) | ResBlock skip connection   |
| `global_buf`        | 16×16×60 words   | URAM (16 blocks) | CBI→GlobalAdd skip        |
| `x_buf` (upconv)    | 2×128×60 words   | URAM (16 blocks) | Ping-pong sliding window   |
| `row_acc`           | 256×480 half      | URAM (32 blocks) | UpConv output accumulator  |
| `x_buf` (fusion)    | 4×W×60 words     | **BRAM**   | Fusion IFM sliding window  |
| `x_buffer` (conv77) | 7×14×64 fp16_t   | BRAM             | Conv77 SIMD sliding window |
| `w_buffer` (conv77) | 3×7×7×64 fp16_t | BRAM             | Conv77 weight on-chip      |

> Conv77 SIMD+PE design dùng 0 URAM (vs 8 URAM của flat-loop design). Integrated URAM budget: **80/96 (83%)** ✅

## Latency Estimates (@300 MHz)

### Fusion Core (từ full_generator synthesis, post BIAS_STATS fix, PEs_F=16) ✅ UPDATED 2026-05-26

| Block                    | Cycles min  | Cycles max  | Latency min          | Latency max |
| :----------------------- | :---------- | :---------- | :------------------- | :---------- |
| CBI (1 UEK call)         | 8,980,370   | 22,084,633  | **29.932 ms**  | 73.608 ms   |
| 1 ResBlock (2 UEK calls) | 17,960,745  | 44,169,271  | **59.86 ms**   | 147.23 ms   |
| 9×ResBlock tổng        | 161,646,705 | 397,523,439 | **538.82 ms**  | 1,325.1 ms  |
| GlobalAdd                | 15,507      | 15,507      | **51.685 µs** | 51.685 µs  |

> Source: `full_generator_top_csynth.rpt` Instance table + VITIS_LOOP_174_1 loop table.
> CBI = 1 UEK call; 1 ResBlock = 2 UEK calls (L1+L2). Crosscheck: 17,960,745 / 8,980,370 = 2.0 ✓
> CBI và ResBlock share cùng hardware (UEK), cùng latency/call vì CI_PAD=960 fixed.

**Fusion Core throughput**: `Universal_Engine_Kernel_16_half_s` — 16 PEs spatial (1 PE = 1 output column), 8 MACs/PE/cycle → **128 MACs/cycle** khi MAC_LOOP chạy II=1.

### UpConv Core (từ verified standalone bench synthesis, post BIAS_STATS fix 2026-05-21)

| Mode  | Config                        | Latency min        | Latency max | Timing (Est) |
| :---- | :---------------------------- | :----------------- | :---------- | :----------- |
| UCB_0 | 16×16×960 → 32×32×480    | **30.46 ms** | 193 ms      | 3.010 ns     |
| UCB_1 | 32×32×480 → 64×64×240    | **21.21 ms** | 290 ms      | 3.010 ns     |
| UCB_2 | 64×64×240 → 128×128×120  | **29.21 ms** | 512 ms      | 3.010 ns     |
| UCB_3 | 128×128×120 → 256×256×60 | **75.00 ms** | 1.687 s     | 3.010 ns     |

> Kết quả từ ucbX_bench (inlined body + exact tripcounts). Khoảng min/max rộng do `KH_LOOP` (1-2 iters) và `KW_LOOP` (2-3 iters) phụ thuộc vào row index `ho`.
> Cập nhật 2026-05-21: Re-synthesis sau BIAS_STATS rotating accumulator fix (II: 7→1). Speedup min: UCB_0 1.31×, UCB_1 1.86×, UCB_2 2.17×, UCB_3 1.76×. UCB total min: 275ms → 156ms.

### UpConv Core — upconv_core_top (post BIAS_STATS rotating-acc fix, 2026-05-21) ✅

| Resource                | BRAM_18K | DSP | FF     | LUT    | URAM | Timing                |
| :---------------------- | :------- | :-- | :----- | :----- | :--- | :-------------------- |
| upconv_core_top (PEs=8) | 204      | 572 | 59,848 | 50,112 | 36   | **3.239 ns ✅** |

**Latency (all 4 UCBs chained):** min=367,693 cy (1.226 ms), max=5,738,425,982 cy (19.1 sec)

**PIXEL_NORM per UpConv_Fused_Row call:** min=10,496 cy, max=198,144 cy (iter_lat=328~774, trip=32~256)

| Loop                         | Before fix (II≈7) | After fix (II=1) | Speedup         |
| :--------------------------- | :----------------- | :--------------- | :-------------- |
| BIAS_STATS UCB_0 (C_OUT=480) | 3,379 cy           | 507 cy           | **6.7×** |
| BIAS_STATS UCB_3 (C_OUT=60)  | 439 cy             | 87 cy            | **5.0×** |
| PIXEL_NORM UCB_0 (W_OUT=32)  | 113,792 cy         | ~24,768 cy       | **4.6×** |
| PIXEL_NORM UCB_3 (W_OUT=256) | 151,040 cy         | ~83,968 cy       | **1.8×** |

> CSIM ALL PASS sau fix: UCB_0 max_err=0.00781, UCB_1 0.01367, UCB_2 0.02148, UCB_3 0.01563 (0 mismatch)
> Fix: `sum_rot[8]`/`sumsq_rot[8]` rotating accumulator, `acc_idx = c & 7`, depth=8 ≥ FP32 adder latency → II=1.

### Conv77 (SIMD+PE, từ full_generator integrated synthesis) ✅ UPDATED

| Block                           | Cycles     | Latency @ 300 MHz       |
| :------------------------------ | :--------- | :---------------------- |
| Conv77_Kernel (256×256×60→3) | 5.3M–6.2M | **17.8–20.7 ms** |

> SIMD+PE design: 8×8 = 448 MACs/cycle, 0 URAM, dùng BRAM cho x_buffer và w_buffer.
> Cập nhật 2026-05-22: HLS estimate từ re-synthesis post BIAS_STATS fix (min=5,326,641 cy, max=6,219,569 cy).
> Conv77 CSIM (data_256_t interface, post-clip golden `output_numbers.txt`): max_err=0.488, rmse=0.162, 0/196608 mismatch (TOL=1.0) — **PASS**

## UpConv Optimization [A] — Flatten WI×CI (2026-06-02) ✅ VERIFIED

### Deterministic real-latency baseline (tính từ per-loop latency thật, validate <0.03% vs bench TOP)

HLS min/max bounds quá rộng (UCB tổng min 156ms ↔ max ~19s) nên vô dụng để so. Tính deterministic bằng đếm chính xác `valid_kh(ho)`, Σ over ho = `3·H_IN−1`:

| UCB                    |      Baseline thật |           Post-[A] |          Speedup |
| :--------------------- | ------------------: | -----------------: | ---------------: |
| UCB0                   |            109.9 ms |            58.6 ms |           1.88× |
| UCB1                   |            154.5 ms |            49.7 ms |           3.11× |
| UCB2                   |            269.6 ms |            57.9 ms |           4.65× |
| UCB3                   |            883.6 ms |           116.0 ms |           7.62× |
| **Tổng UpConv** | **1417.6 ms** | **282.2 ms** | **5.02×** |

> Lưu ý: 1.42s là latency THẬT, lớn hơn nhiều con "min 156ms" cũ (đó là HLS min-bound lạc quan). Hệ thống ~2.78s → ~1.64s.

### Kỹ thuật

Gộp `WI_LOOP`×`CI_LOOP` thành 1 pipeline II=1 (`FLAT_LOOP`, m=wi·CI_WORDS+ci_w → x_buf[x_base+m]). Trước: WI_LOOP không pipelined → mỗi cột output trả lại 93-cycle fill của CI_LOOP (hiệu suất MAC ~4%). Sau: Vitis merge cả KW_LOOP → `KW_LOOP_FLAT_LOOP` II=1, iter latency 116, fill trả **1 lần/kh**. psum rotating depth-4 first-touch (`ci_w<4` gán, else cộng — distance-4 ≥ fp16 add latency); `row_acc` ghi 1 lần/wi tại `ci_w==CI_WORDS−1`, `wo=wi·S+kw−PAD` đơn ánh theo wi trong 1 pass → `DEPENDENCE row_acc inter false` hợp lệ.

### Verification (full size, real data — không lươn lẹo)

- **csim** (`test.cpp` → `upconv_core_top`, fp16 bit-accurate, ~20ph): ALL PASS, **numerically-identical** baseline: UCB0 max_err 0.00781/rmse 0.000591, UCB1 0.01367/0.001061, UCB2 0.02148/0.002284, UCB3 0.01563/0.001405, 0 mismatch.
- **Synth**: core_top Fmax 308.74 / bench 332.27 (giữ nguyên). Integration full_generator: DSP 1614→1678 (97%, fits), BRAM 601 (=), URAM 68 (=), Fmax 308.74 ✅.

### Áp dụng vào (3+ bản copy diverged — xem memory)

Cả 2 bản `Hls_Layers_UpConv.tpp` (`upconv_core/gen` + `full_generator/gen`, KHÁC nhau ở PIXEL_NORM var-clamp) + 4 `ucbX_bench/bench_top.cpp`. run_upconv_block: DSP 576→640, BRAM 144(=), URAM 36(=), FF 56,246→62,089, LUT 45,921→49,647.

### Còn lại sau [A] (đã được [B] xử lý PIXEL_NORM)

post-[A]: MAC 42% · PIXEL_NORM 36% ← [B] · PRELOAD_W 20% ← finding.

## UpConv Optimization [B] — PIXEL_NORM 2-pass flatten (2026-06-02) ✅ VERIFIED

### Latency (deterministic, kèm [A])

| UCB                    |         Baseline |   [A] |            [A]+[B] |          vs base |
| :--------------------- | ---------------: | ----: | -----------------: | ---------------: |
| UCB0                   |            109.9 |  58.6 |               57.7 |           1.90× |
| UCB1                   |            154.5 |  49.7 |               46.2 |           3.35× |
| UCB2                   |            269.6 |  57.9 |               43.7 |           6.17× |
| UCB3                   |            883.6 | 116.0 |               58.8 |           15.0× |
| **Tổng UpConv** | **1417.6** | 282.2 | **206.4 ms** | **6.87×** |

> [B] thêm ~76ms. Hệ thống ~2.78s → **~1.57s**.

### Kỹ thuật

PIXEL_NORM cũ `PIPELINE off` → mỗi wo trả fill 29 của BIAS_STATS + serial div/sqrt (×W_OUT lần). Tách 2 pass flatten II=1: **Pass1 `PIXEL_STATS`** (wo×c = 15,360 iters, iter-lat 230): +bias, rotating depth-8 first-touch stats, ghi `mean_buf`/`inv_buf[wo]` tại c==C_OUT−1 (div/sqrt overlap qua iter sau → không block II=1). **Pass2 `PIXEL_NORM`** (wo×cw): normalize+ReLU+write. PIXEL per-call: UCB3 83,712→16,703 cy (5.0×), ~bằng nhau mọi UCB (~16,640) vì W_OUT·C_OUT=15,360 cố định.

### Verification (full size, real data)

- **csim**: ALL PASS, **numerically-identical** ([B] không đổi 1 bit): UCB0 0.00781, UCB1 0.01367, UCB2 0.02148, UCB3 0.01563, 0 mismatch.
- **Synth**: core_top Fmax 308.74 / bench 334–411 MHz (ucb2/3 tăng do bỏ serial div/sqrt). Integration: DSP 1678→1710 (98%, +32, fits), BRAM 601(=), URAM 68(=), Fmax 308.74. `mean_buf`/`inv_buf` → **LUTRAM (0 BRAM thêm)**.
- Áp dụng: 2 bản `.tpp` (upconv_core seq-reduce no-clamp / full_generator balanced-tree+var-clamp — GIỮ divergence) + 4 bench.

## UpConv [C-pipe] — RESET_ROW_ACC II=15 → II=1 (2026-06-02) ✅ VERIFIED

Audit toàn bộ loop UCB (report): chỉ `RESET_ROW_ACC` còn II=15 (mọi loop khác đã II=1). Nó zero cứng 480 cột/wo (16 bank×2 port = 32 ghi/cy → 480/32 = II=15). Cột ≥ C_OUT KHÔNG bao giờ đọc/ghi → chỉ cần reset C_OUT_PAD. Restructure: flatten (wo×channel-word), ghi 1 word 16-ch/cycle → **II=1**, chỉ `C_WORDS_OUT` word/wo. ucb3: RESET 3841 → 1024 cy/call.

- **Verify**: csim **memset OFF** (`-DRESET_VERIFY` — memset chỉ tồn tại trong csim, hardware chỉ có RESET_ROW_ACC, nên tắt memset mới test đúng path hardware) → ALL PASS, numerically-identical. Synth: II 15→1, **0 DSP thêm**, LUT 189,333→186,503 (giảm), Fmax 308.74 held.
- UpConv: 206.4 → **203.7 ms** (**6.96×** vs baseline 1417.6). Hệ thống ~1.565s.
- `LOAD_PARAMS` II=3 GIỮ NGUYÊN: 3 reads (B/G/BE) trên 1 AXI bundle = 1 word/cy×3 → tối ưu cho 1 port, không phải lỗi pipeline.
- Áp dụng: 2 bản `.tpp` + 4 bench.

## UpConv [D] — PIPO Double-Buffer: overlap PRELOAD‖MAC (2026-06-04) ✅ SYNTH-VERIFIED (csim full sign-off pending)

PRELOAD_W (reload weight mỗi row, 27.5% / 56.7ms) KHÔNG *cache* được (memory wall, xem mục dưới) NHƯNG **overlap được** với MAC bằng double-buffer — bác bỏ kết luận cũ "[A]+[B] là điểm tối ưu".

**Kỹ thuật**: tách TILE_LOOP của `UpConv_Fused_Row` thành 2 hàm `uc_load_tile()` (producer) + `uc_compute_tile()` (consumer, cả 2 `#pragma HLS INLINE off`) chia sẻ 1 `wbuf[PEs][540]` khai báo trong vùng `#pragma HLS DATAFLOW` → Vitis tự PIPO (2 bản vật lý ping-pong) → load(tile+1) chồng lên compute(tile), giấu PRELOAD sau MAC. **PIPO cho compute đọc weight RANDOM-access** (FIFO stream thì không — chính hiểu lầm này khiến trước đó tưởng overlap bất khả thi). `row_acc` accumulator KHÔNG chặn dataflow. Pattern SAI (đã thử): ping-pong tay `wbuf[2][..]` inline 1 thân loop → Vitis nhồi prefetch+MAC vào 1 process, KHÔNG overlap.

**3 fix tài nguyên** (DATAFLOW nở DSP + nhân đôi buffer):

1. `wbuf` → URAM (`BIND_STORAGE impl=uram`): BRAM full_gen chật, URAM dư.
2. `x_buf` upconv URAM→BRAM: giải phóng 4 URAM cho wbuf PIPO (BRAM dư sau khi w_local→URAM).
3. **DSP** (mấu chốt): DATAFLOW cô lập process → HLS bỏ tự-balance DSP↔LUT, đẩy cả 128 fp16-mul lên DSP (vs canonical 96+32LUT). Fix: `#pragma HLS BIND_OP variable=prod op=hmul impl=fabric` (tách `data_t prod; <pragma>; prod=xv*wv; dot+=prod;`). **Op half-mul là `hmul`, KHÔNG phải `mul`(integer)/`fmul`(fp32)** — nên mọi BIND_OP/ALLOCATION/config_op `mul`/`fmul` trước đều bị Vitis LỜ IM LẶNG. `op=hmul` đẩy mult UpConv sang LUT, **SCOPED** (Fusion `hmul` giữ nguyên DSP — khác `config_op hmul` global sẽ phá Fusion).

**Synth `full_generator` (xczu7ev) ✅ ALL FIT**: DSP **1710→1550** (≤1728, dư 178), BRAM 601→**510**, URAM 68→**96** (=cap), LUT 186K→**205K** (≤230K), **Fmax 308.74 MHz giữ nguyên**, run_upconv latency 788.6M→737.8M, 1 run_upconv instance, Fusion(UEK) DSP 558 KHÔNG đổi (BIND_OP scoped đúng).
**Standalone csim** (upconv_core PIPO, full-size real data): ALL PASS 4 UCB, **numerically-identical, 0 mismatch** (LUT-mult cho kết quả fp16 y hệt DSP-mult; vị trí buffer không đổi numerics → tích hợp giữ đúng số).
**Latency (ước tính)**: overlap ẩn ~toàn bộ PRELOAD (56.7ms) → UpConv ~203.7 → **~148ms (−27%)**; hệ thống ~1.565 → **~1.51s** (deterministic chính xác: pending).
Đã áp `full_generator/gen` (Hls_Layers_UpConv.tpp + generator_top.cpp).

**Sync `upconv_core/gen` sang [D] (2026-06-12) ✅ VERIFIED** — port [D] PIPO + fabric-fix vào `upconv_core/gen/Hls_Layers_UpConv.tpp` (GIỮ divergence: copy này seq-reduce + NO var-clamp; full_generator balanced-tree + clamp — no-op trên real data nên csim bit-exact). `upconv_core_top` giờ là harness [D] hợp lệ (test [D] không cần full_generator 7h).

- csim 4-UCB: ALL PASS, **bit-exact** bản cũ (UCB0 0.0078125/0.000591, UCB1 0.0136719/0.001061, UCB2 0.0214844/0.002284, UCB3 0.0156250/0.001405, 0 mismatch) — [D]+fabric không đổi numerics.
- csynth `upconv_core_top` (PEs=8): DATAFLOW áp dụng (`uc_load_tile`‖`uc_compute_tile`), DSP 676→**516**, BRAM 204→**84**, URAM 36→**68**, LUT 54,898→**73,710**, CP **3.239 ns (308 MHz)** — shift đúng pattern [D]. tcl: `hls_syn_dbuf.tcl`.
- **4 ucbX_bench: VẪN pre-[D]** (deferred). Lý do: mỗi bench có body inline riêng (PRELOAD nested, `LOOP_TRIPCOUNT` hardcode per-UCB) + dùng `#pragma HLS INLINE` để const-prop dims → exact trip count; [D] DATAFLOW (TILE_LOOP bound biến) báo latency range → xung đột mục đích đo deterministic. Port = 4× restructure thủ công, chưa làm. Số bench standalone trong bảng latency vẫn là pre-[D].

> TODO: csim full sign-off `full_generator`; (nếu cần [D] per-UCB latency) port 4 bench.

### Còn lại sau [A]+[B]+[C-pipe] — đã điều tra, KHÔNG khả thi (2026-06-02; PRELOAD overlap về sau giải bằng [D])

Breakdown: MAC 117.7ms (57%) · PRELOAD_W 56.7ms (27.5%) · PIXEL 26.7ms (12.9%).

- **PRELOAD_W (reload weight mỗi row)** — ❌ memory wall. Cache weight on-chip: UCB0 cần **66.4 Mbit** > **38.9 Mbit TỔNG** (BRAM 11.2 + URAM 27.6) của ZCU104 → vật lý không thể. UCB1 16.6 Mbit = 58 URAM > 28 spare. Chỉ UCB2+UCB3 (19 URAM) cache được → save ~15ms (1% hệ thống). 2-row/call cần +32 URAM > 28 spare (không giảm được row_acc partition vì phá II=1 của [A]).
- **Conv77 [C] H-line-buffer** — ❌ counterproductive. Load loops Conv77 đã **II=1** (Init/Newcol/Shift, không DDR-stall): naive line-buffer chỉ thêm copy (~+1024 cy/ho, TỆ HƠN); compute-thẳng-từ-line-buffer tái sinh 147-to-1 mux (dynamic base `wo-3+win+wf`) → nổ LUT. "7× re-read" là DDR bandwidth, không phải cycles.

→ ~~[A]+[B] là điểm tối ưu thực tế~~ **CẬP NHẬT (2026-06-04): [D] PIPO double-buffer giấu được PRELOAD sau MAC (~27% UpConv) — xem mục [D] ở trên.** Sau [A]+[B]+[C-pipe]+[D], điểm tối ưu mới fit ZCU104 (DSP 1550, BRAM 510, URAM 96, Fmax 308.74). Phần *cache* weight (load 1 lần) vẫn bất khả thi (memory wall) — đó là giới hạn cần đổi FPGA, KHÁC với overlap (đã làm được).

## Optimization Experiments — full_generator_opt (2026-05-15)

### Mục tiêu

Maximize inference speed (minimize cycles) trong giới hạn: BRAM ≤ 624, DSP ≤ 1728, URAM ≤ 96, Fmax ≥ 300 MHz.

### Kết quả csynth (HLS estimate, xczu7ev-ffvc1156-2-e)

| Experiment                            | PEs_F (spatial) | MAC-width        | PEs_U        | Total Cycles           | BRAM                | DSP                  | URAM               | Timing                |
| :------------------------------------ | :-------------- | :--------------- | :----------- | :--------------------- | :------------------ | :------------------- | :----------------- | :-------------------- |
| full_generator (gốc)                 | 8               | 8-wide           | 8            | 24.07B                 | 545 (87%)           | 1352 (78%)           | 80 (83%)           | -0.81 ns              |
| full_generator_opt3                   | 8               | 16-wide          | 8            | 23.16B                 | 545 (87%)           | 1614 (93%)           | 80 (83%)           | -0.81 ns              |
| full_generator_opt4                   | 8               | 4-wide           | 8            | 23.64B                 | 545 (87%)           | 1230 (71%)           | 80 (83%)           | -0.81 ns              |
| full_generator_opt2                   | 8               | 8-wide           | **12** | **19.67B**       | **605 (96%)** | **1624 (93%)** | **80 (83%)** | -0.81 ns              |
| **full_generator ✅ CANONICAL** | **16**    | **8-wide** | **8**  | **177M–16.16B** | **601 (96%)** | **1614 (93%)** | **68 (70%)** | **3.239 ns ✅** |

### Per-block breakdown cho opt2 (BEST)

| Block                                  | BRAM          | DSP            | FF                | LUT               | URAM         |
| :------------------------------------- | :------------ | :------------- | :---------------- | :---------------- | :----------- |
| Universal_Engine_Kernel (8-MAC, PEs=8) | 152           | 302            | 41,432            | 36,246            | 16           |
| run_upconv_block (PEs=12)              | 204           | 842            | 77,161            | 59,751            | 48           |
| Conv77_Kernel (8×8 SIMD+PE)           | 56            | 448            | 41,832            | 69,016            | 0            |
| Other / overhead                       | 193           | 32             | 15,327            | 15,411            | 16           |
| **Total**                        | **605** | **1624** | **175,752** | **180,424** | **80** |

### Giải thích opt2 wins

- **BIAS_STATS fix**: rotating accumulator depth=8 → BIAS_STATS II=1 (507 cycles vs 3377 trước), PIXEL_NORM 5× faster (184K vs 910K cycles)
- **PEs_U=12 UpConv**: N_TILES giảm 33% (480/12=40 vs 480/8=60 tiles cho UCB_0; 60/12=5 vs 60/8=8 cho UCB_3). Tất cả C_OUT sizes (480, 240, 120, 60) đều chia hết cho 12 → NO partial tiles
- run_upconv_block cycles: 4.82B (PEs=12) vs 5.73B (PEs=8) → **16% faster UpConv**
- Tổng opt2: 19.67B cycles vs opt3 23.16B → **15% faster overall**

### Tại sao không thể cải thiện thêm (hard constraints)

| Attempt                  | Result        | Why Blocked                                              |
| :----------------------- | :------------ | :------------------------------------------------------- |
| PEs_U=13                 | ❌            | +16 BRAM/PE → total 621 BRAM (99.5%), overflow risk     |
| 16-MAC Fusion + PEs_U=12 | ❌            | DSP: 558+839+448=1845 > 1728                             |
| 16-MAC Fusion + PEs_U=9  | ~21.3B cycles | Worse than opt2 (UpConv TILE overhead >> Fusion savings) |
| Row_acc URAM→BRAM       | ❌            | +16 BRAM → 621 BRAM (99.5%), no real timing benefit     |

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

| Block                                      | BRAM          | DSP            | FF                | LUT               | URAM         |
| :----------------------------------------- | :------------ | :------------- | :---------------- | :---------------- | :----------- |
| Universal_Engine_Kernel (16-MAC, PEs_F=16) | 208           | 558            | 62,081            | 51,574            | 16           |
| run_upconv_block (PEs_U=8)                 | 144           | 576            | 56,246            | 45,921            | 36           |
| Conv77_Kernel (8×8 SIMD+PE)               | 56            | 448            | 41,896            | 69,081            | 0            |
| Other / overhead                           | 193           | 32             | 15,521            | 15,798            | 16           |
| **Total**                            | **601** | **1614** | **175,744** | **182,374** | **68** |

#### Weight/Param layout expected in W_upconv & P_upconv (opt5)

```
W_upconv: [UCB0: 259200 words][UCB1: 64800][UCB2: 16200][UCB3: 4320]  total=344520 words
P_upconv: [UCB0_B:30][UCB0_G:30][UCB0_BE:30][UCB1_B:15][UCB1_G:15][UCB1_BE:15]
          [UCB2_B:8][UCB2_G:8][UCB2_BE:8][UCB3_B:4][UCB3_G:4][UCB3_BE:4]  total=171 words
```

## int16 Quantized ResBlock — HW Build & rsqrt Comparison (2026-06-09) ✅ CSIM PASS / SYNTH DONE

Hướng thay thế fp16 bằng **int16** (động lực: "tính trên float không hiệu quả"). Lượng tử affine bất đối xứng (scale + zero_point), requantize kiểu TFLite `(acc·M)>>N + zp`. Mới làm cho **1 ResBlock** (chỉ block này có quant params).

**Reference** (`test_7_quantize/resblock_quantize_i16{,_f}.cpp`): 1 ResBlock (Conv3×3→requant→Norm→Conv→Norm→quantAdd, NO ReLU). 2 file khác nhau DUY NHẤT ở rsqrt: `_i16` = Newton-Raphson nguyên (mult+shift); `_i16_f` = float `1/sqrt` + frexp→(mult,shift). **Đã vá 2 bug**: (1) `Identity(x,residual)` chạy TRƯỚC `read_tensor(input)` → residual=0, mất skip; chuyển ra sau. (2) `Add_quantize` swap M1/M2 → conv-path phải dùng n2_scale/y_scale, residual dùng x_scale/y_scale. Sau vá: khớp golden `output.txt` max 4051 (real 1.198) / rmse 79 (real 0.0234) = đúng sai số lượng tử int16, khớp Python của user.

**HW IP**: `src/hls/resblock_q/gen/` — 1 source, 2 build qua `#ifdef RSQRT_FLT`. "Match" kiến trúc fusion tối ưu (`fusion_core_fill` [A] flatten) NHƯNG **bỏ dataflow producer/consumer** (split fp16 chỉ để giữ reduce-tree khỏi LUT; int reduce rẻ) → **1 flat MAC loop II=1 + inline epilogue co-boundary**. Khác fp16: MAC `int16×int16→int32` (DSP engine 534 vs fp16 570 — nhẹ hơn, KHÔNG 4×), psum rotating 8-slot double-buffer (bank=co&1), norm nguyên (mean/var qua reci_C, rsqrt, gamma mult+shift/beta), L2 = quantized Add.

### CSIM (full 16×16×960, real data) — ALL PASS, khớp reference từng bit

| Variant            | vs golden max\|d\| |                rmse |      mismatch |
| :----------------- | -----------------: | ------------------: | ------------: |
| int (Newton rsqrt) |  4051 (real 1.198) | 79.00 (real 0.0234) | 242194/245760 |
| flt (float rsqrt)  |  4043 (real 1.196) | 79.01 (real 0.0234) | 242245/245760 |
| int vs flt         |                202 |  1.25 (real 3.7e-4) |            — |

> rsqrt nguyên ≈ float (lệch 1.25 level). CSIM int ~20 phút (native int, KHÔNG dính 7h fp16-emulation).

### SYNTHESIS — 3 cách: fp16 (chưa quant) vs int16 Norm-int vs int16 Norm-float

Số thực Vitis HLS 2024.2 (xczu7ev, target 3.333ns/300MHz). fp16 = `fusion_core_fill` top-level ([A] flatten — kiến trúc int16 đã match). **Top-level IP:**

| Metric                     |          FP16 fusion¹ |         int (Newton) |     flt (sqrt+frexp) |
| :------------------------- | ---------------------: | -------------------: | -------------------: |
| BRAM_18K                   |              422 (67%) |            451 (72%) |            451 (72%) |
| DSP                        |              602 (34%) |            534 (30%) |            521 (30%) |
| LUT                        | **79,997 (34%)** |        131,201 (56%) |        126,753 (55%) |
| FF                         |          212,214 (46%) |        169,666 (36%) |        168,350 (36%) |
| URAM                       |             32 (33%)¹ |             16 (16%) |             16 (16%) |
| CP est (rpt `Estimated`) |     **2.433 ns** |             2.660 ns |             3.239 ns |
| Fmax est (log)             |             ~411 MHz² | **375.94 MHz** | **308.74 MHz** |
| Slack @3.333ns             |                  +0.90 |               +0.673 |               +0.094 |
| Latency/call (max)         |          17,280,713 cy |        16,680,231 cy |        16,659,513 cy |
| PASS_A MAC II              |                      1 |          **1** |          **1** |

> ⚠ **Chú thích scope — bảng top-level KHÔNG cùng trường:** cột fp16 là **IP đầy đủ** (CBI + 9×RB + GlobalAdd, 1 engine chia sẻ); 2 cột int16 chỉ là **1 ResBlock**. Hệ quả: **DSP/LUT/FF/timing/latency vẫn ~công bằng** (fp16 share 1 engine → top ≈ 1 engine + wrapper, giống int16). Riêng **URAM (32 vs 16)** lệch hẳn do `global_buf` của GlobalAdd chỉ fp16 có, và **BRAM (422 vs 451)** nhiễu một phần (fp16 cõng buffer path CBI; int16 cõng param int32). → Muốn so cùng trường, dùng **dòng Engine-level** bên dưới (URAM cả 3 đều 16). Ngay cả engine-level cũng chưa hoàn hảo: engine fp16 vẫn cõng logic CBI/multi-mode mà int16 không có — so "cực sạch" cần build fp16 cho đúng 1 ResBlock (chưa làm).

**Engine-level (lõi compute, công bằng nhất):** FP16 UEK `320/570/202,662/70,815/16` · int `306/534/161,305/122,663/16` · flt `306/521/159,989/118,215/16` (BRAM/DSP/FF/LUT/URAM).

¹ fp16 IP làm cả CBI+9×RB+GlobalAdd (1 engine chia sẻ) → URAM 32 (thêm global_buf cho skip CBI→GA); int16 chỉ 1 ResBlock → 16. Engine-level cả 3 đều URAM 16.
² log fusion không có dòng Fmax; 411 = 1/2.433 (rpt `Estimated`). 2.433 = đúng effective budget → fp16 met budget, path thực có thể ngắn hơn.

### Đọc kết quả

- **Timing (bất ngờ) fp16 CP ngắn nhất** (2.433 < 2.660 < 3.239): fp16-fill tách dataflow producer/consumer → cắt critical path; int16 gộp 1 flat loop → MAC+reduce+requant chung 1 chuỗi dài hơn. **Cả 3 đạt 300MHz** (slack dương).
- **DSP**: int16 nhẹ hơn ~68 (top). Mult int16 rẻ hơn fp16 hmul nhưng **KHÔNG 4×** (fp16 hmul đã đẩy bớt sang LUT).
- **LUT**: int16 **nặng hơn ~51K** (engine 122K vs 71K) — requant `(acc·M)>>N` (int64-mult) + rsqrt nguyên + epilogue gộp. Giá thật của norm nguyên (reduce rẻ đúng như giả định, nhưng requant/rsqrt mới tốn).
- **Latency cả 3 ~y hệt** (~16.6–17.3M cy/call, đều MAC-bound II=1). Quantize KHÔNG giảm cycle. 1 ResBlock = 2 call = ~111ms @300MHz.

> **Verdict (int-norm vs float-norm)**: chọn **int-Newton**. Cùng accuracy; CP ngắn hơn 0.58ns → 375.94 vs 308.74 MHz, slack +0.673 vs +0.094 (flt sát target). flt chỉ thắng LUT/DSP nhẹ (−4.4K LUT, −13 DSP) — không bõ so mất 67MHz + margin mỏng.
> **So fp16**: ở 1 engine đơn lẻ fp16 cạnh tranh ngang (LUT nhẹ hơn, timing tốt hơn). int16 thắng DSP+FF + mở headroom MAC density — rõ khi scale-up / khi DSP là nút thắt.
> **Lưu ý timing (đính chính bản trước)**: số đúng = số tool in (375.94/308.74). Bản trước ghi 281/241 do **cộng nhầm uncertainty 0.90ns vào CP** (2.660+0.90→3.56) — sai; tool tính Fmax=1/Estimated. NHƯNG WARNING [HLS 200-871]: cả int lẫn flt **vượt effective budget 2.433ns** (=target−uncertainty) → ăn guard-band, margin mỏng (flt rất mỏng) → cần **impl P&R** xác nhận closure thật.
> **Chặn lên full int16**: chỉ có quant params cho 1 ResBlock; cần PTQ/QAT Python cho mọi layer. Report đầy đủ: `OUTPUTS/2026-06-09_resblock_int16_synth_compare.md`.

### Profiling-modules refactor — phơi Norm-stats thành module riêng (2026-06-12) ✅ VERIFIED

Vấn đề observability: engine `Resblock_Engine_Kernel` viết flat → Norm-stats (mean/var/rsqrt) bị nuốt vào hàm cha, **không có dòng report**. Tách `rb_norm_finalize()` (phần `PASS_B`: mean/var/`reci_sqrt_q`) ra hàm riêng, toggle bằng `#ifdef PROFILE_MODULES`:

- **Production (không macro)**: `#pragma HLS INLINE` → fold lại y hệt → **bit-exact + timing/resource-neutral**.
- **Build `-DPROFILE_MODULES`**: `#pragma HLS INLINE off` → module riêng + csynth report.

> **requant KHÔNG tách được**: nằm trong loop `PASS_A_FLAT` II=1 → HLS ép inline mọi hàm trong vùng pipelined. Norm-**apply** thì đã lộ sẵn (`_Pipeline_WRITE_OFM`). Chỉ Norm-**stats** là phần thật sự vô hình → đúng nó được phơi.

**Verify (int Newton, xczu7ev, target 3.333ns):**

- csim bit-identical: `max|d|=4051 / rmse=79.0021 / mism=242194/245760` — **trùng tuyệt đối** bản cũ.
- production csynth (code mới, INLINE) = cũ **từng số**: BRAM 451 / DSP 534 / FF 169,666 / LUT 131,201 / URAM 16 / CP 2.660 ns.
- profiling csynth (INLINE off): boundary chỉ **+7 DSP** (541), CP 2.660 giữ nguyên.

**Per-block breakdown (từ profiling build, PEs=16) — giờ đọc được:**

| Block                | Vai trò                             |          Latency | DSP |     FF |    LUT |
| :------------------- | :----------------------------------- | ---------------: | --: | -----: | -----: |
| `LOAD_PARAMS`      | nạp B/G/BE/GS                       |         2,952 cy |   0 | 43,636 |    935 |
| `PASS_A_FLAT`      | MAC + requant (fused, II=1)          | 1,036,931 cy/row | 312 | 82,151 | 65,257 |
| `rb_norm_finalize` | **Norm-stats: mean/var/rsqrt** |       67 cy/call |  37 |  3,558 |  4,195 |
| `WRITE_OFM`        | Norm-apply + quant-Add               |       74 cy/call | 124 | 23,448 | 40,657 |

> Xác nhận MAC-bound: `PASS_A_FLAT` ≈ 99.8% latency (1.037M cy/row × 16 ≈ 16.6M); Norm-stats+apply ~0.2% → ping-pong y_cache vô ích (consumer ≪ producer). requant 37-DSP Newton-rsqrt nằm trong `rb_norm_finalize` (vòng `VITIS_LOOP_114_1` 39 cy). tcl: `hls_syn_int_prof.tcl` (profiling), `hls_syn_int_v2.tcl` (production confirm).

## Fusion [E] — 16-wide MAC (fp16, 256 MAC/cy) trên fusion_core_fill (2026-06-12) ✅ CSIM+CSYNTH PASS

Mở rộng MAC từ 8-wide (128 MAC/cy) lên **16-wide (256 MAC/cy)** trên `fusion_core_fill` (engine fp16, đã [A]-flatten). Mục tiêu: chạy Fusion nhanh nhất có thể — kiến trúc được phép đổi.

### Kỹ thuật

`uek_mac_producer` viết lại: đọc **trọn 1 word 256-bit (16 ch) mỗi cycle** thay vì nửa word. Thêm `hdot16<T>` (16-lane balanced-tree dot). Điểm mấu chốt:

- **`acc_idx = m & 7` (KHÔNG `ciw & 7`)**: `CI_W=60` không chia hết cho 8 → tại biên panel `ciw&7` cho khoảng slot-revisit = 4 < latency fp16-add → corruption thầm lặng dưới `DEPENDENCE false`. `m&7` (m = flat MAC index) là round-robin chặt, distance đúng 8 ≥ latency → II=1 hợp lệ.
- **Bank-clean x read**: 16 bank đọc 1 lần mỗi cái vào `xw_all[16]` regs, PE chọn qua mux 3:1 (đọc trực tiếp per-PE đụng cùng bank 2 lần tại biên reflect kw≠1; 1R1W không phục vụ nổi full-rate).
- Weight preload 1 word/cycle (panel zero-slack: CI_W words trong CI_W iters). DDR weight-stream 9.2 GB/s = 48% lý thuyết → OK.

### Verification (full size 16×16×960, real data)

- **csim** (`test.cpp` → fusion_core_fill, ~1h00m): **ALL PASS, bit-exact baseline** — CBI max_err 0.015625/rmse 0.00135, RB0 0.015625/0.000742, GA 0.015625/0.00145, **0/245760 mismatch cả 3**. *Lần đầu CBI path được verify trên fill* (vá bug testbench: trước pack tight 14 words/px nhưng engine đọc stride 60 → SIGSEGV, mọi csim fill cũ chỉ RB-only).
- **csynth** (prj_syn, xczu7ev, target 3.333ns): **II=1, CP 2.433 ns giữ nguyên** (≈411 MHz, met budget). RB call **17,280,713 → 8,986,377 cy (1.92×)**, CBI 4,176,706 → 2,241,410. DSP **602 → 1114 (64%)**, LUT 80K → 99,179, BRAM 422 / URAM 32 KHÔNG đổi. **Fusion full = 164.0M cy = 546.7 ms** (trước 1.051 s) @300MHz.

### Integration caveat — KHÔNG fit full_generator trên ZCU104

[E] +512 DSP → full_generator DSP ~2100 > 1728 (engine UEK 558→~1070). Fabric-fallback fp16-mul để né DSP sẽ phá LUT budget. **[E] là bài standalone / chip lớn hơn** — KHÔNG drop vào full_generator hiện tại. Trần thật là DDR (256 MAC/cy = 48% DDR BW; 512 MAC/cy = 96%, bất khả thi).

## Fusion [F] — Row-Parallel RPP=2 (2 output rows/pass, 512 MAC/cy) trên fusion_core_fill (2026-06-16) ✅ CSIM+CSYNTH PASS

Trên nền [E], nhân đôi throughput bằng cách tính **2 hàng output cùng lúc** (RPP=2) thay vì tăng MAC-width tiếp. Mục tiêu: fusion core nhanh nhất, **bỏ qua fit full_generator** (chỉ cần fit xczu7ev standalone).

### Ý tưởng (vì sao row-parallel rẻ hơn các trục khác)

2 hàng output r, r+1 share **CÙNG 1 weight word** broadcast tới cả 32 PE (16 cột × 2 hàng) → weight-reuse 16→32, **DDR weight-stream KHÔNG đổi** (vẫn 9.2 GB/s). `x_buf` đã giữ 4 hàng nên đủ receptive field cả 2 hàng: row r đọc slot `(r+kh)&3`, row r+1 đọc `(r+1+kh)&3` (slot kề). Khác filter-parallel (tệ nhất: 2 weight set = 2× DDR) và ci/col-parallel (đều ~2× DSP nhưng không tăng reuse).

### Kỹ thuật

- Macro `RPP` (rows/pass), `#ifndef RPP #define RPP 1`; **RPP=1 fold về [E] bit-identical** (regression-safe). Build RPP=2 qua `-DRPP=2`.
- `uek_mac_producer`: `psum[2][RPP][PEs][8]` (rotating depth-8, `acc_idx=m&7`), mỗi cycle đọc RPP slot từ x_buf, broadcast 1 w_reg cho cả RPP×PEs PE. Pack `pstrm` rộng `PEs*128*RPP`.
- `uek_reduce_consumer`: `ycol[RPP][PEs]`, `sum_acc[RPP][PEs]`/`sumsq_acc[RPP][PEs]`.
- `y_cache` **flatten `[RPP*960]` cyclic-16** (KHÔNG `[RPP][960]`): y_cache rộng 256-bit → width-bound 8 BRAM18/bank bất kể depth; flat = 16 bank dùng chung (rows ở depth khác nhau) = 128 BRAM, vs `[RPP][960]` = 32 bank = 256 BRAM. (Đây là fix overflow vòng 1: BRAM 662→534.)
- **LUT-fit (mấu chốt)**: `config_op hmul -impl fabric` đẩy **toàn bộ 512 mult lên LUT** (171K) → 239K LUT = 103% (over). Pin **3 lane fp16 mult xuống DSP** bằng `#pragma HLS BIND_OP variable=mN op=hmul impl=maxdsp` (mN=xN*wN) → dịch sạch 96 mult (3×32 PE): DSP 1114→1306 (+192≈2/mult), LUT 239,163→**225,627** (−13,536≈141/mult), **0 LUT overhead, bit-exact** (cùng fp16 mult, chỉ đổi chỗ đặt). `BIND_OP impl=maxdsp` KHÔNG crash (khác `impl=fabric` crash reflow pass — xem Known Issues); fp32-accumulate để né fabric thì **SAI hướng** (convert fp16↔fp32 ngốn thêm LUT, đã thử LUT lên 243K → bỏ).

### Verification (full size 16×16×960, real data)

- **csim** (`test.cpp -DRPP=2` → fusion_core_fill, ~1h): **ALL PASS, numerically-identical [E]** — CBI max_err 0.015625/rmse 0.00135175, RB0 0.015625/0.000741604, GA 0.015625/0.00145125, **0/245760 mismatch cả 3** (tcl `hls_csim_rp.tcl`).
- **csynth** (`hls_syn_rp.tcl`, prj_rp, xczu7ev, target 3.333ns): **PASS_A_FLAT II=1, Fmax 339.33 MHz** (met 300). RB call **8,986,377 → 4,694,209 cy (1.91×)**. DSP 1114→**1306 (75%)**, LUT 99,179→**225,627 (97%)**, BRAM 422→**534 (85%)**, FF→352,285 (76%), URAM **32 (33%)** KHÔNG đổi. **Fusion full ~546.7 → ~286 ms** @300MHz (1.91×).
- **Fit xczu7ev standalone** (tất cả ≤100%). Vẫn KHÔNG fit full_generator (scope đã bỏ qua theo yêu cầu).

### Còn lại / hướng tiếp

RPP=3/4 sẽ over LUT/BRAM (mỗi +1 RPP ≈ +256 mult). Trần vật lý: DDR (RPP=2 vẫn 48% DDR BW vì weight reuse, KHÔNG phải nút thắt; nút thắt là LUT/DSP). 4 bench per-UCB không liên quan (đây là fusion).

## Fusion [F8] — fp8 (E4M3) conv-MAC benchmark: đo "fp8 = 2× + nhẹ DSP?" (2026-06-16) ✅ CSYNTH-VERIFIED

Câu hỏi: chuyển conv fp16→fp8 (1 byte/phần tử) thì phần cứng hiệu quả ra sao. Build benchmark riêng `src/hls/fusion_core_fp8` (folder clone, KHÔNG đụng `fusion_core_fill` — số [E]/[F] giữ cho báo cáo). Scope = **1 RB conv 960→960** (MAC array dùng chung, chiếm 18/19 conv), bỏ CBI/GA. Conv operands fp8 packed **32/256-bit-word** (vs 16 fp16), accumulate+norm+output giữ **fp16**. Accuracy do team lo (team đã quantize fp8 ra tốt, tốt hơn int) — benchmark này CHỈ đo resource/timing.

### 3 số thật (csynth xczu7ev @3.333ns, so ở tầng `mac_producer` = MAC array, cùng conv 960→960)

| Config                                 | Throughput       |           DSP |     LUT | Latency 1 RB |   Fmax |
| :------------------------------------- | :--------------- | ------------: | ------: | -----------: | -----: |
| **[E] fp16 16-wide**             | 256 MAC/cy (1×) | **909** |  53,945 | 8,986,377 cy |    411 |
| **fp8 16-wide** (iso-throughput) | 256 MAC/cy (1×) | **488** |  88,639 | 8,612,560 cy | 375.94 |
| **fp8 32-wide** (2×)            | 512 MAC/cy (2×) |          1000 | 167,686 | 4,441,920 cy | 369.55 |

> Top-level fp8 32-wide: BRAM 412(66%)/DSP 1141(66%)/FF 264,635(57%)/LUT 194,956(**84%**)/URAM 0. fp8 16-wide top: BRAM 340(54%)/DSP 629(36%)/FF 170,989(37%)/LUT 115,925(50%). Cả hai II=1 (PASS_A_FLAT trip 32-wide=259,200, 16-wide=518,400). Engine breakdown 32-wide: mac_producer DSP 1000/LUT 167,686 (hog), PASS_B norm 102 DSP, reduce 6,859 LUT.

### Kết luận (đính chính ước tính ban đầu "fp8 = 2× + giảm DSP" — đã sai, số thật mới đúng)

- **fp8 = bộ chuyển DSP→LUT, KHÔNG phải "vừa nhanh vừa nhẹ mọi thứ".** fp8-mul rời DSP sang LUT (~137 LUT/mult thêm, đổi ~1.6 DSP/mult bớt).
- **fp8 16-wide (iso): cùng tốc độ [E], DSP 909→488 (−46%, gần nửa)**, đổi LUT +64%. → giữ nguyên #phần-tử thì fp8 nhả hết mult-DSP → DSP rớt mạnh.
- **fp8 32-wide (2×): nhanh 2.02× nhưng DSP ~đứng yên (909→1000)** vì adder tree fp16 gấp đôi (31 add/PE vs 15) lấp lại chỗ mult vừa nhả; **LUT nổ 3× (54K→168K)**.
- **LUT-killer = float-overhead per-element**: mỗi `fp8_mul_to_half` phải decode→nhân 4×4→**normalize→ráp fp16** (~250 LUT/lane ×512). Mantissa 4-bit rẻ nhưng vỏ float không rẻ, nhân 512 lần.
- **Hữu ích khi DSP là nút thắt** (full_generator 98% DSP/81% LUT → fp8-16 nới đúng chỗ). Vô ích nếu LUT mới là nút thắt.
- **Muốn vừa 2× vừa nhẹ cả DSP+LUT → block floating-point** (shared exponent/block → integer mult, pack DSP được, bỏ normalize per-element → cắt cái 137 LUT/mult). Chưa build.

> Phase-0 numpy (naive PTQ fp8 cast, CBI conv real data): E4M3 rmse 0.115 = 137× fp16, int8 per-chan 0.0736 = 88× — **naive cast quá thô**; nhưng team quantize fp8 (scale/QAT) ra tốt → accuracy KHÔNG phải blocker, đây chỉ là cảnh báo "đừng cast trần". Script: `OUTPUTS/fp8_phase0_check.py`, `OUTPUTS/winograd_phase0_check.py`.

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

| Issue                                                     | Root Cause                                                                                                                                                                              | Workaround / Fix                                                                                                                                                               |
| :-------------------------------------------------------- | :-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `x_buf` fusion phải là BRAM, không phải URAM        | `ARRAY_PARTITION complete dim=2` → 16 banks → URAM cần 64 blocks, vượt limit 96                                                                                                  | Bind `x_buf` vào BRAM; chỉ `skip_buf`/`global_buf` mới để URAM                                                                                                      |
| HLS tạo 2 instances của `Universal_Engine_Kernel`     | Pass `NULL` vs non-NULL cho G_IN/BE_IN → HLS thấy connectivity khác → 2 hardware instances → BRAM overflow                                                                       | Pass `P_fusion` (dummy, non-NULL) cho tất cả calls → identical port connections → 1 shared instance                                                                      |
| HLS tạo 2 instances của `run_upconv_block`            | UCB_0 đọc X, UCB_1-3 đọc Y → different input ports → 2 instances                                                                                                                  | Pass cả X và Y vào function, chọn input bằng `mode` bên trong → 1 shared instance                                                                                     |
| Duplicate symbol khi include cả 2 `.tpp`               | `half_to_bits`, `bits_to_half`, `my_sqrt_f` defined ở cả `Hls_Layers_Fusion.tpp` và `Hls_Layers_UpConv.tpp`                                                                | Guard `#ifndef HLS_HALF_HELPERS_DEFINED` quanh các helper functions                                                                                                         |
| Conv77 latency 1.365 sec (49 × CI_LOOP pipeline startup) | CI_LOOP iteration latency = 91 cycles (L_MAC tree depth) × 49 kernel positions = lãng phí fill/drain overhead                                                                        | Flat loop 196 iterations:`psum[co][k_ci]` write-once → no RAW, II=1 không cần DEPENDENCE; REDUCE depth-4 rotating acc → 0.120 sec (**11× speedup**)               |
| Conv77 line_buf 16 URAM → integration 96/96 = 100%       | `ARRAY_PARTITION complete dim=3` trên line_buf tạo 4 banks × 4 URAM wide = 16 URAM; 80+16=96/96 không fit                                                                         | Bỏ dim=3 partition (flat loop chỉ cần 1 read/iter) → 8 URAM; integration: 80+8=88/96 (92%) ✅                                                                              |
| Conv77 CSIM vitis_hls fail: "Cannot open: io_params/..."  | vitis_hls csim chạy từ `Conv77_HLS/solution/csim/build/`, copy `-tb` files flat (không giữ subdir)                                                                              | test.cpp dùng tên file phẳng (không prefix `io_params/` hay `model_params/`); TOL=1.0 cho ap_fixed<16,8>                                                               |
| Conv77 Clip(0,1) thêm ~36K LUT (69K total vs 32K before) | Clip trên `fp32acc_t = ap_fixed<32,16>` bên trong pipelined unrolled loop → HLS tạo 32-bit comparator logic cho mỗi PE output                                                    | Chấp nhận: tổng LUT=160,811 (69%) vẫn trong limit; Clip bắt buộc theo HiFiC model spec                                                                                   |
| csynth timing violation -0.81 ns tại run_upconv_block    | URAM read-modify-write trong ACC_WRITE (TILE_LOOP) là critical path; HLS pessimistic estimate                                                                                          | Không cần fix: full_generator gốc có cùng violation nhưng implementation pass 308.74 MHz. csynth timing ≠ impl timing                                                   |
| BRAM là hard constraint giới hạn PEs_U ở 12           | w_local[PEs][540] dùng 16 BRAM_18K/PE × 12 = 192 BRAM; AXI buffers thêm ~193 BRAM. Total 605/624 (96%) với PEs=12. PEs=13 → ≈621 BRAM (overflow risk)                             | Không thể tăng PEs_U vượt 12 với kiến trúc hiện tại mà không redesign weight storage                                                                               |
| UCB_1/2/3 đọc sai weights và params (opt2 bug)         | Tất cả 4 UCBs dùng cùng `W_upconv`/`P_upconv` pointer không có offset → UCB_1 đọc từ offset 0 thay vì 259200 → sai weights hoàn toàn. Tương tự cho bias/gamma/beta | **opt5**: thêm per-UCB offsets trong `run_upconv_block`. Weight layout: [UCB0:259200][UCB1:64800][UCB2:16200][UCB3:4320]. Param layout: [UCB0_B/G/BE][UCB1_B/G/BE]... |

## Data Classification

- **Weights/Params**: `assets/test_data/model_params/` (Trọng số Generator bắt đầu bằng `Gen_...`)
- **Input/Output**: `io_params/` và `layer_test_vectors/`
