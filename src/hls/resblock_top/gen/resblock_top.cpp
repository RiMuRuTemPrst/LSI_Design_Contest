#include "Core.h"
#include "resblock_top.h"

static const int PEs   = 16;
static const int C_IN  = MODEL_C;
static const int C_OUT = MODEL_C;
static const int BATCH = 1;

extern "C" {

void resblock_top(
    data_256_t* X,
    const data_256_t* W1,
    const data_256_t* B1,
    const data_256_t* G1,
    const data_256_t* BE1,
    const data_256_t* W2,
    const data_256_t* B2,
    const data_256_t* G2,
    const data_256_t* BE2,
    data_256_t* Y,
    data_t epsilon
) {
#pragma HLS INTERFACE m_axi port=X   offset=slave bundle=gmem_in     depth=DEPTH_X_WIDE \
                                     max_read_burst_length=256        \
                                     max_write_burst_length=256       
                                
#pragma HLS INTERFACE m_axi port=Y   offset=slave bundle=gmem_out    depth=DEPTH_X_WIDE \
                                     max_write_burst_length=256       

#pragma HLS INTERFACE m_axi port=W1  offset=slave bundle=gmem_weight depth=DEPTH_W_WIDE \
                                     max_read_burst_length=256        
                                    
#pragma HLS INTERFACE m_axi port=W2  offset=slave bundle=gmem_weight depth=DEPTH_W_WIDE \
                                     max_read_burst_length=256        

#pragma HLS INTERFACE m_axi port=B1  offset=slave bundle=gmem_param  depth=DEPTH_B_WIDE \
                                     max_read_burst_length=16         
#pragma HLS INTERFACE m_axi port=B2  offset=slave bundle=gmem_param  depth=DEPTH_B_WIDE \
                                     max_read_burst_length=16        
#pragma HLS INTERFACE m_axi port=G1  offset=slave bundle=gmem_param  depth=DEPTH_B_WIDE \
                                     max_read_burst_length=16         
#pragma HLS INTERFACE m_axi port=G2  offset=slave bundle=gmem_param  depth=DEPTH_B_WIDE \
                                     max_read_burst_length=16        
#pragma HLS INTERFACE m_axi port=BE1 offset=slave bundle=gmem_param  depth=DEPTH_B_WIDE \
                                     max_read_burst_length=16         
#pragma HLS INTERFACE m_axi port=BE2 offset=slave bundle=gmem_param  depth=DEPTH_B_WIDE \
                                     max_read_burst_length=16         

#pragma HLS INTERFACE s_axilite port=X       bundle=control
#pragma HLS INTERFACE s_axilite port=W1      bundle=control
#pragma HLS INTERFACE s_axilite port=B1      bundle=control
#pragma HLS INTERFACE s_axilite port=G1      bundle=control
#pragma HLS INTERFACE s_axilite port=BE1     bundle=control
#pragma HLS INTERFACE s_axilite port=W2      bundle=control
#pragma HLS INTERFACE s_axilite port=B2      bundle=control
#pragma HLS INTERFACE s_axilite port=G2      bundle=control
#pragma HLS INTERFACE s_axilite port=BE2     bundle=control
#pragma HLS INTERFACE s_axilite port=Y       bundle=control
#pragma HLS INTERFACE s_axilite port=epsilon bundle=control
#pragma HLS INTERFACE s_axilite port=return  bundle=control

    // Invoke the renamed top-level kernel
    ResidualBlock_Kernel<PEs, C_IN, C_OUT, BATCH, data_t>(
        X, W1, B1, G1, BE1, W2, B2, G2, BE2, epsilon, Y
    );
}
} // extern "C"
