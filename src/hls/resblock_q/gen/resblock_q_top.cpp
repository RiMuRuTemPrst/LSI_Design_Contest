#include "Core.h"
#include "resblock_q.h"
#include "Hls_Layers_ResblockQ.tpp"

static const int PEs = 16;

// Persistent residual (skip) buffer: saved in L1, consumed by the L2 quantized Add.
static data_256_t skip_buf[MODEL_H * MODEL_W * 60];

extern "C" {

void resblock_q_top(
    data_256_t*       X,
    const data_256_t* W,
    const int32_t*    B,
    const int32_t*    G,
    const int8_t*     GS,
    const int32_t*    BE,
    data_256_t*       Y,
    int M_conv, int N_conv, int x_zp, int conv_zp,
    int reci_C_mult, int reci_C_shift, int epsilon,
    int M_path, int N_path, int M_skip, int N_skip, int res_zp,
    int mode
) {
#pragma HLS BIND_STORAGE variable=skip_buf type=ram_2p impl=uram
#pragma HLS INTERFACE m_axi port=X  offset=slave bundle=gmem_data   max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port=W  offset=slave bundle=gmem_weight max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=B  offset=slave bundle=gmem_param  max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=G  offset=slave bundle=gmem_param  max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=BE offset=slave bundle=gmem_param  max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=GS offset=slave bundle=gmem_gs     max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=Y  offset=slave bundle=gmem_data   max_write_burst_length=64

#pragma HLS INTERFACE s_axilite port=X            bundle=control
#pragma HLS INTERFACE s_axilite port=W            bundle=control
#pragma HLS INTERFACE s_axilite port=B            bundle=control
#pragma HLS INTERFACE s_axilite port=G            bundle=control
#pragma HLS INTERFACE s_axilite port=GS           bundle=control
#pragma HLS INTERFACE s_axilite port=BE           bundle=control
#pragma HLS INTERFACE s_axilite port=Y            bundle=control
#pragma HLS INTERFACE s_axilite port=M_conv       bundle=control
#pragma HLS INTERFACE s_axilite port=N_conv       bundle=control
#pragma HLS INTERFACE s_axilite port=x_zp         bundle=control
#pragma HLS INTERFACE s_axilite port=conv_zp      bundle=control
#pragma HLS INTERFACE s_axilite port=reci_C_mult  bundle=control
#pragma HLS INTERFACE s_axilite port=reci_C_shift bundle=control
#pragma HLS INTERFACE s_axilite port=epsilon      bundle=control
#pragma HLS INTERFACE s_axilite port=M_path       bundle=control
#pragma HLS INTERFACE s_axilite port=N_path       bundle=control
#pragma HLS INTERFACE s_axilite port=M_skip       bundle=control
#pragma HLS INTERFACE s_axilite port=N_skip       bundle=control
#pragma HLS INTERFACE s_axilite port=res_zp       bundle=control
#pragma HLS INTERFACE s_axilite port=mode         bundle=control
#pragma HLS INTERFACE s_axilite port=return       bundle=control

    Resblock_Engine_Kernel<PEs>(
        X, W, B, G, GS, BE, Y, skip_buf,
        M_conv, N_conv, x_zp, conv_zp,
        reci_C_mult, reci_C_shift, epsilon,
        M_path, N_path, M_skip, N_skip, res_zp,
        mode);
}

} // extern "C"
