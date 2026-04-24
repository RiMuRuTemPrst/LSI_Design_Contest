#pragma once
#include "Core.h"

#if !defined(__SYNTHESIS__)
  #include <cassert>
  #include <cmath>
  #include <cstdio>
#endif
#if defined(__SYNTHESIS__)
  #include <hls_math.h>
#endif
#include <cstring>

// ============================================================
// SHARED HELPER FUNCTIONS
// ============================================================

template <typename T>
static inline ap_uint<16> half_to_bits(T val) {
#pragma HLS INLINE
#if defined(__SYNTHESIS__)
    union { T f; uint16_t i; } u;
    u.f = val;
    return ap_uint<16>(u.i);
#else
    ap_uint<16> out;
    unsigned short temp;
    std::memcpy(&temp, &val, sizeof(unsigned short));
    out = temp;
    return out;
#endif
}

template <typename T>
static inline T bits_to_half(ap_uint<16> bits) {
#pragma HLS INLINE
#if defined(__SYNTHESIS__)
    union { uint16_t i; T f; } u;
    u.i = bits.to_uint();
    return u.f;
#else
    T out;
    unsigned short val = bits.to_uint();
    std::memcpy(&out, &val, sizeof(unsigned short));
    return out;
#endif
}

static inline int flat_idx(int n, int h, int w, int c, int H, int W, int C) {
#pragma HLS INLINE
    return ((n * H + h) * W + w) * C + c;
}

static inline int w_flat_idx(int co, int kh, int kw, int ci, int C_STRIDE) {
#pragma HLS INLINE
    return ((co * 3 + kh) * 3 + kw) * C_STRIDE + ci;
}

static inline float my_sqrt_f(float x) {
#pragma HLS INLINE
#if defined(__SYNTHESIS__)
    return hls::sqrtf(x);
#else
    return std::sqrt(x);
#endif
}

// ============================================================
// UNIVERSAL ENGINE KERNEL (Unified Architecture)
// ============================================================

template<int PEs, typename T>
static void Universal_Engine_Kernel(
    DDR_CONST_PTR X_ptr,
    DDR_CONST_PTR W_ptr,
    DDR_CONST_PTR B_ptr,
    DDR_CONST_PTR G_ptr,
    DDR_CONST_PTR BE_ptr,
    DDR_CONST_PTR G_IN_ptr,   // Only for MODE_CBI
    DDR_CONST_PTR BE_IN_ptr,  // Only for MODE_CBI
    DDR_PTR       Out_ptr,
    T             epsilon,
    int           mode        // 0=CBI, 1=RB_L1, 2=RB_L2
) {
#pragma HLS INLINE off
    constexpr int H = MODEL_H;
    constexpr int W = MODEL_W;

    const bool is_cbi = (mode == 0);
    const bool is_l1  = (mode == 1);
    const bool is_l2  = (mode == 2);

    const int CI_PAD = is_cbi ? 224 : 960;
    const int CI_W   = CI_PAD / 16;
    const int num    = 960;
    const float pre_div    = 1.0f / my_sqrt_f((float)(num - 1));
    const float pre_div_sq = pre_div * pre_div;

    // --- Static buffers for resource sharing ---
    #if defined(__SYNTHESIS__)
    static data_256_t x_buf[4][W][60];
    #else
    data_256_t x_buf[4][W][60];
    #endif
    #pragma HLS BIND_STORAGE variable=x_buf type=ram_2p impl=bram
    #pragma HLS ARRAY_PARTITION variable=x_buf complete dim=2

    #if defined(__SYNTHESIS__)
    static data_256_t skip_buf[H * W * 60];
    #else
    static data_256_t* skip_buf_ptr = new data_256_t[H * W * 60];
    data_256_t* skip_buf = skip_buf_ptr;
    #endif
    #pragma HLS BIND_STORAGE variable=skip_buf type=ram_2p impl=uram

    // --- Local buffers ---
    data_256_t g_buf[60], be_buf[60], b_buf[60];
    data_256_t g_in_buf[14], be_in_buf[14];
    data_256_t w_buf[2][60];
#pragma HLS ARRAY_PARTITION variable=w_buf complete dim=1

    T y_cache[PEs][960];
#pragma HLS BIND_STORAGE variable=y_cache type=ram_2p impl=bram
#pragma HLS ARRAY_PARTITION variable=y_cache cyclic factor=16 dim=2

    // --- Load Params ---
    LOAD_PARAMS: for (int i = 0; i < 60; i++) {
#pragma HLS PIPELINE II=1
        g_buf[i] = G_ptr[i]; be_buf[i] = BE_ptr[i]; b_buf[i] = B_ptr[i];
    }
    if (is_cbi) {
        LOAD_CBI_PARAMS: for (int i = 0; i < 14; i++) {
#pragma HLS PIPELINE II=1
            g_in_buf[i] = G_IN_ptr[i]; be_in_buf[i] = BE_IN_ptr[i];
        }
    }

    // MAIN LOOP
    Slide_PEs:
    for (int r = 0; r < H; r++) {
#if !defined(__SYNTHESIS__)
        printf("  [Mode %d] Row %d/%d\n", mode, r+1, H); fflush(stdout);
#endif

        float sum_acc[PEs], sumsq_acc[PEs];
#pragma HLS ARRAY_PARTITION variable=sum_acc complete
#pragma HLS ARRAY_PARTITION variable=sumsq_acc complete
        for (int pe=0; pe<PEs; pe++) { sum_acc[pe]=0; sumsq_acc[pe]=0; }

        // STEP 1: Load (Optimized for II=1)
        int lc = (r == 0) ? 3 : 1;
        for (int i = 0; i < lc; i++) {
            int h = (r == 0) ? (i - 1) : (r + 1);
            int h_c = (h < 0) ? -h : ((h >= H) ? (H<<1)-h-2 : h);
            int slot = (h + 1) & 3;
            for (int w = 0; w < W; w++) {
                int ddr_base = flat_idx(0, h_c, w, 0, H, W, CI_PAD) / 16;
                if (is_cbi) {
                    T partial_sum[14], partial_sumsq[14];
                    data_256_t pix[14];
#pragma HLS ARRAY_PARTITION variable=partial_sum complete
#pragma HLS ARRAY_PARTITION variable=partial_sumsq complete

                    CBI_STATS: for(int j=0; j<14; j++) {
#pragma HLS PIPELINE II=1
                        data_256_t word = X_ptr[ddr_base + j]; pix[j] = word;
                        T s_local = 0, sq_local = 0;
                        for(int k=0; k<16; k++) {
#pragma HLS UNROLL
                            if (j*16+k < 220) {
                                T val = bits_to_half<T>(word.range(16*k+15, 16*k));
                                s_local += val; sq_local += val*val;
                            }
                        }
                        partial_sum[j] = s_local; partial_sumsq[j] = sq_local;
                    }
                    float sum=0, sumsq=0;
                    for(int j=0; j<14; j++) { sum += (float)partial_sum[j]; sumsq += (float)partial_sumsq[j]; }
                    float mean = sum / 220.0f;
                    float var = sumsq / 219.0f - mean*mean * 220.0f/219.0f;
                    T inv_std = (T)(1.0f / my_sqrt_f(var + (float)epsilon));

                    CBI_NORM: for(int j=0; j<14; j++) {
#pragma HLS PIPELINE II=1
                        data_256_t out_w = 0;
                        for(int k=0; k<16; k++) {
#pragma HLS UNROLL
                            if (j*16+k < 220) {
                                T vv = bits_to_half<T>(pix[j].range(16*k+15, 16*k));
                                T g = bits_to_half<T>(g_in_buf[j].range(16*k+15, 16*k));
                                T be = bits_to_half<T>(be_in_buf[j].range(16*k+15, 16*k));
                                out_w.range(16*k+15, 16*k) = half_to_bits((vv-(T)mean)*inv_std*g + be);
                            }
                        }
                        x_buf[slot][w][j] = out_w;
                    }
                } else {
                    RB_LOAD: for(int j=0; j<60; j++) {
#pragma HLS PIPELINE II=1
                        data_256_t word = X_ptr[ddr_base + j];
                        x_buf[slot][w][j] = word;
                        if (is_l1 && h >= 0 && h < H) skip_buf[((h*W)+w)*60 + j] = word;
                    }
                }
            }
        }

        // STEP 2: Conv (Shared MACs)
        int ping = 0;
        int w_init = w_flat_idx(0, 0, 0, 0, CI_PAD) / 16;
        for(int j=0; j<CI_W; j++) {
#pragma HLS PIPELINE II=1
            w_buf[0][j] = W_ptr[w_init + j];
        }

        PASS_A: for (int co = 0; co < 960; co++) {
            T psum[PEs][8];
#pragma HLS ARRAY_PARTITION variable=psum complete dim=0
            for(int p=0; p<PEs; p++) for(int l=0; l<8; l++) psum[p][l] = 0;

            CONV_CORE: for (int step = 0; step < 9; step++) {
                int kh=step/3, kw=step%3, ns=step+1, nkh=ns/3, nkw=ns%3;
                int slot = (r + kh) & 3;
                int wn_base; bool do_load = false;
                if (ns < 9) { wn_base = w_flat_idx(co, nkh, nkw, 0, CI_PAD)/16; do_load=true; }
                else if (co < 959) { wn_base = w_flat_idx(co+1, 0, 0, 0, CI_PAD)/16; do_load=true; }

                data_256_t w_reg; data_256_t x_regs[PEs];
#pragma HLS ARRAY_PARTITION variable=x_regs complete

                MAC_LOOP: for (int ci = 0; ci < CI_PAD; ci += 4) {
#pragma HLS PIPELINE II=1
#pragma HLS DEPENDENCE variable=psum type=inter false
                    int acc_idx=(ci/4)&7, widx=ci/16, boff=ci%16;
                    if (boff==0) w_reg=w_buf[ping][widx]; else w_reg>>=64;
                    T w0=bits_to_half<T>(w_reg.range(15,0)), w1=bits_to_half<T>(w_reg.range(31,16)), w2=bits_to_half<T>(w_reg.range(47,32)), w3=bits_to_half<T>(w_reg.range(63,48));
                    for (int p=0; p<PEs; p++) {
#pragma HLS UNROLL
                        int wx = p + (kw - 1);
                        int wc = (wx < 0) ? -wx : ((wx >= W) ? (W<<1)-wx-2 : wx);
                        if (boff==0) x_regs[p]=x_buf[slot][wc][widx]; else x_regs[p]>>=64;
                        T x0=bits_to_half<T>(x_regs[p].range(15,0)), x1=bits_to_half<T>(x_regs[p].range(31,16)), x2=bits_to_half<T>(x_regs[p].range(47,32)), x3=bits_to_half<T>(x_regs[p].range(63,48));
                        psum[p][acc_idx] += (x0*w0 + x1*w1) + (x2*w2 + x3*w3);
                    }
                    if (do_load && boff==0) w_buf[1-ping][widx] = W_ptr[wn_base+widx];
                }
                ping ^= 1;
            }
            T bias = bits_to_half<T>(b_buf[co/16].range(16*(co%16)+15, 16*(co%16)));
            STATS_ACC: for (int p=0; p<PEs; p++) {
#pragma HLS PIPELINE II=1
                T tot=0; for(int l=0; l<8; l++) tot += psum[p][l];
                T fout = tot + bias; y_cache[p][co] = fout;
                float ff = (float)fout; sum_acc[p] += ff; sumsq_acc[p] += ff*ff;
            }
        }

        // STEP 3: Out
        PASS_B: for (int p=0; p<PEs; p++) {
            float mean_f = sum_acc[p] / 960.0f;
            float var_f  = sumsq_acc[p] * pre_div_sq;
            float denom  = var_f - mean_f * mean_f * (960.0f/959.0f) + (float)epsilon;
            T inv = (T)(1.0f / my_sqrt_f(denom));
            int out_px_base = (r * W + p) * 60;
            WRITE_OFM: for (int j=0; j<60; j++) {
#pragma HLS PIPELINE II=1
                data_256_t out_w=0, sk_w=(is_l2)?skip_buf[out_px_base+j]:(data_256_t)0;
                for (int k = 0; k < 16; k++) {
#pragma HLS UNROLL
                    T val=y_cache[p][j*16+k], g=bits_to_half<T>(g_buf[j].range(16*k+15,16*k)), be=bits_to_half<T>(be_buf[j].range(16*k+15,16*k));
                    T norm = (val-(T)mean_f)*inv*g + be;
                    T act = is_cbi ? norm : (is_l1 ? ((norm<(T)0)?(T)0:norm) : (norm+bits_to_half<T>(sk_w.range(16*k+15,16*k))));
                    out_w.range(16*k+15,16*k) = half_to_bits(act);
                }
                Out_ptr[out_px_base+j] = out_w;
            }
        }
    }
}
