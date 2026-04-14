# CLAUDE.md

This file provides guidance to Claude Code when working with the independent `Relu` IP Core.

## Component: Relu — Vectorized ReLU HLS IP Core
Target: ZCU104 (xczu7ev-ffvc1156-2-e) @ 300 MHz (3.333 ns).

## File Structure
- `gen/relu.cpp`: Top-level AXI interface and vectorized ReLU logic.
- `test.cpp`: Basic testbench.
- `hls_run.tcl`: Script to run C simulation and synthesis via Vitis HLS.

## Key Features
- **Data Interface:** Uses `data_256_t` (256-bit AXI width) wrapping 16 `half` (FP16) values per beat.
- **Performance:** Highly vectorized. Processes 16 FP16 values per clock cycle (II=1).
- **Latency:** Dependent directly on input size `N` (1 cycle per 256-bit word).

## Synthesis Estimates (ZCU104)
- **Clock:** Target 3.333 ns, Estimated 2.433 ns
- **Utilization:** BRAM: 73, DSP: 0, FF: 3,637, LUT: 3,215
