# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an LSI Design Contest project implementing a hardware-accelerated **HiFiC (High-Fidelity Image Compression)** GAN inference pipeline on the **Xilinx ZCU104 FPGA** (Zynq UltraScale+) using **Vitis HLS 2024.2**.

---

## Build Commands

### C++ Software Simulation (HiFiC/C_Simulation)
```bash
cd HiFiC/C_Simulation
./run.sh
# Equivalent to:
cmake -S . -B build && cmake --build build && ./build/run
```

### Standalone convtranspose testbench (HLS/convtranspose/src)
Compile and run `test.cpp` directly with g++ (no CMake):
```bash
cd HLS/convtranspose/src
g++ -std=c++17 -O2 test.cpp -o test_conv && ./test_conv
```

### Vitis HLS (synthesis / co-sim)
Open the component in Vitis HLS 2024.2. Include path for legacy headers:
```
-I/home/rimurutempest/Tool/Xilinx/Vitis/2024.2/include/legacy
```

### Demo app (on ZCU104 board, cross-compile)
```bash
g++ hific_app.cpp -o hific_app -O3 -mcpu=cortex-a53 `pkg-config --cflags --libs opencv4 gstreamer-1.0`
```

---

## Architecture

### Top-level layout
```
HiFiC/           – Full HiFiC model: C++ ops library + HLS kernels
  C_Simulation/  – Pure C++ reference simulation (CMake project)
  GAN_HLS/       – HLS ResBlock kernel (resblock_top)
  include/Core.h – Shared dual-mode header (HLS ↔ SW sim)
  src/           – Op implementations as .tpp files
HLS/             – Individual Vitis HLS IP components
  convtranspose/ – ConvTranspose HLS IP (primary working kernel)
  GAN/           – GAN-level HLS component
  Adder32bit/, Mul32bit/, Mulfp16/, DMA/  – Other HLS IPs
HiFiC_Demo/      – ZCU104 live demo app (OpenCV + GStreamer)
Test/            – Standalone C++ test harness (CMake)
HLS_DesignFlow/  – Vitis HLS tutorial labs
Doc/             – Reference PDFs (Zynq Book, optimization guide)
```

### Dual-mode compilation pattern
Every kernel file compiles as **both** pure C++ (software simulation) and HLS synthesis by toggling macros:

| Condition | `data_t` | memory pointers | stream type |
|---|---|---|---|
| `__SYNTHESIS__` or `USE_HLS` | `ap_fixed<16,8>` | `ap_uint<64>*` (4× fp16 packed) | `hls::stream<T>` |
| SW sim (default) | `float` | `T*` | queue-based `MyStream<T>` |

The `ENV_LOCAL` / `ENV_SERVER` macros in `non_gen/Core.h` switch absolute path macros (`PATH_IO_PARAMS`, etc.) between developer machines.

### Key types (`include/Core.h`, `non_gen/Core.h`)
- **`data_t`** – the numeric type (see table above)
- **`TensorMem<T>`** – 4D tensor in **NHWC layout** with raw pointer ownership flag
- **`Shape`** – `{N, H, W, C}` struct
- **`Arena`** / `fake_stack.h` – bump allocator that simulates hardware on-chip memory; `alloc<T>(count)` pushes, `pop()` frees last allocation. Used only in SW sim testbenches.
- **`MyStream<T>`** – `hls::stream` in synthesis, `std::queue` wrapper in SW

### HLS kernel conventions (`HLS/convtranspose/src/gen/Hard_op_2.tpp`)
- All tensor dimensions are **compile-time template parameters** (shapes must be known at elaboration)
- In synthesis the function takes `ap_uint<64>*` packed pointers; in SW sim it takes `T*` raw pointers — guarded by `#ifdef __SYNTHESIS__`
- HLS pragmas (`PIPELINE`, `UNROLL`, `ARRAY_PARTITION`, `BIND_STORAGE`, `BIND_OP`) are co-located with the loops they target
- On-chip buffers: `x_buf` → URAM, `w_tile` → LUTRAM, `b_buf`/overlap buffers → BRAM
- The IOM (Incremental Output Mapping) 3-pass algorithm handles convtranspose overlap-add in place without a full output buffer

### gen/ vs non_gen/ split
- **`gen/`** – kernel-specific generated code that changes per layer (e.g., `Hard_op_2.tpp`, `resblock_top.cpp`, parameter `.txt` files)
- **`non_gen/`** – reusable infrastructure shared across all components (`Core.h`, `fake_stack.h`, `tensor_io.tpp`, `class_tensor.tpp`)

### I/O parameter files
Text files under `io_params/` and `model_params/` hold float weights/activations (one value per line) read by `read_tensor()` / `write_tensor()` from `tensor_io.tpp`. These are the golden reference data for testbenches.

### HiFiC pipeline (ZCU104 demo)
Camera → GStreamer capture → C++ host (`hific_app.cpp`) → FPGA ResBlock accelerator (UIO driver) → DisplayPort output via GStreamer compositor. Button SW14 triggers one inference cycle.

---

## HLS Optimization Log (`Hard_op_2.tpp` — v2, 2026-04-02)

### Baseline (v1)
- Latency HW_ConvTranspose_0: **5.28M–11.67M cycles** (17.6–38.9 ms @ 300 MHz)
- DSP: 576/1728 (33%) | URAM: 64/96 (66%) | LUT: 37%
- Bottleneck analysis: Y-AXI write ~17 cyc/transaction × 32 writes/interior-pixel = ~480 cyc overhead per pixel (out of 688 total). Mac_loop was only 125 cyc/pixel.

### Optimizations applied (v2)

| # | Change | File | Expected gain |
|---|--------|------|---------------|
| OPT-1 | `LANE` 8→16 | `Hard_op_2.tpp` line 25 | Mac_loop 125→~65 cyc/pixel (halved); same URAM (64) |
| OPT-2 | `b_buf` cyclic partition `factor=TILE_C` | `Hard_op_2.tpp` | Removes BRAM port conflict on 8-wide parallel bias reads |
| OPT-3 | `row_buff` `complete dim=1` (W_IN=16) | `Hard_op_2.tpp` | Converts 16-bank BRAM → 256 FF; eliminates address-decode latency in IOM pass 3 |
| OPT-4 | Y output packed as `ap_uint<64>*` (4 fp16/word) | `Hard_op_2.tpp` + `convtranspose_top.cpp` | AXI write transactions ~32→~8 per interior pixel (~4× reduction, main latency win) |

**Estimated post-opt latency (HW_ConvTranspose_0):** ~2–4M cycles (6–13 ms) — run synthesis to confirm.

**Resource delta after OPT-1:** DSP 576→1152 (33%→67%). URAM unchanged (64/96). LUT slightly higher.

### Constraints & compatibility
- OPT-1 (`LANE=16`): only valid when `C_IN % 16 == 0`. Satisfied for Layer 0–2 (C_IN=960/480/240). **NOT** for Layer 3 (C_IN=120). `syn.top=HW_ConvTranspose_0` so Layer 3 is never synthesized in this component.
- OPT-4 (packed Y): `Y` port changed from `data_t*` → `wide_t*` (`ap_uint<64>*` in synthesis). Host/PS code reading Y must treat it as packed fp16×4. SW sim (test.cpp) is unaffected (`wide_t=data_t=float`).
- Y depth in m_axi pragma divided by 4 (e.g., 491520→122880 for Layer 0).

### Backups
- `gen/Hard_op_2_v1_original.tpp` — original pre-optimization kernel
- `gen/convtranspose_top_v1_original.cpp` — original top-level wrapper

### SW simulation verification
Both v1 and v2 produce **bit-identical** float outputs (verified 2026-04-02). Discrepancy vs `Gen_ucb1_ct_output_0.txt` is pre-existing (float SW-sim vs ap_fixed<16,8> HLS co-sim precision) and unrelated to these optimizations.
