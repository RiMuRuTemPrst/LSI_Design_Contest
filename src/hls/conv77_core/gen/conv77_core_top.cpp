#include "conv77_core.h"
#include "Hls_Layers_Conv77.tpp"

extern "C" {

void conv77_core_top(
    DDR_CONST_PTR     X,
    const data_256_t* W,
    const data_256_t* B,
    DDR_PTR           Y
) {
#pragma HLS INTERFACE m_axi port=X offset=slave bundle=gmem_x   max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=W offset=slave bundle=gmem_w   max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem_b   max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=Y offset=slave bundle=gmem_y   max_write_burst_length=64

#pragma HLS INTERFACE s_axilite port=X      bundle=control
#pragma HLS INTERFACE s_axilite port=W      bundle=control
#pragma HLS INTERFACE s_axilite port=B      bundle=control
#pragma HLS INTERFACE s_axilite port=Y      bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    Conv77_Core(X, W, B, Y);
}

} // extern "C"
