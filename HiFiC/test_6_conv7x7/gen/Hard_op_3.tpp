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

template<int PEs, int BATCH, int C_IN, int C_OUT, 
            int H_IN, int W_IN, int H_R, int W_R, int H_OUT, int W_OUT, typename T>
void Conv_7x7(TensorMem<T> &X, TensorMem<T> &W, TensorMem<T> &B, TensorMem<T> &Y) {

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


    load_buffer<C_OUT>(b_buffer, B.raw());
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
                    load_buffer<C_IN>(x_buffer[hf][wf], X.raw_at(n, hi, wi, 0));
                }
            else {
                shift_left_buffer<H_R, W_R, C_IN>(x_buffer);
                for (int hf = 0; hf < H_R; hf++) {
                    int hi = start_height + hf;
                    if (hi < 0) hi = -hi;
                    else if (hi >= H_IN) {
                        hi = ((H_IN - 1) << 1) - hi;
                    }
                    load_buffer<C_IN>(x_buffer[hf][W_R - 1], X.raw_at(n, hi, last_sub_width, 0));
                }
            }
                
            Channel_out_loop:
            for (int co = 0; co < C_OUT; co++) {
                // load weight
                for (int hf = 0; hf < H_R; hf++) 
                for (int wf = 0; wf < W_R; wf++) {
                    load_buffer<C_IN>(w_buffer[hf][wf], W.raw_at(co, hf, wf, 0));
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
            store_buffer<C_OUT>(y_buffer, Y.raw_at(n, ho, wo, 0));
        }
    }
}