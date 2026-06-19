# fp8 32-wide Fusion Core — Profiling chi tiết khối con + Chi phí MAC theo kiểu dữ liệu

> Phân tích đầy đủ engine **`fusion_core_fp8` bản 32-wide** (`prj_fp8`, `FP8_LANES=32`):
> utilization / latency / efficiency của **từng khối con**, đào sâu mảng MAC từ **RTL**, và
> micro-benchmark **chi phí 1 phép MAC** cho fp32/fp16/fp8/int16/int8. Tất cả là **số thực từ
> Vitis HLS 2024.2 csynth** (xczu7ev-ffvc1156-2-e, target 3.333 ns / 300 MHz). Ngày: 2026-06-18
> (cập nhật 2026-06-19: fp16 nhân `a*b` giờ cho **product full-precision float32** — xem §9).

## Mục lục
1. [Tổng quan & scope](#1-tổng-quan--scope)
2. [Top-level `fp8_conv_top`](#2-top-level-fp8_conv_top)
3. [Cấu trúc phân cấp & dataflow](#3-cấu-trúc-phân-cấp--dataflow)
4. [Profiling khối con — Utilization](#4-profiling-khối-con--utilization)
5. [Profiling khối con — Latency, II, Efficiency](#5-profiling-khối-con--latency-ii-efficiency)
6. [Đào sâu MAC array (từ RTL)](#6-đào-sâu-mac-array-từ-rtl)
7. [Phân tích efficiency engine](#7-phân-tích-efficiency-engine)
8. [Throughput: GOPS & GOPS/DSP](#8-throughput-gops--gopsdsp)
9. [Micro-benchmark chi phí MAC theo kiểu](#9-micro-benchmark-chi-phí-mac-theo-kiểu)
10. [Kết luận tổng](#10-kết-luận-tổng)
11. [Nguồn](#11-nguồn)

---

## 1. Tổng quan & scope
- Engine: `Fp8_Conv_Engine<PEs=16, LANES=32>` — benchmark conv fp8 (E4M3), **KHÔNG drop vào full_generator** (vượt LUT).
- Scope: **1 ResBlock conv 960→960**, spatial 16×16, kernel 3×3 (RB_L1 path). MAC = 16·16·960·960·9 = **2.123 G**.
- Operand conv **fp8** pack 32/word-256b → `CI_W = 960/32 = 30`; accumulate + norm + output giữ **fp16**.
- `prj_fp8` = 32-wide (2×), `prj_fp8_16` = 16-wide (iso-throughput, `-DFP8_LANES=16`). Tài liệu này = **32-wide**.

## 2. Top-level `fp8_conv_top`
| Resource | Used | % | Ghi chú |
| :--- | ---: | ---: | :--- |
| BRAM_18K | 412 | 66% | engine 296 + AXI 116 |
| DSP | 1141 | 66% | toàn bộ ở engine |
| FF | 264,635 | 57% | |
| LUT | **194,956** | **84%** | nút thắt |
| URAM | 0 | 0% | |

- **Timing:** CP est **2.706 ns** → **Fmax 369.55 MHz** (slack +0.627 so target 3.333).
- **Latency:** 4,426,560 – 4,441,920 cy = **11.98 ms @369.55 MHz** (= 14.75 ms nếu ép 300 MHz).
- AXI adapter: `gmem_data` 58 BRAM, `gmem_weight` 29, `gmem_param` 29 (DSP 0).

## 3. Cấu trúc phân cấp & dataflow
```
Fp8_Conv_Engine (4,426,560 cy)
 ├─ LOAD_PARAMS              ×1     254 cy        (nạp B/G/BE)
 └─ Slide_PEs (r = 0..15)    ×16    (16 hàng output)
     ├─ RB_LOAD (IFM panel fp8)        r=0:1518 / r≥1:558 cy
     ├─ #pragma HLS DATAFLOW { producer ‖ consumer }
     │     ├─ fp8_mac_producer (PASS_A_FLAT)  259,456 cy   ← MAC 512-wide
     │     └─ fp8_reduce_consumer (CONS)       15,449 cy   ← reduce→fp16 + bias + stats
     └─ PASS_B + WRITE_OFM        1,105 cy     (norm fp16: mean/var/inv + ReLU + ghi)
```
> `Slide_PEs` trip 16 = H (16 hàng output). LOAD_PARAMS chạy 1 lần trước vòng; 4 khối còn lại lặp 16 lần.

## 4. Profiling khối con — Utilization
| Khối con | DSP | FF | LUT | BRAM | Vai trò |
| :--- | ---: | ---: | ---: | ---: | :--- |
| **mac_producer** (PASS_A_FLAT) | **1000** | 198,176 | **167,686** | 16 | MAC array 512-wide (fp8 nhân + fp16 cộng) |
| reduce_consumer (CONS) | 0 | 4,660 | 6,859 | 0 | reduce tree→fp16 + bias + sum/sumsq |
| PASS_B + WRITE_OFM | 102 | 9,251 | 5,430 | 0 | mean/var/inv + (val−mean)·inv·g+be + ReLU |
| LOAD_PARAMS | 0 | 41,273 | 773 | 0 | nạp B/G/BE (registers) |
| RB_LOAD | 0 | 1,331 | 2,885 | 0 | nạp IFM panel fp8 |
| norm scalar (fadd/faddfsub/fmul + 16×hadd) | 39 | ~2,200 | ~2,400 | 0 | reduce sum/var cho PASS_B |
| engine buffers (x_buf / y_cache / g/b/be) | 0 | 0 | 0 | 280 | on-chip storage |
| **Engine tổng** | **1141** | 258,858 | 189,011 | 296 | |

> **mac_producer chiếm 87.6% DSP và 88.7% LUT của engine** → đúng là "hog".

## 5. Profiling khối con — Latency, II, Efficiency
| Khối con | II | Trip | Iter-lat | Cy/call | Calls | Total cy | % engine |
| :--- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| **mac_producer / PASS_A_FLAT** | **1** ✅ | 259,200 | 149 | 259,456 | 16 | **4,151,296** | **93.8%** |
| reduce_consumer / CONS | **1** ✅ | 15,360 | 68 | 15,449 | 16 | 247,184 | 5.58% |
| PASS_B + WRITE_OFM | 1 | 960 | — | 1,105 | 16 | 17,680 | 0.40% |
| RB_LOAD | 1 | 30/px | — | 558–1518 | 16 | 9,888 | 0.22% |
| LOAD_PARAMS | (183) | 60 | — | 254 | 1 | 254 | 0.006% |
| **Engine** | — | — | — | — | — | **4,426,560** | 100% |

**Efficiency (so MAC-bound 4,147,200 cy = 2.123 G ÷ 512):**
- **MAC core (PASS_A_FLAT): II=1, 4,147,200 / 4,151,296 = 99.9%** — gần như hoàn hảo, 512 MAC/cy.
- **Engine tổng: 4,147,200 / 4,426,560 = 93.7%** MAC-efficiency.

## 6. Đào sâu MAC array (từ RTL)
1 PE = 32 lane fp8 → **32 nhân `mul_4ns_4ns_8` (4×4→8) + 31 cộng fp16 (balanced tree)**. ×16 PE:

| Thành phần | Số (RTL đếm) | Tài nguyên | Lý do |
| :--- | ---: | :--- | :--- |
| nhân mantissa fp8 (4×4) | **512** (514 inst) | **LUT** (~49K Expression LUT) | nhân 4-bit quá nhỏ → LUT, **0 DSP** |
| adder tree fp16 (`hadd_16`) | **496** | **DSP** (×2 = ~992 ≈ 1000) | accumulate fp16 → DSP |
| nhân DSP (hmul/fmul) | **0** | — | xác nhận: MAC core không có nhân-DSP |

> Kiểm chứng từ file RTL `..._PASS_A_FLAT.v`: **496 instance `hadd_16ns...full_dsp`**, **0 `hmul/fmul`**,
> **514 instance `mul_4ns_4ns_8`** (512 lane + 2). → **1000 DSP của MAC array 100% là *cộng*, không
> phải *nhân*.** fp8 đẩy nhân khỏi DSP nhưng adder-tree fp16 (31 cộng/PE) lấp lại chỗ DSP vừa nhả.

## 7. Phân tích efficiency engine
6.3% mất so MAC-bound = chủ yếu do **reduce_consumer KHÔNG được `DATAFLOW` giấu**:
- Per-row csynth tính `dataflow ≈ producer + consumer` (nối tiếp), consumer cộng **5.6%** (247k cy).
- norm (PASS_B) + IFM load (RB_LOAD): +0.6%.
- ⚠ Đây là điểm tối ưu khả dĩ: nếu overlap thật được consumer dưới producer → engine ~4.17M cy (~99%).

## 8. Throughput: GOPS & GOPS/DSP
- **512 MAC/cy, II=1.**
- Peak GOPS = MAC/cy × 2 × f:
  - @300 MHz: 512 × 2 × 300e6 = **307.2 GOPS**; đạt (93.7%) = **287.8 GOPS**.
  - @Fmax 369.55: **378.4 GOPS**; đạt = **354.6 GOPS**.
- **GOPS/DSP** (MAC array, 1000 DSP): **0.307 @300** / 0.378 @Fmax. Engine (1141 DSP): 0.252 @300.

> So sánh DSP-efficiency (per-MAC, xem mục 9): fp8 2 DSP/MAC — tốt hơn fp16 full-precision (5) 2.5× nhưng kém int16/int8 (1).
> Thế mạnh thật của fp8 KHÔNG ở GOPS/DSP mà ở **mật độ băng thông** (1 byte/phần tử → 2× phần tử/word).

---

## 9. Micro-benchmark chi phí MAC theo kiểu
Cô lập **đúng 1 phép tính/kernel** (nhân, cộng, MAC `a*b+c`), return-by-value (cổng `ap_return`,
không AXI → DSP/LUT thuần arithmetic). Source `src/hls/mac_probe/`.

### 9.1. Kết quả — DSP / LUT / FF
| Phép | fp32 | fp16 | fp8 (E4M3) | int16 | int8 |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **nhân `a*b`** | 3 / 166 / 204 | **3 / 177 / 270** | **0** / 123 / 19 | 1 / 5 / 0 | **0** / 40 / 0 |
| **cộng (accumulate)** | 2 / 301 / 380 | 2 / 144 / 114 | 2 / 144 / 114 | 0 / 39 / 0 | 0 / 39 / 0 |
| **MAC `a*b+c`** | **5** / 456 / 616 | **5 / 465 / 682** | **2** / 265 / 147 | **1** / 26 / 4 | **1** / 26 / 4 |

> Mỗi ô: **DSP / LUT / FF**. Hàng "cộng" = bộ cộng fp16-native (half) dùng cho *cộng dồn*; fp8 tích lũy
> ở **fp16** nên trùng cột fp16 (2/144/114); int16/int8 cộng ở **int32** (0/39/0).
> Quan hệ MAC = nhân + cộng: fp8 `2 = 0+2`, int16 `1 = 1+0`, fp32 `5 = 3+2`.
>
> **fp16 (cập nhật 2026-06-19):** nhân `fp16×fp16` giờ cho **product full-precision float32** (không ép
> về half), nên cả nhân (3 DSP, = fp32) lẫn cộng-tích-lũy (float32, 2 DSP) đều bằng fp32 → MAC `5 = 3+2`
> ≈ fp32. Hàng "cộng" cột fp16 (2/144/114) là bộ cộng **fp16-half** mà fp8 dùng, KHÔNG phải bộ cộng của
> MAC fp16 full-precision (bộ đó là cộng **float32**, cột fp32 = 2/301/380).

### 9.2. DSP mỗi MAC — xếp hạng
| Kiểu | DSP/MAC | LUT/MAC | Gộp `×+` vào 1 DSP? | Cơ chế |
| :--- | :---: | :---: | :---: | :--- |
| **int8** | **1** | 26 | ✅ | DSP48E2 `P = A×B + C` (số nguyên) |
| **int16** | **1** | 26 | ✅ | như trên |
| **fp8** | 2 | 265 | ❌ | nhân→LUT (0 DSP), cộng fp16→2 DSP |
| **fp16** | **5** | 465 | ❌ | nhân full-precision→float32 (3) + cộng float32 (2) ⇒ = fp32 |
| **fp32** | 5 | 456 | ❌ | 3 (nhân) + 2 (cộng) |

### 9.3. Đọc kết quả
- **Số nguyên gộp `×+` vào 1 DSP — float thì không (đo được).** int16/int8 MAC = 1 DSP (cộng dồn
  gập miễn phí vào tích lũy DSP48E2). Float: MAC = nhân + cộng đúng tổng → adder float là khối DSP riêng.
- **fp16 full-precision = fp32.** Khi nhân `fp16×fp16` giữ product không mất mát → ra **float32**, nên
  nhân = 3 DSP (= fp32) và MAC = 5 DSP; LUT/MAC (465) còn nhỉnh hơn fp32 (456) do thêm convert half→float
  ở input. fp16 **chỉ** rẻ hơn fp32 nếu chấp nhận **truncate product về half** (như mảng [E] thật) → khi
  đó MAC ~4 DSP. Tức "fp16 rẻ" là đánh đổi precision, không miễn phí.
- **fp8 = bộ chuyển DSP→LUT:** nhân 0 DSP/123 LUT, cộng fp16 2 DSP → MAC 2 DSP; LUT/MAC = 265 (cao thứ 3,
  sau fp16 465 và fp32 456).
- **Số nguyên thắng cả 2 trục:** 1 DSP + 26 LUT, rẻ hơn fp8 và fp16 ở mọi mặt.

### 9.4. Đối chiếu micro-benchmark ↔ array thật
| Hiện tượng array đầy đủ | Micro-benchmark giải thích |
| :--- | :--- |
| fp8-32: 512 MAC nhưng **~1000 DSP** | nhân fp8 = 0 DSP; **496 adder fp16 × 2 = ~992** → DSP toàn là cộng |
| fp8-32 LUT nổ (167K mac_producer) | nhân fp8 = 123 LUT × 512 ≈ 63K + ráp |
| resblock_q int16 đo **2.44 DSP/MAC** | MAC int16 thuần chỉ **1 DSP**; phần dư là **requantize** (nhân int64), KHÔNG phải MAC |
| fp16 [E] array ~3.5 DSP/MAC | array [E] **truncate product về fp16** (rẻ) → ~3.5–4 DSP/MAC; benchmark mới giữ product **float32** nên = fp32 (5 DSP). Chênh lệch chính là **precision của product** |

---

## 10. Kết luận tổng
1. **fp8-32 fusion core đạt MAC gần tối ưu:** MAC core II=1, **99.9%** MAC-efficiency; engine **93.7%**
   (điểm rò rỉ duy nhất = reduce_consumer nối tiếp 5.6%, có thể overlap thêm).
2. **1000 DSP của mảng MAC = 100% adder fp16**, không phải nhân (RTL xác nhận: 496 hadd, 0 nhân-DSP,
   512 nhân fp8 ở LUT). fp8 = **dời DSP→LUT**, không tạo hiệu quả tổng.
3. **Chi phí DSP/MAC (đo): int8 = int16 = 1 < fp8 = 2 < fp16 = fp32 = 5** (fp16 nhân full-precision ra
   float32 ⇒ = fp32; chỉ ~4 DSP nếu truncate product về half như mảng [E]). "1 DSP = 1 MAC" chỉ đúng với
   số nguyên (DSP48E2 gập `×+` native); float không bao giờ gập được.
4. **Số nguyên rẻ nhất cả DSP lẫn LUT per-MAC** → chọn khi nút thắt là DSP/compute. **fp8 chỉ đáng khi
   nút thắt là băng thông bộ nhớ** (2× phần tử/byte), không phải vì rẻ DSP.
5. Giá của số nguyên: cần lượng tử hóa + **requantize** mỗi layer — requant mới là phần ăn DSP/LUT thêm
   (xem `resblock_q`), không phải bản thân MAC.

## 11. Nguồn
| Loại | Đường dẫn |
| :--- | :--- |
| Engine fp8-32 source | `src/hls/fusion_core_fp8/gen/Hls_Layers_Fp8.tpp` |
| Top wrapper | `src/hls/fusion_core_fp8/gen/fp8_conv_top.cpp` |
| Report fp8-32 | `src/hls/fusion_core_fp8/prj_fp8/solution1/syn/report/*.rpt` |
| RTL MAC core | `.../prj_fp8/solution1/syn/verilog/..._fp8_mac_producer_16_Pipeline_PASS_A_FLAT.v` |
| MAC-cost benchmark | `src/hls/mac_probe/mac_probe.cpp` + `run_probe.tcl` |
| Report MAC-cost | `src/hls/mac_probe/prj_<top>/sol/syn/report/<top>_csynth.rpt` |

> Mọi số là **số thực từ csynth + RTL** (không estimate). Số per-MAC ở mục 9 là chi phí mức 1 phép;
> array đầy đủ cộng thêm overhead reduce/norm/requant/load.
