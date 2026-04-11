#include "..\non_gen\Core.h"
#include <cassert>




template<int SIZE, typename T>
static inline void load_buffer(T buffer[SIZE], T* data) {
    #pragma HLS INLINE
    for (int i = 0; i < SIZE; i++) {
        #pragma HLS PIPELINE II=1
        buffer[i] = data[i];
    }
}
template<int SIZE, typename T>
static inline void store_buffer(T buffer[SIZE], T* data) {
    #pragma HLS INLINE
    for (int i = 0; i < SIZE; i++) {
        #pragma HLS PIPELINE II=1
        data[i] = buffer[i];
    }
}
template<int H_R, int W_R, int DEPTH, typename T>
static inline void shift_left_buffer(T buffer[H_R][W_R][DEPTH]) {
    #pragma HLS INLINE
    for (int i = 0; i < DEPTH; i++) {
        #pragma HLS PIPELINE II=1
        for (int h = 0; h < H_R; h++) {
            #pragma HLS UNROLL
            for (int w = 1; w < W_R; w++) {
                #pragma HLS UNROLL
                buffer[h][w - 1][i] = buffer[h][w][i];
            }
        }
    }
}
inline int get_index(int n, int h, int w, int c, int H, int W, int C) {
    #pragma HLS INLINE
    return ((n * H + h) * W + w) * C + c;
}

template<int PEs, int BATCH, int C_IN, int C_OUT, 
            int H_IN, int W_IN, int H_R, int W_R, int H_OUT, int W_OUT, typename T>
void Conv_7x7(T* X, T* W, T* B, T* Y) {

    constexpr int TC_IN = C_IN / PEs;

    T x_buffer[H_R][W_R][C_IN];
    T w_buffer[H_R][W_R][C_IN];
    T b_buffer[C_OUT];
    T y_buffer[C_OUT];
    T y_partial[PEs];

    #pragma HLS ARRAY_PARTITION variable=x_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=x_buffer complete dim=2
    #pragma HLS ARRAY_PARTITION variable=w_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=w_buffer complete dim=2
    #pragma HLS ARRAY_PARTITION variable=x_buffer block factor=PEs dim=3
    #pragma HLS ARRAY_PARTITION variable=w_buffer block factor=PEs dim=3
    #pragma HLS ARRAY_PARTITION variable=y_partial complete dim=1


    load_buffer<C_OUT>(b_buffer, B);
    Batch_loop:
    for (int n = 0; n < BATCH; n++) {

        Layer_out_loop:
        for (int ho = 0; ho < H_OUT; ho++) 
        for (int wo = 0; wo < W_OUT; wo++) {
            int start_height = -3 + ho, start_width = -3 + wo;
            int last_sub_width = wo + 3;
            if (last_sub_width < 0) 
                last_sub_width = -last_sub_width;
            else if (last_sub_width >= W_IN) {
                last_sub_width = ((W_IN - 1) << 1) - last_sub_width;
            }

            // load input
            if (wo == 0) 
                for (int hf = 0; hf < H_R; hf++) 
                for (int wf = 0; wf < W_R; wf++) {
                    int hi = start_height + hf, wi = start_width + wf;
                    if (hi < 0) hi = -hi;
                    else if (hi >= H_IN) {
                        hi = ((H_IN - 1) << 1) - hi;
                    }
                    if (wi < 0) wi = -wi;
                    else if (wi >= W_IN) {
                        wi = ((W_IN - 1) << 1) - wi;
                    }
                    int x_id = get_index(n, hi, wi, 0, H_IN, W_IN, C_IN);
                    load_buffer<C_IN>(x_buffer[hf][wf], &X[x_id]);
                }
            else {
                shift_left_buffer<H_R, W_R, C_IN>(x_buffer);
                for (int hf = 0; hf < H_R; hf++) {
                    int hi = start_height + hf;
                    if (hi < 0) hi = -hi;
                    else if (hi >= H_IN) {
                        hi = ((H_IN - 1) << 1) - hi;
                    }
                    int x_id = get_index(n, hi, last_sub_width, 0, H_IN, W_IN, C_IN);
                    load_buffer<C_IN>(x_buffer[hf][W_R - 1], &X[x_id]);
                }
            }
                
            Channel_out_loop:
            for (int co = 0; co < C_OUT; co++) {
                // load weight
                for (int hf = 0; hf < H_R; hf++) 
                for (int wf = 0; wf < W_R; wf++) {
                    int w_id = get_index(co, hf, wf, 0, H_R, W_R, C_IN);
                    load_buffer<C_IN>(w_buffer[hf][wf], &W[w_id]);
                }

                for (int pe = 0; pe < PEs; pe++) {
                    #pragma HLS UNROLL
                    y_partial[pe] = 0;
                }

                Partial_channel_in_loop:
                for (int tci = 0; tci < TC_IN; tci++) {
                    for (int hf = 0; hf < H_R; hf++) {
                        #pragma HLS PIPELINE II=1

                        PE_loop_for_compute:
                        for (int pe = 0; pe < PEs; pe++) {
                            #pragma HLS UNROLL
                            int ci = pe* TC_IN + tci;

                            for (int wf = 0; wf < W_R; wf++) {
                                #pragma HLS UNROLL
                                y_partial[pe] += x_buffer[hf][wf][ci]* w_buffer[hf][wf][ci];
                            }
                        }
                    }
                }
                y_buffer[co] = b_buffer[co];
                for (int pe = 0; pe < PEs; pe++) {
                    #pragma HLS UNROLL
                    y_buffer[co] += y_partial[pe];
                }
            }
            // store output
            int y_id = get_index(n, ho, wo, 0, H_OUT, W_OUT, C_OUT);
            store_buffer<C_OUT>(y_buffer, &Y[y_id]);
        }
    }
}

void HW_Conv7x7(
    float* X, float* W, float* B, float* Y
) {
    // 1. Khai báo cổng AXI Master để IP có thể cắm vào DRAM (DDR)
    #pragma HLS INTERFACE m_axi port=X bundle=gmem_X depth=3932160
    #pragma HLS INTERFACE m_axi port=W bundle=gmem_W depth=8820
    #pragma HLS INTERFACE m_axi port=B bundle=gmem_B depth=3
    #pragma HLS INTERFACE m_axi port=Y bundle=gmem_Y depth=196608
    
    // 2. Khai báo cổng AXI-Lite để CPU (ARM/Host) điều khiển truyền địa chỉ bộ nhớ
    #pragma HLS INTERFACE s_axilite port=X bundle=control
    #pragma HLS INTERFACE s_axilite port=W bundle=control
    #pragma HLS INTERFACE s_axilite port=B bundle=control
    #pragma HLS INTERFACE s_axilite port=Y bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    // 3. Gọi Template tương ứng
    Conv_7x7<12, 1, 60, 3, 256, 256, 7, 7, 256, 256>(X, W, B, Y);
}
