#include "../../resblock_top/gen/Core.h"
#include "Hls_Layers_Conv3x3.tpp"

extern "C" {
void Conv3x3_IP(
    DDR_PTR X,
    DDR_CONST_PTR W,
    DDR_CONST_PTR B,
    data_t epsilon,
    DDR_PTR Y
) {
#pragma HLS INTERFACE m_axi port=X offset=slave bundle=gmem_X max_read_burst_length=256
#pragma HLS INTERFACE m_axi port=W offset=slave bundle=gmem_W max_read_burst_length=256
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem_B max_read_burst_length=16
#pragma HLS INTERFACE m_axi port=Y offset=slave bundle=gmem_Y max_write_burst_length=256
#pragma HLS INTERFACE s_axilite port=X bundle=control
#pragma HLS INTERFACE s_axilite port=W bundle=control
#pragma HLS INTERFACE s_axilite port=B bundle=control
#pragma HLS INTERFACE s_axilite port=Y bundle=control
#pragma HLS INTERFACE s_axilite port=epsilon bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    data_256_t b_buf[MODEL_C / PACK_256];
    for (int i=0; i<MODEL_C/PACK_256; i++) {
        #pragma HLS PIPELINE II=1
        b_buf[i] = B[i];
    }

    data_256_t x_buf_256[4][MODEL_W][MODEL_C / PACK_256];
#pragma HLS BIND_STORAGE variable=x_buf_256 type=ram_2p impl=bram
#pragma HLS ARRAY_PARTITION variable=x_buf_256 complete dim=2

    data_256_t w_buf_256[2][MODEL_C / PACK_256];
#pragma HLS ARRAY_PARTITION variable=w_buf_256 complete dim=1

    data_t y_cache_16[16][MODEL_C];
#pragma HLS BIND_STORAGE variable=y_cache_16 type=ram_2p impl=bram
#pragma HLS ARRAY_PARTITION variable=y_cache_16 cyclic factor=16 dim=2

    // Note: skip_buf_256 is no longer needed in standalone Conv3x3
    data_256_t dummy_skip[1]; 
    init_rows<MODEL_W, MODEL_C, false, data_t>(X, x_buf_256, dummy_skip, 0);

    Conv3x3_Kernel<16, MODEL_C, MODEL_C, data_t>(
        X, W, Y,
        b_buf, 
        x_buf_256, w_buf_256, y_cache_16,
        epsilon, 0
    );
}
}
