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

### Standalone convtranspose testbench (src/hls/convtranspose/src)
Compile and run `test.cpp` directly with g++ (no CMake):
```bash
cd src/hls/convtranspose/src
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

Paths in `non_gen/Core.h` use **relative paths** for portability.

### Key types (`non_gen/Core.h`)
- **`data_t`** – the numeric type (see table above)
- **`TensorMem<T>`** – 4D tensor in **NHWC layout** with raw pointer ownership flag
- **`Shape`** – `{N, H, W, C}` struct
- **`Arena`** / `fake_stack.h` – bump allocator that simulates hardware on-chip memory. Used only in SW sim testbenches.
- **`MyStream<T>`** – `hls::stream` in synthesis, `std::queue` wrapper in SW

### HLS kernel conventions (`gen/Hls_Layers_ConvTranspose.tpp`)
- All tensor dimensions are **compile-time template parameters**
- In synthesis the function takes `ap_uint<64>*` packed pointers; in SW sim it takes `T*` raw pointers — guarded by `#ifdef __SYNTHESIS__`
- HLS pragmas (`PIPELINE`, `UNROLL`, `ARRAY_PARTITION`, `BIND_STORAGE`, `BIND_OP`) are co-located with the loops they target
- On-chip buffers: `x_buf` → URAM, `w_tile` → LUTRAM, `b_buf`/overlap buffers → BRAM
- The IOM (Incremental Output Mapping) 3-pass algorithm handles convtranspose overlap-add in place without a full output buffer
- **Standardized naming:** Kernel functions use the `_Kernel` suffix (e.g., `ConvTranspose_Kernel`).

### gen/ vs non_gen/ split
- **`gen/`** – kernel-specific generated code (e.g., `Hls_Layers_ConvTranspose.tpp`, `convtranspose_top.cpp`)
- **`non_gen/`** – reusable infrastructure shared across components (`Core.h`, `fake_stack.h`, `tensor_io.tpp`, `class_tensor.tpp`)

### I/O parameter files
Text files under `io_params/` and `model_params/` hold float weights/activations read by `read_tensor()` / `write_tensor()`. These are the golden reference data for testbenches.

---

## HLS Optimization Log (`Hls_Layers_ConvTranspose.tpp` — v2, 2026-04-02)

### Baseline (v1)
- Latency HW_ConvTranspose_0: **5.28M–11.67M cycles** (17.6–38.9 ms @ 300 MHz)
- DSP: 576/1728 (33%) | URAM: 64/96 (66%) | LUT: 37%
- Bottleneck analysis: Y-AXI write ~17 cyc/transaction × 32 writes/interior-pixel = ~480 cyc overhead per pixel.

### Optimizations applied (v2)

| # | Change | File | Expected gain |
|---|--------|------|---------------|
| OPT-1 | `LANE` 8→16 | `Hls_Layers_ConvTranspose.tpp` | Mac_loop trip count halved; same URAM (64) |
| OPT-2 | `b_buf` cyclic partition `factor=TILE_C` | `Hls_Layers_ConvTranspose.tpp` | Removes BRAM port conflict on 8-wide parallel bias reads |
| OPT-3 | `row_buff` `complete dim=1` (W_IN=16) | `Hls_Layers_ConvTranspose.tpp` | Converts BRAM → 256 FF; eliminates address-decode latency |
| OPT-4 | Y output packed as `ap_uint<64>*` (4 fp16/word) | `Hls_Layers_ConvTranspose.tpp` + `convtranspose_top.cpp` | AXI write transactions reduction (~4× win) |

**Estimated post-opt latency (HW_ConvTranspose_0):** ~2–4M cycles (6–13 ms).

**Resource delta after OPT-1:** DSP 576→1152 (33%→67%). URAM unchanged (64/96).

### Constraints & compatibility
- OPT-1 (`LANE=16`): only valid when `C_IN % 16 == 0`. Satisfied for Layer 0–2. **NOT** for Layer 3.
- OPT-4 (packed Y): PS code reading Y must treat it as packed fp16×4. SW sim (test.cpp) is unaffected.

### SW simulation verification
Both v1 and v2 produce **bit-identical** float outputs. Discrepancy vs reference is due to `float` vs `ap_fixed<16,8>` precision.
