#pragma once
#include "../../resblock_top/gen/Core.h"

// Standardized 3x3 Convolution Kernel (Standalone)
template<int PEs, int C_IN, int C_OUT, typename T>
static void Conv3x3_Kernel(
    DDR_CONST_PTR   X_ptr,                                         
    DDR_CONST_PTR   W_ptr,                                         
    DDR_PTR         Out_ptr,                                       
    data_256_t      b_buf_256   [C_OUT / PACK_256],                
    data_256_t      x_buf_256[4][MODEL_W][C_IN / PACK_256],        
    data_256_t      w_buf_256[2][C_IN / PACK_256],                 
    T               y_cache_16[PEs][C_OUT],                        
    T               epsilon,
    int             n
) {
#pragma HLS INLINE off
    constexpr int H         = MODEL_H;
    constexpr int W         = MODEL_W;
    constexpr int width     = PEs / W;
    constexpr int rolls     = H / width;
    constexpr int C_WORDS   = C_IN / PACK_256;      
    constexpr int WPR       = MODEL_W * C_IN / PACK_256; 

    Slide_PEs_loop:
    for (int r = 0; r < rolls; r++) {

        int ping = 0;
        int w_init_base = w_flat_idx(0, 0, 0, 0, C_IN) / PACK_256;
        LOAD_1_WEIGHT_PINGPONG: for (int i = 0; i < C_WORDS; i++) {
#pragma HLS PIPELINE II=1
            w_buf_256[0][i] = W_ptr[w_init_base + i];
        }

        data_256_t b_word_reg; 

        PASS_A: for (int co = 0; co < C_OUT; co++) {

            T psum[PEs][8];
#pragma HLS ARRAY_PARTITION variable=psum complete dim=0
            INIT_PSUM: for (int pe = 0; pe < PEs; pe++) {
#pragma HLS UNROLL
                for (int l = 0; l < 8; l++) {
#pragma HLS UNROLL
                    psum[pe][l] = (T)0;
                }
            }

            CONV_STEP_LOOP: for (int step = 0; step < 9; step++) {
                int kh = step / 3, kw = step % 3;
                int ns   = step + 1;
                int n_kh = ns / 3, n_kw = ns % 3;

                int h_row = r * width + (kh - 1);
                int slot  = (h_row + 1) & 3;

                int wn_base;
                bool do_load = false;
                if (ns < 9) {
                    wn_base = w_flat_idx(co, n_kh, n_kw, 0, C_IN) / PACK_256;
                    do_load = true;
                } else if (co < C_OUT - 1) { 
                    wn_base = w_flat_idx(co + 1, 0, 0, 0, C_IN) / PACK_256;
                    do_load = true;
                }

                data_256_t w_word_reg;
                data_256_t x_word_reg[PEs];
#pragma HLS ARRAY_PARTITION variable=x_word_reg complete

                COMPUTE_LOAD_LOOP: for (int ci = 0; ci < C_IN; ci += 4) {
#pragma HLS PIPELINE II=1
                    int acc_idx = (ci / 4) & 7;
                    int widx    = ci / PACK_256;   
                    int boff    = ci % PACK_256;   
#pragma HLS DEPENDENCE variable=psum type=inter false

                    if (boff == 0) {
                        w_word_reg = w_buf_256[ping][widx];
                    } else {
                        w_word_reg >>= 64; 
                    }

                    T w0 = bits_to_half<T>(w_word_reg.range(15, 0));
                    T w1 = bits_to_half<T>(w_word_reg.range(31, 16));
                    T w2 = bits_to_half<T>(w_word_reg.range(47, 32));
                    T w3 = bits_to_half<T>(w_word_reg.range(63, 48));

                    PE_UNROLL: for (int pe = 0; pe < PEs; pe++) {
#pragma HLS UNROLL
                        int wx  = pe + (kw - 1);
                        int wc  = (wx < 0) ? -wx : ((wx >= W) ? (W<<1)-wx-2 : wx);
                        
                        if (boff == 0) {
                            x_word_reg[pe] = x_buf_256[slot][wc][widx];
                        } else {
                            x_word_reg[pe] >>= 64; 
                        }

                        T x0 = bits_to_half<T>(x_word_reg[pe].range(15, 0));
                        T x1 = bits_to_half<T>(x_word_reg[pe].range(31, 16));
                        T x2 = bits_to_half<T>(x_word_reg[pe].range(47, 32));
                        T x3 = bits_to_half<T>(x_word_reg[pe].range(63, 48));

                        psum[pe][acc_idx] += (x0*w0 + x1*w1) + (x2*w2 + x3*w3);
                    }

                    if (do_load && boff == 0) {
                        w_buf_256[1-ping][widx] = W_ptr[wn_base + widx];
                    }
                } 
                ping ^= 1; 
            } 

            if ((co % PACK_256) == 0) {
                b_word_reg = b_buf_256[co / PACK_256];
            } else {
                b_word_reg >>= 16; 
            }
            T bias = bits_to_half<T>(b_word_reg.range(15, 0)); 

            FINAL_ACCUMULATION: for (int pe = 0; pe < PEs; pe++) {
#pragma HLS PIPELINE II=1
                T total = (T)0;
                for (int l = 0; l < 8; l++) total += psum[pe][l];

                T fout = total + bias;
                y_cache_16[pe][co] = fout;
            }
        } 

        if (r < rolls - 1) {
            int h_next = r + 2;
            int h_c    = (h_next >= H) ? (H<<1)-h_next-2 : h_next;
            int slot   = (h_next + 1) & 3;
            int base   = flat_idx(n, h_c, 0, 0, H, W, C_IN);

            PREFETCH_NEXT_INPUT_ROW: for (int i = 0; i < WPR; i++) {
#pragma HLS PIPELINE II=1
                x_buf_256[slot][i / C_WORDS][i % C_WORDS] = X_ptr[base / PACK_256 + i];
            }
        }

        // Output directly to DDR
        WRITE_OUT: for (int pe = 0; pe < PEs; pe++) {
            int out_off_256 = flat_idx(n, r*width, pe, 0, H, W, C_OUT) / PACK_256;
            for (int co = 0; co < C_OUT; co += PACK_256) {
#pragma HLS PIPELINE II=1
                int word_idx = co / PACK_256;
                data_256_t out_word = 0;
                for (int k = 0; k < PACK_256; k++) {
#pragma HLS UNROLL
                    T val = y_cache_16[pe][co + k];
                    out_word.range(16*k+15, 16*k) = (ap_uint<16>)half_to_bits(val);
                }
                Out_ptr[out_off_256 + word_idx] = out_word;
            }
        }
    } 
}
