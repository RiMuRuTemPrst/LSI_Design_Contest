#include "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/gen/Core.h"
#include "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/gen/resblock_top.h"
// QUAN TRỌNG: Phải include file cài đặt template để compiler nhìn thấy thân hàm
#include "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/gen/Hard_op.tpp"

static const int PEs   = 16;
static const int C_IN  = 960;
static const int C_OUT = 960;
static const int BATCH = 1;

extern "C" {
void resblock_top(
    data_t* X,              // NHWC [1][16][16][960]
    const data_t* W1,       // [960][3][3][960]
    const data_t* B1,       // [960]
    const data_t* G1,       // [960]
    const data_t* BE1,      // [960]
    const data_t* W2,
    const data_t* B2,
    const data_t* G2,
    const data_t* BE2,
    data_t* Y,              // NHWC [1][16][16][960]
    data_t epsilon
) {
    // Config Interface AXI Master
    // UPDATE: Thêm depth=... để C/RTL Co-simulation biết kích thước cấp phát bộ nhớ
    // X, Y size: 1 * 16 * 16 * 960 = 245760
    
#pragma HLS INTERFACE m_axi port=X  offset=slave bundle=gmem0 depth=245760 max_read_burst_length=256 max_write_burst_length=256
#pragma HLS INTERFACE m_axi port=Y  offset=slave bundle=gmem1 depth=245760 max_read_burst_length=256 max_write_burst_length=256

#pragma HLS INTERFACE m_axi port=W1 offset=slave bundle=gmem2 depth=8294400 max_read_burst_length=256
#pragma HLS INTERFACE m_axi port=W2 offset=slave bundle=gmem3 depth=8294400 max_read_burst_length=256

#pragma HLS INTERFACE m_axi port=B1 offset=slave bundle=gmem4 depth=960
#pragma HLS INTERFACE m_axi port=B2 offset=slave bundle=gmem5 depth=960

#pragma HLS INTERFACE m_axi port=G1 offset=slave bundle=gmem6 depth=960
#pragma HLS INTERFACE m_axi port=G2 offset=slave bundle=gmem7 depth=960

#pragma HLS INTERFACE m_axi port=BE1 offset=slave bundle=gmem8 depth=960
#pragma HLS INTERFACE m_axi port=BE2 offset=slave bundle=gmem9 depth=960

#pragma HLS INTERFACE s_axilite port=epsilon bundle=control
#pragma HLS INTERFACE s_axilite port=return  bundle=control


    // Wrappers
    // Lưu ý: data_t phải được định nghĩa trong Core.h (thường là float hoặc half)
    TensorMem<data_t> tX(X,  Shape(1,16,16,960), false);
    TensorMem<data_t> tY(Y,  Shape(1,16,16,960), false);

    // Cast const data_t* về data_t* cho constructor TensorMem
    // (TensorMem chỉ đọc các biến này nên an toàn)
    TensorMem<data_t> tW1((data_t*)W1, Shape(960,3,3,960), false);
    TensorMem<data_t> tW2((data_t*)W2, Shape(960,3,3,960), false);

    TensorMem<data_t> tB1((data_t*)B1, Shape(1,1,1,960), false);
    TensorMem<data_t> tB2((data_t*)B2, Shape(1,1,1,960), false);

    TensorMem<data_t> tG1((data_t*)G1, Shape(1,1,1,960), false);
    TensorMem<data_t> tBE1((data_t*)BE1, Shape(1,1,1,960), false);

    TensorMem<data_t> tG2((data_t*)G2, Shape(1,1,1,960), false);
    TensorMem<data_t> tBE2((data_t*)BE2, Shape(1,1,1,960), false);

    // Call Template Kernel
    Resblock___Pad_ref_Conv_11133111111_CNorm_Relu___<PEs, C_IN, C_OUT, BATCH, data_t>(
        tX,
        tW1, tB1, tG1, tBE1,
        tW2, tB2, tG2, tBE2,
        epsilon,
        tY
    );
}
}