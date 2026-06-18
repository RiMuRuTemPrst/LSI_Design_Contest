#include "Core.h"
#include "Hls_Layers_Fp8.tpp"

static const int PEs = 16;

// fp8 RB-conv benchmark top: conv operands fp8 (32/word), norm/output fp16.
extern "C" {
void fp8_conv_top(
    data_256_t*       X,      // gmem_data: fp8-packed activations (32 fp8 / 256-bit word)
    const data_256_t* W,      // gmem_weight: fp8-packed weights
    const data_256_t* B,      // gmem_param: bias  (fp16, 16/word)
    const data_256_t* G,      // gmem_param: gamma (fp16)
    const data_256_t* BE,     // gmem_param: beta  (fp16)
    data_256_t*       Y,      // gmem_data: fp16 output
    data_t            epsilon
) {
#pragma HLS INTERFACE m_axi port=X  offset=slave bundle=gmem_data   max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port=W  offset=slave bundle=gmem_weight max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=B  offset=slave bundle=gmem_param  max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=G  offset=slave bundle=gmem_param  max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=BE offset=slave bundle=gmem_param  max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=Y  offset=slave bundle=gmem_data   max_write_burst_length=64

#pragma HLS INTERFACE s_axilite port=X       bundle=control
#pragma HLS INTERFACE s_axilite port=W       bundle=control
#pragma HLS INTERFACE s_axilite port=B       bundle=control
#pragma HLS INTERFACE s_axilite port=G       bundle=control
#pragma HLS INTERFACE s_axilite port=BE      bundle=control
#pragma HLS INTERFACE s_axilite port=Y       bundle=control
#pragma HLS INTERFACE s_axilite port=epsilon bundle=control
#pragma HLS INTERFACE s_axilite port=return  bundle=control

    Fp8_Conv_Engine<PEs, FP8_LANES>(X, W, B, G, BE, Y, epsilon);
}
} // extern "C"
