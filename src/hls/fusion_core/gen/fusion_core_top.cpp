#include "Core.h"
#include "fusion_core.h"
#include "Hls_Layers_Fusion.tpp"

static const int PEs = 16;

extern "C" {

void fusion_core_top(
    data_256_t*       X,      // gmem_data (inout)
    const data_256_t* W,      // gmem_weight
    const data_256_t* B,      // gmem_param
    const data_256_t* G,      // gmem_param
    const data_256_t* BE,     // gmem_param
    const data_256_t* G_IN,   // gmem_param
    const data_256_t* BE_IN,  // gmem_param
    data_256_t*       Y,      // gmem_data
    data_t            epsilon,
    int               mode
) {
// Optimized burst length to 64 to save ~150 BRAMs
#pragma HLS INTERFACE m_axi port=X     offset=slave bundle=gmem_data  max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port=W     offset=slave bundle=gmem_weight max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=B     offset=slave bundle=gmem_param  max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=G     offset=slave bundle=gmem_param  max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=BE    offset=slave bundle=gmem_param  max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=G_IN  offset=slave bundle=gmem_param  max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=BE_IN offset=slave bundle=gmem_param  max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=Y     offset=slave bundle=gmem_data  max_write_burst_length=64

#pragma HLS INTERFACE s_axilite port=X       bundle=control
#pragma HLS INTERFACE s_axilite port=W       bundle=control
#pragma HLS INTERFACE s_axilite port=B       bundle=control
#pragma HLS INTERFACE s_axilite port=G       bundle=control
#pragma HLS INTERFACE s_axilite port=BE      bundle=control
#pragma HLS INTERFACE s_axilite port=G_IN    bundle=control
#pragma HLS INTERFACE s_axilite port=BE_IN   bundle=control
#pragma HLS INTERFACE s_axilite port=Y       bundle=control
#pragma HLS INTERFACE s_axilite port=epsilon bundle=control
#pragma HLS INTERFACE s_axilite port=mode    bundle=control
#pragma HLS INTERFACE s_axilite port=return  bundle=control

    Universal_Engine_Kernel<PEs, data_t>(
        X, W, B, G, BE, G_IN, BE_IN, Y, epsilon, mode
    );
}

} // extern "C"
