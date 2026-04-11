#pragma once
#include "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/gen/Core.h"

#if !defined(__SYNTHESIS__)
  #include <cassert>
  #include <cmath>
#endif

#if defined(__SYNTHESIS__)
  #include <hls_math.h>
#endif

// ============================================================
// Small utilities
// ============================================================
template<int SIZE, typename T>
static inline void load_buffer(T buffer[SIZE], const T* data) {
#pragma HLS INLINE
    for (int i = 0; i < SIZE; i++) {
#pragma HLS PIPELINE II=1
        buffer[i] = data[i];
    }
}

template<int SIZE, typename T>
static inline void store_buffer(const T buffer[SIZE], T* data) {
#pragma HLS INLINE
    for (int i = 0; i < SIZE; i++) {
#pragma HLS PIPELINE II=1
        data[i] = buffer[i];
    }
}

static inline int mod_3(int n) {
#pragma HLS INLINE
    switch (n) {
        case 0: case 3: case 6: case 9:  case 12: case 15: return 0;
        case 1: case 4: case 7: case 10: case 13: case 16: return 1;
        case 2: case 5: case 8: case 11: case 14: case 17: return 2;
        default: return 0;
    }
}

static inline float my_sqrt_f(float x) {
#pragma HLS INLINE
#if defined(__SYNTHESIS__)
    return hls::sqrtf(x);
#else
    return std::sqrt(x);
#endif
}

template<typename T>
static inline T fmul_dsp(T a, T b) {
#pragma HLS INLINE
    T p = a * b;
#pragma HLS BIND_OP variable=p op=fmul impl=dsp
    return p;
}

template<typename T>
static inline T fadd_lat4(T a, T b) {
#pragma HLS INLINE
    T s = a + b;
#pragma HLS BIND_OP variable=s op=fadd latency=4
    return s;
}

// ============================================================
// Direction-2 compute kernel: C_OUT blocking + 2-pass norm
// ============================================================

template<
    int PEs, int C_IN, int C_OUT,
    int LANE, int ACC_DEPTH, int ACC_MASK, int C_OBLK,
    typename T
>
static inline void conv_block_one_pe(
    const Shape &x_shape,
    const Shape &y_shape,
    int r, int width,
    int pe,
    T x_buffer[],
    T w_buffer[],
    const T b_buffer[],
    TensorMem<T> &W_1,
    TensorMem<T> &W_2,
    int times,
    int co0,
    T y_blk_out[C_OBLK]
) {
#pragma HLS INLINE

    const int y_h = r * width;
    (void)y_h;
    const int y_w = pe;
    (void)y_w;
    const int x_h = -1 + y_h;
    const int x_w = -1 + y_w;

    int i, j, *w_h, *w_w, _dh, _dw;
    T* x_points_1;
    T* x_points_2;

    if (x_w < 0 || x_w + 2 == x_shape.W) {
        w_h = &j; w_w = &i; _dh = 0; _dw = 2;
    } else {
        w_h = &i; w_w = &j; _dh = 2; _dw = 0;
    }

#ifndef __SYNTHESIS__
    assert((co0 & 1) == 0); // keep (co odd -> co-1) inside block
#endif

CO_LOOP:
    for (int t = 0; t < C_OBLK; t++) {
        const int co = co0 + t;

        T v_sum1[LANE][ACC_DEPTH];
        T v_sum2[LANE][ACC_DEPTH];
#pragma HLS ARRAY_PARTITION variable=v_sum1 complete dim=0
#pragma HLS ARRAY_PARTITION variable=v_sum2 complete dim=0

RST_K:
        for (int k = 0; k < LANE; k++) {
#pragma HLS UNROLL
RST_D:
            for (int d = 0; d < ACC_DEPTH; d++) {
#pragma HLS UNROLL
                v_sum1[k][d] = (T)0;
                v_sum2[k][d] = (T)0;
            }
        }

        int d_h, d_w;

KERNEL_IJ:
        for (i = 0, d_h = _dh, d_w = _dw; i < 2; i++, d_h = 2 - d_h, d_w = 2 - d_w)
        for (j = 0; j < 3; j++) {

            int h = x_h + *w_h;
            int w = x_w + *w_w;

            // Middle point logic (match original)
            if (i == 1 && j == 1) {
                if (co & 1) {
                    T v_prev[LANE][ACC_DEPTH];
#pragma HLS ARRAY_PARTITION variable=v_prev complete dim=0

RST_PRE_K:
                    for (int k = 0; k < LANE; k++) {
#pragma HLS UNROLL
RST_PRE_D:
                        for (int d = 0; d < ACC_DEPTH; d++) {
#pragma HLS UNROLL
                            v_prev[k][d] = (T)0;
                        }
                    }

                    x_points_1 = x_points_2 = &x_buffer[(mod_3(h + 1) * y_shape.W + w) * C_IN];

                    if (!times) {
                        load_buffer<C_IN>(w_buffer,        W_1.raw_at(co,     i, j, 0));
                        load_buffer<C_IN>(&w_buffer[C_IN], W_1.raw_at(co - 1, i, j, 0));
                    } else {
                        load_buffer<C_IN>(w_buffer,        W_2.raw_at(co,     i, j, 0));
                        load_buffer<C_IN>(&w_buffer[C_IN], W_2.raw_at(co - 1, i, j, 0));
                    }

                    int acc_idx = 0;

CI_CENTER:
                    for (int ci = 0; ci < C_IN; ci += LANE) {
#pragma HLS PIPELINE II=1
K_CENTER:
                        for (int k = 0; k < LANE; k++) {
#pragma HLS UNROLL
                            T xval = x_points_1[ci + k];
                            T wcur = w_buffer[ci + k];
                            T wpre = w_buffer[ci + k + C_IN];

                            T val_cur = fmul_dsp(xval, wcur);
                            T val_pre = fmul_dsp(xval, wpre);

                            v_sum1[k][acc_idx] = fadd_lat4(v_sum1[k][acc_idx], val_cur);
                            v_prev[k][acc_idx] = fadd_lat4(v_prev[k][acc_idx], val_pre);
                        }
                        acc_idx = (acc_idx + 1) & ACC_MASK;
                    }

                    T prev_sum = (T)0;
RED_PRE_K:
                    for (int k = 0; k < LANE; k++) {
#pragma HLS UNROLL
RED_PRE_D:
                        for (int d = 0; d < ACC_DEPTH; d++) {
#pragma HLS UNROLL
                            prev_sum = fadd_lat4(prev_sum, v_prev[k][d]);
                        }
                    }

                    y_blk_out[t - 1] = fadd_lat4(y_blk_out[t - 1], prev_sum);
                }
                break;
            }

            // Normal convolution logic (reflect padding + dup)
            char dup = 0;
            int ops_h = h + d_h, ops_w = w + d_w;
            int ops_w_h = *w_h + d_h, ops_w_w = *w_w + d_w;

            if (h < 0) { dup = 1; h = -h; }
            else if (h >= x_shape.H) { dup = 1; h = (x_shape.H << 1) - h - 2; }
            if (w < 0) { dup = 1; w = -w; }
            else if (w >= x_shape.W) { dup = 1; w = (x_shape.W << 1) - w - 2; }
            if (ops_h < 0 || ops_h >= x_shape.H || ops_w < 0 || ops_w >= x_shape.W) dup = 1;

            x_points_1 = &x_buffer[(mod_3(h + 1) * y_shape.W + w) * C_IN];
            if (dup) x_points_2 = x_points_1;
            else     x_points_2 = &x_buffer[(mod_3(ops_h + 1) * y_shape.W + ops_w) * C_IN];

            if (!times) {
                load_buffer<C_IN>(w_buffer,        W_1.raw_at(co, *w_h, *w_w, 0));
                load_buffer<C_IN>(&w_buffer[C_IN], W_1.raw_at(co, ops_w_h, ops_w_w, 0));
            } else {
                load_buffer<C_IN>(w_buffer,        W_2.raw_at(co, *w_h, *w_w, 0));
                load_buffer<C_IN>(&w_buffer[C_IN], W_2.raw_at(co, ops_w_h, ops_w_w, 0));
            }

            int acc_idx = 0;

CI_LOOP:
            for (int ci = 0; ci < C_IN; ci += LANE) {
#pragma HLS PIPELINE II=1
K_LOOP:
                for (int k = 0; k < LANE; k++) {
#pragma HLS UNROLL
                    T x1 = x_points_1[ci + k];
                    T x2 = x_points_2[ci + k];
                    T w1 = w_buffer[ci + k];
                    T w2 = w_buffer[ci + k + C_IN];

                    T val1 = fmul_dsp(x1, w1);
                    T val2 = fmul_dsp(x2, w2);

                    v_sum1[k][acc_idx] = fadd_lat4(v_sum1[k][acc_idx], val1);
                    v_sum2[k][acc_idx] = fadd_lat4(v_sum2[k][acc_idx], val2);
                }
                acc_idx = (acc_idx + 1) & ACC_MASK;
            }
        }

        T psum = (T)0;
RED_K:
        for (int k = 0; k < LANE; k++) {
#pragma HLS UNROLL
RED_D:
            for (int d = 0; d < ACC_DEPTH; d++) {
#pragma HLS UNROLL
                T tmp = fadd_lat4(v_sum1[k][d], v_sum2[k][d]);
                psum = fadd_lat4(psum, tmp);
            }
        }

        y_blk_out[t] = fadd_lat4(psum, b_buffer[co]);
    }
}

// ============================================================
// Main kernel
// ============================================================
template<int PEs, int C_IN, int C_OUT, int BATCH, typename T>
void Resblock___Pad_ref_Conv_11133111111_CNorm_Relu___(
    TensorMem<T> &X,
    TensorMem<T> &W_1, TensorMem<T> &B_1,
    TensorMem<T> &gamma_1, TensorMem<T> &beta_1,
    TensorMem<T> &W_2, TensorMem<T> &B_2,
    TensorMem<T> &gamma_2, TensorMem<T> &beta_2,
    T epsilon,
    TensorMem<T> &Y
) {
    Shape x_shape = X.shape;
    Shape w_shape = W_1.shape;
    Shape y_shape = Y.shape;

#ifndef __SYNTHESIS__
    if (3 != w_shape.H || 3 != w_shape.W)
        assert(0 && "Error: Unappropriate input sizes for Conv !!\n");
#endif

    const int width = PEs / y_shape.W;
    const int rolls = y_shape.H / width;

    const int num = C_OUT;
    const T adjustment_scale = (T)num / (T)(num - 1);
    const T pre_div = (T)1 / (T)my_sqrt_f((float)(num - 1));

    constexpr int vector_depth_size = PEs * C_IN;

    // =====================
    // TIMING-CLEAN CONFIG
    // =====================
    static const int LANE       = 4;
    static const int ACC_DEPTH = 4;
    static const int ACC_MASK  = 3;
    static const int C_OBLK    = 120;   // 960/120=8 blocks
    static const int PE_UNROLL = 4;     // <<< key change: reduce routing/uncertainty

#ifndef __SYNTHESIS__
    assert((C_OUT % C_OBLK) == 0);
    assert((C_OBLK % 2) == 0);
#endif

    // --- Buffers ---
    // [QUAN TRỌNG] FIX 1: Dùng 'static' để tránh tràn Stack (SIGSEGV) khi mô phỏng
    static T skip_buffer[C_OUT * 256];
    
    // [QUAN TRỌNG] FIX 2: Dùng 'impl=uram' để ép dùng UltraRAM trên chip ZCU104
    // giúp tránh tràn tài nguyên BRAM (Implementation Failed)
#pragma HLS BIND_STORAGE variable=skip_buffer type=ram_2p impl=uram

    // Các buffer nhỏ khác có thể để tự động hoặc ép vào BRAM
    static T x_buffer[3 * vector_depth_size];
#pragma HLS BIND_STORAGE variable=x_buffer type=ram_2p impl=bram

    static T w_buffer[C_IN << 1];
#pragma HLS BIND_STORAGE variable=w_buffer type=ram_2p impl=bram

    static T b_buffer[C_OUT];
    static T gamma_buffer[C_OUT];
    static T beta_buffer[C_OUT];

#pragma HLS ARRAY_PARTITION variable=x_buffer cyclic factor=LANE
#pragma HLS ARRAY_PARTITION variable=w_buffer cyclic factor=LANE

    static T y_blk[PEs][C_OBLK];
#pragma HLS ARRAY_PARTITION variable=y_blk complete dim=1
#pragma HLS ARRAY_PARTITION variable=y_blk cyclic factor=LANE dim=2
#pragma HLS BIND_STORAGE variable=y_blk type=ram_2p impl=bram

    static float mean_buf[PEs];
    static float var_buf[PEs];
#pragma HLS ARRAY_PARTITION variable=mean_buf complete dim=1
#pragma HLS ARRAY_PARTITION variable=var_buf  complete dim=1

Batch_loop:
    for (int n = 0; n < BATCH; n++) {
        int skip_buf_id = 0;

Res_block_loop:
        for (int times = 0; times < 2; times++) {

            T* raw_ifm_data = X.raw_at(n, 0, 0, 0);
            load_buffer<vector_depth_size>(&x_buffer[vector_depth_size], raw_ifm_data);

            if (!times) {
                load_buffer<vector_depth_size>(&skip_buffer[skip_buf_id], raw_ifm_data);
                skip_buf_id += vector_depth_size;

                load_buffer<C_OUT>(b_buffer,      B_1.raw());
                load_buffer<C_OUT>(gamma_buffer, gamma_1.raw());
                load_buffer<C_OUT>(beta_buffer,  beta_1.raw());
            } else {
                load_buffer<C_OUT>(b_buffer,      B_2.raw());
                load_buffer<C_OUT>(gamma_buffer, gamma_2.raw());
                load_buffer<C_OUT>(beta_buffer,  beta_2.raw());
            }

Slide_PEs_loop:
            for (int r = 0, load_id = 2; r < rolls; r++, load_id++) {
#pragma HLS LOOP_TRIPCOUNT min=16 max=16  // for 16x16
                if (load_id == 3) load_id = 0;

                if (r < rolls - 1) {
                    T* raw_ifm_data_inner = X.raw_at(n, r + width, 0, 0);
                    load_buffer<vector_depth_size>(&x_buffer[load_id * vector_depth_size], raw_ifm_data_inner);
                    if (!times) {
                        load_buffer<vector_depth_size>(&skip_buffer[skip_buf_id], raw_ifm_data_inner);
                        skip_buf_id += vector_depth_size;
                    }
                }

                // =========================================================
                // PASS A: mean/var
                // =========================================================
PE_PASSA:
#pragma HLS UNROLL factor=PE_UNROLL
                for (int pe = 0; pe < PEs; pe++) {
                    float sum   = 0.0f;
                    float sumsq = 0.0f;

CO0_PASSA:
                    for (int co0 = 0; co0 < C_OUT; co0 += C_OBLK) {
#pragma HLS LOOP_TRIPCOUNT min=8 max=8   // 960/120
                        conv_block_one_pe<PEs, C_IN, C_OUT, LANE, ACC_DEPTH, ACC_MASK, C_OBLK, T>(
                            x_shape, y_shape, r, width, pe,
                            x_buffer, w_buffer, b_buffer,
                            W_1, W_2, times,
                            co0,
                            y_blk[pe]
                        );

STAT_LOOP:
                        for (int t = 0; t < C_OBLK; t++) {
#pragma HLS PIPELINE II=1
                            float yv = (float)y_blk[pe][t];
                            sum += yv;
                            float yn = yv * (float)pre_div;
                            sumsq += yn * yn;
                        }
                    }

                    mean_buf[pe] = sum / (float)C_OUT;
                    var_buf[pe]  = sumsq;
                }

                // =========================================================
                // PASS B: conv + affine + relu/add + store
                // =========================================================
PE_PASSB:
#pragma HLS UNROLL factor=PE_UNROLL
                for (int pe = 0; pe < PEs; pe++) {
                    const int y_h = r * width;
                    const int y_w = pe;

                    const float mean_f = mean_buf[pe];
                    const float var_f  = var_buf[pe];

                    const float denom_f =
                        (var_f - mean_f * mean_f * (float)adjustment_scale + (float)epsilon);
                    const float invstd_f = 1.0f / my_sqrt_f(denom_f);

                    const T inv_std = (T)invstd_f;
                    const T mean_t  = (T)mean_f;

                    T* yptr = Y.raw_at(n, y_h, y_w, 0);
                    T* xptr = X.raw_at(n, y_h, y_w, 0);
                    const int skip_base = (y_h * y_shape.W + y_w) * C_OUT;

CO0_PASSB:
                    for (int co0 = 0; co0 < C_OUT; co0 += C_OBLK) {
#pragma HLS LOOP_TRIPCOUNT min=8 max=8
                        conv_block_one_pe<PEs, C_IN, C_OUT, LANE, ACC_DEPTH, ACC_MASK, C_OBLK, T>(
                            x_shape, y_shape, r, width, pe,
                            x_buffer, w_buffer, b_buffer,
                            W_1, W_2, times,
                            co0,
                            y_blk[pe]
                        );

AFFINE_STORE:
                        for (int t = 0; t < C_OBLK; t++) {
#pragma HLS PIPELINE II=1
                            const int co = co0 + t;

                            T weight = fmul_dsp(gamma_buffer[co], inv_std);
                            T mean_w = fmul_dsp(mean_t, weight);
                            T bias   = beta_buffer[co] - mean_w;

                            T pw  = fmul_dsp(y_blk[pe][t], weight);
                            T out = pw + bias;

                            if (times) {
                                out = out + skip_buffer[skip_base + co];
                                yptr[co] = out;
                            } else {
                                if (out < (T)0) out = (T)0;
                                xptr[co] = out;
                            }
                        }
                    }
                }
            }
        }
    }
}