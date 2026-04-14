# CLAUDE.md

This file provides guidance to Claude Code when working with Conv77.

## Component: Conv77 — 7×7 Convolution HLS IP Core

Vitis HLS 2024.2. Target: ZCU104 (xczu7ev-ffvc1156-2-e) @ 300 MHz (3.333 ns).

## File Structure

```
Conv77/
  gen/
    Hls_Layers_Conv77.tpp # HLS kernel (Standardized — fp16 SIMD + spatial-PE design)
    conv77_top.cpp        # Top-level AXI interface (calls Conv77_Kernel)
  non_gen/
    Core.h                # TensorMem<T>, Shape, dual-mode env detection
    fake_stack.h          # Arena allocator
    class_tensor.tpp
    tensor_io.tpp         # read_tensor / write_tensor
  io_params/
    Gen_ucb4_Relu_output_0.txt   # Input tensor X (1×256×256×60)
    Gen_cbo_Conv_output_0.txt    # Gold output (float32 reference)
  model_params/
    Gen_cbo_weight.txt           # Conv weights (3×7×7×60)
    Gen_cbo_bias.txt             # Conv bias (3)
  test.cpp             # Testbench
  hls_run.tcl          # Vitis HLS run script (csim → csynth → export)
```

## Architecture: fp16 SIMD + Spatial-PE Design

### PE Structure
- **1 PE = SIMD_DEPTH=8 parallel MACs** along the depth (channel) dimension
- Each PE accumulates into a single output pixel for one output channel
- Data type: `ap_fixed<16,8>` — fp16 via HLS ap library (resolution ≈ 0.004)
- Accumulator: `ap_fixed<32,16>` — wide to prevent overflow over 2940 MACs
- **DSP Optimization:** By performing implicit 16x16 multiplications (`fp16_t * fp16_t`) before accumulating into the 32-bit `fp32acc_t`, the design forces Vitis HLS to use exactly 1 DSP48E2 block per MAC, capping total DSP usage at 448.

### Spatial Parallelism
- **NUM_WIN_PEs=8 PEs** process 8 adjacent output windows simultaneously
- Shared `x_buffer[H_R][W_BUF][C_PAD]` — loaded once, used by all 8 PEs
- Combined buffer width: `W_BUF = W_R + NUM_WIN_PEs - 1 = 14` columns

### Data Reuse Strategy
- Loop order: `ho → wo_tile (step 8) → [load x_buffer] → co → [compute]`
- x_buffer loaded **once per (ho, wo_tile)**, reused for all C_OUT=3 output channels
- Weight buffer loaded **once per kernel call**, stored on-chip for all spatial tiles
- NHWC layout: depth-consecutive memory → loading 8 consecutive channels = burst

### Padding
- C_IN=60 → C_PAD=64 (next multiple of SIMD_DEPTH=8); last 4 slots zero-filled
- TC_IN = C_PAD / SIMD_DEPTH = 8 depth tiles
- Boundary: reflect padding (`i < 0 → -i`, `i >= MAX → (MAX-1)*2 - i`)

## Kernel Parameters

```
Conv77_Kernel<SIMD_DEPTH=8, NUM_WIN_PEs=8, BATCH=1,
              C_IN=60, C_OUT=3, H_IN=256, W_IN=256,
              H_R=7, W_R=7, H_OUT=256, W_OUT=256>
```

| Symbol | Value | Description |
|--------|-------|-------------|
| SIMD_DEPTH | 8 | MACs per PE (depth parallelism) |
| NUM_WIN_PEs | 8 | Adjacent windows (spatial parallelism) |
| C_PAD | 64 | C_IN rounded up to multiple of 8 |
| TC_IN | 8 | Depth tiles (C_PAD / SIMD_DEPTH) |
| W_BUF | 14 | Input buffer width (W_R + NUM_WIN_PEs - 1) |
| MACs/cycle | 448 | NUM_WIN_PEs × W_R × SIMD_DEPTH at II=1 (Uses 448 DSPs) |

## Array Partitioning

| Buffer | Shape | Partition |
|--------|-------|-----------|
| x_buffer | [7][14][64] | complete dim=2; cyclic(8) dim=3 |
| w_buffer | [3][7][7][64] | complete dim=3; cyclic(8) dim=4 |
| b_buffer | [3] | complete dim=1 |
| acc | [3][8] | complete dim=1,2 |

`cyclic(8)` on channel dim: element `c` → bank `c%8`.
By removing complete partitioning on dynamic loop indices (like hf or co) and using it strictly on fully unrolled inner dimensions (wf and col), we entirely eliminate massive 147-to-1 Multiplexers. Data is read directly from completely independent small BRAM/LUTRAM banks.

## Loop Structure

```
Preload weights (once)
Batch_loop (1):
  Spatial_h_loop (256):
    Spatial_w_loop (32 tiles, step=8):    ← wo increments by NUM_WIN_PEs
      Load x_buffer (init or shift+new)   ← shared across all C_OUT
      Init_acc_co (3) UNROLL
        Init_acc_win (8) UNROLL
      Compute_hf (7):
        Compute_co (3):
          Compute_tci (8) PIPELINE II=1:
            Win_loop (8) UNROLL
              Wf_loop (7) UNROLL:         ← 448 MACs/cycle
                Simd_loop (8) UNROLL
      Write 3×8=24 outputs
```

## Estimated Latency @ 300 MHz

| Phase | Cycles | Notes |
|-------|--------|-------|
| Weight preload | ~9,408 | C_OUT×H_R×W_R×C_PAD, once |
| x_buffer init load | 6,272 | H_R×W_BUF×C_PAD (wo=0) |
| x_buffer shift+load | 3,640 | Shift (56) + Newcol (3584) (wo>0 tiles) |
| Compute per tile | 168 | H_R×C_OUT×TC_IN at II=1 |
| **Per tile total** | **~3,808** | load-dominated |
| **Total** | **~31M** | 8192 tiles × 3808 |
| **@ 300 MHz** | **~103 ms** | target: < 500 ms ✓ |

## Resource Utilization (ZCU104)

- **DSP:** 448 (25%) — Perfectly maps 448 MACs/cycle via implicit 16x16 multiplication.
- **LUT:** ~32K (14%) — Dramatically reduced by removing dynamic loop index partition bottlenecks.
- **FF:** ~32K (6%)
- **BRAM:** 64 (10%)
- **Timing:** Estimated 2.620 ns (Target 3.333 ns / 300 MHz) ✓

## Running Software Simulation

```bash
g++ -std=c++17 -O2 -I. test.cpp -x c++ gen/Hls_Layers_Conv77.tpp -o test_sw && ./test_sw
```

Expected: `RESULT: PASS` with `max_err < 0.5` (fp16 quantization tolerance).

## Running HLS (Vitis HLS 2024.2)

```bash
vitis_hls -f hls_run.tcl
```

Flow: csim → csynth → export IP catalog
Report: `Conv77_HLS/solution/syn/report/csynth.rpt`

## Verification Tolerance

`TOL = 1.0` (absolute) for `ap_fixed<16,8>` fp16:
- Resolution: 1/256 ≈ 0.004 per element
- Over 2940 MACs: accumulated quantization error — measured max_err=0.531, mean_err=0.212
- Float32 gold output is the reference; fp16 diverges by design
- csim result: 0/196608 mismatches at TOL=1.0 ✓

## AXI Interface

| Port | Bundle | Depth |
|------|--------|-------|
| X | gmem_X | 3,932,160 |
| W | gmem_W | 8,820 |
| B | gmem_B | 3 |
| Y | gmem_Y | 196,608 |
| return+args | control | — |

## Tensor Layout

All tensors: NHWC. `idx = ((n*H+h)*W+w)*C+c`
Weights: C_OUT-first. `idx = ((co*H_R+hf)*W_R+wf)*C_IN+ci`
