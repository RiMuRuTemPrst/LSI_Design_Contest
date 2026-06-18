// COSIM PROBE — UCB_3 datapath, reduced H (h_in=4 -> h_out=8), W/C full.
// Goal: measure real xsim throughput + test whether [COSIM 212-5] TV-gen bug
// bites at small scope. Same UpConv_Fused_Row<8> netlist + same AXI bundles as
// upconv_core_top (faithful for the apatb adapter), just driven with few rows.
#include "upconv_core.h"
#include "Hls_Layers_UpConv.tpp"

extern "C" {

void probe_top(
    DDR_CONST_PTR X,
    const data_256_t* W,
    const data_256_t* B,
    const data_256_t* G,
    const data_256_t* BE,
    DDR_PTR       Y,
    data_t        epsilon
) {
#pragma HLS INTERFACE m_axi port=X     offset=slave bundle=gmem_in     max_read_burst_length=64  depth=32
#pragma HLS INTERFACE m_axi port=W     offset=slave bundle=gmem_weight  max_read_burst_length=64  depth=4320
#pragma HLS INTERFACE m_axi port=B     offset=slave bundle=gmem_param   max_read_burst_length=64  depth=4
#pragma HLS INTERFACE m_axi port=G     offset=slave bundle=gmem_param   max_read_burst_length=64  depth=4
#pragma HLS INTERFACE m_axi port=BE    offset=slave bundle=gmem_param   max_read_burst_length=64  depth=4
#pragma HLS INTERFACE m_axi port=Y     offset=slave bundle=gmem_out     max_write_burst_length=64  depth=64
#pragma HLS INTERFACE s_axilite port=X       bundle=control
#pragma HLS INTERFACE s_axilite port=W       bundle=control
#pragma HLS INTERFACE s_axilite port=B       bundle=control
#pragma HLS INTERFACE s_axilite port=G       bundle=control
#pragma HLS INTERFACE s_axilite port=BE      bundle=control
#pragma HLS INTERFACE s_axilite port=Y       bundle=control
#pragma HLS INTERFACE s_axilite port=epsilon bundle=control
#pragma HLS INTERFACE s_axilite port=return  bundle=control

    const int PEs = 8;
    static data_256_t x_buf[2 * 1024];
#pragma HLS BIND_STORAGE variable=x_buf type=ram_t2p impl=uram

    // UCB_3 dims, H reduced 128 -> 4 (output rows 256 -> 8). W/C kept full so
    // sliding window, row_acc[W_OUT][C_OUT], PIXEL_STATS, RESET_ROW_ACC and the
    // rotating accumulators all run at real trip counts.
    const int h_in = 1, w_in = 4, c_in = 120, c_out = 60;  // FAST throughput probe: minimal spatial, C full (netlist unchanged)
    int ci_words = (c_in + 15) / 16;

    LOAD_ROW0: for (int wi = 0; wi < w_in; wi++) {
        for (int ciw = 0; ciw < ci_words; ciw++) {
#pragma HLS PIPELINE II=1
            x_buf[(0 * w_in + wi) * ci_words + ciw] = X[(0 * w_in + wi) * ci_words + ciw];
        }
    }
    UpConv_Fused_Row<PEs>(x_buf, W, B, G, BE, Y, epsilon, h_in, w_in, c_in, c_out, 0);

    ROW_LOOP: for (int hi = 1; hi < h_in; hi++) {
        int slot = hi % 2;
        LOAD_ROW: for (int wi = 0; wi < w_in; wi++) {
            for (int ciw = 0; ciw < ci_words; ciw++) {
#pragma HLS PIPELINE II=1
                x_buf[(slot * w_in + wi) * ci_words + ciw] = X[(hi * w_in + wi) * ci_words + ciw];
            }
        }
        UpConv_Fused_Row<PEs>(x_buf, W, B, G, BE, Y, epsilon, h_in, w_in, c_in, c_out, 2*hi-1);
        UpConv_Fused_Row<PEs>(x_buf, W, B, G, BE, Y, epsilon, h_in, w_in, c_in, c_out, 2*hi);
    }
    UpConv_Fused_Row<PEs>(x_buf, W, B, G, BE, Y, epsilon, h_in, w_in, c_in, c_out, 2*h_in-1);
}

} // extern "C"
