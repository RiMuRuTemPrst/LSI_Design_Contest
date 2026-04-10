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

    // =====================================================================
    // AXI Master Interface Configurations
    // Port type = ap_uint<256> -> 256-bit native data width (16x FP16 per beat)
    // Depth is calculated based on 256-bit words (DEPTH_*_WIDE = DEPTH_*_VAL / 16)
    // =====================================================================

    // X: READ+WRITE (Used for storing intermediate results when 'times == 0')
#pragma HLS INTERFACE m_axi port=X   offset=slave bundle=gmem_in     depth=DEPTH_X_WIDE \
                                     max_read_burst_length=256        \
                                     max_write_burst_length=256       \
                                

    // Y: WRITE_ONLY (Final processed output)
#pragma HLS INTERFACE m_axi port=Y   offset=slave bundle=gmem_out    depth=DEPTH_X_WIDE \
                                     max_write_burst_length=256       \
                                     

    // W1, W2: Weights - READ_ONLY
#pragma HLS INTERFACE m_axi port=W1  offset=slave bundle=gmem_weight depth=DEPTH_W_WIDE \
                                     max_read_burst_length=256        \
                                    
#pragma HLS INTERFACE m_axi port=W2  offset=slave bundle=gmem_weight depth=DEPTH_W_WIDE \
                                     max_read_burst_length=256        \
                                   

    // B1, B2, G1, G2, BE1, BE2: Small parameters (Bias/Gamma/Beta) - READ_ONLY
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

    // =====================================================================
    // AXI-Lite Control Interface
    // =====================================================================
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

    // =====================================================================
    // Invoke the template kernel with raw DDR pointers
    // =====================================================================
    Resblock___Pad_ref_Conv_11133111111_CNorm_Relu___<PEs, C_IN, C_OUT, BATCH, data_t>(
        X, W1, B1, G1, BE1, W2, B2, G2, BE2, epsilon, Y
    );
}
} // extern "C"
