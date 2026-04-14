#include "../../resblock_top/gen/Core.h"

extern "C" {
void Add_IP(data_256_t* X1, data_256_t* X2, data_256_t* Y, int N) {
#pragma HLS INTERFACE m_axi port=X1 offset=slave bundle=gmem0 max_read_burst_length=256
#pragma HLS INTERFACE m_axi port=X2 offset=slave bundle=gmem1 max_read_burst_length=256
#pragma HLS INTERFACE m_axi port=Y offset=slave bundle=gmem2 max_write_burst_length=256
#pragma HLS INTERFACE s_axilite port=X1 bundle=control
#pragma HLS INTERFACE s_axilite port=X2 bundle=control
#pragma HLS INTERFACE s_axilite port=Y bundle=control
#pragma HLS INTERFACE s_axilite port=N bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    for (int i = 0; i < N; i++) {
#pragma HLS PIPELINE II=1
        data_256_t x1_val = X1[i];
        data_256_t x2_val = X2[i];
        data_256_t y_val = 0;
        for (int k = 0; k < 16; k++) {
#pragma HLS UNROLL
            data_t val1 = bits_to_half<data_t>(x1_val.range(16*k+15, 16*k));
            data_t val2 = bits_to_half<data_t>(x2_val.range(16*k+15, 16*k));
            y_val.range(16*k+15, 16*k) = half_to_bits(val1 + val2);
        }
        Y[i] = y_val;
    }
}
}
