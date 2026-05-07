# conv77_core — Synthesis Report (2026-05-07)

## IP Overview

**Function:** `conv77_core_top` — Conv 7×7 final output layer  
**Input:** 256×256×60 (UCB_3 output, packed as `data_256_t[H×W×4]`)  
**Output:** 256×256×3 (final RGB, packed as `data_256_t[H×W×1]`, bits[47:0])  
**Target:** xczu7ev-ffvc1156-2-e @ 300 MHz (3.333 ns)

---

## CSIM Result

| Metric | Value | Pass? |
| :--- | :--- | :--- |
| Max absolute error | 0.00586 | ✅ (< 0.5) |
| RMSE | 0.000886 | ✅ |
| Mismatch (> 0.5) | 0 / 196,608 | ✅ |
| **Result** | **PASS** | ✅ |

Gold reference: `assets/test_data/io_params/Gen_cbo_Conv_output_0.txt`

---

## Synthesis Results

| Resource | Used | Available | % |
| :--- | :--- | :--- | :--- |
| BRAM_18K | 198 | 624 | 31% |
| DSP | 192 | 1728 | 11% |
| LUT | 25,838 | 230,400 | 11% |
| FF | 38,243 | 460,800 | 8% |
| **URAM** | **8** | **96** | **8%** |
| **CP Achieved** | **2.554 ns** | 3.333 ns | **✅ Pass** |

**Estimated Latency:** 35.6M–35.9M cycles → **~119–120 ms @ 300 MHz**

---

## Design Optimizations Applied

### 1. Flat Loop (11× Latency Speedup)

**Problem:** Original design dùng nested KH×KW×CI_LOOP. CI_LOOP có iteration latency = 91 cycles (L_MAC tree depth) nhưng chỉ 4 iterations → pipeline fill/drain overhead 91 cycles × 49 kernel positions = ~4,500 cycles/pixel → tổng 1.365 sec.

**Fix:** Gộp KH×KW×CI thành 1 `FLAT_LOOP` (196 iterations, II=1):
- `psum[co][k_ci]` write-once per iteration → **không có inter-iteration RAW dependency**
- Không cần `#pragma HLS DEPENDENCE false` trên psum
- Pipeline fill amortized over 196 iterations (thay vì 4): ~286 cycles/pixel
- `REDUCE` loop (196 iters, depth-4 rotating acc): ~200 cycles/pixel
- **Tổng: ~486 cycles/pixel → ~120 ms (vs 1.365 sec trước)**

### 2. URAM Reduction (16 → 8 blocks)

**Problem:** `ARRAY_PARTITION complete dim=3` trên `line_buf[7][256][4]` tạo 4 banks × 4 URAM per bank (256-bit width) = 16 URAM. Với full_generator đang dùng 80/96 URAM: 80+16=96/96=100% → không integrate được.

**Fix:** Bỏ `ARRAY_PARTITION complete dim=3` trên `line_buf`:
- Flat loop chỉ đọc 1 entry `line_buf[row_slot][abs_col][ci_w]` per iteration → không cần parallel access
- Single T2P URAM, depth 7×256×4=7168: 4 URAM wide × 2 URAM deep = **8 URAM**
- Integration budget: 80 (existing) + 8 = **88/96 (92%)** ✅

---

## Integration Budget (khi merge vào full_generator)

| Resource | full_generator | conv77 kernel | **Tổng** | Available | % |
| :--- | :--- | :--- | :--- | :--- | :--- |
| BRAM_18K | 416 | ~141 | **~557** | 624 | ~89% |
| DSP | 776 | 192 | **~968** | 1728 | ~56% |
| LUT | 86,544 | 19,129 | **~105,673** | 230,400 | ~46% |
| FF | 97,755 | 31,891 | **~129,646** | 460,800 | ~28% |
| URAM | 80 | 8 | **88** | 96 | **92%** ✅ |

---

## Source Files

```
src/hls/conv77_core/
  gen/
    Core.h                   # data_t=half, data_256_t=ap_uint<256>
    conv77_core.h            # Function declaration
    Hls_Layers_Conv77.tpp    # Conv77_Core kernel (flat loop)
    conv77_core_top.cpp      # AXI wrapper
  test.cpp                   # Testbench (real test data)
  csim_only.tcl
  hls_run.tcl
```

## Next Steps

1. Integrate `conv77_core` vào `full_generator_top` (thêm ports W_conv77, B_conv77, Z)
2. CSIM full generator
3. Synthesis full generator → kiểm tra timing và resources
