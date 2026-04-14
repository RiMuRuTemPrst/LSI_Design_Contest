# CLAUDE.md

This file provides guidance to Claude Code when working with the independent `ChannelNorm` IP Core.

## Component: ChannelNorm — Channel Normalization HLS IP Core
Target: ZCU104 (xczu7ev-ffvc1156-2-e) @ 300 MHz (3.333 ns).

## File Structure
- `gen/ChannelNorm.cpp`: Top-level AXI interface and core kernel logic (`ChannelNorm_Kernel`).
- `test.cpp`: Basic testbench.
- `hls_run.tcl`: Script to run C simulation and synthesis via Vitis HLS.

## Key Features
- **Data Interface:** Uses `data_256_t` (256-bit AXI width) wrapping 16 `half` (FP16) values per beat.
- **2-Pass Algorithm:** 
  1. Computes spatial mean and variance across the channel.
  2. Normalizes using `gamma` and `beta` parameters.
- **Performance:** Processes fully vectorized channel chunks.

## Synthesis Estimates (ZCU104)
- **Clock:** Target 3.333 ns, Estimated 2.433 ns
- **Latency (16x16, 960 channels):** 775 cycles (~2.58 us)
- **Utilization:** BRAM: 259, DSP: 288, FF: 41,196, LUT: 25,872
