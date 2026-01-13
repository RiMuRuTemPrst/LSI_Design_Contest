#include "..\include\Core.h"
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

template<int PEs, int C_IN, int C_OUT, char RELU, typename T>
void Pad_ref_Conv_11133111111_CNorm_Relu(TensorMem<T> &X, TensorMem<T> &W, TensorMem<T> &B, 
                                        TensorMem<T> &gamma, TensorMem<T> &beta, T epsilon, TensorMem<T> &Y) {
    Shape x_shape = X.shape;
    Shape w_shape = W.shape;
    Shape b_shape = B.shape;
    Shape y_shape = Y.shape;

    if (3 != w_shape.H || 3 != w_shape.W) 
        assert(0 && "Error: Unappropriate input sizes for Conv !!\n");
    
    if (y_shape.H != (x_shape.H - (w_shape.H - 1)* 1 
        + 1 + 1 - 1) / 1 + 1 
        || y_shape.W != (x_shape.W - (w_shape.W - 1)* 1 
        + 1 + 1 - 1) / 1 + 1 
        || y_shape.N != x_shape.N || y_shape.C != w_shape.N || C_IN != w_shape.C || C_OUT != w_shape.N)
        assert(0 && "Error: Unappropriate output sizes for Conv !!\n");

    int start_height = -1, start_width = -1;
    int batch = x_shape.N;
    //b_shape.N = b_shape.H = b_shape.W = 0;
    assert(!(PEs % y_shape.W));
    int width = PEs / y_shape.W;
    int rolls = y_shape.H / width;
    int num = y_shape.C;
    T adjustment_scale = (T) num / (num - 1);
    T pre_div = (T) 1 / (T) (sqrtf(num - 1));


    T x_buffer[C_IN << 1];
    T w_buffer[C_IN << 1];
    T y_buffer[PEs][C_OUT];
    #pragma HLS ARRAY_PARTITION variable=x_buffer cyclic factor=4
    #pragma HLS ARRAY_PARTITION variable=w_buffer cyclic factor=4
    #pragma HLS ARRAY_PARTITION variable=y_buffer complete dim=2

    
    for (int n = 0; n < batch; n++) {
        x_shape.N = y_shape.N = n;
        for (int r = 0; r < rolls; r++) {
    #pragma HLS UNROLL
        for (int pe = 0; pe < PEs; pe++) {
            int y_h = r* width + pe / y_shape.W, y_w = pe % y_shape.W, w_h, w_w;
            x_shape.H = start_height + y_h, x_shape.W = start_width + y_w;
            int* _h, *_w, _dh, _dw;
            if (x_shape.W < 0 || x_shape.W + 2 == X.shape.W) {
                _h = &w_w, _w = &w_h;
                _dh = 0, _dw = 2;
            } else {
                _h = &w_h, _w = &w_w;
                _dh = 2, _dw = 0;
            }
            for (int co = 0; co < C_OUT; co++) {
                y_shape.C = w_shape.N = co;
                T psum = 0;
                int d_h, d_w;
                for (w_h = 0, d_h = _dh, d_w = _dw; w_h < 2; w_h++, d_h = 2 - d_h, d_w = 2 - d_w) 
                for (w_w = 0; w_w < 3; w_w++) {
                    int h = x_shape.H + *_h, w = x_shape.W + *_w;
                    if (w_h == 1 && w_w == 1) {
                        if (co & 1) {
                            T pre_psum = 0;
                            load_buffer<C_IN>(x_buffer, X.raw_at(x_shape.N, h, w, 0));
                            load_buffer<C_IN>(w_buffer, W.raw_at(w_shape.N, w_h, w_w, 0));
                            load_buffer<C_IN>(&w_buffer[C_IN], W.raw_at(w_shape.N - 1, w_h, w_w, 0));
                            for (int ci = 0; ci < C_IN; ci++) {
                                psum += x_buffer[ci]* w_buffer[ci];
                                pre_psum += x_buffer[ci]* w_buffer[ci + C_IN];
                            }
                            y_buffer[pe][y_shape.C - 1] += pre_psum;
                        }
                        break;
                    }
                    char dup = 0;
                    int ops_h = h + d_h, ops_w = w + d_w;
                    int ops_w_h = *_h + d_h, ops_w_w = *_w + d_w;
                    if (h < 0) 
                        dup = 1, h = -h;
                    else if (h >= X.shape.H) 
                        dup = 1, h = (X.shape.H << 1) - h - 2;
                    if (w < 0) 
                        dup = 1, w = -w;
                    else if (w >= X.shape.W) 
                        dup = 1, w = (X.shape.W << 1) - w - 2;
                    if (ops_h < 0 || ops_h >= X.shape.H || ops_w < 0 || ops_w >= X.shape.W) dup = 1;

                    load_buffer<C_IN>(x_buffer, X.raw_at(x_shape.N, h, w, 0));
                    if (dup) load_buffer<C_IN>(&x_buffer[C_IN], x_buffer);
                    else load_buffer<C_IN>(&x_buffer[C_IN], X.raw_at(x_shape.N, ops_h, ops_w, 0));

                    load_buffer<C_IN>(w_buffer, W.raw_at(w_shape.N, *_h, *_w, 0));
                    load_buffer<C_IN>(&w_buffer[C_IN], W.raw_at(w_shape.N, ops_w_h, ops_w_w, 0));
                    
                    for (int ci = 0; ci < C_IN << 1; ci++) {
    #pragma HLS PIPELINE II=1
                        psum += x_buffer[ci]* w_buffer[ci];
                    }
                }
                y_buffer[pe][y_shape.C] = psum + B.raw()[co];
            }
        }
        for (int non_pe = 0; non_pe < PEs; non_pe++) {
            int y_h = r* width + non_pe / y_shape.W, y_w = non_pe % y_shape.W;
            T mean = 0, var = 0;
            for (int co = 0; co < C_OUT; co++) {
    #pragma HLS PIPELINE II=1
                T cur = y_buffer[non_pe][co];
                mean += cur;
                T cur_n = cur* pre_div;
                var += cur_n* cur_n;
            }
            mean /= num;
            var = (T) 1 / static_cast<T>(sqrtf(var - mean* mean* adjustment_scale + epsilon));
            for (int co = 0; co < C_OUT; co++) {
    #pragma HLS PIPELINE II=1
                T weight = gamma.raw()[co]* var;
                T bias = beta.raw()[co] - mean* weight;
                T &point = y_buffer[non_pe][co];
                point = point* weight + bias;
                if (RELU && point < 0) point = 0;
            }
            store_buffer<C_OUT>(y_buffer[non_pe], Y.raw_at(y_shape.N, y_h, y_w, 0));
        }
        }
    }
}


/*
template<int PEs, int C_IN, int C_OUT, char RELU, typename T>
void Pad_ref_Conv_11133111111_CNorm_Relu(TensorMem<T> &X, TensorMem<T> &W, TensorMem<T> &B, 
                                        TensorMem<T> &gamma, TensorMem<T> &beta, T epsilon, TensorMem<T> &Y) {
    Shape x_shape = X.shape;
    Shape w_shape = W.shape;
    Shape b_shape = B.shape;
    Shape y_shape = Y.shape;

    if (3 != w_shape.H || 3 != w_shape.W) 
        assert(0 && "Error: Unappropriate input sizes for Conv !!\n");
    
    if (y_shape.H != (x_shape.H - (w_shape.H - 1)* 1 
        + 1 + 1 - 1) / 1 + 1 
        || y_shape.W != (x_shape.W - (w_shape.W - 1)* 1 
        + 1 + 1 - 1) / 1 + 1 
        || y_shape.N != x_shape.N || y_shape.C != w_shape.N || C_IN != w_shape.C || C_OUT != w_shape.N)
        assert(0 && "Error: Unappropriate output sizes for Conv !!\n");

    int start_height = -1, start_width = -1;
    int batch = x_shape.N;
    //b_shape.N = b_shape.H = b_shape.W = 0;
    assert(!(PEs % y_shape.W));
    int width = PEs / y_shape.W;
    int rolls = y_shape.H / width;
    int num = y_shape.C;
    T adjustment_scale = (T) num / (num - 1);
    T pre_div = (T) 1 / (T) (sqrtf(num - 1));


    T x_buffer[C_IN << 1];
    T w_buffer[C_IN << 1];
    T y_buffer[PEs][C_OUT];
    #pragma HLS ARRAY_PARTITION variable=x_buffer cyclic factor=4
    #pragma HLS ARRAY_PARTITION variable=w_buffer cyclic factor=4
    #pragma HLS ARRAY_PARTITION variable=y_buffer complete dim=2

    
    for (int n = 0; n < batch; n++) {
        x_shape.N = y_shape.N = n;
        for (int r = 0; r < rolls; r++) {
    #pragma HLS UNROLL
        for (int pe = 0; pe < PEs; pe++) {
            int y_h = r* width + pe / y_shape.W, y_w = pe % y_shape.W;
            x_shape.H = start_height + y_h, x_shape.W = start_width + y_w;
            for (int co = 0; co < C_OUT; co++) {
                y_shape.C = w_shape.N = co;
                T psum = 0;
                for (int w_h = 0; w_h < 3; w_h++) 
                for (int w_w = 0; w_w < 3; w_w++) {
                    int h = x_shape.H + w_h, w = x_shape.W + w_w;
                    if (w_h == 1 && w_w == 1) {
                        if (co & 1) {
                            T pre_psum = 0;
                            load_buffer<C_IN>(x_buffer, X.raw_at(x_shape.N, h, w, 0));
                            load_buffer<C_IN>(w_buffer, W.raw_at(w_shape.N, w_h, w_w, 0));
                            load_buffer<C_IN>(&w_buffer[C_IN], W.raw_at(w_shape.N - 1, w_h, w_w, 0));
                            for (int ci = 0; ci < C_IN; ci++) {
                                psum += x_buffer[ci]* w_buffer[ci];
                                pre_psum += x_buffer[ci]* w_buffer[ci + C_IN];
                            }
                            y_buffer[pe][y_shape.C - 1] += pre_psum;
                        }
                        w_h++;
                        break;
                    }
                    int ops_h = x_shape.H + 2 - w_h, ops_w = x_shape.W + 2 - w_w;
                    int ops_w_h = 2 - w_h, ops_w_w = 2 - w_w;
                    if (h < 0) 
                        h = -h;
                    else if (h >= X.shape.H) 
                        h = (X.shape.H << 1) - h - 2;
                    if (w < 0) 
                        w = -w;
                    else if (w >= X.shape.W) 
                        w = (X.shape.W << 1) - w - 2;
                    if (ops_h < 0) 
                        ops_h = -ops_h;
                    else if (ops_h >= X.shape.H) 
                        ops_h = (X.shape.H << 1) - ops_h - 2;
                    if (ops_w < 0) 
                        ops_w = -ops_w;
                    else if (ops_w >= X.shape.W) 
                        ops_w = (X.shape.W << 1) - ops_w - 2;

                    load_buffer<C_IN>(x_buffer, X.raw_at(x_shape.N, h, w, 0));
                    load_buffer<C_IN>(&x_buffer[C_IN], X.raw_at(x_shape.N, ops_h, ops_w, 0));

                    load_buffer<C_IN>(w_buffer, W.raw_at(w_shape.N, w_h, w_w, 0));
                    load_buffer<C_IN>(&w_buffer[C_IN], W.raw_at(w_shape.N, ops_w_h, ops_w_w, 0));
                    
                    for (int ci = 0; ci < C_IN << 1; ci++) {
    #pragma HLS PIPELINE II=1
                        psum += x_buffer[ci]* w_buffer[ci];
                    }
                }
                y_buffer[pe][y_shape.C] = psum + B.raw()[co];
            }
        }
        for (int non_pe = 0; non_pe < PEs; non_pe++) {
            int y_h = r* width + non_pe / y_shape.W, y_w = non_pe % y_shape.W;
            T mean = 0, var = 0;
            for (int co = 0; co < C_OUT; co++) {
    #pragma HLS PIPELINE II=1
                T cur = y_buffer[non_pe][co];
                mean += cur;
                T cur_n = cur* pre_div;
                var += cur_n* cur_n;
            }
            mean /= num;
            var = (T) 1 / static_cast<T>(sqrtf(var - mean* mean* adjustment_scale + epsilon));
            for (int co = 0; co < C_OUT; co++) {
    #pragma HLS PIPELINE II=1
                T weight = gamma.raw()[co]* var;
                T bias = beta.raw()[co] - mean* weight;
                T &point = y_buffer[non_pe][co];
                point = point* weight + bias;
                if (RELU && point < 0) point = 0;
            }
            store_buffer<C_OUT>(y_buffer[non_pe], Y.raw_at(y_shape.N, y_h, y_w, 0));
        }
        }
    }
}*/