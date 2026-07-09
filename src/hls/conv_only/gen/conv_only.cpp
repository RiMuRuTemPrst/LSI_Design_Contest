#include "Core.h"
#include "conv_only.h"

static const int PEs   = 16;
static const int C_IN  = MODEL_C;
static const int C_OUT = MODEL_C;
static const int BATCH = 1;

extern "C" {

void conv_only_top(
    data_256_t* X,
    const data_256_t* W1,
    const data_256_t* B1,
    data_256_t* Y,
    data_t epsilon,
    ap_uint<128>* PSUM_PACKED  // [N][H][W][C_OUT] packed {win_cnt[9:0], psum_double[63:0]}
) {

    // =====================================================================
    // AXI Master Interface — minimal conv-only port set
    // X read-only, Y write-only (separate bundles, no in-place aliasing)
    // =====================================================================
#pragma HLS INTERFACE m_axi port=X  offset=slave bundle=gmem_in     depth=DEPTH_X_WIDE \
                             max_read_burst_length=256              \
                             num_read_outstanding=4

#pragma HLS INTERFACE m_axi port=Y  offset=slave bundle=gmem_out    depth=DEPTH_X_WIDE \
                             max_write_burst_length=256             \
                             num_write_outstanding=4

#pragma HLS INTERFACE m_axi port=W1 offset=slave bundle=gmem_weight  depth=DEPTH_W_WIDE \
                             max_read_burst_length=64               \
                             num_read_outstanding=2

#pragma HLS INTERFACE m_axi port=B1 offset=slave bundle=gmem_param   depth=DEPTH_B_WIDE \
                             max_read_burst_length=64

    // PSUM_PACKED: WRITE ONLY — {10-bit win_cnt, 64-bit double psum} per output element
#pragma HLS INTERFACE m_axi port=PSUM_PACKED offset=slave bundle=gmem_packed depth=DEPTH_PSUM_PACKED \
                             max_write_burst_length=64              \
                             num_write_outstanding=4

    // =====================================================================
    // AXI-Lite Control Interface
    // =====================================================================
#pragma HLS INTERFACE s_axilite port=X       bundle=control
#pragma HLS INTERFACE s_axilite port=W1      bundle=control
#pragma HLS INTERFACE s_axilite port=B1      bundle=control
#pragma HLS INTERFACE s_axilite port=Y       bundle=control
#pragma HLS INTERFACE s_axilite port=epsilon      bundle=control
#pragma HLS INTERFACE s_axilite port=PSUM_PACKED  bundle=control
#pragma HLS INTERFACE s_axilite port=return       bundle=control

    ConvOnly_Kernel<PEs, C_IN, C_OUT, BATCH, data_t>(
        X, W1, B1, epsilon, Y, PSUM_PACKED
    );
}
} // extern "C"
