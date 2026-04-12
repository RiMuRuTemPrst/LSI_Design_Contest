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
// SUB-FUNCTION 1: load_params
// ============================================================
// Purpose : Load bias / gamma / beta from DDR → on-chip data_256_t buffers.
// Shared  : Called identically for Layer 1 (B1/G1/BE1) and Layer 2 (B2/G2/BE2).
// Transfer: DDR ──256-bit/beat──► b_buf_256 / gamma_buf_256 / beta_buf_256
//           Data remains packed as 256-bit words; unpacking is deferred to PASS B.
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
    // Three separate burst loops → each AXI port delivers II=1 independently.
    // One combined loop caused II=3 because B_ptr/G_ptr/BE_ptr share gmem_param bundle.
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
// SUB-FUNCTION 2: init_rows
// ============================================================
// Purpose  : Load 3 IFM seed rows from DDR → circular x_buf_256 (BRAM).
// Shared   : SAVE_SKIP=true for Layer 1 (copies input identity to URAM skip buffer).
//            SAVE_SKIP=false for Layer 2 (row load only; skip already saved).
// Transfer : DDR ──256-bit/beat──► x_buf_256[slot][w][c_word]
//            x_buf_256 ──256-bit/beat──► skip_buf_256 (Layer 1 only)
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
    constexpr int C_WORDS       = C_IN / PACK_256;         // e.g. 60 for C_IN=960
    constexpr int WORDS_PER_ROW = W_DIM * C_WORDS;         // 256-bit words per IFM row

    INIT_3_INPUT_ROWS: for (int row_init = 0; row_init < 3; row_init++) {
        int h       = row_init - 1;
        int h_clamp = (h < 0) ? -h : ((h >= H) ? (H << 1) - h - 2 : h);
        int slot    = (h + 1) & 3;                     // Circular buffer slot [0..3]
        int base    = flat_idx(n, h_clamp, 0, 0, H, W, C_IN); // Flat element offset

        // Load one full IFM row as 256-bit words
        LOAD_1_INPUT_ROW: for (int i = 0; i < WORDS_PER_ROW; i++) {
#pragma HLS PIPELINE II=1
            x_buf_256[slot][i / C_WORDS][i % C_WORDS] = X_ptr[base / PACK_256 + i];
        }

        // Save identity skip connection (Layer 1 only — compile-time eliminated for L2)
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
// SUB-FUNCTION 3: conv_cnorm_act
// ============================================================
// Purpose     : Full ResBlock compute stage for one layer.
//               IS_FINAL_LAYER=false → Layer 1 (Conv → CNorm → ReLU → X_ptr)
//               IS_FINAL_LAYER=true  → Layer 2 (Conv → CNorm → Add Skip → Y_ptr)
//
// Internal 256-bit data paths:
//   x_buf_256 ──256b──► unpack 4 FP16/PE ──► PE×16 accumulate ──pack──► y_cache_256
//   y_cache_256 ──256b──► unpack 16 FP16 ──► CNorm ──► ReLU/AddSkip ──pack──► DDR
//   skip_buf_256 ──256b──► unpack 16 FP16 (Layer 2 Add)
//   W_ptr DDR ──256b──► w_buf_256 ping-pong ──► weight unpacking
// ============================================================
template<int PEs, int C_IN, int C_OUT, typename T>
static void conv_cnorm_act(
    DDR_CONST_PTR   X_ptr,                                         // IFM (DDR, 256-bit)
    DDR_CONST_PTR   W_ptr,                                         // Weights (DDR, 256-bit)
    DDR_PTR         Out_ptr,                                       // Output (DDR, 256-bit)
    data_256_t      b_buf_256   [C_OUT / PACK_256],                // Bias   (on-chip, 256-bit)
    data_256_t      gamma_buf_256[C_OUT / PACK_256],               // Gamma  (on-chip, 256-bit)
    data_256_t      beta_buf_256 [C_OUT / PACK_256],               // Beta   (on-chip, 256-bit)
    data_256_t      x_buf_256[4][MODEL_W][C_IN / PACK_256],        // IFM circular buffer
    data_256_t      skip_buf_256[MODEL_H * MODEL_W * C_OUT / PACK_256], // URAM skip buffer
    data_256_t      w_buf_256[2][C_IN / PACK_256],                 // Weight ping-pong
    T               y_cache_16[PEs][C_OUT],                        // Conv output (FP16 array to strictly avoid wide RMW)
    T               epsilon,
    int             n,                                             // Batch index
    bool            is_final_layer                                 // Extracted from template to enable Resource Sharing
) {
#pragma HLS INLINE off
    constexpr int H         = MODEL_H;
    constexpr int W         = MODEL_W;
    constexpr int width     = PEs / W;
    constexpr int rolls     = H / width;
    constexpr int C_WORDS   = C_IN / PACK_256;      // 256-bit words per channel set
    constexpr int WPR       = MODEL_W * C_IN / PACK_256; // Words per IFM row

    const int   num              = C_OUT;
    const T     adjustment_scale = (T)num / (T)(num - 1);
    const float pre_div          = 1.0f / my_sqrt_f((float)(num - 1));
    const float pre_div_sq       = pre_div * pre_div;

    Slide_PEs_loop:
    for (int r = 0; r < rolls; r++) {

        // ChannelNorm statistics accumulators (one per PE)
        float sum_acc[PEs];
        float sumsq_acc[PEs];
#pragma HLS ARRAY_PARTITION variable=sum_acc   complete
#pragma HLS ARRAY_PARTITION variable=sumsq_acc complete
        INIT_STATS: for (int pe = 0; pe < PEs; pe++) { sum_acc[pe] = 0.0f; sumsq_acc[pe] = 0.0f; }

        // ================================================================
        // PASS A: Convolution
        // Transfer: x_buf_256 ──256-bit──► unpack 4 FP16/PE/cycle
        //           ──► PE×16 MAC ──pack──► y_cache_256
        // ================================================================
        
        int ping = 0;

        // Pre-load very first Ping buffer (co=0, step=0) BEFORE the co loop
        // to completely eliminate the 57,600 cycle LOAD_1_WEIGHT_PINGPONG penalty.
        int w_init_base = w_flat_idx(0, 0, 0, 0, C_IN) / PACK_256;
        LOAD_1_WEIGHT_PINGPONG: for (int i = 0; i < C_WORDS; i++) {
#pragma HLS PIPELINE II=1
            w_buf_256[0][i] = W_ptr[w_init_base + i];
        }

        data_256_t b_word_reg; // Gearbox Shift Register cho Bias

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

            // Scan all 9 positions of the 3×3 kernel
            CONV_STEP_LOOP: for (int step = 0; step < 9; step++) {
                int kh = step / 3, kw = step % 3;
                int ns   = step + 1;
                int n_kh = ns / 3, n_kw = ns % 3;

                int h_row = r * width + (kh - 1);
                int slot  = (h_row + 1) & 3;

                // Pre-compute Pong weight word base (for next step OR next co output channel)
                int wn_base;
                bool do_load = false;
                if (ns < 9) {
                    wn_base = w_flat_idx(co, n_kh, n_kw, 0, C_IN) / PACK_256;
                    do_load = true;
                } else if (co < C_OUT - 1) { // step == 8, load step=0 of Next Channel!
                    wn_base = w_flat_idx(co + 1, 0, 0, 0, C_IN) / PACK_256;
                    do_load = true;
                }

                // Gearbox Shift Registers (Replaces dynamic MUXing)
                data_256_t w_word_reg;
                data_256_t x_word_reg[PEs];
#pragma HLS ARRAY_PARTITION variable=x_word_reg complete

                // Compute + Pong-load interleaved (PIPELINE II=1)
                // ci steps by 4 — unpack 4 FP16 from 256-bit word per cycle
                COMPUTE_LOAD_LOOP: for (int ci = 0; ci < C_IN; ci += 4) {
#pragma HLS PIPELINE II=1
                    int acc_idx = (ci / 4) & 7;
                    int widx    = ci / PACK_256;   // Which 256-bit word
                    int boff    = ci % PACK_256;   // Bit offset within word: 0, 4, 8, 12
#pragma HLS DEPENDENCE variable=psum type=inter false

                    // --- Gearbox: Load from BRAM or Shift ---
                    if (boff == 0) {
                        w_word_reg = w_buf_256[ping][widx];
                    } else {
                        w_word_reg >>= 64; // Zero-LUT shift down
                    }

                    // Extract static 64-bit slice perfectly mapped to MUX-free wire routing
                    T w0 = bits_to_half<T>(w_word_reg.range(15, 0));
                    T w1 = bits_to_half<T>(w_word_reg.range(31, 16));
                    T w2 = bits_to_half<T>(w_word_reg.range(47, 32));
                    T w3 = bits_to_half<T>(w_word_reg.range(63, 48));

                    // --- 256-bit x_buf_256 → PE kernel ---
                    // Each PE reads a 256-bit word and unpacks 4 FP16 values per cycle
                    PE_UNROLL: for (int pe = 0; pe < PEs; pe++) {
#pragma HLS UNROLL
                        int wx  = pe + (kw - 1);
                        int wc  = (wx < 0) ? -wx : ((wx >= W) ? (W<<1)-wx-2 : wx);
                        
                        if (boff == 0) {
                            x_word_reg[pe] = x_buf_256[slot][wc][widx];
                        } else {
                            x_word_reg[pe] >>= 64; // Zero-LUT shift down
                        }

                        T x0 = bits_to_half<T>(x_word_reg[pe].range(15, 0));
                        T x1 = bits_to_half<T>(x_word_reg[pe].range(31, 16));
                        T x2 = bits_to_half<T>(x_word_reg[pe].range(47, 32));
                        T x3 = bits_to_half<T>(x_word_reg[pe].range(63, 48));

                        psum[pe][acc_idx] += (x0*w0 + x1*w1) + (x2*w2 + x3*w3);
                    }

                    // --- Pong load: one 256-bit word per 4 ci steps (boff==0) ---
                    // Loads next-step or next-channel weights while current-step computation runs
                    if (do_load && boff == 0) {
                        w_buf_256[1-ping][widx] = W_ptr[wn_base + widx];
                    }
                } // End COMPUTE_LOAD_LOOP
                ping ^= 1; // Swap ping and pong
            } // End CONV_STEP_LOOP

            // Unpack bias for channel co using Gearbox ! (0 LUT MUX, 1 BRAM read per 16 co)
            if ((co % PACK_256) == 0) {
                b_word_reg = b_buf_256[co / PACK_256];
            } else {
                b_word_reg >>= 16; // Shift down by 16 bits (one FP16)
            }
            T bias = bits_to_half<T>(b_word_reg.range(15, 0)); // Hard-wiring, Zero-LUT MUX

            // --- Final reduction → write into y_cache_16 ---
            // Transfer: Conv output ─FP16─► y_cache_16[pe][co] (Conv → CNorm interface)
            FINAL_ACCUMULATION: for (int pe = 0; pe < PEs; pe++) {
#pragma HLS PIPELINE II=1
                T total = (T)0;
                for (int l = 0; l < 8; l++) total += psum[pe][l];

                T fout = total + bias;

                // Write single FP16 directly (Tốn đúng 1 nhịp Write, KHÔNG Read-Modify-Write)
                y_cache_16[pe][co] = fout;

                float ff = (float)fout;
                sum_acc[pe]   += ff;
                sumsq_acc[pe] += ff * ff;
            }
        } // End PASS_A

        // --- Pre-fetch next IFM row + optionally save skip (Layer 1 only) ---
        if (r < rolls - 1) {
            int h_next = r + 2;
            int h_c    = (h_next >= H) ? (H<<1)-h_next-2 : h_next;
            int slot   = (h_next + 1) & 3;
            int base   = flat_idx(n, h_c, 0, 0, H, W, C_IN);

            PREFETCH_NEXT_INPUT_ROW: for (int i = 0; i < WPR; i++) {
#pragma HLS PIPELINE II=1
                x_buf_256[slot][i / C_WORDS][i % C_WORDS] = X_ptr[base / PACK_256 + i];
            }

            // Save skip for newly prefetched row (runtime-checked for is_final_layer)
            if (!is_final_layer && h_next < H) {
                int skip_base = h_next * WPR;
                FETCH_NEXT_SKIP_ROW: for (int i = 0; i < WPR; i++) {
#pragma HLS PIPELINE II=1
                    skip_buf_256[skip_base + i] = x_buf_256[slot][i / C_WORDS][i % C_WORDS];
                }
            }
        }

        // ================================================================
        // PASS B: ChannelNorm → ReLU / Add Skip → DDR
        // Transfer: y_cache_256 ──256-bit──► unpack 16 FP16/cycle
        //           ──► CNorm ──► ReLU (L1) or + skip_buf_256 (L2)
        //           ──pack─► out_word ──256-bit──► DDR
        // ================================================================
        PASS_B: for (int pe = 0; pe < PEs; pe++) {
            float mean_f  = sum_acc[pe] / (float)C_OUT;
            float var_f   = sumsq_acc[pe] * pre_div_sq;
            float denom_f = var_f - mean_f * mean_f * (float)adjustment_scale + (float)epsilon;
            T inv_std     = (T)(1.0f / my_sqrt_f(denom_f));
            T mean_t      = (T)mean_f;

            // Skip connection base index (pixel: row r, column pe)
            int skip_256_base = ((r * W) + pe) * (C_OUT / PACK_256);
            // Output DDR word base
            int out_off_256   = flat_idx(n, r*width, pe, 0, H, W, C_OUT) / PACK_256;

            // 16 FP16 channels processed per pipeline cycle (true 256-bit/cycle throughput)
            CHANNEL_NORM_PASS_B: for (int co = 0; co < C_OUT; co += PACK_256) {
#pragma HLS PIPELINE II=1
                int word_idx = co / PACK_256;

                // 256-bit read: CNorm parameters
                data_256_t g_word   = gamma_buf_256[word_idx];
                data_256_t bet_word = beta_buf_256 [word_idx];
                // 256-bit read: skip connection (L2 only — compile-time mux)
                data_256_t sk_word  = is_final_layer
                                      ? skip_buf_256[skip_256_base + word_idx]
                                      : (data_256_t)0;

                data_256_t out_word = 0;

                // Unroll 16 FP16 lanes (CNorm → ReLU/AddSkip interface at 256-bit)
                UNROLL_16: for (int k = 0; k < PACK_256; k++) {
#pragma HLS UNROLL
                    T yvT   = y_cache_16[pe][co + k]; // Read 16 items cleanly from 16 cyclically partitioned banks
                    T gamma = bits_to_half<T>(g_word.range  (16*k+15, 16*k));
                    T beta  = bits_to_half<T>(bet_word.range(16*k+15, 16*k));

                    // ChannelNorm: normalize using per-tile statistics
                    T weight   = gamma * inv_std;
                    T bias_v   = beta - (mean_t * weight);
                    T norm_out = yvT * weight + bias_v;

                    T act_out;
                    if (is_final_layer) {
                        // Layer 2: Add Skip Connection (256-bit skip transfer)
                        T skip_v = bits_to_half<T>(sk_word.range(16*k+15, 16*k));
                        act_out  = norm_out + skip_v;
                    } else {
                        // Layer 1: ReLU activation
                        act_out = (norm_out < (T)0) ? (T)0 : norm_out;
                    }
                    // Pack 16 FP16 results into one 256-bit output word
                    out_word.range(16*k+15, 16*k) = (ap_uint<16>)half_to_bits(act_out);
                }

                // Write packed 256-bit output word directly to DDR
                Out_ptr[out_off_256 + word_idx] = out_word;
            } // End CHANNEL_PASS_B
        } // End PASS_B
    } // End Slide_PEs_loop
}

// ============================================================
// MAIN RESBLOCK KERNEL  (Top-level caller)
// ============================================================
// Orchestrates 2 layers by calling the 3 shared sub-functions:
//   Layer 1: load_params → init_rows<SAVE_SKIP=true> → conv_cnorm_act<IS_FINAL=false>
//   Layer 2: load_params → init_rows<SAVE_SKIP=false> → conv_cnorm_act<IS_FINAL=true>
//
// ALL on-chip buffers are typed as data_256_t (256-bit).
// ============================================================
template<int PEs, int C_IN, int C_OUT, int BATCH, typename T>
void Resblock___Pad_ref_Conv_11133111111_CNorm_Relu___(
    DDR_PTR         X_ptr,
    DDR_CONST_PTR   W1_ptr, DDR_CONST_PTR B1_ptr,
    DDR_CONST_PTR   G1_ptr, DDR_CONST_PTR BE1_ptr,
    DDR_CONST_PTR   W2_ptr, DDR_CONST_PTR B2_ptr,
    DDR_CONST_PTR   G2_ptr, DDR_CONST_PTR BE2_ptr,
    T               epsilon,
    DDR_PTR         Y_ptr
) {
    // ----------------------------------------------------------
    // ON-CHIP MEMORY — ALL DATA_256_T (256-bit/cycle transfers)
    // ----------------------------------------------------------

    // 1. IFM Circular Buffer: x_buf_256
    //    Layout: [4 slots][W pixel positions][C_IN/16 channel words]
    //    dim=1 (slots): unpartitioned → we only read from ONE slot during any cycle. Saves 75% BRAM!
    //    dim=2 (W)    : complete partition → 16 independent banks.
    //                   Guarantees at most 2 PE accesses per bank even with boundary reflection, keeping II=1.
    //    Total banks: 16 (was 64 when dim=1 complete)
    data_256_t x_buf_256[4][MODEL_W][C_IN / PACK_256];
#pragma HLS BIND_STORAGE variable=x_buf_256 type=ram_2p impl=bram
#pragma HLS ARRAY_PARTITION variable=x_buf_256 complete dim=2

    // 2. URAM Skip Buffer: stores original input for residual Add (Layer 2)
    //    Packed 256-bit: skip_buf_256[pixel] = 16 FP16 channels per word
    data_256_t skip_buf_256[MODEL_H * MODEL_W * C_OUT / PACK_256];
#pragma HLS BIND_STORAGE variable=skip_buf_256 type=ram_2p impl=uram

    // 3. Conv Output Cache: y_cache_16  (Conv → CNorm interface)
    //    Layout: [PEs rows][C_OUT channel FP16 words]
    //    Not static: HLS reuses the single instance across both layer calls.
    //    Cyclic dim=2 avoids 256-bit structural Read-Modify-Write and allows 16 parallel FP16 reads.
    T y_cache_16[PEs][C_OUT];
#pragma HLS BIND_STORAGE variable=y_cache_16 type=ram_2p impl=bram
#pragma HLS ARRAY_PARTITION variable=y_cache_16 cyclic factor=16 dim=2

    // 4. CNorm Parameter Buffers (256-bit packed)
    data_256_t b_buf_256   [C_OUT / PACK_256];
    data_256_t gamma_buf_256[C_OUT / PACK_256];
    data_256_t beta_buf_256 [C_OUT / PACK_256];

    // 5. Weight Ping-Pong Buffer (256-bit, 2 banks)
    //    ping = w_buf_256[0], pong = w_buf_256[1]
    data_256_t w_buf_256[2][C_IN / PACK_256];
#pragma HLS ARRAY_PARTITION variable=w_buf_256 complete dim=1

    // ----------------------------------------------------------
    // BATCH LOOP
    // ----------------------------------------------------------
    Batch_loop:
    for (int n = 0; n < BATCH; n++) {

        // ════ LAYER 1 (times = 0): Input → Conv1 → ChannelNorm → ReLU ════
        load_params<C_OUT, T>(
            B1_ptr, G1_ptr, BE1_ptr,
            b_buf_256, gamma_buf_256, beta_buf_256);

        init_rows<MODEL_W, C_IN, /*SAVE_SKIP=*/true, T>(
            X_ptr, x_buf_256, skip_buf_256, n);

        conv_cnorm_act<PEs, C_IN, C_OUT, T>(
            X_ptr, W1_ptr, /*Out=*/X_ptr,   // Write result back to X_ptr
            b_buf_256, gamma_buf_256, beta_buf_256,
            x_buf_256, skip_buf_256, w_buf_256, y_cache_16,
            epsilon, n, /*is_final_layer=*/false);

        // ════ LAYER 2 (times = 1): passA → Conv2 → ChannelNorm → Add Skip ════
        load_params<C_OUT, T>(
            B2_ptr, G2_ptr, BE2_ptr,
            b_buf_256, gamma_buf_256, beta_buf_256);

        init_rows<MODEL_W, C_IN, /*SAVE_SKIP=*/false, T>(
            X_ptr, x_buf_256, skip_buf_256, n);   // Reload from L1 output; skip already saved

        conv_cnorm_act<PEs, C_IN, C_OUT, T>(
            X_ptr, W2_ptr, /*Out=*/Y_ptr,  // Write final result to Y_ptr
            b_buf_256, gamma_buf_256, beta_buf_256,
            x_buf_256, skip_buf_256, w_buf_256, y_cache_16,
            epsilon, n, /*is_final_layer=*/true);
    }
}