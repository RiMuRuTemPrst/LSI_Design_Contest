#include "Core.h"                              // MODEL_H/W/C + data types
#include "fusion_core.h"                       // MODE_CBI, MODE_RB_L1, MODE_RB_L2, MODE_GLOBAL_ADD
#include "upconv_core.h"                       // MODE_UCB_0..3
#include "Hls_Layers_Fusion.tpp"
#include "Hls_Layers_UpConv.tpp"
#include "Hls_Layers_Conv77.tpp"               // Conv77_Kernel: Y(256x256x60) -> Z(256x256x3)

static const int PEs_F = 8;
static const int PEs_U = 8;

// Persistent URAM: ResBlock skip-connection and global skip buffers
static data_256_t skip_buf  [MODEL_H * MODEL_W * 60];
static data_256_t global_buf[MODEL_H * MODEL_W * 60];

// Run one UpConv block.
// Both X and Y are always passed so HLS creates only ONE hardware instance
// shared across all 4 UCB calls. Input selection (X vs Y) happens internally.
static void run_upconv_block(
    DDR_CONST_PTR     X,
    DDR_PTR           Y,
    const data_256_t* W,
    const data_256_t* B,
    const data_256_t* G,
    const data_256_t* BE,
    data_t            epsilon,
    int               mode
) {
#pragma HLS INLINE off

    // Ping-pong sliding window buffer (max 2 rows × 128 cols × 60 words/row)
    static data_256_t x_buf[2 * 128 * 60];
#pragma HLS BIND_STORAGE variable=x_buf type=ram_t2p impl=uram

    int h_in, w_in, c_in, c_out;
    if      (mode == MODE_UCB_0) { h_in = 16;  w_in = 16;  c_in = 960; c_out = 480; }
    else if (mode == MODE_UCB_1) { h_in = 32;  w_in = 32;  c_in = 480; c_out = 240; }
    else if (mode == MODE_UCB_2) { h_in = 64;  w_in = 64;  c_in = 240; c_out = 120; }
    else                         { h_in = 128; w_in = 128; c_in = 120; c_out = 60;  }

    // UCB_0 reads from X (latent space); UCB_1-3 read from Y (upconv output)
    DDR_CONST_PTR X_in = (mode == MODE_UCB_0) ? X : (DDR_CONST_PTR)Y;

    int ci_words = (c_in + 15) / 16;

    LOAD_ROW0: for (int wi = 0; wi < w_in; wi++) {
#pragma HLS LOOP_TRIPCOUNT min=16 max=128 avg=60
        for (int ciw = 0; ciw < ci_words; ciw++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=8 max=60 avg=28
            x_buf[(0 * w_in + wi) * ci_words + ciw] = X_in[(0 * w_in + wi) * ci_words + ciw];
        }
    }
    UpConv_Fused_Row<PEs_U>(x_buf, W, B, G, BE, Y, epsilon, h_in, w_in, c_in, c_out, 0);

    ROW_LOOP: for (int hi = 1; hi < h_in; hi++) {
#pragma HLS LOOP_TRIPCOUNT min=15 max=127 avg=59
        int slot = hi % 2;
        LOAD_ROW: for (int wi = 0; wi < w_in; wi++) {
#pragma HLS LOOP_TRIPCOUNT min=16 max=128 avg=60
            for (int ciw = 0; ciw < ci_words; ciw++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=8 max=60 avg=28
                x_buf[(slot * w_in + wi) * ci_words + ciw] = X_in[(hi * w_in + wi) * ci_words + ciw];
            }
        }
        UpConv_Fused_Row<PEs_U>(x_buf, W, B, G, BE, Y, epsilon, h_in, w_in, c_in, c_out, 2*hi-1);
        UpConv_Fused_Row<PEs_U>(x_buf, W, B, G, BE, Y, epsilon, h_in, w_in, c_in, c_out, 2*hi);
    }
    UpConv_Fused_Row<PEs_U>(x_buf, W, B, G, BE, Y, epsilon, h_in, w_in, c_in, c_out, 2*h_in-1);
}

extern "C" {

void full_generator_top(
    data_256_t*       X,          // Latent input (inout: fusion reads/writes; UCB_0 reads)
    const data_256_t* W_fusion,   // Fusion weights
    const data_256_t* P_fusion,   // Fusion params (B / G / BE / G_IN / BE_IN)
    const data_256_t* W_upconv,   // UpConv weights
    const data_256_t* P_upconv,   // UpConv params (B / G / BE)
    data_256_t*       Y,          // Intermediate 256×256×60 (inout: UCB_1-3 read+write; Conv77 reads)
    const data_256_t* W_conv77,   // Conv77 weights: 3×7×7×4 = 588 words
    const data_256_t* B_conv77,   // Conv77 bias: 1 word (bits[47:0] = RGB half)
    data_256_t*       Z,          // Final output 256×256×3: 65536 words (1 pixel/word)
    data_t            epsilon
) {
#pragma HLS BIND_STORAGE variable=skip_buf   type=ram_2p impl=uram
#pragma HLS BIND_STORAGE variable=global_buf type=ram_2p impl=uram

#pragma HLS INTERFACE m_axi port=X        offset=slave bundle=gmem_x  max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port=W_fusion offset=slave bundle=gmem_wf max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=P_fusion offset=slave bundle=gmem_pf max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=W_upconv offset=slave bundle=gmem_wu max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=P_upconv offset=slave bundle=gmem_pu max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=Y        offset=slave bundle=gmem_y  max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port=W_conv77 offset=slave bundle=gmem_wc max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=B_conv77 offset=slave bundle=gmem_bc max_read_burst_length=4
#pragma HLS INTERFACE m_axi port=Z        offset=slave bundle=gmem_z  max_write_burst_length=64

#pragma HLS INTERFACE s_axilite port=X        bundle=control
#pragma HLS INTERFACE s_axilite port=W_fusion bundle=control
#pragma HLS INTERFACE s_axilite port=P_fusion bundle=control
#pragma HLS INTERFACE s_axilite port=W_upconv bundle=control
#pragma HLS INTERFACE s_axilite port=P_upconv bundle=control
#pragma HLS INTERFACE s_axilite port=Y        bundle=control
#pragma HLS INTERFACE s_axilite port=W_conv77 bundle=control
#pragma HLS INTERFACE s_axilite port=B_conv77 bundle=control
#pragma HLS INTERFACE s_axilite port=Z        bundle=control
#pragma HLS INTERFACE s_axilite port=epsilon  bundle=control
#pragma HLS INTERFACE s_axilite port=return   bundle=control

    // =========================================================
    // STAGE 1: FUSION CORE
    // =========================================================

    // CBI: Norm-on-load -> Conv(960->960) -> Norm -> Bypass
    Universal_Engine_Kernel<PEs_F, data_t>(
        X, W_fusion, P_fusion, P_fusion, P_fusion, P_fusion, P_fusion,
        X, skip_buf, global_buf, epsilon, MODE_CBI
    );

    // 9x ResBlocks: pass P_fusion (not NULL) for G_IN/BE_IN so HLS sees identical
    // port connectivity to the CBI call above -> forces single shared hardware instance
    for (int i = 0; i < 9; i++) {
#pragma HLS LOOP_TRIPCOUNT min=9 max=9
        Universal_Engine_Kernel<PEs_F, data_t>(
            X, W_fusion, P_fusion, P_fusion, P_fusion, P_fusion, P_fusion,
            X, skip_buf, global_buf, epsilon, MODE_RB_L1
        );
        Universal_Engine_Kernel<PEs_F, data_t>(
            X, W_fusion, P_fusion, P_fusion, P_fusion, P_fusion, P_fusion,
            X, skip_buf, global_buf, epsilon, MODE_RB_L2
        );
    }

    // Global Add: X += global_buf
    GlobalAdd_Kernel<PEs_F>(X, global_buf, X, MODEL_H * MODEL_W * 60);

    // =========================================================
    // STAGE 2: UPCONV CORE
    // All 4 calls pass both X and Y -> HLS creates one shared hardware instance
    // =========================================================

    // UCB_0: 16×16×960  -> 32×32×480
    run_upconv_block(X, Y, W_upconv, P_upconv, P_upconv, P_upconv, epsilon, MODE_UCB_0);

    // UCB_1: 32×32×480  -> 64×64×240
    run_upconv_block(X, Y, W_upconv, P_upconv, P_upconv, P_upconv, epsilon, MODE_UCB_1);

    // UCB_2: 64×64×240  -> 128×128×120
    run_upconv_block(X, Y, W_upconv, P_upconv, P_upconv, P_upconv, epsilon, MODE_UCB_2);

    // UCB_3: 128×128×120 -> 256×256×60
    run_upconv_block(X, Y, W_upconv, P_upconv, P_upconv, P_upconv, epsilon, MODE_UCB_3);

    // =========================================================
    // STAGE 3: CONV77 — Y(256×256×60) -> Z(256×256×3)
    // =========================================================
    Conv77_Kernel<8, 8, 1, 60, 3, 256, 256, 7, 7, 256, 256>(Y, W_conv77, B_conv77, Z);
}

} // extern "C"
