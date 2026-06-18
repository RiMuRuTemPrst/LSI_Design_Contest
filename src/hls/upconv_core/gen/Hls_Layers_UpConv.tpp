#pragma once
#include "Core.h"

#if !defined(__SYNTHESIS__)
  #include <cassert>
  #include <cmath>
  #include <cstdio>
#endif
#if defined(__SYNTHESIS__)
  #include <hls_math.h>
#endif
#include <cstring>

// ============================================================
// SHARED HELPER FUNCTIONS
// ============================================================

#ifndef HLS_HALF_HELPERS_DEFINED
#define HLS_HALF_HELPERS_DEFINED

template <typename T>
static inline ap_uint<16> half_to_bits(T val) {
#pragma HLS INLINE
#if defined(__SYNTHESIS__)
    union { T f; uint16_t i; } u;
    u.f = val;
    return ap_uint<16>(u.i);
#else
    ap_uint<16> out;
    unsigned short temp;
    std::memcpy(&temp, &val, sizeof(unsigned short));
    out = temp;
    return out;
#endif
}

template <typename T>
static inline T bits_to_half(ap_uint<16> bits) {
#pragma HLS INLINE
#if defined(__SYNTHESIS__)
    union { uint16_t i; T f; } u;
    u.i = bits.to_uint();
    return u.f;
#else
    T out;
    unsigned short val = bits.to_uint();
    std::memcpy(&out, &val, sizeof(unsigned short));
    return out;
#endif
}

static inline float my_sqrt_f(float x) {
#pragma HLS INLINE
#if defined(__SYNTHESIS__)
    return hls::sqrtf(x);
#else
    return std::sqrt(x);
#endif
}

#endif // HLS_HALF_HELPERS_DEFINED

// ============================================================
// UPCONV FUSED ROW — [A]+[B]+[C-pipe]+[D] (synced from full_generator canonical)
//   [D] PIPO double-buffer: uc_load_tile (producer) ‖ uc_compute_tile (consumer)
//       overlap PRELOAD‖MAC via DATAFLOW; wbuf auto-PIPO.
//   fabric-fix: BIND_OP hmul impl=fabric → fp16 mult to LUT, frees DSP.
//   DIVERGENCE kept: this (upconv_core) copy uses seq-reduce + NO var-clamp in
//   PIXEL_STATS (full_generator uses balanced-tree + var<0 clamp). Numerically a
//   no-op on real data (var never < 0) → csim stays bit-exact vs prior upconv_core.
// ============================================================

// [DBUF-DATAFLOW] tile producer/consumer for canonical PIPO double-buffer (overlap PRELOAD‖MAC)
template<int PEs>
static void uc_load_tile(const data_256_t* W_ptr, int tile, int C_OUT, int CI_WORDS,
                         data_256_t wbuf[PEs][540]) {
#pragma HLS INLINE off
    int co_base = tile * PEs;
    PRELOAD_W: for (int tc = 0; tc < PEs; tc++) {
        int co = co_base + tc;
        if (co < C_OUT) {
            int k_cnt = 0, ci_cnt = 0;
            W_FLAT: for (int kci = 0; kci < 9 * CI_WORDS; kci++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=72 max=540 avg=252
                wbuf[tc][k_cnt * 60 + ci_cnt] = W_ptr[co * 9 * CI_WORDS + kci];
                ci_cnt++;
                if (ci_cnt >= CI_WORDS) { ci_cnt = 0; k_cnt++; }
            }
        }
    }
}

template<int PEs>
static void uc_compute_tile(data_256_t wbuf[PEs][540], data_256_t* x_buf,
                            data_t row_acc[256][480], int tile, int ho,
                            int H_IN, int W_IN, int CI_WORDS, int W_OUT, int C_OUT) {
#pragma HLS INLINE off
    const int S = 2, PAD = 1;
    int co_base = tile * PEs;
    KH_LOOP: for (int kh = 0; kh < 3; kh++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=2
        int hpk = ho + PAD - kh;
        if (hpk < 0 || hpk % S != 0) continue;
        int hi = hpk / S;
        if (hi < 0 || hi >= H_IN) continue;
        int x_row = hi % 2;

        KW_LOOP: for (int kw = 0; kw < 3; kw++) {
#pragma HLS LOOP_TRIPCOUNT min=2 max=3
            int k = kh * 3 + kw;
            const int M_TOTAL = W_IN * CI_WORDS;
            const int x_base  = x_row * W_IN * CI_WORDS;
            const int w_off   = k * 60;

            data_t psum[PEs][4];
#pragma HLS ARRAY_PARTITION variable=psum complete dim=0

            int wi = 0, ci_w = 0;
            FLAT_LOOP: for (int m = 0; m < M_TOTAL; m++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=128 max=7680 avg=1680
#pragma HLS DEPENDENCE variable=psum    type=inter false
#pragma HLS DEPENDENCE variable=row_acc type=inter false
                int acc_idx = ci_w & 3;
                data_256_t x_word = x_buf[x_base + m];

                TC_MAC: for (int tc = 0; tc < PEs; tc++) {
#pragma HLS UNROLL
                    data_256_t w_word = wbuf[tc][w_off + ci_w];
                    data_t dot = 0;
                    for (int l = 0; l < 16; l++) {
#pragma HLS UNROLL
                        data_t xv = bits_to_half<data_t>(x_word.range(16*l+15, 16*l));
                        data_t wv = bits_to_half<data_t>(w_word.range(16*l+15, 16*l));
                        data_t prod;
#pragma HLS BIND_OP variable=prod op=hmul impl=fabric
                        prod = xv * wv;
                        dot += prod;
                    }
                    if (ci_w < 4) psum[tc][acc_idx]  = dot;
                    else          psum[tc][acc_idx] += dot;
                }

                if (ci_w == CI_WORDS - 1) {
                    int wo = wi * S + kw - PAD;
                    if (wo >= 0 && wo < W_OUT) {
                        ACC_WRITE: for (int tc = 0; tc < PEs; tc++) {
#pragma HLS UNROLL
                            if (co_base + tc < C_OUT) {
                                data_t total = psum[tc][0] + psum[tc][1] + psum[tc][2] + psum[tc][3];
                                row_acc[wo][co_base + tc] += total;
                            }
                        }
                    }
                    ci_w = 0; wi++;
                } else {
                    ci_w++;
                }
            }
        }
    }
}

template<int PEs>
void UpConv_Fused_Row(
    data_256_t*       x_buf,    // Ping-pong [2][W_IN][CI_WORDS]
    const data_256_t* W_ptr,
    const data_256_t* B_ptr,
    const data_256_t* G_ptr,
    const data_256_t* BE_ptr,
    DDR_PTR           Y,
    data_t            epsilon,
    int H_IN, int W_IN, int C_IN, int C_OUT,
    int ho
) {
#pragma HLS INLINE off

    const int S        = 2;
    const int PAD      = 1;
    const int H_OUT    = H_IN * S;
    const int W_OUT    = W_IN * S;
    const int CI_WORDS = (C_IN + 15) / 16;
    const int C_WORDS_OUT = (C_OUT + 15) / 16;
    const int C_OUT_PAD   = C_WORDS_OUT * 16;
    const int N_TILES     = (C_OUT + PEs - 1) / PEs;

    // Line buffer for accumulating output row (URAM)
    static data_t row_acc[256][480];
#pragma HLS BIND_STORAGE variable=row_acc type=ram_t2p impl=uram
#pragma HLS ARRAY_PARTITION variable=row_acc cyclic factor=16 dim=2

#if !defined(__SYNTHESIS__)
    std::memset(row_acc, 0, sizeof(row_acc));
#endif

    // [C-pipe] Reset only the C_OUT_PAD channels actually used (cols >= C_OUT are never
    // read/written), one 16-channel word per cycle → II=1 (was II=15 zeroing hardcoded 480).
    int rw = 0, rcw = 0;
    RESET_ROW_ACC: for (int m = 0; m < W_OUT * C_WORDS_OUT; m++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=128 max=1024 avg=420
        for (int l = 0; l < 16; l++) {
#pragma HLS UNROLL
            row_acc[rw][rcw * 16 + l] = 0;
        }
        if (rcw == C_WORDS_OUT - 1) { rcw = 0; rw++; } else { rcw++; }
    }

    // [DBUF-DATAFLOW] PIPO double-buffer: uc_load_tile (producer) ‖ uc_compute_tile (consumer)
    // share `wbuf` → Vitis auto-PIPOs it so load(tile+1) overlaps compute(tile). Bound to URAM
    // (matches full_generator: frees BRAM, URAM has headroom).
    TILE_LOOP: for (int tile = 0; tile < N_TILES; tile++) {
#pragma HLS LOOP_TRIPCOUNT min=6 max=41 avg=20
#pragma HLS DATAFLOW
        data_256_t wbuf[PEs][540];
#pragma HLS BIND_STORAGE variable=wbuf type=ram_t2p impl=uram
#pragma HLS ARRAY_PARTITION variable=wbuf complete dim=1
        uc_load_tile<PEs>(W_ptr, tile, C_OUT, CI_WORDS, wbuf);
        uc_compute_tile<PEs>(wbuf, x_buf, row_acc, tile, ho, H_IN, W_IN, CI_WORDS, W_OUT, C_OUT);
    }

    data_256_t b_buf[30], g_buf[30], be_buf[30];
#pragma HLS BIND_STORAGE variable=b_buf  type=ram_t2p impl=bram
#pragma HLS BIND_STORAGE variable=g_buf  type=ram_t2p impl=bram
#pragma HLS BIND_STORAGE variable=be_buf type=ram_t2p impl=bram

    LOAD_PARAMS: for (int i = 0; i < C_WORDS_OUT; i++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=4 max=30 avg=14
        b_buf[i]  = B_ptr[i];
        g_buf[i]  = G_ptr[i];
        be_buf[i] = BE_ptr[i];
    }

    // [B] PIXEL_NORM as two flattened II=1 passes: amortize the per-wo BIAS_STATS
    // fill and overlap the per-pixel div/sqrt across output columns (was PIPELINE off).
    // Pass1 computes mean/inv_std per wo (rotating depth-8, first-touch reset); Pass2 normalizes.
    static float  mean_buf[256];
    static data_t inv_buf [256];
#pragma HLS BIND_STORAGE variable=mean_buf type=ram_2p impl=lutram
#pragma HLS BIND_STORAGE variable=inv_buf  type=ram_2p impl=lutram

    float sum_rot[8], sumsq_rot[8];
#pragma HLS ARRAY_PARTITION variable=sum_rot   complete
#pragma HLS ARRAY_PARTITION variable=sumsq_rot complete

    int ws = 0, cs = 0;
    PIXEL_STATS: for (int m = 0; m < W_OUT * C_OUT; m++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=1920 max=15360 avg=7200
#pragma HLS DEPENDENCE variable=sum_rot   type=inter false
#pragma HLS DEPENDENCE variable=sumsq_rot type=inter false
#pragma HLS DEPENDENCE variable=row_acc   type=inter false
        int acc_idx = cs & 7;
        data_t bias = bits_to_half<data_t>(b_buf[cs/16].range(16*(cs%16)+15, 16*(cs%16)));
        data_t val = row_acc[ws][cs] + bias;
        row_acc[ws][cs] = val;
        float fv = (float)val;
        if (cs < 8) { sum_rot[acc_idx]  = fv; sumsq_rot[acc_idx]  = fv * fv; }
        else        { sum_rot[acc_idx] += fv; sumsq_rot[acc_idx] += fv * fv; }

        if (cs == C_OUT - 1) {
            float sum = 0, sumsq = 0;
            for (int r = 0; r < 8; r++) {
#pragma HLS UNROLL
                sum   += sum_rot[r];
                sumsq += sumsq_rot[r];
            }
            float mean = sum / (float)C_OUT;
            float var  = sumsq / (float)C_OUT - mean * mean;
            mean_buf[ws] = mean;
            inv_buf[ws]  = (data_t)(1.0f / my_sqrt_f(var + (float)epsilon));
            cs = 0; ws++;
        } else {
            cs++;
        }
    }

    int wn = 0, cwn = 0;
    PIXEL_NORM: for (int m = 0; m < W_OUT * C_WORDS_OUT; m++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=128 max=7680 avg=1680
        data_t mean_w = (data_t)mean_buf[wn];
        data_t inv_w  = inv_buf[wn];
        data_256_t out_word;
        for (int l = 0; l < 16; l++) {
#pragma HLS UNROLL
            int c = cwn * 16 + l;
            data_t val  = (c < C_OUT) ? row_acc[wn][c] : (data_t)0;
            data_t g    = bits_to_half<data_t>(g_buf[cwn].range(16*l+15, 16*l));
            data_t be   = bits_to_half<data_t>(be_buf[cwn].range(16*l+15, 16*l));
            data_t norm = (c < C_OUT) ? ((val - mean_w) * inv_w * g + be) : (data_t)0;
            data_t relu = (norm < (data_t)0) ? (data_t)0 : norm;
            out_word.range(16*l+15, 16*l) = half_to_bits(relu);
        }
        Y[(ho * W_OUT + wn) * C_WORDS_OUT + cwn] = out_word;
        if (cwn == C_WORDS_OUT - 1) { cwn = 0; wn++; } else { cwn++; }
    }
}
