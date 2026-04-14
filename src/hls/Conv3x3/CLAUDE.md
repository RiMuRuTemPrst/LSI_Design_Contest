# CLAUDE.md

This file provides guidance to Claude Code when working with the independent `Conv3x3` IP Core.

## Component: Conv3x3 — 3×3 Convolution HLS IP Core
Target: ZCU104 (xczu7ev-ffvc1156-2-e) @ 300 MHz (3.333 ns).

## File Structure
- `gen/Conv3x3.cpp`: Top-level AXI interface calling the core kernel.
- `gen/Hls_Layers_Conv3x3.tpp`: Standardized kernel logic (`Conv3x3_Kernel`), derived from the `resblock` backbone. Features On-the-fly Reflect Padding and Ping-pong buffering.
- `test.cpp`: Basic testbench.
- `hls_run.tcl`: Script to run C simulation and synthesis via Vitis HLS.

## Key Features
- **Data Interface:** Uses `data_256_t` (256-bit AXI width) wrapping 16 `half` (FP16) values per beat to maximize DDR bandwidth.
- **Padding:** "On-the-fly Reflect Padding" is built directly into the circular buffer load logic (`init_rows` and column logic). No separate padding IP is needed.
- **Performance:** Synthesizes with II=1 using deeply unrolled spatial processing.

## Synthesis Estimates (ZCU104)
- **Clock:** Target 3.333 ns, Estimated 2.433 ns
- **Latency (16x16, 960 channels):** 60,002 ~ 61,458 cycles (~0.20 ms)
- **Utilization:** BRAM: 360, DSP: 256, FF: 37,314, LUT: 32,895
