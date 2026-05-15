#include "Core.h"
#include "fusion_core.h"
#include "upconv_core.h"
#include "Hls_Layers_Fusion.tpp"
#include "Hls_Layers_UpConv.tpp"
#include "Hls_Layers_Conv77.tpp"

static const int PEs_F = 8;
static const int PEs_U = 12;

static data_256_t skip_buf  [MODEL_H * MODEL_W * 60];
static data_256_t global_buf[MODEL_H * MODEL_W * 60];

// Weight layout in W_upconv (data_256_t words):
//   [0        .. 259199] UCB_0: C_OUT=480, K=9, CI_WORDS=60  (480*9*60=259200)
//   [259200   .. 323999] UCB_1: C_OUT=240, K=9, CI_WORDS=30  (240*9*30=64800)
//   [324000   .. 340199] UCB_2: C_OUT=120, K=9, CI_WORDS=15  (120*9*15=16200)
//   [340200   .. 344519] UCB_3: C_OUT=60,  K=9, CI_WORDS=8   (60*9*8=4320)
//
// Param layout in P_upconv (data_256_t words, C_WORDS_OUT = ceil(C_OUT/16)):
//   UCB_0: B[0..29],   G[30..59],  BE[60..89]    (C_WORDS_OUT=30)
//   UCB_1: B[90..104], G[105..119],BE[120..134]  (C_WORDS_OUT=15)
//   UCB_2: B[135..142],G[143..150],BE[151..158]  (C_WORDS_OUT=8)
//   UCB_3: B[159..162],G[163..166],BE[167..170]  (C_WORDS_OUT=4)

static void run_upconv_block(
    DDR_CONST_PTR     X,
    DDR_PTR           Y,
    const data_256_t* W,
    const data_256_t* P,
    data_t            epsilon,
    int               mode
) {
#pragma HLS INLINE off

    static data_256_t x_buf[2 * 128 * 60];
#pragma HLS BIND_STORAGE variable=x_buf type=ram_t2p impl=uram

    int h_in, w_in, c_in, c_out;
    int w_off;
    int b_off, g_off, be_off;

    if (mode == MODE_UCB_0) {
        h_in = 16;  w_in = 16;  c_in = 960; c_out = 480;
        w_off = 0;
        b_off = 0;   g_off = 30;  be_off = 60;
    } else if (mode == MODE_UCB_1) {
        h_in = 32;  w_in = 32;  c_in = 480; c_out = 240;
        w_off = 259200;
        b_off = 90;  g_off = 105; be_off = 120;
    } else if (mode == MODE_UCB_2) {
        h_in = 64;  w_in = 64;  c_in = 240; c_out = 120;
        w_off = 324000;
        b_off = 135; g_off = 143; be_off = 151;
    } else {                              // MODE_UCB_3
        h_in = 128; w_in = 128; c_in = 120; c_out = 60;
        w_off = 340200;
        b_off = 159; g_off = 163; be_off = 167;
    }

    const data_256_t* W_ucb  = W + w_off;
    const data_256_t* B_ucb  = P + b_off;
    const data_256_t* G_ucb  = P + g_off;
    const data_256_t* BE_ucb = P + be_off;

    int ci_words = (c_in + 15) / 16;
    DDR_CONST_PTR X_in = (mode == MODE_UCB_0) ? X : (DDR_CONST_PTR)Y;

    LOAD_ROW0: for (int wi = 0; wi < w_in; wi++) {
#pragma HLS LOOP_TRIPCOUNT min=16 max=128 avg=60
        for (int ciw = 0; ciw < ci_words; ciw++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=8 max=60 avg=28
            x_buf[(0 * w_in + wi) * ci_words + ciw] = X_in[(0 * w_in + wi) * ci_words + ciw];
        }
    }
    UpConv_Fused_Row<PEs_U>(x_buf, W_ucb, B_ucb, G_ucb, BE_ucb, Y, epsilon, h_in, w_in, c_in, c_out, 0);

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
        UpConv_Fused_Row<PEs_U>(x_buf, W_ucb, B_ucb, G_ucb, BE_ucb, Y, epsilon, h_in, w_in, c_in, c_out, 2*hi-1);
        UpConv_Fused_Row<PEs_U>(x_buf, W_ucb, B_ucb, G_ucb, BE_ucb, Y, epsilon, h_in, w_in, c_in, c_out, 2*hi);
    }
    UpConv_Fused_Row<PEs_U>(x_buf, W_ucb, B_ucb, G_ucb, BE_ucb, Y, epsilon, h_in, w_in, c_in, c_out, 2*h_in-1);
}

extern "C" {

void full_generator_top(
    data_256_t*       X,
    const data_256_t* W_fusion,
    const data_256_t* P_fusion,
    const data_256_t* W_upconv,
    const data_256_t* P_upconv,
    data_256_t*       Y,
    const data_256_t* W_conv77,
    const data_256_t* B_conv77,
    data_256_t*       Z,
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

    Universal_Engine_Kernel<PEs_F, data_t>(
        X, W_fusion, P_fusion, P_fusion, P_fusion, P_fusion, P_fusion,
        X, skip_buf, global_buf, epsilon, MODE_CBI
    );

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

    GlobalAdd_Kernel<PEs_F>(X, global_buf, X, MODEL_H * MODEL_W * 60);

    run_upconv_block(X, Y, W_upconv, P_upconv, epsilon, MODE_UCB_0);
    run_upconv_block(X, Y, W_upconv, P_upconv, epsilon, MODE_UCB_1);
    run_upconv_block(X, Y, W_upconv, P_upconv, epsilon, MODE_UCB_2);
    run_upconv_block(X, Y, W_upconv, P_upconv, epsilon, MODE_UCB_3);

    Conv77_Kernel<8, 8, 1, 60, 3, 256, 256, 7, 7, 256, 256>(Y, W_conv77, B_conv77, Z);
}

} // extern "C"
