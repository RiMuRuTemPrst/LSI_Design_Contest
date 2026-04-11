#include <ap_int.h>
// #include "/home/qanh_k67/Project/GAN_HLS/HLS/Conv_Transpose_layer/src/non_gen/Core.h"
#include "/home/rimurutempest/Code/LSI_Design_Contest/HLS/convtranspose/src/non_gen/Core.h"
#ifdef __SYNTHESIS__
typedef ap_uint<64> wide_t;
#else
typedef data_t wide_t;  // C-sim: wide_t = data_t, không cần unpack
#endif

extern "C" {

// OPT: Y đổi từ data_t* → wide_t* (ap_uint<64>* trong synthesis)
// Packed write: 4 fp16/word, giảm AXI write transactions ~4×
// depth Y = H_OUT×W_OUT×C_OUT / PACK (chia 4 so với bản gốc)
// SW sim: wide_t = data_t = float, interface test.cpp không thay đổi

void HW_ConvTranspose_0(wide_t* X, wide_t* W, wide_t* B, wide_t* Y) {
    #pragma HLS INTERFACE m_axi port=X offset=slave bundle=gmem0 depth=61440   max_read_burst_length=256
    #pragma HLS INTERFACE m_axi port=W offset=slave bundle=gmem1 depth=331776  max_read_burst_length=256
    #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem2 depth=120
    #pragma HLS INTERFACE m_axi port=Y offset=slave bundle=gmem3 depth=122880  max_write_burst_length=256
    #pragma HLS INTERFACE s_axilite port=X      bundle=control
    #pragma HLS INTERFACE s_axilite port=W      bundle=control
    #pragma HLS INTERFACE s_axilite port=B      bundle=control
    #pragma HLS INTERFACE s_axilite port=Y      bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control
    ConvTranspose<16, 1, 960, 480, 16, 16, 3, 3, 32, 32, data_t>(X, W, B, Y);
}

void HW_ConvTranspose_1(wide_t* X, wide_t* W, wide_t* B, wide_t* Y) {
    #pragma HLS INTERFACE m_axi port=X offset=slave bundle=gmem0 depth=122880  max_read_burst_length=256
    #pragma HLS INTERFACE m_axi port=W offset=slave bundle=gmem1 depth=82944   max_read_burst_length=256
    #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem2 depth=60
    #pragma HLS INTERFACE m_axi port=Y offset=slave bundle=gmem3 depth=245760  max_write_burst_length=256
    #pragma HLS INTERFACE s_axilite port=X      bundle=control
    #pragma HLS INTERFACE s_axilite port=W      bundle=control
    #pragma HLS INTERFACE s_axilite port=B      bundle=control
    #pragma HLS INTERFACE s_axilite port=Y      bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control
    ConvTranspose<16, 1, 480, 240, 32, 32, 3, 3, 64, 64, data_t>(X, W, B, Y);
}

void HW_ConvTranspose_2(wide_t* X, wide_t* W, wide_t* B, wide_t* Y) {
    #pragma HLS INTERFACE m_axi port=X offset=slave bundle=gmem0 depth=245760  max_read_burst_length=256
    #pragma HLS INTERFACE m_axi port=W offset=slave bundle=gmem1 depth=20736   max_read_burst_length=256
    #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem2 depth=30
    #pragma HLS INTERFACE m_axi port=Y offset=slave bundle=gmem3 depth=491520  max_write_burst_length=256
    #pragma HLS INTERFACE s_axilite port=X      bundle=control
    #pragma HLS INTERFACE s_axilite port=W      bundle=control
    #pragma HLS INTERFACE s_axilite port=B      bundle=control
    #pragma HLS INTERFACE s_axilite port=Y      bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control
    ConvTranspose<16, 1, 240, 120, 64, 64, 3, 3, 128, 128, data_t>(X, W, B, Y);
}

void HW_ConvTranspose_3(wide_t* X, wide_t* W, wide_t* B, wide_t* Y) {
    #pragma HLS INTERFACE m_axi port=X offset=slave bundle=gmem0 depth=491520  max_read_burst_length=256
    #pragma HLS INTERFACE m_axi port=W offset=slave bundle=gmem1 depth=5184    max_read_burst_length=256
    #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem2 depth=15
    #pragma HLS INTERFACE m_axi port=Y offset=slave bundle=gmem3 depth=983040  max_write_burst_length=256
    #pragma HLS INTERFACE s_axilite port=X      bundle=control
    #pragma HLS INTERFACE s_axilite port=W      bundle=control
    #pragma HLS INTERFACE s_axilite port=B      bundle=control
    #pragma HLS INTERFACE s_axilite port=Y      bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control
    // NOTE: Layer 3 (C_IN=120, C_OUT=60) incompatible with LANE=16 and TILE_C=8.
    // Not synthesized in this component (syn.top=HW_ConvTranspose_0).
    ConvTranspose<16, 1, 120, 60, 128, 128, 3, 3, 256, 256, data_t>(X, W, B, Y);
}

} // extern "C"
