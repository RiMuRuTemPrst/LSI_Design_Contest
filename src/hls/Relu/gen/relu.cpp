#include "../../resblock_top/gen/Core.h"

extern "C" {
void Relu_IP(data_256_t* X, data_256_t* Y, int N) {
#pragma HLS INTERFACE m_axi port=X offset=slave bundle=gmem0 max_read_burst_length=256
#pragma HLS INTERFACE m_axi port=Y offset=slave bundle=gmem1 max_write_burst_length=256
#pragma HLS INTERFACE s_axilite port=X bundle=control
#pragma HLS INTERFACE s_axilite port=Y bundle=control
#pragma HLS INTERFACE s_axilite port=N bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    for (int i = 0; i < N; i++) {
#pragma HLS PIPELINE II=1
        data_256_t x_val = X[i];
        data_256_t y_val = 0;
        for (int k = 0; k < 16; k++) {
#pragma HLS UNROLL
            ap_uint<16> bits = x_val.range(16*k+15, 16*k);
            data_t val = bits_to_half<data_t>(bits);
            if (val < (data_t)0) val = (data_t)0;
            y_val.range(16*k+15, 16*k) = half_to_bits(val);
        }
        Y[i] = y_val;
    }
}
}
