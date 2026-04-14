#include "../../resblock_top/gen/Core.h"

// Independent Channel Norm IP
template<int C_OUT, int BATCH, int H_OUT, int W_OUT, typename T>
void ChannelNorm_Kernel(
    DDR_CONST_PTR X_ptr,
    DDR_CONST_PTR G_ptr,
    DDR_CONST_PTR B_ptr,
    T epsilon,
    DDR_PTR Y_ptr
) {
    constexpr int C_WORDS = C_OUT / PACK_256;
    
    data_256_t gamma_buf[C_WORDS];
    data_256_t beta_buf[C_WORDS];
    
    // Load Gamma and Beta
    LOAD_GAMMA: for (int i = 0; i < C_WORDS; i++) {
#pragma HLS PIPELINE II=1
        gamma_buf[i] = G_ptr[i];
    }
    LOAD_BETA: for (int i = 0; i < C_WORDS; i++) {
#pragma HLS PIPELINE II=1
        beta_buf[i] = B_ptr[i];
    }

    const int num = C_OUT;
    const T adjustment_scale = (T)num / (T)(num - 1);
    const float pre_div = 1.0f / my_sqrt_f((float)(num - 1));
    const float pre_div_sq = pre_div * pre_div;

    // Buffer for one pixel to avoid DDR re-read
    data_256_t pixel_buf[C_WORDS];

    Batch_loop: for (int n = 0; n < BATCH; n++) {
        Spatial_loop: for (int h = 0; h < H_OUT; h++) {
            for (int w = 0; w < W_OUT; w++) {
                int base_idx = flat_idx(n, h, w, 0, H_OUT, W_OUT, C_OUT) / PACK_256;
                
                float sum_acc = 0.0f;
                float sumsq_acc = 0.0f;

                // Pass 1: Compute Mean and Variance
                COMPUTE_STATS: for (int i = 0; i < C_WORDS; i++) {
#pragma HLS PIPELINE II=1
                    data_256_t word = X_ptr[base_idx + i];
                    pixel_buf[i] = word;
                    
                    for (int k = 0; k < PACK_256; k++) {
#pragma HLS UNROLL
                        T val = bits_to_half<T>(word.range(16*k+15, 16*k));
                        float f_val = (float)val;
                        sum_acc += f_val;
                        sumsq_acc += f_val * f_val;
                    }
                }

                float mean_f = sum_acc / (float)C_OUT;
                float var_f = sumsq_acc * pre_div_sq;
                float denom_f = var_f - mean_f * mean_f * (float)adjustment_scale + (float)epsilon;
                T inv_std = (T)(1.0f / my_sqrt_f(denom_f));
                T mean_t = (T)mean_f;

                // Pass 2: Normalize and Output
                NORMALIZE: for (int i = 0; i < C_WORDS; i++) {
#pragma HLS PIPELINE II=1
                    data_256_t in_word = pixel_buf[i];
                    data_256_t g_word = gamma_buf[i];
                    data_256_t b_word = beta_buf[i];
                    data_256_t out_word = 0;

                    for (int k = 0; k < PACK_256; k++) {
#pragma HLS UNROLL
                        T val = bits_to_half<T>(in_word.range(16*k+15, 16*k));
                        T gamma = bits_to_half<T>(g_word.range(16*k+15, 16*k));
                        T beta = bits_to_half<T>(b_word.range(16*k+15, 16*k));

                        T weight = gamma * inv_std;
                        T bias_v = beta - (mean_t * weight);
                        T norm_out = val * weight + bias_v;

                        out_word.range(16*k+15, 16*k) = (ap_uint<16>)half_to_bits(norm_out);
                    }
                    Y_ptr[base_idx + i] = out_word;
                }
            }
        }
    }
}

extern "C" {
void ChannelNorm_IP(
    data_256_t* X,
    const data_256_t* G,
    const data_256_t* B,
    data_t epsilon,
    data_256_t* Y
) {
#pragma HLS INTERFACE m_axi port=X offset=slave bundle=gmem0 max_read_burst_length=256
#pragma HLS INTERFACE m_axi port=G offset=slave bundle=gmem1 max_read_burst_length=256
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem2 max_read_burst_length=256
#pragma HLS INTERFACE m_axi port=Y offset=slave bundle=gmem3 max_write_burst_length=256
#pragma HLS INTERFACE s_axilite port=X bundle=control
#pragma HLS INTERFACE s_axilite port=G bundle=control
#pragma HLS INTERFACE s_axilite port=B bundle=control
#pragma HLS INTERFACE s_axilite port=Y bundle=control
#pragma HLS INTERFACE s_axilite port=epsilon bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    ChannelNorm_Kernel<MODEL_C, 1, MODEL_H, MODEL_W, data_t>(X, G, B, epsilon, Y);
}
}
