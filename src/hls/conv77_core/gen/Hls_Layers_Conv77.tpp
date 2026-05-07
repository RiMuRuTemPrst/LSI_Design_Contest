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

// ============================================================
// reflect padding: mirrors at boundaries (same as Conv77 ref)
// ============================================================
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
// Inner CI_LOOP pipelined II=1 with rotating psum[C_OUT][4]:
//   depth=4 >= FP16 adder latency → DEPENDENCE false valid
// ============================================================
void Conv77_Core(
    const data_256_t* X,
    const data_256_t* W,
    const data_256_t* B,
    data_256_t*       Y
) {
#pragma HLS INLINE off

    const int H       = 256;
    const int W_IN    = 256;
    const int C_OUT   = 3;
    const int CI_WORDS = 4;   // ceil(60/16)
    const int KR      = 7;
    const int PAD     = 3;

    // 7-row line buffer (URAM): slot = row_idx % 7
    static data_256_t line_buf[KR][W_IN][CI_WORDS];
#pragma HLS BIND_STORAGE variable=line_buf type=ram_t2p impl=uram
#pragma HLS ARRAY_PARTITION variable=line_buf complete dim=3

    // Weight buffer (BRAM): fully partitioned on co and ci_w for 3 parallel reads
    data_256_t w_buf[C_OUT][49][CI_WORDS];
#pragma HLS BIND_STORAGE variable=w_buf type=ram_t2p impl=bram
#pragma HLS ARRAY_PARTITION variable=w_buf complete dim=1
#pragma HLS ARRAY_PARTITION variable=w_buf complete dim=3

    // --- Load weights ---
    LOAD_W: for (int co = 0; co < C_OUT; co++)
        for (int k = 0; k < 49; k++)
            for (int ci_w = 0; ci_w < CI_WORDS; ci_w++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=4 max=4 avg=4
                w_buf[co][k][ci_w] = W[(co * 49 + k) * CI_WORDS + ci_w];
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

        // Load the new bottom row when it becomes available (rows KR..H-1)
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
            data_t acc[C_OUT];
#pragma HLS ARRAY_PARTITION variable=acc complete dim=1
            for (int co = 0; co < C_OUT; co++) acc[co] = 0;

            KH_LOOP: for (int kh = 0; kh < KR; kh++) {
                int abs_row  = reflect_pad(ho - PAD + kh, H);
                int row_slot = abs_row % KR;

                KW_LOOP: for (int kw = 0; kw < KR; kw++) {
                    int k       = kh * KR + kw;
                    int abs_col = reflect_pad(wo - PAD + kw, W_IN);

                    // Rotating psum — depth 4 >= FP16 adder latency
                    data_t psum[C_OUT][4];
#pragma HLS ARRAY_PARTITION variable=psum complete dim=0
                    for (int co = 0; co < C_OUT; co++)
                        for (int r = 0; r < 4; r++)
                            psum[co][r] = 0;

                    CI_LOOP: for (int ci_w = 0; ci_w < CI_WORDS; ci_w++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=4 max=4 avg=4
#pragma HLS DEPENDENCE variable=psum type=inter false
                        int acc_idx = ci_w & 3;
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
                            psum[co][acc_idx] += dot;
                        }
                    }

                    for (int co = 0; co < C_OUT; co++)
                        acc[co] += psum[co][0] + psum[co][1] + psum[co][2] + psum[co][3];
                }
            }

            // Apply bias and pack 3 half values into one 256-bit word
            data_256_t out_word = 0;
            for (int co = 0; co < C_OUT; co++) {
                data_t val = acc[co] + bias[co];
                out_word.range(16*co+15, 16*co) = half_to_bits(val);
            }
            Y[ho * W_IN + wo] = out_word;
        }
    }
}
