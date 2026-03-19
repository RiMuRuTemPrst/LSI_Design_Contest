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
template<int SIZE, typename T>
static inline void shift_load_buffer(T buffer[2][SIZE], T* data) {
    #pragma HLS INLINE
    for (int i = 0; i < SIZE; i++) {
        buffer[0][i] = buffer[1][i];
        buffer[1][i] = data[i];
    }
}
inline int get_index(int n, int h, int w, int c, int H, int W, int C) {
    #pragma HLS INLINE
    return ((n * H + h) * W + w) * C + c;
}

template <int PEs, int BATCH, int C_IN, int C_OUT, 
            int H_IN, int W_IN, int H_OUT, int W_OUT, typename T>
void ConvTranspose_hw_2(T* X, T* W, T* B, T* Y) {
    //assert(X && W && B);

    constexpr int TC_OUT = C_OUT / PEs;

    T x_buffer[2][2][C_IN];
    T w_1x1_buffer[PEs][C_IN];
    T w_1x2_buffer[PEs][2][C_IN];
    T w_2x1_buffer[PEs][2][C_IN];
    T w_2x2_buffer[PEs][2][2][C_IN];
    T b_buffer[C_OUT];
    T y_buffer[2][2][C_OUT];

    #pragma HLS ARRAY_PARTITION variable=x_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=x_buffer complete dim=2
    #pragma HLS ARRAY_PARTITION variable=w_1x1_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=w_1x2_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=w_1x2_buffer complete dim=2
    #pragma HLS ARRAY_PARTITION variable=w_2x1_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=w_2x1_buffer complete dim=2
    #pragma HLS ARRAY_PARTITION variable=w_2x2_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=w_2x2_buffer complete dim=2
    #pragma HLS ARRAY_PARTITION variable=w_2x2_buffer complete dim=3
    #pragma HLS ARRAY_PARTITION variable=y_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=y_buffer complete dim=2


    load_buffer<C_OUT>(b_buffer, B);
    for (int n = 0; n < BATCH; n++) {
        for (int hi = 0, i = 0; hi < H_IN; hi++, i += 2) 
        for (int wi = 0, j = 0; wi < W_IN; wi++, j += 2) {
            int x_id;
            if (wi == 0) {
                x_id = get_index(n, hi, 0, 0, H_IN, W_IN, C_IN);
                load_buffer<C_IN>(x_buffer[0][0], &X[x_id]);
                x_id = get_index(n, hi, 1, 0, H_IN, W_IN, C_IN);
                load_buffer<C_IN>(x_buffer[0][1], &X[x_id]);
                if (hi != H_IN - 1) {
                    x_id = get_index(n, hi + 1, 0, 0, H_IN, W_IN, C_IN);
                    load_buffer<C_IN>(x_buffer[1][0], &X[x_id]);
                    x_id = get_index(n, hi + 1, 1, 0, H_IN, W_IN, C_IN);
                    load_buffer<C_IN>(x_buffer[1][1], &X[x_id]);
                }
            } else if (wi == W_IN - 1) {
                load_buffer<C_IN>(x_buffer[0][0], x_buffer[0][1]);
                if (hi != H_IN - 1) 
                    load_buffer<C_IN>(x_buffer[1][0], x_buffer[1][1]);
            } else {
                x_id = get_index(n, hi, wi + 1, 0, H_IN, W_IN, C_IN);
                shift_load_buffer<C_IN>(x_buffer[0], &X[x_id]);
                if (hi != H_IN - 1) {
                    x_id = get_index(n, hi + 1, wi + 1, 0, H_IN, W_IN, C_IN);
                    shift_load_buffer<C_IN>(x_buffer[1], &X[x_id]);
                }
            }

            for (int pe = 0; pe < PEs; pe++) {
                #pragma HLS UNROLL
                for (int tco = 0; tco < TC_OUT; tco++) {
                    int co = pe* TC_OUT + tco;
                    int w_id;
                    w_id = get_index(co, 1, 1, 0, 3, 3, C_IN);
                    load_buffer<C_IN>(w_1x1_buffer[pe], &W[w_id]);
                    w_id = get_index(co, 1, 2, 0, 3, 3, C_IN);
                    load_buffer<C_IN>(w_1x2_buffer[pe][0], &W[w_id]);
                    w_id = get_index(co, 1, 0, 0, 3, 3, C_IN);
                    load_buffer<C_IN>(w_1x2_buffer[pe][1], &W[w_id]);
                    w_id = get_index(co, 2, 1, 0, 3, 3, C_IN);
                    load_buffer<C_IN>(w_2x1_buffer[pe][0], &W[w_id]);
                    w_id = get_index(co, 0, 1, 0, 3, 3, C_IN);
                    load_buffer<C_IN>(w_2x1_buffer[pe][1], &W[w_id]);
                    w_id = get_index(co, 2, 2, 0, 3, 3, C_IN);
                    load_buffer<C_IN>(w_2x2_buffer[pe][0][0], &W[w_id]);
                    w_id = get_index(co, 2, 0, 0, 3, 3, C_IN);
                    load_buffer<C_IN>(w_2x2_buffer[pe][0][1], &W[w_id]);
                    w_id = get_index(co, 0, 2, 0, 3, 3, C_IN);
                    load_buffer<C_IN>(w_2x2_buffer[pe][1][0], &W[w_id]);
                    w_id = get_index(co, 0, 0, 0, 3, 3, C_IN);
                    load_buffer<C_IN>(w_2x2_buffer[pe][1][1], &W[w_id]);

                    y_buffer[0][0][co] = 0;
                    y_buffer[0][1][co] = 0;
                    y_buffer[1][0][co] = 0;
                    y_buffer[1][1][co] = 0;
                    for (int ci = 0; ci < C_IN; ci++) {
                        #pragma HLS PIPELINE II=1
                        #pragma HLS DEPENDENCE variable=y_buffer type=inter false

                        T sub_1x2[2];
                        T sub_2x1[2];
                        T sub_2x2[2][2];

                        y_buffer[0][0][co] += x_buffer[0][0][ci]* w_1x1_buffer[pe][ci];

                        for (int sub_id = 0; sub_id < 2; sub_id++) {
                            #pragma HLS UNROLL

                            if (wi + sub_id < W_IN) 
                                sub_1x2[sub_id] = x_buffer[0][sub_id][ci]* w_1x2_buffer[pe][sub_id][ci];
                            else 
                                sub_1x2[sub_id] = 0;
                            if (hi + sub_id < H_IN) 
                                sub_2x1[sub_id] = x_buffer[sub_id][0][ci]* w_2x1_buffer[pe][sub_id][ci];
                            else 
                                sub_2x1[sub_id] = 0;
                        }
                        y_buffer[0][1][co] += sub_1x2[0] + sub_1x2[1];
                        y_buffer[1][0][co] += sub_2x1[0] + sub_2x1[1];

                        for (int sid_0 = 0; sid_0 < 2; sid_0++) {
                            #pragma HLS UNROLL
                            for (int sid_1 = 0; sid_1 < 2; sid_1++) {
                                #pragma HLS UNROLL
                                if (hi + sid_0 < H_IN && wi + sid_1 < W_IN) 
                                    sub_2x2[sid_0][sid_1] = x_buffer[sid_0][sid_1][ci]* w_2x2_buffer[pe][sid_0][sid_1][ci];
                                else 
                                    sub_2x2[sid_0][sid_1] = 0;
                            }
                        }
                        y_buffer[1][1][co] += (sub_2x2[0][0] + sub_2x2[0][1]) + (sub_2x2[1][0] + sub_2x2[1][1]);
                    }
                    for (int sid_0 = 0; sid_0 < 2; sid_0++) {
                        #pragma HLS UNROLL
                        for (int sid_1 = 0; sid_1 < 2; sid_1++) {
                            #pragma HLS UNROLL
                            y_buffer[sid_0][sid_1][co] += b_buffer[co];
                        }
                    }
                }
            }
            for (int sho = 0; sho < 2; sho++) {
                #pragma HLS UNROLL
                for (int swo = 0; swo < 2; swo++) {
                    #pragma HLS UNROLL
                    int ho = i + sho, wo = j + swo;
                    if (ho < H_OUT && wo < W_OUT) {
                        int y_id = get_index(n, ho, wo, 0, H_OUT, W_OUT, C_OUT);
                        store_buffer<C_OUT>(y_buffer[sho][swo], Y[y_id]);
                    }
                }
            }
        }
    }
}

// Hàm Top-Level phần cứng để Vitis HLS tổng hợp
void HW_ConvTranspose_2(
    float* X, float* W, float* B, float* Y, int id
) {
    // 1. Khai báo cổng AXI Master để IP có thể cắm vào DRAM (DDR)
    #pragma HLS INTERFACE m_axi port=X bundle=gmem_X depth=1966080 
    #pragma HLS INTERFACE m_axi port=W bundle=gmem_W depth=1327104 
    #pragma HLS INTERFACE m_axi port=B bundle=gmem_B depth=480
    #pragma HLS INTERFACE m_axi port=Y bundle=gmem_Y depth=3932160
    
    // 2. Khai báo cổng AXI-Lite để CPU (ARM/Host) điều khiển truyền địa chỉ bộ nhớ
    #pragma HLS INTERFACE s_axilite port=X bundle=control
    #pragma HLS INTERFACE s_axilite port=W bundle=control
    #pragma HLS INTERFACE s_axilite port=B bundle=control
    #pragma HLS INTERFACE s_axilite port=Y bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    // 3. Gọi Template tương ứng
    switch (id) {
        case 0: ConvTranspose_hw_2<16, 1, 960, 480, 16, 16, 32, 32, float>(X, W, B, Y); break;
        case 1: ConvTranspose_hw_2<16, 1, 480, 240, 32, 32, 64, 64, float>(X, W, B, Y); break;
        case 2: ConvTranspose_hw_2<16, 1, 240, 120, 64, 64, 128, 128, float>(X, W, B, Y); break;
        case 3: ConvTranspose_hw_2<16, 1, 120, 60, 128, 128, 256, 256, float>(X, W, B, Y); break;
    }
}