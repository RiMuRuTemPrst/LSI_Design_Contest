# CLAUDE.md

This file provides guidance to Claude Code when working with the independent `Add` IP Core.

## Component: Add — Element-wise Addition HLS IP Core
Target: ZCU104 (xczu7ev-ffvc1156-2-e) @ 300 MHz (3.333 ns).

## File Structure
- `gen/Add.cpp`: Top-level AXI interface and vectorized addition logic.
- `test.cpp`: Basic testbench.
- `hls_run.tcl`: Script to run C simulation and synthesis via Vitis HLS.

## Key Features
- **Purpose:** Used for Global Skip connections and other element-wise tensor additions.
- **Data Interface:** Uses `data_256_t` (256-bit AXI width) wrapping 16 `half` (FP16) values per beat.
- **Performance:** Highly vectorized. Computes addition of 16 pairs of FP16 values per clock cycle (II=1).
- **Latency:** Dependent directly on input size `N` (1 cycle per 256-bit word).

## Synthesis Estimates (ZCU104)
- **Clock:** Target 3.333 ns, Estimated 2.433 ns
- **Utilization:** BRAM: 131, DSP: 32, FF: 6,562, LUT: 6,029
