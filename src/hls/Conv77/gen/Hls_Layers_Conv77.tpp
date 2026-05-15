// Conv77 HLS Kernel — data_256_t/half interface, fp16 SIMD + Spatial-PE
//
// AXI interface:
//   X: data_256_t* — 256x256x60 input, CI_WORDS_IN=4 words/pixel (60ch padded to 64)
//   W: data_256_t* — 3x7x7x60 weights, CI_WORDS_IN=4 words/kernel-pos = 588 words
//   B: data_256_t* — bias, 1 word (bits[47:0] = B[0..2] as half IEEE)
//   Z: data_256_t* — 256x256 output, 1 pixel/word (bits[47:0] = RGB half)
//
// Internal computation: fp16_t = ap_fixed<16,8> for 1 DSP per MAC
// Accumulator: fp32acc_t = ap_fixed<32,16>
// Architecture: SIMD_DEPTH=8 MACs/PE, NUM_WIN_PEs=8 adjacent windows
//   → 448 MACs/pipeline cycle, TC_IN=8 depth tiles, W_BUF=14 column buffer
//
// Load from data_256_t: extract IEEE FP16 bits → convert to ap_fixed<16,8>
// Store to data_256_t Z: convert ap_fixed<32,16> → half IEEE → pack bits[47:0]
//
// Clip(0,1) applied at output — matches HiFiC model's final activation.

#pragma once

#include "ap_fixed.h"
#include "ap_int.h"
#include <stdint.h>

#if !defined(__SYNTHESIS__)
  #include <cmath>
  #include <cstring>
#endif
typedef ap_fixed<16, 8>  fp16_t;
typedef ap_fixed<32, 16> fp32acc_t;

// ============================================================
// HALF BIT-CONVERSION HELPERS
// Guard shared with Hls_Layers_Fusion.tpp / Hls_Layers_UpConv.tpp
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

#endif // HLS_HALF_HELPERS_DEFINED

// ============================================================
// REFLECT PADDING (7x7 conv, pad=3)
// ============================================================
static inline int reflect77(int i, int MAX) {
#pragma HLS INLINE
    if (i < 0)    return -i;
    if (i >= MAX) return (MAX - 1) * 2 - i;
    return i;
}

// ============================================================
// Conv77_Kernel
// ============================================================
template<int SIMD_DEPTH, int NUM_WIN_PEs,
         int BATCH, int C_IN, int C_OUT,
         int H_IN, int W_IN, int H_R, int W_R,
         int H_OUT, int W_OUT>
void Conv77_Kernel(const data_256_t* X, const data_256_t* W,
                   const data_256_t* B, data_256_t* Z) {

    constexpr int HALF_H      = H_R / 2;
    constexpr int HALF_W      = W_R / 2;
    constexpr int C_PAD       = ((C_IN + SIMD_DEPTH - 1) / SIMD_DEPTH) * SIMD_DEPTH;
    constexpr int TC_IN       = C_PAD / SIMD_DEPTH;
    constexpr int W_BUF       = W_R + NUM_WIN_PEs - 1;
    constexpr int CI_WORDS_IN = (C_PAD + 15) / 16;  // = 4 for C_PAD=64

    // ------------------------------------------------------------------
    // On-chip buffers
    // x_buffer: fp16_t [H_R=7][W_BUF=14][C_PAD=64]
    // w_buffer: fp16_t [C_OUT=3][H_R=7][W_R=7][C_PAD=64]
    // b_buffer: fp16_t [C_OUT=3]
    // acc:      fp32acc_t [C_OUT=3][NUM_WIN_PEs=8]
    // ------------------------------------------------------------------
    fp16_t    x_buffer[H_R][W_BUF][C_PAD];
    fp16_t    w_buffer[C_OUT][H_R][W_R][C_PAD];
    fp16_t    b_buffer[C_OUT];
    fp32acc_t acc[C_OUT][NUM_WIN_PEs];

    // Partition x_buffer: col dim completely independent, channel dim cyclic(16)
    // cyclic(16) = 1 DDR word width → write 16 channels to 16 unique banks in 1 pipeline cycle (II=1)
    // depth per bank = 7*4=28 elements → LUTRAM (no BRAM cost)
    #pragma HLS ARRAY_PARTITION variable=x_buffer complete dim=2
    #pragma HLS ARRAY_PARTITION variable=x_buffer cyclic factor=16 dim=3

    // Partition w_buffer: wf dim completely independent, channel dim cyclic(8)
    // cyclic(8) matches SIMD_DEPTH → compute loop reads 8 unique banks per tci, no conflict
    // depth per bank = 3*7*8=168 elements → 1 BRAM each (7*8=56 BRAM total)
    #pragma HLS ARRAY_PARTITION variable=w_buffer complete dim=3
    #pragma HLS ARRAY_PARTITION variable=w_buffer cyclic factor=8 dim=4

    #pragma HLS ARRAY_PARTITION variable=b_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=acc      complete dim=1
    #pragma HLS ARRAY_PARTITION variable=acc      complete dim=2

    // ------------------------------------------------------------------
    // Load bias: 1 data_256_t word, bits[co*16+15 : co*16] = half(B[co])
    // ------------------------------------------------------------------
    {
        data_256_t b_word = B[0];
        for (int co = 0; co < C_OUT; co++) {
            #pragma HLS UNROLL
            b_buffer[co] = (fp16_t)((float)bits_to_half<half>(
                b_word.range(co*16+15, co*16)));
        }
    }

    // ------------------------------------------------------------------
    // Preload weights: shape (C_OUT, H_R, W_R, C_IN), CI_WORDS_IN words/pos
    // IEEE FP16 bits in DDR → convert to ap_fixed<16,8> for 1 DSP/MAC
    // ------------------------------------------------------------------
    Preload_co:
    for (int co = 0; co < C_OUT; co++) {
        Preload_hf:
        for (int hf = 0; hf < H_R; hf++) {
            Preload_wf:
            for (int wf = 0; wf < W_R; wf++) {
                const int w_base = ((co * H_R + hf) * W_R + wf) * CI_WORDS_IN;
                Preload_ciw:
                for (int ciw = 0; ciw < CI_WORDS_IN; ciw++) {
                    data_256_t word = W[w_base + ciw];  // 1 DDR read per 16ch
                    // 2 half-passes × 8ch: each pass writes to 8 unique cyclic(8) banks (no conflict)
                    for (int hi = 0; hi < 2; hi++) {
                        #pragma HLS PIPELINE II=1
                        for (int k = 0; k < 8; k++) {
                            #pragma HLS UNROLL
                            int ci = ciw * 16 + hi * 8 + k;
                            w_buffer[co][hf][wf][ci] = (ci < C_IN)
                                ? (fp16_t)((float)bits_to_half<half>(
                                    word.range((hi*8+k)*16+15, (hi*8+k)*16)))
                                : fp16_t(0);
                        }
                    }
                }
            }
        }
    }

    // ------------------------------------------------------------------
    // Main computation loops
    // ------------------------------------------------------------------
    Batch_loop:
    for (int n = 0; n < BATCH; n++) {
        const int x_n_base = n * H_IN * W_IN * CI_WORDS_IN;
        const int z_n_base = n * H_OUT * W_OUT;

        Spatial_h_loop:
        for (int ho = 0; ho < H_OUT; ho++) {

            // Precompute row base DDR word addresses for each kernel row
            int x_row_base[H_R];
            #pragma HLS ARRAY_PARTITION variable=x_row_base complete dim=1
            Precomp_hi:
            for (int hf = 0; hf < H_R; hf++) {
                #pragma HLS UNROLL
                int hi = reflect77(ho - HALF_H + hf, H_IN);
                x_row_base[hf] = x_n_base + hi * W_IN * CI_WORDS_IN;
            }

            Spatial_w_loop:
            for (int wo = 0; wo < W_OUT; wo += NUM_WIN_PEs) {
                #pragma HLS PIPELINE off

                const int col_start = wo - HALF_W;

                // ---- LOAD x_buffer ----
                if (wo == 0) {
                    // Full init: all W_BUF=14 columns
                    Init_hf:
                    for (int hf = 0; hf < H_R; hf++) {
                        Init_col:
                        for (int col = 0; col < W_BUF; col++) {
                            const int wi      = reflect77(col_start + col, W_IN);
                            const int px_base = x_row_base[hf] + wi * CI_WORDS_IN;
                            Init_ciw:
                            for (int ciw = 0; ciw < CI_WORDS_IN; ciw++) {
                                #pragma HLS PIPELINE II=1
                                data_256_t word = X[px_base + ciw];
                                for (int k = 0; k < 16; k++) {
                                    #pragma HLS UNROLL
                                    int ci = ciw * 16 + k;
                                    x_buffer[hf][col][ci] = (ci < C_IN)
                                        ? (fp16_t)((float)bits_to_half<half>(word.range(k*16+15, k*16)))
                                        : fp16_t(0);
                                }
                            }
                        }
                    }
                } else {
                    // Shift left: cols 0..(W_BUF-NUM_WIN_PEs-1) ← cols NUM_WIN_PEs..W_BUF-1
                    Shift_hf:
                    for (int hf = 0; hf < H_R; hf++) {
                        Shift_tci:
                        for (int tci = 0; tci < TC_IN; tci++) {
                            #pragma HLS PIPELINE II=1
                            Shift_col:
                            for (int col = 0; col < W_BUF - NUM_WIN_PEs; col++) {
                                #pragma HLS UNROLL
                                Shift_s:
                                for (int s = 0; s < SIMD_DEPTH; s++) {
                                    #pragma HLS UNROLL
                                    x_buffer[hf][col][tci*SIMD_DEPTH+s] =
                                        x_buffer[hf][col+NUM_WIN_PEs][tci*SIMD_DEPTH+s];
                                }
                            }
                        }
                    }
                    // Load NUM_WIN_PEs=8 new rightmost columns from DDR
                    Newcol_hf:
                    for (int hf = 0; hf < H_R; hf++) {
                        Newcol_nc:
                        for (int nc = 0; nc < NUM_WIN_PEs; nc++) {
                            const int col     = W_BUF - NUM_WIN_PEs + nc;
                            const int wi      = reflect77(col_start + col, W_IN);
                            const int px_base = x_row_base[hf] + wi * CI_WORDS_IN;
                            Newcol_ciw:
                            for (int ciw = 0; ciw < CI_WORDS_IN; ciw++) {
                                #pragma HLS PIPELINE II=1
                                data_256_t word = X[px_base + ciw];
                                for (int k = 0; k < 16; k++) {
                                    #pragma HLS UNROLL
                                    int ci = ciw * 16 + k;
                                    x_buffer[hf][col][ci] = (ci < C_IN)
                                        ? (fp16_t)((float)bits_to_half<half>(word.range(k*16+15, k*16)))
                                        : fp16_t(0);
                                }
                            }
                        }
                    }
                }

                // ---- COMPUTE: accumulate into acc[co][win] ----
                Init_acc_co:
                for (int co = 0; co < C_OUT; co++) {
                    #pragma HLS UNROLL
                    Init_acc_win:
                    for (int win = 0; win < NUM_WIN_PEs; win++) {
                        #pragma HLS UNROLL
                        acc[co][win] = fp32acc_t(0);
                    }
                }

                Compute_hf:
                for (int hf = 0; hf < H_R; hf++) {
                    Compute_co:
                    for (int co = 0; co < C_OUT; co++) {
                        Compute_tci:
                        for (int tci = 0; tci < TC_IN; tci++) {
                            #pragma HLS PIPELINE II=1

                            // Feed-forward adder tree (not loop-carried) per win
                            fp32acc_t sum[NUM_WIN_PEs];
                            #pragma HLS ARRAY_PARTITION variable=sum complete dim=1
                            Sum_init:
                            for (int win = 0; win < NUM_WIN_PEs; win++) {
                                #pragma HLS UNROLL
                                sum[win] = fp32acc_t(0);
                            }

                            Wf_loop:
                            for (int wf = 0; wf < W_R; wf++) {
                                #pragma HLS UNROLL
                                Win_loop:
                                for (int win = 0; win < NUM_WIN_PEs; win++) {
                                    #pragma HLS UNROLL
                                    Simd_loop:
                                    for (int s = 0; s < SIMD_DEPTH; s++) {
                                        #pragma HLS UNROLL
                                        // ap_fixed<16,8> * ap_fixed<16,8> → 1 DSP each
                                        fp32acc_t prod =
                                            x_buffer[hf][win+wf][tci*SIMD_DEPTH+s] *
                                            w_buffer[co][hf][wf][tci*SIMD_DEPTH+s];
                                        sum[win] += prod;
                                    }
                                }
                            }

                            // Loop-carried accumulation (distance=TC_IN=8 ≥ fp32acc latency)
                            Sum_acc:
                            for (int win = 0; win < NUM_WIN_PEs; win++) {
                                #pragma HLS UNROLL
                                acc[co][win] += sum[win];
                            }
                        }
                    }
                }

                // ---- WRITE Z: 1 pixel/word, bits[co*16+15:co*16] = half(output) ----
                Write_win:
                for (int win = 0; win < NUM_WIN_PEs; win++) {
                    #pragma HLS PIPELINE II=1
                    const int wo_actual = wo + win;
                    if (wo_actual < W_OUT) {
                        data_256_t z_word = 0;
                        Write_co:
                        for (int co = 0; co < C_OUT; co++) {
                            #pragma HLS UNROLL
                            fp32acc_t val  = acc[co][win] + (fp32acc_t)b_buffer[co];
                            if (val < (fp32acc_t)0) val = (fp32acc_t)0;
                            if (val > (fp32acc_t)1) val = (fp32acc_t)1;
                            half      out_h = (half)((float)val);
                            z_word.range(co*16+15, co*16) = half_to_bits(out_h);
                        }
                        Z[z_n_base + ho * W_OUT + wo_actual] = z_word;
                    }
                }

            } // Spatial_w_loop
        } // Spatial_h_loop
    } // Batch_loop
}

// ============================================================
// HW_Conv7x7 — synthesizable AXI top-level
//
// Port depths:
//   X: 256x256x4 = 262144 data_256_t words (60ch padded to 64)
//   W: 3x7x7x4   = 588    data_256_t words
//   B: 1                   data_256_t word
//   Z: 256x256   = 65536  data_256_t words (1 pixel/word, bits[47:0]=RGB)
// ============================================================
inline void HW_Conv7x7(
    const data_256_t* X,
    const data_256_t* W,
    const data_256_t* B,
    data_256_t*       Z
) {
    #pragma HLS INTERFACE m_axi port=X bundle=gmem_X depth=262144 \
        max_read_burst_length=64  num_read_outstanding=4  latency=64
    #pragma HLS INTERFACE m_axi port=W bundle=gmem_W depth=588 \
        max_read_burst_length=64  num_read_outstanding=4  latency=64
    #pragma HLS INTERFACE m_axi port=B bundle=gmem_B depth=1 \
        max_read_burst_length=4
    #pragma HLS INTERFACE m_axi port=Z bundle=gmem_Z depth=65536 \
        max_write_burst_length=64 num_write_outstanding=4

    #pragma HLS INTERFACE s_axilite port=X      bundle=control
    #pragma HLS INTERFACE s_axilite port=W      bundle=control
    #pragma HLS INTERFACE s_axilite port=B      bundle=control
    #pragma HLS INTERFACE s_axilite port=Z      bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    Conv77_Kernel<
        8,    // SIMD_DEPTH  — 8 MACs per PE depth tile
        8,    // NUM_WIN_PEs — 8 adjacent output windows processed simultaneously
        1,    // BATCH
        60,   // C_IN
        3,    // C_OUT
        256,  // H_IN
        256,  // W_IN
        7,    // H_R
        7,    // W_R
        256,  // H_OUT
        256   // W_OUT
    >(X, W, B, Z);
}
