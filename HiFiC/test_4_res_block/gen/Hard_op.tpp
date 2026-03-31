#include "Core.h"
#include <cassert>


template<int SIZE, typename T>
static inline void load_buffer(T buffer[SIZE], T* data) {
    #pragma HLS INLINE
    for (int i = 0, j = 0; i < SIZE; i++, j++) {
        buffer[i] = data[j];
    }
}
template<int SIZE, typename T>
static inline void store_buffer(T buffer[SIZE], T* data) {
    #pragma HLS INLINE
    for (int i = 0, j = 0; i < SIZE; i++, j++) {
        data[j] = buffer[i];
    }
}
static inline int mod_3(int n) {
    #pragma HLS INLINE
    switch (n) {
        case 0: case 3: case 6: case 9: case 12: case 15: return 0;
        case 1: case 4: case 7: case 10: case 13: case 16: return 1;
        case 2: case 5: case 8: case 11: case 14: case 17: return 2;
    }
    return 0;
}

template<int PEs, int C_IN, int C_OUT, int BATCH, typename T>
void Resblock___Pad_ref_Conv_11133111111_CNorm_Relu___(TensorMem<T> &X, 
                                        TensorMem<T> &W_1, TensorMem<T> &B_1, 
                                        /*TensorMem<T> &gamma_1, TensorMem<T> &beta_1, 
                                        TensorMem<T> &W_2, TensorMem<T> &B_2, 
                                        TensorMem<T> &gamma_2, TensorMem<T> &beta_2, T epsilon,*/ TensorMem<T> &Y) {
    Shape x_shape = X.shape;
    Shape w_shape = W_1.shape;
    Shape y_shape = Y.shape;

    if (3 != w_shape.H || 3 != w_shape.W) 
        assert(0 && "Error: Unappropriate input sizes for Conv !!\n");
    
    if (y_shape.H != (x_shape.H - (w_shape.H - 1)* 1 
        + 1 + 1 - 1) / 1 + 1 
        || y_shape.W != (x_shape.W - (w_shape.W - 1)* 1 
        + 1 + 1 - 1) / 1 + 1 
        || y_shape.N != x_shape.N || y_shape.C != w_shape.N || C_IN != w_shape.C || C_OUT != w_shape.N || BATCH != x_shape.N)
        assert(0 && "Error: Unappropriate output sizes for Conv !!\n");

    //assert(!(PEs % y_shape.W));
    int width = PEs / y_shape.W;
    int rolls = y_shape.H / width;
    // int num = C_OUT;
    // T adjustment_scale = (T) num / (num - 1);
    // T pre_div = (T) 1 / (T) (sqrtf(num - 1));

    constexpr int vector_depth_size = PEs* C_IN;
    T x_buffer[3* vector_depth_size];
    T w_buffer[C_IN << 1];
    T b_buffer[C_OUT];
    T y_buffer[PEs][C_OUT];
    // T gamma_buffer[C_OUT];
    // T beta_buffer[C_OUT];
    #pragma HLS ARRAY_PARTITION variable=x_buffer cyclic factor=4
    #pragma HLS ARRAY_PARTITION variable=w_buffer cyclic factor=4
    #pragma HLS ARRAY_PARTITION variable=b_buffer cyclic factor=4
    #pragma HLS ARRAY_PARTITION variable=y_buffer complete dim=2
    // #pragma HLS ARRAY_PARTITION variable=gamma_buffer cyclic factor=4
    // #pragma HLS ARRAY_PARTITION variable=beta_buffer cyclic factor=4


    Batch_loop:
    for (int n = 0; n < BATCH; n++) {

        T* raw_ifm_data = X.raw_at(n, 0, 0, 0);
        load_buffer<vector_depth_size>(&x_buffer[vector_depth_size], raw_ifm_data);
        load_buffer<C_OUT>(b_buffer, B_1.raw());
        // load_buffer<C_OUT>(gamma_buffer, gamma_1.raw());
        // load_buffer<C_OUT>(beta_buffer, beta_1.raw());

    Slide_PEs_loop:
        for (int r = 0, load_id = 2; r < rolls; r++, load_id++) {
            if (load_id == 3) load_id = 0;
            if (r < rolls - 1) {
                T* raw_ifm_data_inner = X.raw_at(n, r + width, 0, 0);
                load_buffer<vector_depth_size>(&x_buffer[load_id* vector_depth_size], raw_ifm_data_inner);
            }

    #pragma HLS UNROLL
    PEs_loop:
        for (int pe = 0; pe < PEs; pe++) {
            int y_h = r* width, y_w = pe, i, j;
            int x_h = -1 + y_h, x_w = -1 + y_w;
            int* w_h, *w_w, _dh, _dw;
            T* x_points_1, *x_points_2;
            if (x_w < 0 || x_w + 2 == x_shape.W) {
                w_h = &j, w_w = &i;
                _dh = 0, _dw = 2;
            } else {
                w_h = &i, w_w = &j;
                _dh = 2, _dw = 0;
            }
    Channel_out_loop:
            for (int co = 0; co < C_OUT; co++) {
                T psum = 0;
                int d_h, d_w;
    Kernel_loop:
                for (i = 0, d_h = _dh, d_w = _dw; i < 2; i++, d_h = 2 - d_h, d_w = 2 - d_w) 
                for (j = 0; j < 3; j++) {
                    int h = x_h + *w_h, w = x_w + *w_w;
                    if (i == 1 && j == 1) {
                        if (co & 1) {
                            T pre_psum = 0;
                            x_points_1 = x_points_2 = &x_buffer[(mod_3(h + 1)* y_shape.W + w)* C_IN];

                            load_buffer<C_IN>(w_buffer, W_1.raw_at(co, i, j, 0));
                            load_buffer<C_IN>(&w_buffer[C_IN], W_1.raw_at(co - 1, i, j, 0));

                            for (int ci = 0; ci < C_IN; ci++) {
                                psum += x_points_1[ci]* w_buffer[ci];
                                pre_psum += x_points_2[ci]* w_buffer[ci + C_IN];
                            }
                            y_buffer[pe][co - 1] += pre_psum;
                        }
                        break;
                    }
                    char dup = 0;
                    int ops_h = h + d_h, ops_w = w + d_w;
                    int ops_w_h = *w_h + d_h, ops_w_w = *w_w + d_w;
                    if (h < 0) 
                        dup = 1, h = -h;
                    else if (h >= X.shape.H) 
                        dup = 1, h = (X.shape.H << 1) - h - 2;
                    if (w < 0) 
                        dup = 1, w = -w;
                    else if (w >= x_shape.W) 
                        dup = 1, w = (x_shape.W << 1) - w - 2;
                    if (ops_h < 0 || ops_h >= X.shape.H || ops_w < 0 || ops_w >= x_shape.W) dup = 1;


                    x_points_1 = &x_buffer[(mod_3(h + 1)* y_shape.W + w)* C_IN];
                    if (dup) x_points_2 = x_points_1;
                    else x_points_2 = &x_buffer[(mod_3(ops_h + 1)* y_shape.W + ops_w)* C_IN];

                    load_buffer<C_IN>(w_buffer, W_1.raw_at(co, *w_h, *w_w, 0));
                    load_buffer<C_IN>(&w_buffer[C_IN], W_1.raw_at(co, ops_w_h, ops_w_w, 0));
    
    Channel_in_loop:
                    for (int ci = 0; ci < C_IN; ci++) {
    #pragma HLS PIPELINE II=1
                        psum += x_points_1[ci]* w_buffer[ci];
                        psum += x_points_2[ci]* w_buffer[ci + C_IN];
                    }
                }
                y_buffer[pe][co] = psum + b_buffer[co];
            }
        }

    Norm_loop:
        for (int non_pe = 0; non_pe < PEs; non_pe++) {
            int y_h = r* width, y_w = non_pe;
    //         T mean = 0, var = 0;
    //         for (int co = 0; co < C_OUT; co++) {
    // #pragma HLS PIPELINE II=1
    //             T cur = y_buffer[non_pe][co];
    //             mean += cur;
    //             T cur_n = cur* pre_div;
    //             var += cur_n* cur_n;
    //         }
    //         mean /= num;
    //         var = (T) 1 / static_cast<T>(sqrtf(var - mean* mean* adjustment_scale + epsilon));
    //         for (int co = 0; co < C_OUT; co++) {
    // #pragma HLS PIPELINE II=1
    //             T weight = gamma_buffer[co]* var;
    //             T bias = beta_buffer[co] - mean* weight;
    //             T &point = y_buffer[non_pe][co];
    //             point = point* weight + bias;
    //             if (times) point += skip_buffer[(y_h* y_shape.W + y_w)* C_OUT + co];
    //             else if (point < 0) point = 0;
    //         }
            store_buffer<C_OUT>(y_buffer[non_pe], Y.raw_at(n, y_h, y_w, 0));
        }
        }
    }
}

template<int PEs, int BATCH, int C, int H, int W, typename T>
void Channel_Norm_rb(TensorMem<T> &X, TensorMem<T> &gamma, TensorMem<T> &beta, T epsilon, TensorMem<T> &Y) {
    T buffer[C];
    T gamma_buffer[C];
    T beta_buffer[C];
    #pragma HLS ARRAY_PARTITION variable=y_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=gamma_buffer cyclic factor=4
    #pragma HLS ARRAY_PARTITION variable=beta_buffer cyclic factor=4

    constexpr int SUB_W = PEs / W;
    constexpr int ROLLS = H / SUB_W;

    T adjustment_scale = (T) C / (C - 1);
    T pre_div = (T) 1 / (T) (sqrtf(C - 1));


    load_buffer<C>(gamma_buffer, gamma.raw());
    load_buffer<C>(beta_buffer, beta.raw());
    for (int n = 0; n < BATCH; n++) {
        for (int r = 0; r < ROLLS; r++) {
            for (int non_pe = 0; non_pe < PEs; non_pe++) {
                int h = r* SUB_W, w = non_pe;
                load_buffer<C>(buffer, X.raw_at(n, h, w, 0));
                T mean = 0, var = 0;

                for (int co = 0; co < C; co++) {
                    #pragma HLS PIPELINE II=1

                    T cur = buffer[co];
                    mean += cur;
                    T cur_n = cur* pre_div;
                    var += cur_n* cur_n;
                }
                mean /= C;
                var = (T) 1 / static_cast<T>(sqrtf(var - mean* mean* adjustment_scale + epsilon));
                
                for (int co = 0; co < C; co++) {
                    #pragma HLS PIPELINE II=1

                    T weight = gamma_buffer[co]* var;
                    T bias = beta_buffer[co] - mean* weight;
                    T &point = buffer[co];
                    point = point* weight + bias;
                }
                store_buffer<C>(buffer, Y.raw_at(n, h, w, 0));
            }
        }
    }
}

template<int BATCH, int C, int H, int W, typename T>
void Relu_rb(TensorMem<T> &X) {
    for (int n = 0; n < BATCH; n++) {
        for (int h = 0; h < H; h++) 
        for (int w = 0; w < W; w++) {
            for (int c = 0; c < C; c++) {
                T cur = X.get(n, h, w, c);
                if (cur < 0) X.at(n, h, w, c) = 0;
            }
        }
    }
}

template<int BATCH, int C, int H, int W, typename T>
void Add_rb(TensorMem<T> &X1, TensorMem<T> &X2, TensorMem<T> &Y) {
    int size = BATCH* H* W* C;
    for (int i = 0; i < size; i++) 
        Y.raw()[i] = X1.raw()[i] + X2.raw()[i];
}