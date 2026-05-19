#include "upconv_core.h"
#include "Hls_Layers_UpConv.tpp"

extern "C" {

void upconv_core_top(
    DDR_CONST_PTR X,      // data_in
    const data_256_t* W,  // weights (ConvTranspose)
    const data_256_t* B,  // bias (ConvTranspose)
    const data_256_t* G,  // gamma (Norm)
    const data_256_t* BE, // beta (Norm)
    DDR_PTR       Y,      // data_out
    data_t        epsilon,
    int           mode
) {
#pragma HLS INTERFACE m_axi port=X     offset=slave bundle=gmem_in     max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=W     offset=slave bundle=gmem_weight  max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=B     offset=slave bundle=gmem_param   max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=G     offset=slave bundle=gmem_param   max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=BE    offset=slave bundle=gmem_param   max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=Y     offset=slave bundle=gmem_out     max_write_burst_length=64

#pragma HLS INTERFACE s_axilite port=X       bundle=control
#pragma HLS INTERFACE s_axilite port=W       bundle=control
#pragma HLS INTERFACE s_axilite port=B       bundle=control
#pragma HLS INTERFACE s_axilite port=G       bundle=control
#pragma HLS INTERFACE s_axilite port=BE      bundle=control
#pragma HLS INTERFACE s_axilite port=Y       bundle=control
#pragma HLS INTERFACE s_axilite port=epsilon bundle=control
#pragma HLS INTERFACE s_axilite port=mode    bundle=control
#pragma HLS INTERFACE s_axilite port=return  bundle=control

    const int PEs = 8;

    // x_buf: max(W_IN*CI_WORDS) = max(16*60,32*30,64*15,128*8) = 1024
    static data_256_t x_buf[2 * 1024];
#pragma HLS BIND_STORAGE variable=x_buf type=ram_t2p impl=uram

    int h_in, w_in, c_in, c_out;

    if (mode == MODE_UCB_0) {
        h_in = 16; w_in = 16; c_in = 960; c_out = 480;
    } else if (mode == MODE_UCB_1) {
        h_in = 32; w_in = 32; c_in = 480; c_out = 240;
    } else if (mode == MODE_UCB_2) {
        h_in = 64; w_in = 64; c_in = 240; c_out = 120;
    } else if (mode == MODE_UCB_3) {
        h_in = 128; w_in = 128; c_in = 120; c_out = 60;
    } else { // MODE_UCB_TEST
        h_in = 16; w_in = 16; c_in = 16; c_out = 16;
    }

    int ci_words = (c_in + 15) / 16;

    // Load row 0
    LOAD_ROW0: for (int wi = 0; wi < w_in; wi++) {
#pragma HLS LOOP_TRIPCOUNT min=16 max=128 avg=60
        for (int ciw = 0; ciw < ci_words; ciw++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=8 max=60 avg=28
            x_buf[(0 * w_in + wi) * ci_words + ciw] = X[(0 * w_in + wi) * ci_words + ciw];
        }
    }
    UpConv_Fused_Row<PEs>(x_buf, W, B, G, BE, Y, epsilon, h_in, w_in, c_in, c_out, 0);

    // ROW_LOOP
    ROW_LOOP: for (int hi = 1; hi < h_in; hi++) {
#pragma HLS LOOP_TRIPCOUNT min=15 max=127 avg=59
        int slot = hi % 2;
        LOAD_ROW: for (int wi = 0; wi < w_in; wi++) {
#pragma HLS LOOP_TRIPCOUNT min=16 max=128 avg=60
            for (int ciw = 0; ciw < ci_words; ciw++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=8 max=60 avg=28
                x_buf[(slot * w_in + wi) * ci_words + ciw] = X[(hi * w_in + wi) * ci_words + ciw];
            }
        }
        UpConv_Fused_Row<PEs>(x_buf, W, B, G, BE, Y, epsilon, h_in, w_in, c_in, c_out, 2*hi-1);
        UpConv_Fused_Row<PEs>(x_buf, W, B, G, BE, Y, epsilon, h_in, w_in, c_in, c_out, 2*hi);
    }

    // Last row
    UpConv_Fused_Row<PEs>(x_buf, W, B, G, BE, Y, epsilon, h_in, w_in, c_in, c_out, 2*h_in-1);
}

} // extern "C"
