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
inline int get_index(int n, int h, int w, int c, int H, int W, int C) {
    #pragma HLS INLINE
    return ((n * H + h) * W + w) * C + c;
}

template <int PEs, int BATCH, int C_IN, int C_OUT, 
            int H_IN, int W_IN, int H_R, int W_R, int H_OUT, int W_OUT, typename T>
void ConvTranspose(T* X, T* W, T* B, T* Y) {
    //assert(X && W && B);

    int start_height = -1, start_width = -1;
    constexpr int TC_OUT = C_OUT / PEs;

    T x_buffer[C_IN];
    #pragma HLS ARRAY_PARTITION variable=x_buffer type=complete
    T w_buffer[PEs][H_R][W_R][C_IN];
    #pragma HLS ARRAY_PARTITION variable=w_buffer type=complete dim=1
    #pragma HLS ARRAY_PARTITION variable=w_buffer type=complete dim=2
    #pragma HLS ARRAY_PARTITION variable=w_buffer type=complete dim=3
    T b_buffer[C_OUT];
    #pragma HLS ARRAY_PARTITION variable=b_buffer type=complete
    T y_buffer[H_R][W_R][C_OUT];
    #pragma HLS ARRAY_PARTITION variable=y_buffer type=complete

    T col_buff[C_OUT][H_R][W_R];
    T row_buff[W_IN][C_OUT][H_R][W_R];
    #pragma HLS ARRAY_PARTITION variable=col_buff type=block factor=PEs dim=1
    #pragma HLS ARRAY_PARTITION variable=col_buff type=complete dim=2
    #pragma HLS ARRAY_PARTITION variable=col_buff type=complete dim=3
    
    #pragma HLS ARRAY_PARTITION variable=row_buff type=block factor=PEs dim=2
    #pragma HLS ARRAY_PARTITION variable=row_buff type=complete dim=3
    #pragma HLS ARRAY_PARTITION variable=row_buff type=complete dim=4


    load_buffer<C_OUT>(b_buffer, B);
    Batch_loop:
    for (int n = 0; n < BATCH; n++) {

        Layer_in_loop:
        for (int hi = 0, i = 0; hi < H_IN; hi++, i += 2) 
        for (int wi = 0, j = 0; wi < W_IN; wi++, j += 2) {
            int start_wind_h = start_height + i, start_wind_w = start_width + j;
            int x_id = get_index(n, hi, wi, 0, H_IN, W_IN, C_IN);
            load_buffer<C_IN>(x_buffer, &X[x_id]);

            PEs_loop:
            for (int pe = 0; pe < PEs; pe++) {
                #pragma HLS UNROLL

                T patch_buff[H_R][W_R];
                T out_col[H_R][W_R];
                T out_row[H_R][W_R];

                #pragma HLS ARRAY_PARTITION variable=patch_buff type=complete dim=0
                #pragma HLS ARRAY_PARTITION variable=out_col type=complete dim=0
                #pragma HLS ARRAY_PARTITION variable=out_row type=complete dim=0


                Partial_channel_out_loop:
                for (int tco = 0; tco < TC_OUT; tco++) {
                    int co = pe* TC_OUT + tco;

                    for (int hf = 0; hf < H_R; hf++) 
                    for (int wf = 0; wf < W_R; wf++) {
                        int w_id = get_index(co, hf, wf, 0, H_R, W_R, C_IN);
                        load_buffer<C_IN>(w_buffer[pe][hf][wf], &W[w_id]);
                    }

                    // Reset patch_buff
                    for (int hf = 0; hf < H_R; hf++) {
                        #pragma HLS UNROLL
                        for (int wf = 0; wf < W_R; wf++) {
                            #pragma HLS UNROLL
                            patch_buff[hf][wf] = 0;
                        }
                    }

                    for (int ci = 0; ci < C_IN; ci++) {
                        #pragma HLS PIPELINE II=1
                        for (int hf = 0; hf < H_R; hf++) {
                            #pragma HLS UNROLL
                            for (int wf = 0; wf < W_R; wf++) {
                                #pragma HLS UNROLL
                                patch_buff[hf][wf] += x_buffer[ci]* w_buffer[pe][hf][wf][ci];
                            }
                        }
                    }

                    for (int hf = 0; hf < H_R; hf++) {
                        #pragma HLS UNROLL
                        for (int wf = 0; wf < W_R; wf++) {
                            #pragma HLS UNROLL
                            if (wf < W_R - 2) 
                                if (wi == 0) 
                                    out_col[hf][wf] = patch_buff[hf][wf];
                                else 
                                    out_col[hf][wf] = patch_buff[hf][wf] + col_buff[co][hf][wf];
                            else 
                                out_col[hf][wf] = patch_buff[hf][wf];

                            if (wf >= 2) 
                                col_buff[co][hf][wf - 2] = out_col[hf][wf];

                            if (hf < H_R - 2) 
                                if (hi == 0) 
                                    out_row[hf][wf] = out_col[hf][wf];
                                else 
                                    out_row[hf][wf] = out_col[hf][wf] + row_buff[wi][co][hf][wf];
                            else 
                                out_row[hf][wf] = out_col[hf][wf];

                            if (hf >= 2) 
                                row_buff[wi][co][hf - 2][wf] = out_row[hf][wf];

                            if ((hf < 2 || hi == H_IN - 1) && (wf < 2 || wi == W_IN - 1)) 
                                y_buffer[hf][wf][co] = out_row[hf][wf] + b_buffer[co];
                        }
                    }
                }  // end tco
            }  // end pe

            Partial_output_window_loop:
            for (int hf = 0; hf < H_R; hf++) 
            for (int wf = 0; wf < W_R; wf++) {
                #pragma HLS PIPELINE II=1

                int h = start_wind_h + hf, w = start_wind_w + wf;
                if (h >= 0 && w >= 0 //&& h < H_OUT - 1 && w < W_OUT - 1
                        && (hf < 2 || hi == H_IN - 1) && (wf < 2 || wi == W_IN - 1)) {
                    int y_id = get_index(n, h, w, 0, H_OUT, W_OUT, C_OUT);
                    store_buffer<C_OUT>(y_buffer[hf][wf], &Y[y_id]);
                }
            }
        }  // end hi-wi
    }  // end batch
}
template void ConvTranspose<16, 1, 960, 480, 16, 16, 3, 3, 32, 32>
        (float*, float*, float*, float*);
template void ConvTranspose<16, 1, 480, 240, 32, 32, 3, 3, 64, 64>
        (float*, float*, float*, float*);
template void ConvTranspose<16, 1, 240, 120, 64, 64, 3, 3, 128, 128>
        (float*, float*, float*, float*);
template void ConvTranspose<16, 1, 120, 60, 128, 128, 3, 3, 256, 256>
        (float*, float*, float*, float*);


// Hàm Top-Level phần cứng để Vitis HLS tổng hợp
void HW_ConvTranspose(
    float* X, float* W, float* B, float* Y, int id
) {
    // 1. Khai báo cổng AXI Master để IP có thể cắm vào DRAM (DDR)
    #pragma HLS INTERFACE m_axi port=X bundle=gmem_X depth=245760 // 1x16x16x960
    #pragma HLS INTERFACE m_axi port=W bundle=gmem_W depth=1327104 // 480x3x3x960
    #pragma HLS INTERFACE m_axi port=B bundle=gmem_B depth=480
    #pragma HLS INTERFACE m_axi port=Y bundle=gmem_Y depth=491520 // 1x32x32x480
    
    // 2. Khai báo cổng AXI-Lite để CPU (ARM/Host) điều khiển truyền địa chỉ bộ nhớ
    #pragma HLS INTERFACE s_axilite port=X bundle=control
    #pragma HLS INTERFACE s_axilite port=W bundle=control
    #pragma HLS INTERFACE s_axilite port=B bundle=control
    #pragma HLS INTERFACE s_axilite port=Y bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    // 3. Gọi Template tương ứng
    switch (id) {
        case 0: ConvTranspose<16, 1, 960, 480, 16, 16, 3, 3, 32, 32, float>(X, W, B, Y); break;
        case 1: ConvTranspose<16, 1, 480, 240, 32, 32, 3, 3, 64, 64, float>(X, W, B, Y); break;
        case 2: ConvTranspose<16, 1, 240, 120, 64, 64, 3, 3, 128, 128, float>(X, W, B, Y); break;
        case 3: ConvTranspose<16, 1, 120, 60, 128, 128, 3, 3, 256, 256, float>(X, W, B, Y); break;
    }
}


