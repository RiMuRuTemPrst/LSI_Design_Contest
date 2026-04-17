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

    // X: READ+WRITE — IFM input & Layer 1 write-back (row = W×C/16 = 16×60 = 960 words = 30.7KB)
    // Burst 256 beats = 8KB → phù hợp row-sequential access
    // num_outstanding=4 → pipeline 4 AXI transaction đồng thời, giảm bubble
#pragma HLS INTERFACE m_axi port=X  offset=slave bundle=gmem_inout    depth=DEPTH_X_WIDE  \
                             max_read_burst_length=256              \
                             max_write_burst_length=256             \
                             num_read_outstanding=4                 \
                             num_write_outstanding=4

    // Y: WRITE ONLY — Final output (30.7KB/row)
    // num_outstanding=4 để không block PASS B khi DDR bận
#pragma HLS INTERFACE m_axi port=Y  offset=slave bundle=gmem_inout   depth=DEPTH_X_WIDE  \
                             max_write_burst_length=256             \
                             num_write_outstanding=4

    // W1: READ ONLY — Ping load = 60 words (1920B), Pong = 1 word ẩn trong pipeline
    // max_read_burst_length=64 đủ cho ping load (60 < 64), tiết kiệm prefetch buffer
    // Bundle riêng (gmem_w1) tránh AXI arbitration với W2
#pragma HLS INTERFACE m_axi port=W1 offset=slave bundle=gmem_weight    depth=DEPTH_W_WIDE  \
                             max_read_burst_length=64               \
                             num_read_outstanding=2

    // W2: READ ONLY — access pattern giống W1, bundle riêng gmem_w2
#pragma HLS INTERFACE m_axi port=W2 offset=slave bundle=gmem_weight    depth=DEPTH_W_WIDE  \
                             max_read_burst_length=64               \
                             num_read_outstanding=2

    // B/G/BE: READ ONLY — load_params() tải đúng 60 words (1920B) mỗi lần
    // Tăng max_read_burst_length=64 (từ 16) → load 60 words trong 1 burst
    // thay vì 4 burst như cũ → giảm 75% AXI handshake overhead
    // Gộp chung gmem_param vì load_params đã tách 3 vòng riêng (không conflict)
#pragma HLS INTERFACE m_axi port=B1  offset=slave bundle=gmem_param depth=DEPTH_B_WIDE \
                             max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=B2  offset=slave bundle=gmem_param depth=DEPTH_B_WIDE \
                             max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=G1  offset=slave bundle=gmem_param depth=DEPTH_B_WIDE \
                             max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=G2  offset=slave bundle=gmem_param depth=DEPTH_B_WIDE \
                             max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=BE1 offset=slave bundle=gmem_param depth=DEPTH_B_WIDE \
                             max_read_burst_length=64
#pragma HLS INTERFACE m_axi port=BE2 offset=slave bundle=gmem_param depth=DEPTH_B_WIDE \
                             max_read_burst_length=64

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
    ResidualBlock_Kernel<PEs, C_IN, C_OUT, BATCH, data_t>(
        X, W1, B1, G1, BE1, W2, B2, G2, BE2, epsilon, Y
    );
}
} // extern "C"
