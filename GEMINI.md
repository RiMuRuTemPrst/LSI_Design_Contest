# Technical Context for AI Agents (HiFiC FPGA Project)

## Project Overview

Dự án triển khai mô hình HiFiC lên FPGA ZCU104.
Kiến trúc Generator (Decoder) được triển khai thành **1 IP duy nhất**: `full_generator_top` (Fusion Core + UpConv Core + Conv77).

## Full Generator Architecture

```
hyper-latent (16×16×960)
    │ [conv_block_init] ✅
    ▼ Norm → Pad → Conv 3×3 → Norm
    │ [9× res_block] ✅
    ▼ Pad→Conv3×3→Norm→ReLU→Pad→Conv3×3→Norm→Add(SC)
    │ [GlobalAdd] ✅
    ▼
    │ [4× up_conv_block] ✅
    ▼ UCB_0→3: 16×16×960 → 32×32×480 → 64×64×240 → 128×128×120 → 256×256×60
    │ [conv_block_out / Conv77] ✅
    ▼ Pad → Conv 7×7 (SIMD 8×8) → Clip(0,1)
    ▼ Output (256×256×3)
```

## Implemented IPs

| IP                   | Scope                           | Source                    |
| :------------------- | :------------------------------ | :------------------------ |
| full_generator_top   | Fusion + UpConv + Conv77        | `src/hls/full_generator/` |
| upconv_core_top      | 4× UCB chained (standalone ref) | `src/hls/upconv_core/`    |
| fusion_core_top      | Fusion only (standalone ref)    | `src/hls/fusion_core/`    |
| conv77_core_top      | Conv77 flat-loop (legacy ref)   | `src/hls/conv77_core/`    |

## Synthesis Results — ZCU104 (xczu7ev-ffvc1156-2-e @ 300 MHz)

### Current: full_generator_top post-[D] PIPO (2026-06-04)

| Resource | Used    | Available | %   |
| :------- | :------ | :-------- | :-- |
| BRAM_18K | 510     | 624       | 82% |
| DSP      | 1550    | 1728      | 90% |
| LUT      | 205,233 | 230,400   | 89% |
| FF       | ~190K   | 460,800   | 41% |
| URAM     | 96      | 96        | **100%** |
| **Fmax** | **308.74 MHz** | 300 MHz | **✅ Pass** |

> **[D] PIPO double-buffer** (2026-06-04): w_local→URAM, x_buf→BRAM, UpConv mult→LUT (BIND_OP fabric). Overlap PRELOAD under MAC (~27% UpConv gain est.).
> Deterministic latency system: ~1.51s (post-[A][B][D]; baseline 2.78s = 1.84× speedup).

### Per-Block Breakdown (current, PEs_F=16, PEs_U=8)

| Block                       | BRAM | DSP  | FF      | LUT    | URAM |
| :-------------------------- | :--- | :--- | :------ | :----- | :--- |
| Universal_Engine (16-MAC)   | 208  | 558  | 62,081  | 51,574 | 16   |
| run_upconv_block (PEs_U=8)  | 144  | 576  | 56,246  | 45,921 | 36   |
| Conv77_Kernel (8×8 SIMD+PE) | 56   | 448  | 41,896  | 69,081 | 0    |
| Other / overhead            | 193  | 32   | 15,521  | 15,798 | 16   |
| **Total**                   | **601** | **1614** | **175,744** | **182,374** | **68** |

> Pre-[D] table; post-[D] resource shift (DSP −160, BRAM −91, LUT +22K) — see MEMORY fusion-opt-history.

## Latency Estimates (@300 MHz)

### Fusion Core (post BIAS_STATS fix, PEs_F=16)

| Block           | Cycles min | Latency min |
| :-------------- | :--------- | :---------- |
| CBI (1 call)    | 8,980,370  | 29.9 ms     |
| 1 ResBlock      | 17,960,745 | 59.9 ms     |
| 9× ResBlock     | 161,646,705| 538.8 ms    |
| GlobalAdd       | 15,507     | 51.7 µs     |

### UpConv Core (post [A]+[B]+[C] optimization, deterministic baseline from hand-calc)

| UCB   | Deterministic Latency |
| :---- | :-------------------- |
| UCB_0 | ~58.6 ms              |
| UCB_1 | ~49.7 ms              |
| UCB_2 | ~57.9 ms              |
| UCB_3 | ~116.0 ms             |
| **Total** | **~282 ms** (5× faster than baseline) |

> HLS min/max bounds (156ms–19s) too wide. Hand-calculated per-loop latency from actual trip counts.
> [D] PIPO overlap est. → ~148ms (−27%).

### Conv77 (SIMD+PE, integrated full_generator)

| Config             | Cycles    | Latency |
| :----------------- | :--------- | :------ |
| 256×256×60→3 | 5.3M–6.2M | 17.8–20.7 ms |

## Key Technical Decisions

### Why x_buf fusion = BRAM (not URAM)

`ARRAY_PARTITION complete dim=2` → 16 banks parallel → URAM would need 64 blocks (>96 limit).

### Rotating Accumulator (II=1 for FP16 MAC)

FP16 adder latency 3-4 cycles. Use `psum[PEs][4]` or `[PEs][8]` with `acc_idx = ciw & 3` (or `m & 7`).
Distance = depth ≥ latency → `#pragma HLS DEPENDENCE false` valid → II=1.

### HLS Single-Instance Sharing

**Engine sharing:** Pass dummy non-NULL `P_fusion` to all calls (CBI + 9×RB) → identical connectivity → 1 hardware instance.
**UpConv sharing:** Pass both X and Y, select via `mode` parameter → 1 shared run_upconv_block for all 4 UCBs.

### Conv77 integrated: Conv77_Kernel<> (not HW_Conv7x7)

`HW_Conv7x7` has conflicting AXI bundles. Use `Conv77_Kernel<8,8,1,60,3,256,256,7,7,256,256>()` directly; ports declared at top-level.

## Resource Budget & Constraints

| Resource | Limit | Used | Margin | Notes                                       |
| :------- | :---- | :--- | :----- | :------------------------------------------ |
| BRAM     | 624   | 510  | 114    | [D] reduced from 601; hard constraint on PEs_U |
| DSP      | 1728  | 1550 | 178    | [D] moved mult→LUT; Fusion 558, UpConv 576, Conv77 448 |
| URAM     | 96    | 96   | **0**  | **AT LIMIT** after [D] (w_local PIPO)       |
| LUT      | 230K  | 205K | 25K    | [D] fabric mult +22K; Fusion+Conv77 heavy   |
| FF       | 461K  | 190K | 271K   | ~41% utilized                               |
| Timing   | 3.333 ns (300 MHz) | **3.239 ns** | **✅ MET** | Fmax 308.74 MHz ✅ |

## CSIM Verification

**full_generator_top (2026-05-18, full Fusion→UCB→Conv77 pipeline, 7h sim):**

| Stage               | Config              | max_err    | rmse     | mismatch |
| :------------------ | :------------------ | :--------- | :------- | :------- |
| Fusion (CBI+9×RB)   | 16×16×960          | 12.3158    | 0.5124   | 0/245K   |
| UCB_3 chained       | →256×256×60        | 0.7151     | 0.010017 | 0/3.93M  |
| Conv77              | →256×256×3         | 0.8636     | 0.165422 | 0/196K   |

**Per-block (2026-05-14):** CBI/ResBlock/GlobalAdd/UCB all PASS with 0 mismatch. Conv77 PASS (TOL=1.0 for clipped output).

## Data Classification

- **Weights/Params:** `assets/test_data/model_params/` (prefix `Gen_*`)
- **Input/Output:** `io_params/` and `layer_test_vectors/`
- **Golden tensors:** layer-wise outputs, golden images clipped to [0,1]

## Related Memory / Archive

- **[[fusion-opt-history]]** — [A][B][C][D][E][F][F8] timeline, all synthesis results
- **[[known-issues-workarounds]]** — Instance duplication, memory walls, timing, precision, bug fixes
- **[[int16-resblock-hw]]** — int16 quantized path (separate experiment, 1 ResBlock only)
- **[[fusion-fill-e16-wip]]** — Fusion [E] 16-wide / [F] RPP=2 / [F8] fp8 benchmark (standalone)
