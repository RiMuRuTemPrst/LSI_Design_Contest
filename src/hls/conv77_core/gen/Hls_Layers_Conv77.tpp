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

#endif // HLS_HALF_HELPERS_DEFINED

static inline int reflect_pad(int i, int MAX) {
#pragma HLS INLINE
    if (i < 0)    return -i;
    if (i >= MAX) return (MAX - 1) * 2 - i;
    return i;
}

// ============================================================
// Conv77_Core — 7×7 Conv, half/data_256_t, row-streaming
//
// X layout : [H=256][W=256][CI_WORDS=4]  (60 channels, padded to 64)
// W layout : [C_OUT=3][49][CI_WORDS=4]
// B layout : word 0, bits [47:0] = 3 half bias values
// Y layout : [H=256][W=256][1]  bits [47:0] = 3 half output values
//
// Flat loop: 196 iterations (49 kernel positions × 4 CI_WORDS) pipelined at II=1.
// psum[co][k_ci] written exactly once per iteration → no inter-iteration RAW.
// REDUCE: depth-4 rotating accumulator, DEPENDENCE false valid (FP16 add latency ≤ 4).
//
// No ARRAY_PARTITION dim=3 on line_buf → 8 URAM blocks (vs 16).
// ============================================================
void Conv77_Core(
    const data_256_t* X,
    const data_256_t* W,
    const data_256_t* B,
    data_256_t*       Y
) {
#pragma HLS INLINE off

    const int H        = 256;
    const int W_IN     = 256;
    const int C_OUT    = 3;
    const int CI_WORDS = 4;
    const int KR       = 7;
    const int PAD      = 3;
    const int K_TOTAL  = KR * KR;           // 49
    const int FLAT_N   = K_TOTAL * CI_WORDS; // 196

    // 7-row circular line buffer: no dim=3 partition → 8 URAM (vs 16 with partition).
    // FLAT_LOOP reads one (row_slot, abs_col, ci_w) per iteration → single port sufficient.
    static data_256_t line_buf[KR][W_IN][CI_WORDS];
#pragma HLS BIND_STORAGE variable=line_buf type=ram_t2p impl=uram

    // Weight buffer (BRAM): partitioned on co and ci_w for 3 simultaneous co reads
    data_256_t w_buf[C_OUT][K_TOTAL][CI_WORDS];
#pragma HLS BIND_STORAGE variable=w_buf type=ram_t2p impl=bram
#pragma HLS ARRAY_PARTITION variable=w_buf complete dim=1
#pragma HLS ARRAY_PARTITION variable=w_buf complete dim=3

    // --- Load weights ---
    LOAD_W: for (int co = 0; co < C_OUT; co++)
        for (int k = 0; k < K_TOTAL; k++)
            for (int ci_w = 0; ci_w < CI_WORDS; ci_w++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=4 max=4 avg=4
                w_buf[co][k][ci_w] = W[(co * K_TOTAL + k) * CI_WORDS + ci_w];
            }

    // --- Load bias ---
    data_256_t b_word = B[0];
    data_t bias[C_OUT];
#pragma HLS ARRAY_PARTITION variable=bias complete dim=1
    for (int co = 0; co < C_OUT; co++)
        bias[co] = bits_to_half<data_t>(b_word.range(16*co+15, 16*co));

    // --- Preload first KR rows into line_buf[0..KR-1] ---
    PRELOAD: for (int row = 0; row < KR; row++)
        for (int col = 0; col < W_IN; col++)
            for (int ci_w = 0; ci_w < CI_WORDS; ci_w++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=4 max=4 avg=4
                line_buf[row][col][ci_w] = X[(row * W_IN + col) * CI_WORDS + ci_w];
            }

    // --- Process output rows ---
    HO_LOOP: for (int ho = 0; ho < H; ho++) {
#pragma HLS LOOP_TRIPCOUNT min=256 max=256 avg=256

        int new_row = ho + PAD;
        if (new_row >= KR && new_row < H) {
            int slot = new_row % KR;
            LOAD_ROW: for (int col = 0; col < W_IN; col++)
                for (int ci_w = 0; ci_w < CI_WORDS; ci_w++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=4 max=4 avg=4
                    line_buf[slot][col][ci_w] =
                        X[(new_row * W_IN + col) * CI_WORDS + ci_w];
                }
        }

        WO_LOOP: for (int wo = 0; wo < W_IN; wo++) {
#pragma HLS LOOP_TRIPCOUNT min=256 max=256 avg=256
#pragma HLS PIPELINE off

            // 196 independent dot products: psum[co][k_ci] written exactly once.
            // No inter-iteration RAW → II=1 with no DEPENDENCE pragma needed.
            data_t psum[C_OUT][FLAT_N];
#pragma HLS ARRAY_PARTITION variable=psum complete dim=0

            FLAT_LOOP: for (int k_ci = 0; k_ci < FLAT_N; k_ci++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=196 max=196 avg=196
                int k    = k_ci >> 2;   // 0..48
                int ci_w = k_ci  & 3;   // 0..3
                int kh   = k / KR;
                int kw   = k % KR;
                int abs_row  = reflect_pad(ho - PAD + kh, H);
                int row_slot = abs_row % KR;
                int abs_col  = reflect_pad(wo - PAD + kw, W_IN);

                data_256_t x_word = line_buf[row_slot][abs_col][ci_w];

                CO_MAC: for (int co = 0; co < C_OUT; co++) {
#pragma HLS UNROLL
                    data_256_t w_word = w_buf[co][k][ci_w];
                    data_t dot = 0;
                    L_MAC: for (int l = 0; l < 16; l++) {
#pragma HLS UNROLL
                        data_t xv = bits_to_half<data_t>(x_word.range(16*l+15, 16*l));
                        data_t wv = bits_to_half<data_t>(w_word.range(16*l+15, 16*l));
                        dot += xv * wv;
                    }
                    psum[co][k_ci] = dot; // write-once: no RAW between iterations
                }
            }

            // REDUCE: depth-4 rotating accumulator sums 196 psum values per channel.
            // acc[co][r] accessed every 4 iters; FP16 add latency ≤ 4 → DEPENDENCE false valid.
            data_t acc[C_OUT][4];
#pragma HLS ARRAY_PARTITION variable=acc complete dim=0
            for (int co = 0; co < C_OUT; co++) {
                acc[co][0] = bias[co];
                acc[co][1] = 0; acc[co][2] = 0; acc[co][3] = 0;
            }

            REDUCE: for (int i = 0; i < FLAT_N; i++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=196 max=196 avg=196
#pragma HLS DEPENDENCE variable=acc type=inter false
                int r = i & 3;
                REDUCE_CO: for (int co = 0; co < C_OUT; co++) {
#pragma HLS UNROLL
                    acc[co][r] += psum[co][i];
                }
            }

            data_256_t out_word = 0;
            FOLD: for (int co = 0; co < C_OUT; co++) {
#pragma HLS UNROLL
                data_t val = acc[co][0] + acc[co][1] + acc[co][2] + acc[co][3];
                out_word.range(16*co+15, 16*co) = half_to_bits(val);
            }
            Y[ho * W_IN + wo] = out_word;
        }
    }
}
