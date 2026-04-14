// Conv77 HLS Kernel — fp16 SIMD + Spatial-PE Architecture
//
// Design:
//   1 PE = SIMD_DEPTH parallel MACs along the depth (channel) dimension
//   NUM_WIN_PEs PEs process adjacent output windows simultaneously
//
// PE structure (one PE, window position win):
//   For tci in 0..TC_IN, hf in 0..H_R, wf in 0..W_R:
//     8 parallel MACs on x_buffer[hf][win+wf][tci*8 .. tci*8+7]
//                      × w_buffer[co][hf][wf][tci*8 .. tci*8+7]
//
// Data reuse:
//   x_buffer shared across all NUM_WIN_PEs PEs for one (ho, wo_tile)
//   x_buffer shared across all C_OUT channels (loop order: spatial outer, co inner)
//   → input loaded once per (ho, wo_tile), reused for all C_OUT computations
//
// Memory layout: NHWC — depth dimension is contiguous in memory.
//   Loading SIMD_DEPTH=8 consecutive elements = single 8-word AXI burst.
//
// Parameters (hardcoded call):
//   SIMD_DEPTH=8, NUM_WIN_PEs=8
//   C_IN=60 → C_PAD=64 (padded to multiple of 8; last 4 channels zero-filled)
//   W_BUF = W_R + NUM_WIN_PEs - 1 = 14 (combined input column buffer)
//   TC_IN = C_PAD / SIMD_DEPTH = 8 tiles
//
// Estimated latency @ 200 MHz (II=1 for ap_fixed multiply):
//   Per (ho, wo_tile): ~6272 load + 1176 compute = 7448 cycles
//   Total tiles: H_OUT × (W_OUT/NUM_WIN_PEs) = 256 × 32 = 8192
//   Total: ~61M cycles = ~305 ms  (target: < 500 ms)
//
// Data type: ap_fixed<16,8> from HLS ap library (fp16)
// Accumulator: ap_fixed<32,16> (wider for stability over 2940 MAC reductions)

#include "ap_fixed.h"
#include "../non_gen/Core.h"

// fp16: 1 sign + 7 integer + 8 fractional bits, range ±127.996, resolution ~0.004
typedef ap_fixed<16, 8>  fp16_t;
// Wide accumulator: 1 sign + 15 integer + 16 fractional — prevents overflow
typedef ap_fixed<32, 16> fp32acc_t;

// Reflect-boundary padding (branchless-friendly after constant folding)
inline int reflect(int i, int MAX) {
    #pragma HLS INLINE
    if (i < 0)    return -i;
    if (i >= MAX) return (MAX - 1) * 2 - i;
    return i;
}

// ---------------------------------------------------------------------------
// Conv_7x7 — fp16 SIMD + spatial-PE convolution
//
// Template params:
//   SIMD_DEPTH  : MACs per PE (parallel depth channels), must divide C_PAD
//   NUM_WIN_PEs : adjacent output windows processed simultaneously
//   BATCH       : batch size
//   C_IN / C_OUT: input / output channels
//   H_IN / W_IN : input spatial dims
//   H_R  / W_R  : kernel size (receptive field)
//   H_OUT/ W_OUT: output spatial dims
// ---------------------------------------------------------------------------
template<int SIMD_DEPTH, int NUM_WIN_PEs,
         int BATCH, int C_IN, int C_OUT,
         int H_IN, int W_IN, int H_R, int W_R,
         int H_OUT, int W_OUT>
void Conv77_Kernel(float* X_f, float* W_f, float* B_f, float* Y_f) {

    constexpr int HALF_H  = H_R / 2;
    constexpr int HALF_W  = W_R / 2;
    // Pad C_IN up to the nearest multiple of SIMD_DEPTH
    constexpr int C_PAD   = ((C_IN + SIMD_DEPTH - 1) / SIMD_DEPTH) * SIMD_DEPTH;
    constexpr int TC_IN   = C_PAD / SIMD_DEPTH;
    // Combined buffer covers all NUM_WIN_PEs output windows + kernel width - 1
    constexpr int W_BUF   = W_R + NUM_WIN_PEs - 1;

    // -----------------------------------------------------------------------
    // On-chip buffers
    // -----------------------------------------------------------------------
    // x_buffer: sliding window — H_R rows × W_BUF cols × C_PAD channels (fp16)
    fp16_t x_buffer[H_R][W_BUF][C_PAD];
    // w_buffer: all C_OUT weight tensors (loaded once, reused every spatial tile)
    fp16_t w_buffer[C_OUT][H_R][W_R][C_PAD];
    fp16_t b_buffer[C_OUT];
    // Accumulators: (C_OUT, NUM_WIN_PEs) — one fp32 accumulator per PE per channel
    fp32acc_t acc[C_OUT][NUM_WIN_PEs];

    // -----------------------------------------------------------------------
    // Array partitioning for parallel access
    //
    // x_buffer:
    //   dim=1 (H_R=7)   : complete → 7 independent row banks
    //   dim=2 (W_BUF=14): complete → 14 independent column banks
    //   dim=3 (C_PAD=64): cyclic(SIMD_DEPTH=8) → 8 channel banks
    //   Compute access: x[hf][win+wf][tci*8+s] for all win∈[0,7], s∈[0,7]
    //     hf=single, win+wf=8 distinct cols (complete), s=8 distinct banks (cyclic) → 64 parallel reads ✓
    //
    // w_buffer:
    //   dim=1 (C_OUT=3), dim=2 (H_R=7), dim=3 (W_R=7): complete
    //   dim=4 (C_PAD=64): cyclic(SIMD_DEPTH=8) → 8 channel banks
    //   Compute access: w[co][hf][wf][tci*8+s] for all s∈[0,7] → 8 parallel reads ✓
    // -----------------------------------------------------------------------
    #pragma HLS ARRAY_PARTITION variable=x_buffer complete dim=2
    #pragma HLS ARRAY_PARTITION variable=x_buffer cyclic factor=SIMD_DEPTH dim=3

    #pragma HLS ARRAY_PARTITION variable=w_buffer complete dim=3
    #pragma HLS ARRAY_PARTITION variable=w_buffer cyclic factor=SIMD_DEPTH dim=4

    #pragma HLS ARRAY_PARTITION variable=b_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=acc      complete dim=1
    #pragma HLS ARRAY_PARTITION variable=acc      complete dim=2

    // -----------------------------------------------------------------------
    // Pre-load weights & biases (once per kernel invocation)
    // Float→fp16 quantization happens here at the AXI read boundary.
    // -----------------------------------------------------------------------
    Load_bias:
    for (int co = 0; co < C_OUT; co++) {
        #pragma HLS PIPELINE II=1
        b_buffer[co] = (fp16_t)B_f[co];
    }

    // Weight layout in DRAM: (C_OUT, H_R, W_R, C_IN) — C_OUT-first
    Preload_co:
    for (int co = 0; co < C_OUT; co++) {
        Preload_hf:
        for (int hf = 0; hf < H_R; hf++) {
            Preload_wf:
            for (int wf = 0; wf < W_R; wf++) {
                const int w_idx = ((co * H_R + hf) * W_R + wf) * C_IN;
                Preload_ci:
                for (int ci = 0; ci < C_PAD; ci++) {
                    #pragma HLS PIPELINE II=1
                    // ci >= C_IN → padding zero (last partial SIMD tile)
                    w_buffer[co][hf][wf][ci] = (ci < C_IN) ?
                        (fp16_t)W_f[w_idx + ci] : fp16_t(0);
                }
            }
        }
    }

    // -----------------------------------------------------------------------
    // Main loops
    // -----------------------------------------------------------------------
    Batch_loop:
    for (int n = 0; n < BATCH; n++) {
        const int x_n_base = n * H_IN * W_IN * C_IN;
        const int y_n_base = n * H_OUT * W_OUT * C_OUT;

        Spatial_h_loop:
        for (int ho = 0; ho < H_OUT; ho++) {

            // Precompute reflected hi values for all H_R kernel rows
            // (loop-invariant over wo)
            int hi_refl[H_R];
            #pragma HLS ARRAY_PARTITION variable=hi_refl complete dim=1
            Precomp_hi:
            for (int hf = 0; hf < H_R; hf++) {
                #pragma HLS UNROLL
                hi_refl[hf] = reflect(ho - HALF_H + hf, H_IN);
            }

            // Precompute X row base addresses
            int x_row_base[H_R];
            #pragma HLS ARRAY_PARTITION variable=x_row_base complete dim=1
            Precomp_xrow:
            for (int hf = 0; hf < H_R; hf++) {
                #pragma HLS UNROLL
                x_row_base[hf] = x_n_base + hi_refl[hf] * W_IN * C_IN;
            }

            // ------------------------------------------------------------------
            // Spatial W loop — tiled by NUM_WIN_PEs
            // Each iteration produces NUM_WIN_PEs × C_OUT output values.
            // ------------------------------------------------------------------
            Spatial_w_loop:
            for (int wo = 0; wo < W_OUT; wo += NUM_WIN_PEs) {
                #pragma HLS PIPELINE off

                // Leftmost input column for this tile
                const int col_start = wo - HALF_W;

                // --------------------------------------------------------------
                // LOAD x_buffer
                // The buffer covers W_BUF=14 input columns (shared by all PEs).
                // NHWC layout: x[n][h][w][c] — channels are contiguous.
                // Loading SIMD_DEPTH channels at once maps to burst reads.
                //
                // wo==0 : full initialization (all W_BUF columns)
                // wo> 0 : shift left by NUM_WIN_PEs, load NUM_WIN_PEs new cols
                // --------------------------------------------------------------
                if (wo == 0) {
                    Init_hf:
                    for (int hf = 0; hf < H_R; hf++) {
                        Init_col:
                        for (int col = 0; col < W_BUF; col++) {
                            const int wi = reflect(col_start + col, W_IN);
                            Init_ci:
                            for (int ci = 0; ci < C_PAD; ci++) {
                                #pragma HLS PIPELINE II=1
                                x_buffer[hf][col][ci] = (ci < C_IN) ?
                                    (fp16_t)X_f[x_row_base[hf] + wi * C_IN + ci]
                                    : fp16_t(0);
                            }
                        }
                    }
                } else {
                    // Shift left: cols 0..(W_BUF-NUM_WIN_PEs-1) ← cols NUM_WIN_PEs..W_BUF-1
                    // Source and dest are in completely separate column banks → no RAW hazard
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
                                    x_buffer[hf][col][tci * SIMD_DEPTH + s] =
                                        x_buffer[hf][col + NUM_WIN_PEs][tci * SIMD_DEPTH + s];
                                }
                            }
                        }
                    }
                    // Load NUM_WIN_PEs new rightmost columns from DRAM
                    Newcol_hf:
                    for (int hf = 0; hf < H_R; hf++) {
                        Newcol_nc:
                        for (int nc = 0; nc < NUM_WIN_PEs; nc++) {
                            // Column index within x_buffer
                            const int col = W_BUF - NUM_WIN_PEs + nc;
                            // Input pixel column (with reflect padding)
                            const int wi = reflect(col_start + col, W_IN);
                            Newcol_ci:
                            for (int ci = 0; ci < C_PAD; ci++) {
                                #pragma HLS PIPELINE II=1
                                x_buffer[hf][col][ci] = (ci < C_IN) ?
                                    (fp16_t)X_f[x_row_base[hf] + wi * C_IN + ci]
                                    : fp16_t(0);
                            }
                        }
                    }
                }

                // --------------------------------------------------------------
                // COMPUTE: for each output channel co, all NUM_WIN_PEs PEs run
                //
                // PIPELINE on tci → II=1 for ap_fixed
                // UNROLL   on win (8), wf (7), and s (8)
                //          → 448 MACs per pipeline cycle
                //
                // To solve timing: the 56-input adder tree per PE is separated into `sum`
                // (pure feed-forward, easily pipelined by HLS with FFs) and then
                // added to the loop-carried `acc` (only 1 addition per cycle).
                // --------------------------------------------------------------
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
                            
                            // Feed-forward adder tree separated from the loop-carried accumulator
                            fp32acc_t sum[NUM_WIN_PEs];
                            #pragma HLS ARRAY_PARTITION variable=sum complete dim=1
                            
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
                                        // Implicit 16x16 -> 32 multiply to use exactly 1 DSP per MAC
                                        fp32acc_t prod = x_buffer[hf][win + wf][tci * SIMD_DEPTH + s]
                                                       * w_buffer[co][hf][wf][tci * SIMD_DEPTH + s];
                                        sum[win] += prod;
                                    }
                                }
                            }
                            
                            // Single loop-carried addition per PE per channel
                            for (int win = 0; win < NUM_WIN_PEs; win++) {
                                #pragma HLS UNROLL
                                acc[co][win] += sum[win];
                            }
                        }
                    }
                }

                // --------------------------------------------------------------
                // WRITE: NUM_WIN_PEs × C_OUT outputs → DRAM (fp32acc → float)
                // Layout: NHWC, stride C_OUT between adjacent spatial positions
                // --------------------------------------------------------------
                Write_co:
                for (int co = 0; co < C_OUT; co++) {
                    #pragma HLS PIPELINE off
                    Write_win:
                    for (int win = 0; win < NUM_WIN_PEs; win++) {
                        #pragma HLS PIPELINE II=1
                        const int wo_actual = wo + win;
                        if (wo_actual < W_OUT) {
                            const int y_idx = y_n_base
                                + (ho * W_OUT + wo_actual) * C_OUT + co;
                            Y_f[y_idx] = (float)(acc[co][win]
                                               + fp32acc_t(b_buffer[co]));
                        }
                    }
                }

            } // Spatial_w_loop
        } // Spatial_h_loop
    } // Batch_loop
}


// ---------------------------------------------------------------------------
// HW_Conv7x7 — AXI master + AXI-lite control interface (synthesizable top)
//
// Instantiation: SIMD_DEPTH=8, NUM_WIN_PEs=8
//   C_PAD=64 (C_IN=60 padded to next multiple of 8, 4 zero channels appended)
//   TC_IN=8, W_BUF=14
// ---------------------------------------------------------------------------
void HW_Conv7x7(float* X, float* W, float* B, float* Y) {

    #pragma HLS INTERFACE m_axi port=X bundle=gmem_X depth=3932160 \
        max_read_burst_length=64 num_read_outstanding=4 latency=64
    #pragma HLS INTERFACE m_axi port=W bundle=gmem_W depth=8820 \
        max_read_burst_length=64 num_read_outstanding=4 latency=64
    #pragma HLS INTERFACE m_axi port=B bundle=gmem_B depth=3 \
        max_read_burst_length=4
    #pragma HLS INTERFACE m_axi port=Y bundle=gmem_Y depth=196608 \
        max_write_burst_length=64 num_write_outstanding=4

    #pragma HLS INTERFACE s_axilite port=X      bundle=control
    #pragma HLS INTERFACE s_axilite port=W      bundle=control
    #pragma HLS INTERFACE s_axilite port=B      bundle=control
    #pragma HLS INTERFACE s_axilite port=Y      bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    Conv77_Kernel<
        8,    // SIMD_DEPTH  : 8 MACs per PE (depth parallelism)
        8,    // NUM_WIN_PEs : 8 adjacent windows (spatial parallelism)
        1,    // BATCH
        60,   // C_IN
        3,    // C_OUT
        256,  // H_IN
        256,  // W_IN
        7,    // H_R
        7,    // W_R
        256,  // H_OUT
        256   // W_OUT
    >(X, W, B, Y);
}
