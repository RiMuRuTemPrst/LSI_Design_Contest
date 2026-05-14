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
// UPCONV FUSED ROW (Iteration 11 - Rotating Psum for II=1)
// ============================================================

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

    RESET_ROW_ACC: for (int wo = 0; wo < W_OUT; wo++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=32 max=256 avg=120
        for (int c = 0; c < 480; c++) {
#pragma HLS UNROLL
            row_acc[wo][c] = 0;
        }
    }

    // Weight buffer (PEs=16 parallel reads)
    data_256_t w_local[PEs][540];
#pragma HLS BIND_STORAGE variable=w_local type=ram_t2p impl=bram
#pragma HLS ARRAY_PARTITION variable=w_local complete dim=1

    TILE_LOOP: for (int tile = 0; tile < N_TILES; tile++) {
#pragma HLS LOOP_TRIPCOUNT min=8 max=60 avg=29
        int co_base = tile * PEs;

        // Preload weights for 16 PEs and all 9 kernel positions for this tile
        PRELOAD_W: for (int tc = 0; tc < PEs; tc++) {
            int co = co_base + tc;
            if (co < C_OUT) {
                for (int k = 0; k < 9; k++) {
                    for (int ci_w = 0; ci_w < CI_WORDS; ci_w++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=8 max=60 avg=28
                        w_local[tc][k * 60 + ci_w] = W_ptr[(co * 9 + k) * CI_WORDS + ci_w];
                    }
                }
            }
        }

        KH_LOOP: for (int kh = 0; kh < 3; kh++) {
            int hpk = ho + PAD - kh;
            if (hpk < 0 || hpk % S != 0) continue;
            int hi = hpk / S;
            if (hi < 0 || hi >= H_IN) continue;
            int x_row = hi % 2;

            KW_LOOP: for (int kw = 0; kw < 3; kw++) {
                int k = kh * 3 + kw;

                WI_LOOP: for (int wi = 0; wi < W_IN; wi++) {
#pragma HLS LOOP_TRIPCOUNT min=16 max=128 avg=60
                    int wo = wi * S + kw - PAD;
                    if (wo < 0 || wo >= W_OUT) continue;

                    // Rotating psum: depth=4 >= FP16 add latency (=3)
                    data_t psum[PEs][4];
#pragma HLS ARRAY_PARTITION variable=psum complete dim=0

                    // Reset rotating slots
                    RESET_PSUM: for (int tc = 0; tc < PEs; tc++) {
#pragma HLS UNROLL
                        for (int r = 0; r < 4; r++) {
#pragma HLS UNROLL
                            psum[tc][r] = 0;
                        }
                    }

                    CI_LOOP: for (int ci_w = 0; ci_w < CI_WORDS; ci_w++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=8 max=60 avg=28
#pragma HLS DEPENDENCE variable=psum type=inter false

                        int acc_idx = ci_w & 3;
                        data_256_t x_word = x_buf[(x_row * W_IN + wi) * CI_WORDS + ci_w];

                        TC_MAC: for (int tc = 0; tc < PEs; tc++) {
#pragma HLS UNROLL
                            data_256_t w_word = w_local[tc][k * 60 + ci_w];
                            data_t dot = 0;
                            for (int l = 0; l < 16; l++) {
#pragma HLS UNROLL
                                data_t xv = bits_to_half<data_t>(x_word.range(16*l+15, 16*l));
                                data_t wv = bits_to_half<data_t>(w_word.range(16*l+15, 16*l));
                                dot += xv * wv;
                            }
                            psum[tc][acc_idx] += dot;
                        }
                    }

                    // Reduce 4 rotating slots -> single value per PE
                    ACC_WRITE: for (int tc = 0; tc < PEs; tc++) {
#pragma HLS UNROLL
                        if (co_base + tc < C_OUT) {
                            data_t total = psum[tc][0] + psum[tc][1] + psum[tc][2] + psum[tc][3];
                            row_acc[wo][co_base + tc] += total;
                        }
                    }
                }
            }
        }
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

    PIXEL_NORM: for (int wo = 0; wo < W_OUT; wo++) {
#pragma HLS PIPELINE off
#pragma HLS LOOP_TRIPCOUNT min=32 max=256 avg=120
        float sum = 0, sumsq = 0;

        BIAS_STATS: for (int c = 0; c < C_OUT; c++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=60 max=480 avg=225
            data_t bias = bits_to_half<data_t>(b_buf[c/16].range(16*(c%16)+15, 16*(c%16)));
            data_t val = row_acc[wo][c] + bias;
            row_acc[wo][c] = val;
            float fv = (float)val;
            sum   += fv;
            sumsq += fv * fv;
        }

        float mean    = sum / (float)C_OUT;
        float var     = sumsq / (float)C_OUT - mean * mean;
        data_t inv_std = (data_t)(1.0f / my_sqrt_f(var + (float)epsilon));

        NORM_WRITE: for (int cw = 0; cw < C_WORDS_OUT; cw++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=4 max=30 avg=14
            data_256_t out_word;
            for (int l = 0; l < 16; l++) {
#pragma HLS UNROLL
                int c = cw * 16 + l;
                data_t val  = (c < C_OUT) ? row_acc[wo][c] : (data_t)0;
                data_t g    = bits_to_half<data_t>(g_buf[cw].range(16*l+15, 16*l));
                data_t be   = bits_to_half<data_t>(be_buf[cw].range(16*l+15, 16*l));
                data_t norm = (c < C_OUT) ? ((val - (data_t)mean) * inv_std * g + be) : (data_t)0;
                data_t relu = (norm < (data_t)0) ? (data_t)0 : norm;
                out_word.range(16*l+15, 16*l) = half_to_bits(relu);
            }
            Y[(ho * W_OUT + wo) * C_WORDS_OUT + cw] = out_word;
        }
    }
}
