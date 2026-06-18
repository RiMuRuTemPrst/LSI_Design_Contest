#include "Core.h"
#include "fusion_core.h"
#include "upconv_core.h"
#include "Hls_Layers_Fusion.tpp"
#include "Hls_Layers_UpConv.tpp"
#include "Hls_Layers_Conv77.tpp"

static const int PEs_F = 16;
static const int PEs_U = 8;

// W_fusion layout (data_256_t words):
//   [0        .. 120959]              CBI:    C_OUT=960, K=9, CI_WORDS=14 (960*9*14=120960)
//   [120960   .. 639359]              RB0_L1: C_OUT=960, K=9, CI_WORDS=60 (960*9*60=518400)
//   [639360   .. 1157759]             RB0_L2: same
//   [120960 + i*518400 .. ]           RBi_L1  (i=0..8, layer index 2*i)
//   [120960 + (i+1)*518400 .. ]       RBi_L2  (layer index 2*i+1)
//   Total: 120960 + 18*518400 = 9452160 words
static const int W_CBI_WORDS = 120960;
static const int W_RB_WORDS  = 518400;

// P_fusion layout (data_256_t words):
//   CBI:      B[0..59], G[60..119], BE[120..179], G_IN[180..193], BE_IN[194..207]
//   RBi_Lj:   B[208+idx*180], G[+60], BE[+120]   where idx = 2*i + j  (i=0..8, j=0/1)
//   Total: 208 + 18*180 = 3448 words
static const int P_CBI_WORDS = 208;
static const int P_RB_WORDS  = 180;

// Y ping-pong: UCB_3 output = 256*256*ceil(60/16) = 256*256*4 = 262144 words
//   Slot A: Y[0         .. 262143] — UCB_0 and UCB_2 output
//   Slot B: Y[262144    .. 524287] — UCB_1 and UCB_3 output
//   Conv77 reads from Slot B (UCB_3 output)
static const int SLOT_B_OFF = 262144;

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
    int               mode,
    int               slot_in_off,
    int               slot_out_off
) {
#pragma HLS INLINE off

    static data_256_t x_buf[2 * 1024];   // max(W_IN*CI_WORDS) = max(16*60,32*30,64*15,128*8) = 1024
#pragma HLS BIND_STORAGE variable=x_buf type=ram_t2p impl=bram   // [DBUF] moved URAM->BRAM (BRAM has headroom after w_local->URAM); frees 4 URAM for wbuf PIPO

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
    DDR_CONST_PTR X_in  = (mode == MODE_UCB_0) ? X : (DDR_CONST_PTR)(Y + slot_in_off);
    DDR_PTR       Y_out = Y + slot_out_off;

    LOAD_ROW0: for (int wi = 0; wi < w_in; wi++) {
#pragma HLS LOOP_TRIPCOUNT min=16 max=128 avg=60
        for (int ciw = 0; ciw < ci_words; ciw++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=8 max=60 avg=28
            x_buf[(0 * w_in + wi) * ci_words + ciw] = X_in[(0 * w_in + wi) * ci_words + ciw];
        }
    }
    UpConv_Fused_Row<PEs_U>(x_buf, W_ucb, B_ucb, G_ucb, BE_ucb, Y_out, epsilon, h_in, w_in, c_in, c_out, 0);

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
        UpConv_Fused_Row<PEs_U>(x_buf, W_ucb, B_ucb, G_ucb, BE_ucb, Y_out, epsilon, h_in, w_in, c_in, c_out, 2*hi-1);
        UpConv_Fused_Row<PEs_U>(x_buf, W_ucb, B_ucb, G_ucb, BE_ucb, Y_out, epsilon, h_in, w_in, c_in, c_out, 2*hi);
    }
    UpConv_Fused_Row<PEs_U>(x_buf, W_ucb, B_ucb, G_ucb, BE_ucb, Y_out, epsilon, h_in, w_in, c_in, c_out, 2*h_in-1);
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

#pragma HLS INTERFACE m_axi port=X        offset=slave bundle=gmem_x  max_read_burst_length=64 max_write_burst_length=64 depth=15360
#pragma HLS INTERFACE m_axi port=W_fusion offset=slave bundle=gmem_wf max_read_burst_length=64 depth=9452160
#pragma HLS INTERFACE m_axi port=P_fusion offset=slave bundle=gmem_pf max_read_burst_length=64 depth=3448
#pragma HLS INTERFACE m_axi port=W_upconv offset=slave bundle=gmem_wu max_read_burst_length=64 depth=344520
#pragma HLS INTERFACE m_axi port=P_upconv offset=slave bundle=gmem_pu max_read_burst_length=64 depth=171
#pragma HLS INTERFACE m_axi port=Y        offset=slave bundle=gmem_y  max_read_burst_length=64 max_write_burst_length=64 depth=524288
#pragma HLS INTERFACE m_axi port=W_conv77 offset=slave bundle=gmem_wc max_read_burst_length=64 depth=588
#pragma HLS INTERFACE m_axi port=B_conv77 offset=slave bundle=gmem_bc max_read_burst_length=4  depth=2
#pragma HLS INTERFACE m_axi port=Z        offset=slave bundle=gmem_z  max_write_burst_length=64 depth=65536

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

    // Stage 1: Fusion — CBI + 9×ResBlock + GlobalAdd
    // All calls pass W_fusion/P_fusion to maintain identical port connectivity → 1 HW instance.
    Universal_Engine_Kernel<PEs_F, data_t>(
        X,
        W_fusion + 0,
        P_fusion + 0,    // B_CBI (960 ch → 60 words)
        P_fusion + 60,   // G_CBI
        P_fusion + 120,  // BE_CBI
        P_fusion + 180,  // G_IN_CBI (220 ch → 14 words)
        P_fusion + 194,  // BE_IN_CBI
        X, skip_buf, global_buf, epsilon, MODE_CBI
    );

    for (int i = 0; i < 9; i++) {
#pragma HLS LOOP_TRIPCOUNT min=9 max=9
        Universal_Engine_Kernel<PEs_F, data_t>(
            X,
            W_fusion + W_CBI_WORDS + (2*i + 0) * W_RB_WORDS,
            P_fusion + P_CBI_WORDS + (2*i + 0) * P_RB_WORDS,
            P_fusion + P_CBI_WORDS + (2*i + 0) * P_RB_WORDS + 60,
            P_fusion + P_CBI_WORDS + (2*i + 0) * P_RB_WORDS + 120,
            P_fusion, P_fusion,   // dummy G_IN, BE_IN (not read in RB mode)
            X, skip_buf, global_buf, epsilon, MODE_RB_L1
        );
        Universal_Engine_Kernel<PEs_F, data_t>(
            X,
            W_fusion + W_CBI_WORDS + (2*i + 1) * W_RB_WORDS,
            P_fusion + P_CBI_WORDS + (2*i + 1) * P_RB_WORDS,
            P_fusion + P_CBI_WORDS + (2*i + 1) * P_RB_WORDS + 60,
            P_fusion + P_CBI_WORDS + (2*i + 1) * P_RB_WORDS + 120,
            P_fusion, P_fusion,   // dummy G_IN, BE_IN
            X, skip_buf, global_buf, epsilon, MODE_RB_L2
        );
    }

    GlobalAdd_Kernel<PEs_F>(X, global_buf, X, MODEL_H * MODEL_W * 60);

    // Stage 2: UpConv — 4 UCBs with Y ping-pong to avoid in-place aliasing
    run_upconv_block(X, Y, W_upconv, P_upconv, epsilon, MODE_UCB_0, 0,          0);
    run_upconv_block(X, Y, W_upconv, P_upconv, epsilon, MODE_UCB_1, 0,          SLOT_B_OFF);
    run_upconv_block(X, Y, W_upconv, P_upconv, epsilon, MODE_UCB_2, SLOT_B_OFF, 0);
    run_upconv_block(X, Y, W_upconv, P_upconv, epsilon, MODE_UCB_3, 0,          SLOT_B_OFF);

    // Stage 3: Conv77 — reads UCB_3 output from slot B
    Conv77_Kernel<8, 8, 1, 60, 3, 256, 256, 7, 7, 256, 256>(Y + SLOT_B_OFF, W_conv77, B_conv77, Z);
}

} // extern "C"
