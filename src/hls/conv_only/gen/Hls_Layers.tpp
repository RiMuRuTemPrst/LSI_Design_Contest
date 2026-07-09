#pragma once
#include "Core.h"

#if !defined(__SYNTHESIS__)
  #include <cassert>
  #include <cmath>
#endif

#if defined(__SYNTHESIS__)
  #include <hls_math.h>
#endif

#include <cstring>

// ============================================================
// TYPE PUNNING & SCALAR HELPERS
// ============================================================

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

// Flattened index for 4D tensor (N, H, W, C)
static inline int flat_idx(int n, int h, int w, int c, int H, int W, int C) {
#pragma HLS INLINE
    return ((n * H + h) * W + w) * C + c;
}

// Flattened index for 4D weight tensor (CO, KH, KW, CI) — 3×3 kernel
static inline int w_flat_idx(int co, int kh, int kw, int ci, int C_IN) {
#pragma HLS INLINE
    return ((co * 3 + kh) * 3 + kw) * C_IN + ci;
}

// Hardware-aware square root
static inline float my_sqrt_f(float x) {
#pragma HLS INLINE
#if defined(__SYNTHESIS__)
    return hls::sqrtf(x);
#else
    return std::sqrt(x);
#endif
}

// ============================================================
// GENERATOR LAYER 1: Load Parameters
// ============================================================
// Purpose : Load bias / gamma / beta from DDR → on-chip data_256_t buffers.
// Transfer: DDR ──256-bit/beat──► b_buf_256 / gamma_buf_256 / beta_buf_256
// ============================================================
template<int C_OUT, typename T>
static void load_params(
    DDR_CONST_PTR  B_ptr,                       // [C_OUT/16] Bias  (256-bit words)
    DDR_CONST_PTR  G_ptr,                       // [C_OUT/16] Gamma (256-bit words)
    DDR_CONST_PTR  BE_ptr,                      // [C_OUT/16] Beta  (256-bit words)
    data_256_t     b_buf   [C_OUT / PACK_256],  // On-chip bias   buffer (256-bit)
    data_256_t     gamma_buf[C_OUT / PACK_256], // On-chip gamma  buffer (256-bit)
    data_256_t     beta_buf [C_OUT / PACK_256]  // On-chip beta   buffer (256-bit)
) {
#pragma HLS INLINE off
    LOAD_BIAS: for (int i = 0; i < C_OUT / PACK_256; i++) {
#pragma HLS PIPELINE II=1
        b_buf[i] = B_ptr[i];
    }
    LOAD_GAMMA: for (int i = 0; i < C_OUT / PACK_256; i++) {
#pragma HLS PIPELINE II=1
        gamma_buf[i] = G_ptr[i];
    }
    LOAD_BETA: for (int i = 0; i < C_OUT / PACK_256; i++) {
#pragma HLS PIPELINE II=1
        beta_buf[i] = BE_ptr[i];
    }
}

// ============================================================
// GENERATOR LAYER 2: Init Input Rows with ON-THE-FLY REFLECT PADDING
// ============================================================
// This function implements Reflect Padding internally:
// (h < 0) ? -h : ((h >= H) ? (H << 1) - h - 2 : h)
// No separate IP or function for padding; integrated into Row Loader.
// ============================================================
template<int W_DIM, int C_IN, bool SAVE_SKIP, typename T>
static void init_rows(
    DDR_CONST_PTR   X_ptr,                                         // Input DDR
    data_256_t      x_buf_256[4][W_DIM][C_IN / PACK_256],          // IFM circular buf
    data_256_t      skip_buf_256[MODEL_H * MODEL_W * C_IN / PACK_256], // URAM skip buf
    int             n                                               // Batch index
) {
#pragma HLS INLINE off
    constexpr int H             = MODEL_H;
    constexpr int W             = MODEL_W;
    constexpr int C_WORDS       = C_IN / PACK_256;         
    constexpr int WORDS_PER_ROW = W_DIM * C_WORDS;         

    INIT_3_INPUT_ROWS: for (int row_init = 0; row_init < 3; row_init++) {
        int h       = row_init - 1;
        // --- ON-THE-FLY REFLECT PADDING LOGIC ---
        int h_clamp = (h < 0) ? -h : ((h >= H) ? (H << 1) - h - 2 : h);
        int slot    = (h + 1) & 3;                     
        int base    = flat_idx(n, h_clamp, 0, 0, H, W, C_IN); 

        LOAD_1_INPUT_ROW: for (int i = 0; i < WORDS_PER_ROW; i++) {
#pragma HLS PIPELINE II=1
            x_buf_256[slot][i / C_WORDS][i % C_WORDS] = X_ptr[base / PACK_256 + i];
        }

        if (SAVE_SKIP && h >= 0 && h < H) {
            int skip_base = h * WORDS_PER_ROW;
            SAVE_1_SKIP_ROW: for (int i = 0; i < WORDS_PER_ROW; i++) {
#pragma HLS PIPELINE II=1
                skip_buf_256[skip_base + i] = x_buf_256[slot][i / C_WORDS][i % C_WORDS];
            }
        }
    }
}

// ============================================================
// GENERATOR LAYER 3: Fused Convolution, ChannelNorm, and Activation
// ============================================================
// This core kernel integrates:
// 1. Convolution 3x3 (with integrated Row/Column Reflect Padding)
// 2. Channel Normalization (Fused logic in PASS B)
// 3. Activation (ReLU or Residual Add Skip)
// ============================================================
template<int PEs, int C_IN, int C_OUT, typename T>
static void Fused_Conv_CNorm_Act(
    DDR_CONST_PTR   X_ptr,                                         
    DDR_CONST_PTR   W_ptr,                                         
    DDR_PTR         Out_ptr,                                       
    data_256_t      b_buf_256   [C_OUT / PACK_256],                
    data_256_t      gamma_buf_256[C_OUT / PACK_256],               
    data_256_t      beta_buf_256 [C_OUT / PACK_256],               
    data_256_t      x_buf_256[4][MODEL_W][C_IN / PACK_256],        
    data_256_t      skip_buf_256[MODEL_H * MODEL_W * C_OUT / PACK_256], 
    data_256_t      w_buf_256[2][C_IN / PACK_256],                 
    T               y_cache_16[PEs][C_OUT],
    T               epsilon,
    int             n,
    bool            is_final_layer,
    bool            conv_only,
    ap_uint<128>*   psum_packed         // [N][H][W][C_OUT] packed {win_cnt[9:0], psum_double[63:0]}
) {
#pragma HLS INLINE off
    constexpr int H         = MODEL_H;
    constexpr int W         = MODEL_W;
    constexpr int width     = PEs / W;
    constexpr int rolls     = H / width;
    constexpr int C_WORDS   = C_IN / PACK_256;      
    constexpr int WPR       = MODEL_W * C_IN / PACK_256; 

    const int   num              = C_OUT;
    const T     adjustment_scale = (T)num / (T)(num - 1);
    const float pre_div          = 1.0f / my_sqrt_f((float)(num - 1));
    const float pre_div_sq       = pre_div * pre_div;

    Slide_PEs_loop:
    for (int r = 0; r < rolls; r++) {

        float sum_acc[PEs];
        float sumsq_acc[PEs];
#pragma HLS ARRAY_PARTITION variable=sum_acc   complete
#pragma HLS ARRAY_PARTITION variable=sumsq_acc complete
        INIT_STATS: for (int pe = 0; pe < PEs; pe++) { sum_acc[pe] = 0.0f; sumsq_acc[pe] = 0.0f; }

        int w_init_base = w_flat_idx(0, 0, 0, 0, C_IN) / PACK_256;
        PRELOAD_1_WEIGHT_PINGPONG: for (int i = 0; i < C_WORDS; i++) {
#pragma HLS PIPELINE II=1
            w_buf_256[0][i] = W_ptr[w_init_base + i];
        }

        data_256_t b_word_reg; 

        PASS_A: for (int co = 0; co < C_OUT; co++) {

            T psum[PEs][8];
#pragma HLS ARRAY_PARTITION variable=psum complete dim=0
            INIT_PSUM: for (int pe = 0; pe < PEs; pe++) {
#pragma HLS UNROLL
                for (int l = 0; l < 8; l++) {
#pragma HLS UNROLL
                    psum[pe][l] = (T)0;
                }
            }

            constexpr int8_t KH_LUT[10] = {0,0,0,1,1,1,2,2,2, 0};
            constexpr int8_t KW_LUT[10] = {0,1,2,0,1,2,0,1,2, 0};
            
            // Precompute co_base ONCE before CONV_STEP_LOOP (Loop-Invariant Code Motion)
            int co_word_base = (co * 9) * (C_IN / PACK_256);

            CONV_STEP_LOOP: for (int step = 0; step < 9; step++) {
                int kh = KH_LUT[step];
                int kw = KW_LUT[step];

                int h_row = r * width + (kh - 1);
                int slot  = (h_row + 1) & 3;

                int wn_base;
                bool do_load = false;
                
                // wn_base prefetch for next step
                if (step < 8) {
                    wn_base = co_word_base + (step + 1) * (C_IN / PACK_256);
                    do_load = true;
                } else if (co < C_OUT - 1) { 
                    wn_base = co_word_base + 9 * (C_IN / PACK_256);
                    do_load = true;
                }

                data_256_t w_word_reg;
                data_256_t x_word_reg[PEs];
#pragma HLS ARRAY_PARTITION variable=x_word_reg complete

                COMPUTE_LOAD_LOOP: for (int ci = 0; ci < C_IN; ci += 16) {
#pragma HLS PIPELINE II=1
                    int widx    = ci / 16;
                    // Global index ensures distance=8 across CONV_STEP_LOOP boundaries in STP pipeline
                    int acc_idx = (step * (C_IN / 16) + widx) & 7;
#pragma HLS DEPENDENCE variable=psum type=inter false

                    // ping derived from co+step avoids the 44-stage RTL lag in loop-carried ping
                    int ping = (co + step) & 1;
                    w_word_reg = w_buf_256[ping][widx];

                    T w0 = bits_to_half<T>(w_word_reg.range(15, 0));
                    T w1 = bits_to_half<T>(w_word_reg.range(31, 16));
                    T w2 = bits_to_half<T>(w_word_reg.range(47, 32));
                    T w3 = bits_to_half<T>(w_word_reg.range(63, 48));
                    T w4 = bits_to_half<T>(w_word_reg.range(79, 64));
                    T w5 = bits_to_half<T>(w_word_reg.range(95, 80));
                    T w6 = bits_to_half<T>(w_word_reg.range(111, 96));
                    T w7 = bits_to_half<T>(w_word_reg.range(127, 112));
                    T w8 = bits_to_half<T>(w_word_reg.range(143, 128));
                    T w9 = bits_to_half<T>(w_word_reg.range(159, 144));
                    T w10 = bits_to_half<T>(w_word_reg.range(175, 160));
                    T w11 = bits_to_half<T>(w_word_reg.range(191, 176));
                    T w12 = bits_to_half<T>(w_word_reg.range(207, 192));
                    T w13 = bits_to_half<T>(w_word_reg.range(223, 208));
                    T w14 = bits_to_half<T>(w_word_reg.range(239, 224));
                    T w15 = bits_to_half<T>(w_word_reg.range(255, 240));

                    PE_UNROLL: for (int pe = 0; pe < PEs; pe++) {
#pragma HLS UNROLL
                        int wx  = pe + ((int)kw - 1);
                        // --- ON-THE-FLY COLUMN REFLECT PADDING LOGIC ---
                        int wc  = (wx < 0) ? -wx : ((wx >= W) ? (W<<1)-wx-2 : wx);
                        
                        x_word_reg[pe] = x_buf_256[slot][wc][widx];

                        T x0 = bits_to_half<T>(x_word_reg[pe].range(15, 0));
                        T x1 = bits_to_half<T>(x_word_reg[pe].range(31, 16));
                        T x2 = bits_to_half<T>(x_word_reg[pe].range(47, 32));
                        T x3 = bits_to_half<T>(x_word_reg[pe].range(63, 48));
                        T x4 = bits_to_half<T>(x_word_reg[pe].range(79, 64));
                        T x5 = bits_to_half<T>(x_word_reg[pe].range(95, 80));
                        T x6 = bits_to_half<T>(x_word_reg[pe].range(111, 96));
                        T x7 = bits_to_half<T>(x_word_reg[pe].range(127, 112));
                        T x8 = bits_to_half<T>(x_word_reg[pe].range(143, 128));
                        T x9 = bits_to_half<T>(x_word_reg[pe].range(159, 144));
                        T x10 = bits_to_half<T>(x_word_reg[pe].range(175, 160));
                        T x11 = bits_to_half<T>(x_word_reg[pe].range(191, 176));
                        T x12 = bits_to_half<T>(x_word_reg[pe].range(207, 192));
                        T x13 = bits_to_half<T>(x_word_reg[pe].range(223, 208));
                        T x14 = bits_to_half<T>(x_word_reg[pe].range(239, 224));
                        T x15 = bits_to_half<T>(x_word_reg[pe].range(255, 240));

                        T sum_16 = (
                                       ( (x0*w0 + x1*w1) + (x2*w2 + x3*w3) ) + 
                                       ( (x4*w4 + x5*w5) + (x6*w6 + x7*w7) )
                                   ) + (
                                       ( (x8*w8 + x9*w9) + (x10*w10 + x11*w11) ) + 
                                       ( (x12*w12 + x13*w13) + (x14*w14 + x15*w15) )
                                   );

                        psum[pe][acc_idx] += sum_16;
                    }

                    if (do_load) {
                        w_buf_256[1-ping][widx] = W_ptr[wn_base + widx];
                    }
                }
            }

            if ((co % PACK_256) == 0) {
                b_word_reg = b_buf_256[co / PACK_256];
            } else {
                b_word_reg >>= 16; 
            }
            T bias = bits_to_half<T>(b_word_reg.range(15, 0)); 

            FINAL_ACCUMULATION: for (int pe = 0; pe < PEs; pe++) {
#pragma HLS PIPELINE II=1
                T total = (
                              ( (psum[pe][0] + psum[pe][1]) + (psum[pe][2] + psum[pe][3]) ) +
                              ( (psum[pe][4] + psum[pe][5]) + (psum[pe][6] + psum[pe][7]) )
                          );

                T fout = total + bias;
                y_cache_16[pe][co] = fout;

                float ff = (float)fout;
                sum_acc[pe]   += ff;
                sumsq_acc[pe] += ff * ff;
            }

            // ---- PACKED OUTPUT: {10-bit NHWC win_cnt, 64-bit double psum} ----
            // win_idx = NHWC flat index: n*H*W*C + h*W*C + w*C + c
            // Kept in a separate loop to preserve FINAL_ACCUMULATION II=1.
            PACK_OUTPUT_PE: for (int pe = 0; pe < PEs; pe++) {
                int win_idx = ((n * H + r) * W + pe) * C_OUT + co;

                // half → float → double preserves full precision path
                double d_val = (double)(float)y_cache_16[pe][co];

                // Bit-cast double to its raw IEEE-754 bits (no value change)
                ap_uint<64> d_bits;
#if defined(__SYNTHESIS__)
                union { double d; unsigned long long u; } _pck;
                _pck.d = d_val;
                d_bits = _pck.u;
#else
                {
                    unsigned long long _tmp;
                    std::memcpy(&_tmp, &d_val, sizeof(_tmp));
                    d_bits = _tmp;
                }
#endif
                // Pack: [127:74]=0 (padding) | [73:64]=win_cnt | [63:0]=psum bits
                ap_uint<128> packed = 0;
                packed.range(63,  0) = d_bits;
                packed.range(73, 64) = (ap_uint<10>)win_idx;

                psum_packed[win_idx] = packed;
            }

        }

        if (r < rolls - 1) {
            int h_next = r + 2;
            // --- ON-THE-FLY ROW REFLECT PADDING LOGIC ---
            int h_c    = (h_next >= H) ? (H<<1)-h_next-2 : h_next;
            int slot   = (h_next + 1) & 3;
            int base   = flat_idx(n, h_c, 0, 0, H, W, C_IN);

            PREFETCH_NEXT_INPUT_ROW: for (int i = 0; i < WPR; i++) {
#pragma HLS PIPELINE II=1
                x_buf_256[slot][i / C_WORDS][i % C_WORDS] = X_ptr[base / PACK_256 + i];
            }

            if (!is_final_layer && !conv_only && h_next < H) {
                int skip_base = h_next * WPR;
                FETCH_NEXT_SKIP_ROW: for (int i = 0; i < WPR; i++) {
#pragma HLS PIPELINE II=1
                    skip_buf_256[skip_base + i] = x_buf_256[slot][i / C_WORDS][i % C_WORDS];
                }
            }
        }

        PASS_B: for (int pe = 0; pe < PEs; pe++) {
            float mean_f  = sum_acc[pe] / (float)C_OUT;
            float var_f   = sumsq_acc[pe] * pre_div_sq;
            float denom_f = var_f - mean_f * mean_f * (float)adjustment_scale + (float)epsilon;
            T inv_std     = (T)(1.0f / my_sqrt_f(denom_f));
            T mean_t      = (T)mean_f;

            int skip_256_base = ((r * W) + pe) * (C_OUT / PACK_256);
            int out_off_256   = flat_idx(n, r*width, pe, 0, H, W, C_OUT) / PACK_256;

            CHANNEL_NORM: for (int co = 0; co < C_OUT; co += PACK_256) {
#pragma HLS PIPELINE II=1
                int word_idx = co / PACK_256;

                data_256_t out_word = 0;

                if (conv_only) {
                    // Raw Conv + bias dump (skip ChannelNorm + activation)
                    OUT_RAW_16: for (int k = 0; k < PACK_256; k++) {
#pragma HLS UNROLL
                        T yvT = y_cache_16[pe][co + k];
                        out_word.range(16*k+15, 16*k) = (ap_uint<16>)half_to_bits(yvT);
                    }
                } else {
                    data_256_t g_word   = gamma_buf_256[word_idx];
                    data_256_t bet_word = beta_buf_256 [word_idx];
                    data_256_t sk_word  = is_final_layer
                                          ? skip_buf_256[skip_256_base + word_idx]
                                          : (data_256_t)0;

                    UNROLL_16: for (int k = 0; k < PACK_256; k++) {
#pragma HLS UNROLL
                        T yvT   = y_cache_16[pe][co + k];
                        T gamma = bits_to_half<T>(g_word.range  (16*k+15, 16*k));
                        T beta  = bits_to_half<T>(bet_word.range(16*k+15, 16*k));

                        T weight   = gamma * inv_std;
                        T bias_v   = beta - (mean_t * weight);
                        T norm_out = yvT * weight + bias_v;

                        T act_out;
                        if (is_final_layer) {
                            T skip_v = bits_to_half<T>(sk_word.range(16*k+15, 16*k));
                            act_out  = norm_out + skip_v;
                        } else {
                            act_out = (norm_out < (T)0) ? (T)0 : norm_out;
                        }
                        out_word.range(16*k+15, 16*k) = (ap_uint<16>)half_to_bits(act_out);
                    }
                }

                Out_ptr[out_off_256 + word_idx] = out_word;
            }
        } 
    } 
}

// ============================================================
// TOP KERNEL: Conv-Only (Layer 1 Conv 3x3 + bias, raw)
// ============================================================
// Isolates the convolution datapath of the ResBlock: runs ONLY the
// first 3x3 convolution + bias (Conv1/W1/B1) and writes the raw result
// to Y. ChannelNorm, ReLU, the second conv and the residual add are all
// removed. PASS_A (MAC / ping-pong weight load / reflect-pad / rotating
// accumulator) is byte-for-byte identical to the full ResidualBlock so
// any board mismatch can be attributed to the conv core alone.
// ============================================================
template<int PEs, int C_IN, int C_OUT, int BATCH, typename T>
void ConvOnly_Kernel(
    DDR_PTR         X_ptr,
    DDR_CONST_PTR   W1_ptr,
    DDR_CONST_PTR   B1_ptr,
    T               epsilon,
    DDR_PTR         Y_ptr,
    ap_uint<128>*   psum_packed         // [N][H][W][C_OUT] packed {win_cnt[9:0], psum_double[63:0]}
) {
    data_256_t x_buf_256[4][MODEL_W][C_IN / PACK_256];
#pragma HLS BIND_STORAGE variable=x_buf_256 type=ram_2p impl=bram
#pragma HLS ARRAY_PARTITION variable=x_buf_256 complete dim=2

    data_256_t skip_buf_256[MODEL_H * MODEL_W * C_OUT / PACK_256];
#pragma HLS BIND_STORAGE variable=skip_buf_256 type=ram_2p impl=uram

    T y_cache_16[PEs][C_OUT];
#pragma HLS BIND_STORAGE variable=y_cache_16 type=ram_2p impl=bram
#pragma HLS ARRAY_PARTITION variable=y_cache_16 cyclic factor=16 dim=2

    data_256_t b_buf_256   [C_OUT / PACK_256];
    data_256_t gamma_buf_256[C_OUT / PACK_256];   // unused (conv_only)
    data_256_t beta_buf_256 [C_OUT / PACK_256];   // unused (conv_only)

    data_256_t w_buf_256[2][C_IN / PACK_256];
#pragma HLS ARRAY_PARTITION variable=w_buf_256 complete dim=1

    Batch_loop:
    for (int n = 0; n < BATCH; n++) {

        LOAD_BIAS_ONLY: for (int i = 0; i < C_OUT / PACK_256; i++) {
#pragma HLS PIPELINE II=1
            b_buf_256[i] = B1_ptr[i];
        }

        init_rows<MODEL_W, C_IN, /*SAVE_SKIP=*/false, T>(
            X_ptr, x_buf_256, skip_buf_256, n);

        Fused_Conv_CNorm_Act<PEs, C_IN, C_OUT, T>(
            X_ptr, W1_ptr, /*Out=*/Y_ptr,
            b_buf_256, gamma_buf_256, beta_buf_256,
            x_buf_256, skip_buf_256, w_buf_256, y_cache_16,
            epsilon, n, /*is_final_layer=*/false, /*conv_only=*/true,
            psum_packed);
    }
}
