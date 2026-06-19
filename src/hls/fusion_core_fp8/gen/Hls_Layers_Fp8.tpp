#pragma once
#include "Core.h"

#if !defined(__SYNTHESIS__)
  #include <cmath>
  #include <cstdio>
#endif
#if defined(__SYNTHESIS__)
  #include <hls_math.h>
#endif
#include <cstring>
#include <hls_stream.h>

// ============================================================
// fp8 (E4M3) conv-MAC benchmark engine.
//   Same architecture as [E] (producer||consumer dataflow, 16 PE spatial columns,
//   rotating depth-8 fp16 psum) BUT conv operands are fp8 packed 32/256-bit-word.
//   -> CI_W = 960/32 = 30 (half of fp16's 60) -> MAC trip halves -> ~2x throughput.
//   fp8 multiply is a NATIVE 4x4 mantissa mult (-> LUT, no DSP); accumulate stays fp16.
//   Norm / params / output kept fp16 (realistic: conv fp8, norm fp16). RB_L1 path only.
// ============================================================

#ifndef HLS_HALF_HELPERS_DEFINED
#define HLS_HALF_HELPERS_DEFINED
template <typename T>
static inline ap_uint<16> half_to_bits(T val) {
#pragma HLS INLINE
#if defined(__SYNTHESIS__)
    union { T f; uint16_t i; } u; u.f = val; return ap_uint<16>(u.i);
#else
    ap_uint<16> out; unsigned short temp; std::memcpy(&temp, &val, sizeof(unsigned short)); out = temp; return out;
#endif
}
template <typename T>
static inline T bits_to_half(ap_uint<16> bits) {
#pragma HLS INLINE
#if defined(__SYNTHESIS__)
    union { uint16_t i; T f; } u; u.i = bits.to_uint(); return u.f;
#else
    T out; unsigned short val = bits.to_uint(); std::memcpy(&out, &val, sizeof(unsigned short)); return out;
#endif
}
static inline int flat_idx(int n,int h,int w,int c,int H,int W,int C){
#pragma HLS INLINE
    return ((n*H+h)*W+w)*C+c;
}
static inline int w_flat_idx(int co,int kh,int kw,int ci,int C_STRIDE){
#pragma HLS INLINE
    return ((co*3+kh)*3+kw)*C_STRIDE+ci;
}
static inline float my_sqrt_f(float x){
#pragma HLS INLINE
#if defined(__SYNTHESIS__)
    return hls::sqrtf(x);
#else
    return std::sqrt(x);
#endif
}
#endif // HLS_HALF_HELPERS_DEFINED

// ---- fp8 E4M3 multiply -> fp16 (native: 4x4 mantissa mult, NOT decode-to-fp16-then-mul) ----
// E4M3 bit layout: [7]=sign, [6:3]=exp(bias 7), [2:0]=mantissa. Subnormals flushed to zero
// (resource-faithful; team owns real accuracy). Result assembled directly as fp16 bits.
static inline half fp8_mul_to_half(ap_uint<8> a, ap_uint<8> b) {
#pragma HLS INLINE
    ap_uint<1> sa=a[7], sb=b[7];
    ap_uint<4> ea=a.range(6,3), eb=b.range(6,3);
    ap_uint<3> ma=a.range(2,0), mb=b.range(2,0);
    bool az=(ea==0), bz=(eb==0);                       // flush subnormals/zero -> 0
    ap_uint<4> Ma = ap_uint<4>((ap_uint<4>(0x8)) | ma); // 1.mmm as 4-bit int (8..15)
    ap_uint<4> Mb = ap_uint<4>((ap_uint<4>(0x8)) | mb);
    ap_uint<8> prod = Ma * Mb;                          // THE 4x4 multiply (8-bit, 64..225)
    ap_uint<1> sr = sa ^ sb;
    ap_uint<16> bits;
    if (az || bz) {
        bits = ap_uint<16>(sr) << 15;                  // signed zero
    } else {
        // value = prod * 2^(ea+eb-20); normalize prod (MSB at bit7 or bit6)
        ap_uint<1> hi = prod[7];
        int lz = hi ? 7 : 6;
        int e16 = lz + (int)ea + (int)eb - 5;          // fp16 exp field (bias 15)
        if (e16 < 1)  e16 = 1;
        if (e16 > 30) e16 = 30;                         // clamp (avoid inf/nan)
        // mantissa: remove leading bit, align to 10-bit fp16 mantissa
        ap_uint<8> frac = hi ? ap_uint<8>(prod - 128) : ap_uint<8>(prod - 64);
        ap_uint<10> m16 = hi ? ap_uint<10>(frac << 3) : ap_uint<10>(frac << 4);
        bits = (ap_uint<16>(sr) << 15) | (ap_uint<16>(e16 & 0x1F) << 10) | ap_uint<16>(m16);
    }
    return bits_to_half<half>(bits);
}

// ---- LANES-lane fp8 dot: LANES fp8 (low LANES*8 bits of the 256-bit word) per PE per cycle,
//      fp16 balanced-tree accumulate. LANES=32 -> 2x throughput; LANES=16 -> iso-throughput vs fp16. ----
template<int LANES>
static inline half hdot_fp8(data_256_t xw, data_256_t ww) {
#pragma HLS INLINE
    half r[LANES];
#pragma HLS ARRAY_PARTITION variable=r complete
    for (int k=0; k<LANES; k++) {
#pragma HLS UNROLL
        r[k] = fp8_mul_to_half(xw.range(8*k+7, 8*k), ww.range(8*k+7, 8*k));
    }
    for (int stride=LANES/2; stride>=1; stride>>=1) {
        for (int k=0; k<stride; k++) {
#pragma HLS UNROLL
            r[k] = r[k] + r[k+stride];
        }
    }
    return r[0];
}

// FP8_LANES = fp8 elements MAC'd per PE per cycle. 32 = 2x throughput (CI_W=30),
// 16 = same throughput as fp16 16-wide (CI_W=60) -> isolates the DSP saving from mults leaving DSP.
#ifndef FP8_LANES
#define FP8_LANES 32
#endif
#define FP8_PACK FP8_LANES

template<int PEs, int LANES>
static void fp8_mac_producer(
    DDR_CONST_PTR W_ptr,
    data_256_t    x_buf[4][MODEL_W][60],
    int           CI_PAD,
    int           CI_W,
    int           r,
    hls::stream<ap_uint<PEs*128> >& pstrm)
{
#pragma HLS INLINE off
    constexpr int W = MODEL_W;
    const int FLAT_N = 960 * 9 * CI_W;   // LANES=32 -> CI_W=30 (half); LANES=16 -> CI_W=60 (iso)

    data_256_t w_buf[2][60];
#pragma HLS ARRAY_PARTITION variable=w_buf complete dim=1

    int w_init = w_flat_idx(0,0,0,0,CI_PAD) / FP8_PACK;
    for (int j=0; j<CI_W; j++) {
#pragma HLS PIPELINE II=1
        w_buf[0][j] = W_ptr[w_init + j];
    }

    half psum[PEs][8];
#pragma HLS ARRAY_PARTITION variable=psum complete dim=1
#pragma HLS ARRAY_PARTITION variable=psum complete dim=2

    int co=0, step=0, ciw=0;
    PASS_A_FLAT: for (int m=0; m<FLAT_N; m++) {
#pragma HLS PIPELINE II=1
#pragma HLS DEPENDENCE variable=psum type=inter false
#pragma HLS LOOP_TRIPCOUNT min=259200 max=518400
        int acc_idx = m & 7;                       // strict round-robin, distance 8 >= fp16 add lat
        int kh = step/3, kw = step%3;
        int ping = (co + step) & 1;
        int ns = step+1, wn_base=0; bool do_load=false;
        if (ns < 9)        { wn_base = w_flat_idx(co,   ns/3, ns%3, 0, CI_PAD)/FP8_PACK; do_load=true; }
        else if (co < 959) { wn_base = w_flat_idx(co+1, 0,    0,    0, CI_PAD)/FP8_PACK; do_load=true; }

        data_256_t w_reg = w_buf[ping][ciw];
        data_256_t xw_all[W];
#pragma HLS ARRAY_PARTITION variable=xw_all complete dim=1
        int slot = (r + kh) & 3;
        for (int b=0; b<W; b++) {
#pragma HLS UNROLL
            xw_all[b] = x_buf[slot][b][ciw];
        }
        for (int p=0; p<PEs; p++) {
#pragma HLS UNROLL
            int wx = p + (kw - 1);
            int wc = (wx < 0) ? -wx : ((wx >= W) ? (W<<1)-wx-2 : wx);
            half prod = hdot_fp8<LANES>(xw_all[wc], w_reg);
            if (step==0 && ciw<8) psum[p][acc_idx]  = prod;
            else                  psum[p][acc_idx] += prod;
        }
        if (do_load) w_buf[1-ping][ciw] = W_ptr[wn_base+ciw];

        if (step==8 && ciw==CI_W-1) {
            ap_uint<PEs*128> pw = 0;
            for (int p=0; p<PEs; p++) {
#pragma HLS UNROLL
                for (int l=0; l<8; l++) {
#pragma HLS UNROLL
                    int o = p*8 + l;
                    pw.range(o*16+15, o*16) = half_to_bits(psum[p][l]);
                }
            }
            pstrm.write(pw);
        }
        if (ciw==CI_W-1) { ciw=0; if (step==8){step=0; co++;} else step++; }
        else ciw++;
    }
}

template<int PEs>
static void fp8_reduce_consumer(
    hls::stream<ap_uint<PEs*128> >& pstrm,
    data_256_t b_buf[60],
    data_256_t y_cache[960],
    float      sum_acc[PEs],
    float      sumsq_acc[PEs])
{
#pragma HLS INLINE off
    for (int p=0; p<PEs; p++) { sum_acc[p]=0; sumsq_acc[p]=0; }
    half ycol[PEs];
#pragma HLS ARRAY_PARTITION variable=ycol complete dim=1
    ap_uint<PEs*128> pw = 0;
    const int NTOT = 960 * PEs;
    int co=0, p=0;
    CONS: for (int idx=0; idx<NTOT; idx++) {
#pragma HLS PIPELINE II=1
#pragma HLS DEPENDENCE variable=sum_acc type=inter false
#pragma HLS DEPENDENCE variable=sumsq_acc type=inter false
        if (p==0) pw = pstrm.read();
        half bias = bits_to_half<half>(b_buf[co/16].range(16*(co%16)+15, 16*(co%16)));
        half tot=0;
        for (int l=0; l<8; l++) {
            int o = p*8 + l;
            tot += bits_to_half<half>(pw.range(o*16+15, o*16));
        }
        half fout = tot + bias;
        ycol[p] = fout;
        float ff = (float)fout; sum_acc[p] += ff; sumsq_acc[p] += ff*ff;
        if (p==PEs-1) {
            data_256_t yword = 0;
            for (int q=0; q<PEs; q++) {
#pragma HLS UNROLL
                yword.range(16*q+15, 16*q) = half_to_bits(ycol[q]);
            }
            y_cache[co] = yword;
            p=0; co++;
        } else p++;
    }
}

// ---- one output row: MAC producer || reduce/stats consumer.
//      DATAFLOW must be on a FUNCTION BODY (or for-loop body) -- a bare { } scope
//      is silently ignored (WARNING [HLS 207-5575]) -> processes serialize. ----
template<int PEs, int LANES>
static void fp8_conv_row(
    DDR_CONST_PTR W_ptr,
    data_256_t    x_buf[4][MODEL_W][60],
    data_256_t    b_buf[60],
    data_256_t    y_cache[960],
    float         sum_acc[PEs],
    float         sumsq_acc[PEs],
    int CI_PAD, int CI_W, int r)
{
#pragma HLS INLINE off
#pragma HLS DATAFLOW
    hls::stream<ap_uint<PEs*128> > pstrm;
#pragma HLS STREAM variable=pstrm depth=4
#pragma HLS BIND_STORAGE variable=pstrm type=fifo impl=srl
    fp8_mac_producer<PEs, LANES>(W_ptr, x_buf, CI_PAD, CI_W, r, pstrm);
    fp8_reduce_consumer<PEs>(pstrm, b_buf, y_cache, sum_acc, sumsq_acc);
}

template<int PEs, int LANES>
void Fp8_Conv_Engine(
    DDR_CONST_PTR X_ptr,    // fp8-packed activations (LANES/word)
    DDR_CONST_PTR W_ptr,    // fp8-packed weights
    DDR_CONST_PTR B_ptr,
    DDR_CONST_PTR G_ptr,
    DDR_CONST_PTR BE_ptr,
    DDR_PTR       Out_ptr,
    half          epsilon)
{
#pragma HLS INLINE off
    constexpr int H = MODEL_H;
    constexpr int W = MODEL_W;
    const int CI_PAD = 960;
    const int CI_W   = CI_PAD / LANES;             // LANES=32 -> 30; LANES=16 -> 60
    const int num    = 960;
    const float pre_div    = 1.0f / my_sqrt_f((float)(num-1));
    const float pre_div_sq = pre_div * pre_div;

#if defined(__SYNTHESIS__)
    static data_256_t x_buf[4][W][60];
#else
    data_256_t x_buf[4][W][60];
#endif
#pragma HLS BIND_STORAGE variable=x_buf type=ram_t2p impl=bram
#pragma HLS ARRAY_PARTITION variable=x_buf complete dim=2

    data_256_t g_buf[60], be_buf[60], b_buf[60];
    data_256_t y_cache[960];
#pragma HLS BIND_STORAGE variable=y_cache type=ram_2p impl=bram
#pragma HLS ARRAY_PARTITION variable=y_cache cyclic factor=16 dim=1

    LOAD_PARAMS: for (int i=0; i<60; i++) {
#pragma HLS PIPELINE II=1
        g_buf[i]=G_ptr[i]; be_buf[i]=BE_ptr[i]; b_buf[i]=B_ptr[i];
    }

    Slide_PEs: for (int r=0; r<H; r++) {
#if !defined(__SYNTHESIS__)
        printf("  [fp8 RB] Row %d/%d\n", r+1, H); fflush(stdout);
#endif
        float sum_acc[PEs], sumsq_acc[PEs];
#pragma HLS ARRAY_PARTITION variable=sum_acc complete dim=0
#pragma HLS ARRAY_PARTITION variable=sumsq_acc complete dim=0

        // STEP 1: load window rows [r-1 .. r+1] (fp8, 30 words/pixel)
        int lc = (r==0) ? 3 : 1;
        for (int i=0; i<lc; i++) {
            int h = (r==0) ? (i-1) : (r+1);
            int h_c = (h<0) ? -h : ((h>=H) ? (H<<1)-h-2 : h);
            int slot = (h+1) & 3;
            for (int w=0; w<W; w++) {
                int ddr_base = flat_idx(0, h_c, w, 0, H, W, MODEL_C) / FP8_PACK;
                RB_LOAD: for (int j=0; j<CI_W; j++) {
#pragma HLS PIPELINE II=1
                    x_buf[slot][w][j] = X_ptr[ddr_base + j];
                }
            }
        }

        // STEP 2: conv (fp8 MAC producer || fp16 reduce/stats consumer)
        //         DATAFLOW now on a function body -> producer || consumer overlap.
        fp8_conv_row<PEs, LANES>(W_ptr, x_buf, b_buf, y_cache, sum_acc, sumsq_acc, CI_PAD, CI_W, r);

        // STEP 3: norm (fp16) + ReLU + write fp16 output
        PASS_B: for (int p=0; p<PEs; p++) {
            float mean_f = sum_acc[p] / 960.0f;
            float var_f  = sumsq_acc[p] * pre_div_sq;
            float denom  = var_f - mean_f*mean_f*(960.0f/959.0f) + (float)epsilon;
            half inv = (half)(1.0f / my_sqrt_f(denom));
            int out_px_base = (r*W + p) * 60;
            WRITE_OFM: for (int j=0; j<60; j++) {
#pragma HLS PIPELINE II=1
                data_256_t out_w=0;
                for (int k=0; k<16; k++) {
#pragma HLS UNROLL
                    half val=bits_to_half<half>(y_cache[j*16+k].range(16*p+15,16*p)),
                         g=bits_to_half<half>(g_buf[j].range(16*k+15,16*k)),
                         be=bits_to_half<half>(be_buf[j].range(16*k+15,16*k));
                    half norm=(val-(half)mean_f)*inv*g + be;
                    half act = (norm<(half)0)?(half)0:norm;
                    out_w.range(16*k+15,16*k) = half_to_bits(act);
                }
                Out_ptr[out_px_base+j] = out_w;
            }
        }
    }
}
