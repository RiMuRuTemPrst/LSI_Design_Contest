#pragma once
#include "Core.h"

#if !defined(__SYNTHESIS__)
  #include <cmath>
  #include <cstdio>
#endif
#if defined(__SYNTHESIS__)
  #include <hls_math.h>
#endif
#include <cstring>

// ============================================================
// int16 quantized ResBlock engine — "match" of the fp16 fusion_core_fill [A]
// flatten arch, retyped to integer. The fp16 producer/consumer DATAFLOW split
// existed only to keep the fp16 reduce tree off the LUT; the integer reduce is
// cheap, so here STEP-2 is ONE flat II=1 MAC loop with an inline co-boundary
// epilogue (8-slot reduce + bias + requantize + LayerNorm stats + y_cache).
// Arithmetic is bit-faithful to test_7_quantize/resblock_quantize_i16{,_f}.cpp.
// ============================================================

// ---- pack/unpack int16 ----
static inline int s16(ap_uint<16> b) {            // unpack signed int16 -> int
#pragma HLS INLINE
    return (short)(unsigned short)b.to_uint();
}
static inline ap_uint<16> u16(int v) {            // pack int16
#pragma HLS INLINE
    return ap_uint<16>((unsigned short)v);
}
static inline short sat16(long long v) {          // saturating cast to int16
#pragma HLS INLINE
    if (v >  32767) return  32767;
    if (v < -32768) return -32768;
    return (short)v;
}
static inline int wq_flat_idx(int co, int kh, int kw, int ci, int C) {
#pragma HLS INLINE
    return ((co * 3 + kh) * 3 + kw) * C + ci;
}
static inline int flatq_idx(int h, int w, int c, int W, int C) {
#pragma HLS INLINE
    return (h * W + w) * C + c;
}
// _mul_quantize_multiplier: prod = x*M; >>shift (round half-up) or <<(-shift)
static inline long long mulq(long long x, long long M, int shift) {
#pragma HLS INLINE
    long long prod = x * M;
    if (shift > 0)      prod = (prod + (1LL << (shift - 1))) >> shift;
    else if (shift < 0) prod = prod << (-shift);
    return prod;
}

// ============================================================
// reciprocal sqrt: two variants (the ONLY difference between the 2 HW builds)
//   RSQRT_INT (default): integer Newton-Raphson, pure mult+shift
//   RSQRT_FLT          : float 1/sqrt + frexp -> (mult, shift)
// ============================================================
#ifdef RSQRT_FLT
static inline float frexp_q(float x, int* e) {
#pragma HLS INLINE
#if defined(__SYNTHESIS__)
    union { float f; uint32_t u; } cv; cv.f = x;
    int exp = (int)((cv.u >> 23) & 0xFF) - 126;       // x = mant * 2^exp, mant in [0.5,1)
    cv.u = (cv.u & 0x807FFFFFu) | (126u << 23);
    *e = exp; return cv.f;
#else
    return std::frexp(x, e);
#endif
}
static inline float invsqrt_q(float x) {
#pragma HLS INLINE
#if defined(__SYNTHESIS__)
    return 1.0f / hls::sqrtf(x);
#else
    return 1.0f / std::sqrt(x);
#endif
}
static void reci_sqrt_q(int32_t x, int32_t* m, int8_t* s) {
    float scale = invsqrt_q((float)x);
    if (scale == 0.0f) { *m = 0; *s = 0; return; }
    int exp = 0;
    float mant = frexp_q(scale, &exp);
    int32_t mul = (int32_t)(mant * (float)(1LL << 30));   // bit_width-1 = 30
    int8_t  sh  = (int8_t)(30 - exp);
    *m = mul; *s = sh;
}
#else  // RSQRT_INT
static int8_t differentiate(int32_t x) {
#pragma HLS INLINE
    if (x >= 1840700270) return 16;
    if (x >= 460175068)  return 15;
    if (x >= 115043767)  return 14;
    if (x >= 28760942)   return 13;
    if (x >= 7190236)    return 12;
    if (x >= 1797559)    return 11;
    if (x >= 449390)     return 10;
    if (x >= 112348)     return 9;
    if (x >= 28087)      return 8;
    if (x >= 7022)       return 7;
    if (x >= 1756)       return 6;
    if (x >= 439)        return 5;
    if (x >= 110)        return 4;
    if (x >= 28)         return 3;
    if (x >= 7)          return 2;
    if (x >= 2)          return 1;
    return 0;
}
static void reci_sqrt_q(int32_t x, int32_t* m, int8_t* s) {
    int64_t mult;
    int8_t first_shift = differentiate(x);
    int64_t _3_2s = 3LL << (first_shift << 1);
    mult = (_3_2s - x) >> (first_shift + 1);
    for (int i = 0; i < 2; i++) {
        mult = mult * (_3_2s - (x * (mult * mult >> first_shift) >> first_shift)) >> ((first_shift << 1) + 1);
    }
    mult = mult * (_3_2s - (x * (mult * mult >> first_shift) >> first_shift));
    int8_t final_shift = (first_shift << 2) + 1;
    int8_t check = final_shift - 32;
    if (check > 0) { mult = mult >> check; final_shift = 32; }
    *m = (int32_t)mult; *s = final_shift;
}
#endif

// ============================================================
// Channel-norm FINALIZE (mean / var / rsqrt) for one output column (one PE).
// Split out so a `-DPROFILE_MODULES` build exposes the Norm-stats stage as its
// own RTL module + csynth report line. In production (macro undefined) it is
// INLINE => folded back into PASS_B exactly as before => BIT-IDENTICAL and
// timing-neutral. requant is NOT separable (it lives inside the II=1 MAC loop;
// HLS force-inlines functions in pipelined regions) — only Norm-stats is exposed.
// ============================================================
static void rb_norm_finalize(int sum_acc, long long sumsq_acc,
                             int reci_C_mult, int reci_C_shift, int epsilon,
                             int* mean_x_out, int32_t* mult_inv_out, int8_t* shift_inv_out) {
#ifdef PROFILE_MODULES
#pragma HLS INLINE off
#else
#pragma HLS INLINE
#endif
    long long mean_ll   = ((long long)sum_acc   * reci_C_mult) >> reci_C_shift;
    long long meansq_ll = ((long long)sumsq_acc * reci_C_mult) >> reci_C_shift;
    int mean_x = (int)mean_ll;
    int var    = (int)(meansq_ll - (long long)mean_x * mean_x);
    reci_sqrt_q(var + epsilon, mult_inv_out, shift_inv_out);
    *mean_x_out = mean_x;
}

// ============================================================
// RESBLOCK ENGINE KERNEL — one conv3x3 + requant + channel-norm (+L2 add)
// ============================================================
template<int PEs>
static void Resblock_Engine_Kernel(
    DDR_CONST_PTR X_ptr,
    DDR_CONST_PTR W_ptr,
    const int32_t* B_ptr,
    const int32_t* G_ptr,
    const int8_t*  GS_ptr,
    const int32_t* BE_ptr,
    DDR_PTR        Out_ptr,
    data_256_t*    skip_buf,
    int M_conv, int N_conv, int x_zp, int conv_zp,
    int reci_C_mult, int reci_C_shift, int epsilon,
    int M_path, int N_path, int M_skip, int N_skip, int res_zp,
    int mode)
{
#pragma HLS INLINE off
    constexpr int H = MODEL_H;
    constexpr int W = MODEL_W;
    const bool is_l1   = (mode == 1);
    const bool is_l2   = (mode == 2);
    const int  CI_PAD  = 960;
    const int  CI_W    = CI_PAD / 16;   // 60
    const int  CI8     = CI_PAD / 8;    // 120

    // --- on-chip buffers ---
    #if defined(__SYNTHESIS__)
    static data_256_t x_buf[4][W][60];
    #else
    data_256_t x_buf[4][W][60];
    #endif
#pragma HLS BIND_STORAGE variable=x_buf type=ram_2p impl=bram
#pragma HLS ARRAY_PARTITION variable=x_buf complete dim=2

    data_256_t y_cache[960];
#pragma HLS BIND_STORAGE variable=y_cache type=ram_2p impl=bram
#pragma HLS ARRAY_PARTITION variable=y_cache cyclic factor=16 dim=1

    int32_t B_buf[960], G_buf[960], BE_buf[960];
    int8_t  GS_buf[960];
#pragma HLS ARRAY_PARTITION variable=G_buf  cyclic factor=16
#pragma HLS ARRAY_PARTITION variable=BE_buf cyclic factor=16
#pragma HLS ARRAY_PARTITION variable=GS_buf cyclic factor=16

    LOAD_PARAMS: for (int i = 0; i < 960; i++) {
#pragma HLS PIPELINE II=1
        B_buf[i] = B_ptr[i]; G_buf[i] = G_ptr[i]; BE_buf[i] = BE_ptr[i]; GS_buf[i] = GS_ptr[i];
    }

    Slide_PEs:
    for (int r = 0; r < H; r++) {
#if !defined(__SYNTHESIS__)
        printf("  [Mode %d] Row %d/%d\n", mode, r + 1, H); fflush(stdout);
#endif
        int   sum_acc[PEs];
        long long sumsq_acc[PEs];
#pragma HLS ARRAY_PARTITION variable=sum_acc complete
#pragma HLS ARRAY_PARTITION variable=sumsq_acc complete
        for (int p = 0; p < PEs; p++) { sum_acc[p] = 0; sumsq_acc[p] = 0; }

        // ---------- STEP 1: slide window load (+save residual to skip in L1) ----------
        int lc = (r == 0) ? 3 : 1;
        for (int i = 0; i < lc; i++) {
            int h   = (r == 0) ? (i - 1) : (r + 1);
            int h_c = (h < 0) ? -h : ((h >= H) ? (H << 1) - h - 2 : h);
            int slot = (h + 1) & 3;
            for (int w = 0; w < W; w++) {
                int ddr_base = flatq_idx(h_c, w, 0, W, MODEL_C) / 16;
                RB_LOAD: for (int j = 0; j < 60; j++) {
#pragma HLS PIPELINE II=1
                    data_256_t word = X_ptr[ddr_base + j];
                    x_buf[slot][w][j] = word;
                    if (is_l1 && h >= 0 && h < H) skip_buf[((h * W) + w) * 60 + j] = word;
                }
            }
        }

        // ---------- STEP 2: flat MAC (II=1) + inline co-boundary epilogue ----------
        data_256_t w_buf[2][60];
#pragma HLS ARRAY_PARTITION variable=w_buf complete dim=1
        int w_init = wq_flat_idx(0, 0, 0, 0, CI_PAD) / 16;
        for (int j = 0; j < CI_W; j++) {
#pragma HLS PIPELINE II=1
            w_buf[0][j] = W_ptr[w_init + j];
        }

        ap_int<48> psum[2][PEs][8];
#pragma HLS ARRAY_PARTITION variable=psum complete dim=1
#pragma HLS ARRAY_PARTITION variable=psum complete dim=2
#pragma HLS ARRAY_PARTITION variable=psum complete dim=3
        data_256_t w_reg; data_256_t x_regs[PEs];
#pragma HLS ARRAY_PARTITION variable=x_regs complete

        const int FLAT_N = 960 * 9 * CI8;
        int co = 0, step = 0, ci8 = 0;
        PASS_A_FLAT: for (int m = 0; m < FLAT_N; m++) {
#pragma HLS PIPELINE II=1
#pragma HLS DEPENDENCE variable=psum type=inter false
#pragma HLS DEPENDENCE variable=sum_acc type=inter false
#pragma HLS DEPENDENCE variable=sumsq_acc type=inter false
#pragma HLS LOOP_TRIPCOUNT min=1036800 max=1036800
            int acc_idx = ci8 & 7, widx = ci8 >> 1, boff = (ci8 & 1) << 3;
            int bank = co & 1;
            int kh = step / 3, kw = step % 3;
            int slot = (r + kh) & 3;
            int ping = (co + step) & 1;
            int ns = step + 1, wn_base = 0; bool do_load = false;
            if (ns < 9)        { wn_base = wq_flat_idx(co,     ns / 3, ns % 3, 0, CI_PAD) / 16; do_load = true; }
            else if (co < 959) { wn_base = wq_flat_idx(co + 1, 0,      0,      0, CI_PAD) / 16; do_load = true; }

            if (boff == 0) w_reg = w_buf[ping][widx]; else w_reg >>= 128;
            int w0 = s16(w_reg.range(15,0)),   w1 = s16(w_reg.range(31,16)),
                w2 = s16(w_reg.range(47,32)),  w3 = s16(w_reg.range(63,48)),
                w4 = s16(w_reg.range(79,64)),  w5 = s16(w_reg.range(95,80)),
                w6 = s16(w_reg.range(111,96)), w7 = s16(w_reg.range(127,112));
            for (int p = 0; p < PEs; p++) {
#pragma HLS UNROLL
                int wx = p + (kw - 1);
                int wc = (wx < 0) ? -wx : ((wx >= W) ? (W << 1) - wx - 2 : wx);
                if (boff == 0) x_regs[p] = x_buf[slot][wc][widx]; else x_regs[p] >>= 128;
                int x0 = s16(x_regs[p].range(15,0)),   x1 = s16(x_regs[p].range(31,16)),
                    x2 = s16(x_regs[p].range(47,32)),  x3 = s16(x_regs[p].range(63,48)),
                    x4 = s16(x_regs[p].range(79,64)),  x5 = s16(x_regs[p].range(95,80)),
                    x6 = s16(x_regs[p].range(111,96)), x7 = s16(x_regs[p].range(127,112));
                long long prod =
                    (long long)(x0 - x_zp) * w0 + (long long)(x1 - x_zp) * w1 +
                    (long long)(x2 - x_zp) * w2 + (long long)(x3 - x_zp) * w3 +
                    (long long)(x4 - x_zp) * w4 + (long long)(x5 - x_zp) * w5 +
                    (long long)(x6 - x_zp) * w6 + (long long)(x7 - x_zp) * w7;
                if (step == 0 && ci8 < 8) psum[bank][p][acc_idx]  = (ap_int<48>)prod;
                else                      psum[bank][p][acc_idx] += (ap_int<48>)prod;
            }
            if (do_load && boff == 0) w_buf[1 - ping][widx] = W_ptr[wn_base + widx];

            if (step == 8 && ci8 == CI8 - 1) {            // co boundary: reduce+bias+requant+stats
                data_256_t yword = 0;
                for (int p = 0; p < PEs; p++) {
#pragma HLS UNROLL
                    ap_int<48> acc = 0;
                    for (int l = 0; l < 8; l++) acc += psum[bank][p][l];
                    long long a   = (long long)acc + (long long)B_buf[co];
                    long long prq = mulq(a, (long long)M_conv, N_conv);
                    short yq = sat16((long long)conv_zp + prq);
                    yword.range(16*p+15, 16*p) = u16(yq);
                    sum_acc[p]   += (int)yq;
                    sumsq_acc[p] += (long long)yq * yq;
                }
                y_cache[co] = yword;
            }

            if (ci8 == CI8 - 1) { ci8 = 0; if (step == 8) { step = 0; co++; } else step++; }
            else ci8++;
        }

        // ---------- STEP 3: channel-norm (+ L2 quantized add) -> Out ----------
        PASS_B: for (int p = 0; p < PEs; p++) {
            int mean_x; int32_t mult_inv; int8_t shift_inv;
            rb_norm_finalize(sum_acc[p], sumsq_acc[p], reci_C_mult, reci_C_shift, epsilon,
                             &mean_x, &mult_inv, &shift_inv);

            int out_px_base = (r * W + p) * 60;
            WRITE_OFM: for (int j = 0; j < 60; j++) {
#pragma HLS PIPELINE II=1
                data_256_t out_w = 0;
                data_256_t sk_w  = is_l2 ? skip_buf[out_px_base + j] : (data_256_t)0;
                for (int k = 0; k < 16; k++) {
#pragma HLS UNROLL
                    int co2 = j * 16 + k;
                    int g = G_buf[co2], be = BE_buf[co2]; int gs = GS_buf[co2];
#ifdef RSQRT_FLT
                    int mult_comb  = (int)(((long long)mult_inv * g) >> 31);
                    int shift_comb = gs + (int)shift_inv - 31;
#else
                    int mult_comb  = (int)(((long long)mult_inv * g) >> ((int)shift_inv - 4));
                    int shift_comb = gs + 4;
#endif
                    int xv = s16(y_cache[co2].range(16*p+15, 16*p));
                    long long val = (long long)(xv - mean_x) * mult_comb;
                    if (shift_comb >= 0) val >>= shift_comb; else val <<= -shift_comb;
                    short nq = sat16(val + (long long)be);

                    short act;
                    if (is_l2) {
                        short skv = s16(sk_w.range(16*k+15, 16*k));
                        long long p1 = mulq((long long)nq,  (long long)M_path, N_path);
                        long long p2 = mulq((long long)skv, (long long)M_skip, N_skip);
                        act = sat16((long long)res_zp + p1 + p2);
                    } else {
                        act = nq;
                    }
                    out_w.range(16*k+15, 16*k) = u16(act);
                }
                Out_ptr[out_px_base + j] = out_w;
            }
        }
    }
}
