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
#include <hls_stream.h>

// ============================================================
// SHARED HELPER FUNCTIONS
// ============================================================

#ifndef HLS_HALF_HELPERS_DEFINED
#define HLS_HALF_HELPERS_DEFINED

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

#endif // HLS_HALF_HELPERS_DEFINED

// ============================================================
// [A]+[B] FUSION STEP-2 DATAFLOW: MAC producer || reduce/stats consumer
//   Producer = flat co*step*ci MAC (II=1, fill paid once/row). At each co boundary
//   it streams the raw 8-slot psum for all PEs (PEs*8 fp16 packed).
//   Consumer reduces 8->1 + bias + LayerNorm stats, PIPELINED OVER p (not unrolled),
//   so the reduce reuses ONE 7-adder tree instead of 16x7 in-pipeline adders
//   (kills ~18K LUT). The consumer co-loop is not II-critical (it runs ~20x faster
//   than the producer) so the reduce latency is free. Reduce + stats order is
//   identical to the canonical (sequential 8-slot reduce; sum over co per PE) ->
//   BIT-IDENTICAL output.
// ============================================================

// [E] 16-wide dot: one full 256-bit word (16 ch) per PE per cycle, balanced tree.
template <typename T>
static inline T hdot16(data_256_t xw, data_256_t ww) {
#pragma HLS INLINE
    T x0=bits_to_half<T>(xw.range(15,0)),    x1=bits_to_half<T>(xw.range(31,16)),
      x2=bits_to_half<T>(xw.range(47,32)),   x3=bits_to_half<T>(xw.range(63,48)),
      x4=bits_to_half<T>(xw.range(79,64)),   x5=bits_to_half<T>(xw.range(95,80)),
      x6=bits_to_half<T>(xw.range(111,96)),  x7=bits_to_half<T>(xw.range(127,112)),
      x8=bits_to_half<T>(xw.range(143,128)), x9=bits_to_half<T>(xw.range(159,144)),
      xA=bits_to_half<T>(xw.range(175,160)), xB=bits_to_half<T>(xw.range(191,176)),
      xC=bits_to_half<T>(xw.range(207,192)), xD=bits_to_half<T>(xw.range(223,208)),
      xE=bits_to_half<T>(xw.range(239,224)), xF=bits_to_half<T>(xw.range(255,240));
    T w0=bits_to_half<T>(ww.range(15,0)),    w1=bits_to_half<T>(ww.range(31,16)),
      w2=bits_to_half<T>(ww.range(47,32)),   w3=bits_to_half<T>(ww.range(63,48)),
      w4=bits_to_half<T>(ww.range(79,64)),   w5=bits_to_half<T>(ww.range(95,80)),
      w6=bits_to_half<T>(ww.range(111,96)),  w7=bits_to_half<T>(ww.range(127,112)),
      w8=bits_to_half<T>(ww.range(143,128)), w9=bits_to_half<T>(ww.range(159,144)),
      wA=bits_to_half<T>(ww.range(175,160)), wB=bits_to_half<T>(ww.range(191,176)),
      wC=bits_to_half<T>(ww.range(207,192)), wD=bits_to_half<T>(ww.range(223,208)),
      wE=bits_to_half<T>(ww.range(239,224)), wF=bits_to_half<T>(ww.range(255,240));
#if RPP > 1
    // [RP] LUT fit: pin 3 fp16 lanes' mult to DSP (config_op hmul=fabric defaults the rest to LUT).
    // No fp32 convert -> 0 extra LUT, pure DSP<-LUT shift of 3*PEs*RPP mults. BIT-EXACT to [E].
    T m0=x0*w0;
#pragma HLS BIND_OP variable=m0 op=hmul impl=maxdsp
    T m1=x1*w1;
#pragma HLS BIND_OP variable=m1 op=hmul impl=maxdsp
    T m2=x2*w2;
#pragma HLS BIND_OP variable=m2 op=hmul impl=maxdsp
    return ( ((m0+m1) + (m2+x3*w3)) + ((x4*w4+x5*w5) + (x6*w6+x7*w7)) )
         + ( ((x8*w8+x9*w9) + (xA*wA+xB*wB)) + ((xC*wC+xD*wD) + (xE*wE+xF*wF)) );
#else
    return ( ((x0*w0+x1*w1) + (x2*w2+x3*w3)) + ((x4*w4+x5*w5) + (x6*w6+x7*w7)) )
         + ( ((x8*w8+x9*w9) + (xA*wA+xB*wB)) + ((xC*wC+xD*wD) + (xE*wE+xF*wF)) );
#endif
}

// [RP] RPP = output rows computed per engine pass. RPP=1 -> [E] 16-wide (bit-identical).
//      RPP=2 -> row-parallel: 2 output rows share ONE weight broadcast (reuse 16->32),
//      weight DDR unchanged. Set via -DRPP=2 at synth/csim.
#ifndef RPP
#define RPP 1
#endif

template<int PEs, typename T>
static void uek_mac_producer(
    DDR_CONST_PTR W_ptr,
    data_256_t    x_buf[4][MODEL_W][60],
    int           CI_PAD,
    int           CI_W,
    int           r,
    hls::stream<ap_uint<PEs*128*RPP> >& pstrm)
{
#pragma HLS INLINE off
    constexpr int W = MODEL_W;
    const int FLAT_N = 960 * 9 * CI_W;   // [E] 16 ch/iter -> trip halves vs 8-wide

    data_256_t w_buf[2][60];
#pragma HLS ARRAY_PARTITION variable=w_buf complete dim=1

    int w_init = w_flat_idx(0, 0, 0, 0, CI_PAD) / 16;
    for (int j = 0; j < CI_W; j++) {
#pragma HLS PIPELINE II=1
        w_buf[0][j] = W_ptr[w_init + j];
    }

    T psum[2][RPP][PEs][8];
#pragma HLS ARRAY_PARTITION variable=psum complete dim=1
#pragma HLS ARRAY_PARTITION variable=psum complete dim=2
#pragma HLS ARRAY_PARTITION variable=psum complete dim=3
#pragma HLS ARRAY_PARTITION variable=psum complete dim=4

    int co=0, step=0, ciw=0;
    PASS_A_FLAT: for (int m=0; m<FLAT_N; m++) {
#pragma HLS PIPELINE II=1
#pragma HLS DEPENDENCE variable=psum type=inter false
#pragma HLS LOOP_TRIPCOUNT min=120960 max=518400
        // [E] acc_idx = m&7 (NOT ciw&7): CI_W=60 is not a multiple of 8, so ciw&7 would
        // revisit a slot only 4 iters after a panel crossing (< hadd latency) and break
        // the DEPENDENCE-false proof. m&7 is a strict round-robin -> every slot revisit
        // distance is EXACTLY 8 >= fp16 add latency, incl. across panels. The slot<->
        // channel-group mapping phase now rotates per co, but the co-boundary pack reads
        // ALL 8 slots, so the full sum is unchanged.
        int acc_idx = m & 7;
        int bank = co & 1;
        int kh = step/3, kw = step%3;
        int ping = (co + step) & 1;                  // (co*9+step)&1 == (co+step)&1 (9 odd)
        int ns = step+1, wn_base=0; bool do_load=false;
        if (ns < 9)        { wn_base = w_flat_idx(co,   ns/3, ns%3, 0, CI_PAD)/16; do_load=true; }
        else if (co < 959) { wn_base = w_flat_idx(co+1, 0,    0,    0, CI_PAD)/16; do_load=true; }

        data_256_t w_reg = w_buf[ping][ciw];         // full word every cycle, SHARED by all RPP rows
        // [E] bank-clean x read: each of the 16 banks is read once per row-window per cycle.
        // [RP] RPP windows: row rr reads slot (r+rr+kh)&3 (adjacent slots) -> needs t2p x_buf
        // for RPP reads/bank/cycle. PEs then select their column from registers (3:1 mux).
        data_256_t xw_all[RPP][W];
#pragma HLS ARRAY_PARTITION variable=xw_all complete dim=1
#pragma HLS ARRAY_PARTITION variable=xw_all complete dim=2
        for (int rr=0; rr<RPP; rr++) {
#pragma HLS UNROLL
            int slot = (r + rr + kh) & 3;
            for (int b=0; b<W; b++) {
#pragma HLS UNROLL
                xw_all[rr][b] = x_buf[slot][b][ciw];
            }
        }
        for (int rr=0; rr<RPP; rr++) {
#pragma HLS UNROLL
            for (int p=0; p<PEs; p++) {
#pragma HLS UNROLL
                int wx = p + (kw - 1);
                int wc = (wx < 0) ? -wx : ((wx >= W) ? (W<<1)-wx-2 : wx);
                T prod = hdot16<T>(xw_all[rr][wc], w_reg);   // SAME w_reg both rows
                if (step==0 && ciw<8) psum[bank][rr][p][acc_idx]  = prod; // first touch == reset+add
                else                  psum[bank][rr][p][acc_idx] += prod;
            }
        }
        // [E] next-panel preload now 1 word/cycle (panel = CI_W words consumed in CI_W iters)
        if (do_load) w_buf[1-ping][ciw] = W_ptr[wn_base+ciw];

        if (step==8 && ciw==CI_W-1) {                               // co boundary -> stream raw psum
            ap_uint<PEs*128*RPP> pw = 0;
            for (int rr=0; rr<RPP; rr++) {
#pragma HLS UNROLL
                for (int p=0; p<PEs; p++) {
#pragma HLS UNROLL
                    for (int l=0; l<8; l++) {
#pragma HLS UNROLL
                        int o = rr*PEs*8 + p*8 + l;
                        pw.range(o*16+15, o*16) = half_to_bits(psum[bank][rr][p][l]);
                    }
                }
            }
            pstrm.write(pw);
        }

        if (ciw==CI_W-1) { ciw=0; if (step==8){step=0; co++;} else step++; }
        else ciw++;
    }
}

template<int PEs, typename T>
static void uek_reduce_consumer(
    hls::stream<ap_uint<PEs*128*RPP> >& pstrm,
    data_256_t b_buf[60],
    data_256_t y_cache[RPP*960],
    float      sum_acc[RPP][PEs],
    float      sumsq_acc[RPP][PEs])
{
#pragma HLS INLINE off
    for (int rr=0; rr<RPP; rr++)
        for (int p=0; p<PEs; p++) { sum_acc[rr][p]=0; sumsq_acc[rr][p]=0; }

    T ycol[RPP][PEs];
#pragma HLS ARRAY_PARTITION variable=ycol complete dim=1
#pragma HLS ARRAY_PARTITION variable=ycol complete dim=2
    ap_uint<PEs*128*RPP> pw = 0;
    const int NTOT = 960 * PEs;
    int co=0, p=0;
    // flattened (co x p) at II=1: 1 reused 7-adder reduce tree PER row, ~PEs*960 cy total (vs
    // per-co REDP fill). read 1 wide token per co (at p==0), hold across p; pack+write y_cache
    // at p==PEs-1. ycol[rr][p] distinct index per iter; sum_acc[rr][p] revisited every PEs iters
    // (distance PEs >= fp32 add latency) -> II=1 valid. [RP] rr unrolled -> 1 reduce tree/row.
    CONS: for (int idx=0; idx<NTOT; idx++) {
#pragma HLS PIPELINE II=1
#pragma HLS DEPENDENCE variable=sum_acc type=inter false
#pragma HLS DEPENDENCE variable=sumsq_acc type=inter false
        if (p==0) pw = pstrm.read();
        T bias = bits_to_half<T>(b_buf[co/16].range(16*(co%16)+15, 16*(co%16)));
        for (int rr=0; rr<RPP; rr++) {
#pragma HLS UNROLL
            T tot=0;
            for (int l=0; l<8; l++) {                             // same sequential order as orig
                int o = rr*PEs*8 + p*8 + l;
                tot += bits_to_half<T>(pw.range(o*16+15, o*16));
            }
            T fout = tot + bias;
            ycol[rr][p] = fout;
            float ff = (float)fout; sum_acc[rr][p] += ff; sumsq_acc[rr][p] += ff*ff;
        }
        if (p==PEs-1) {
            for (int rr=0; rr<RPP; rr++) {
#pragma HLS UNROLL
                data_256_t yword = 0;
                for (int q=0; q<PEs; q++) {
#pragma HLS UNROLL
                    yword.range(16*q+15, 16*q) = half_to_bits(ycol[rr][q]);
                }
                y_cache[rr*960 + co] = yword;
            }
            p=0; co++;
        } else p++;
    }
}

// ============================================================
// GLOBAL ADD KERNEL
// ============================================================

template<int PEs>
void GlobalAdd_Kernel(
    DDR_CONST_PTR X,
    data_256_t*   global_buf,
    DDR_PTR       Y,
    int           total_vecs
) {
#pragma HLS INLINE off
    for (int i = 0; i < total_vecs; i++) {
#pragma HLS PIPELINE II=1
        data_256_t x_vec = X[i];
        data_256_t g_vec = global_buf[i];
        data_256_t out_vec;

        for (int k = 0; k < 16; k++) {
#pragma HLS UNROLL
            half x_val = bits_to_half<half>(x_vec.range(16*k+15, 16*k));
            half g_val = bits_to_half<half>(g_vec.range(16*k+15, 16*k));
            half res = x_val + g_val;
            out_vec.range(16*k+15, 16*k) = half_to_bits(res);
        }
        Y[i] = out_vec;
    }
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
    data_256_t*   skip_buf,   // Persistent ResBlock buffer
    data_256_t*   global_buf, // Persistent Global Skip buffer
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
    #pragma HLS BIND_STORAGE variable=x_buf type=ram_t2p impl=bram
    #pragma HLS ARRAY_PARTITION variable=x_buf complete dim=2

    // --- Local buffers ---
    data_256_t g_buf[60], be_buf[60], b_buf[60];
    data_256_t g_in_buf[14], be_in_buf[14];
    // [A] y_cache PACKED: 16 PE lanes per 256-bit word, indexed by channel co.
    //     co-boundary emits ONE wide word (16 PE results bit-packed -> 1 mem write) instead
    //     of 16 parallel writes -> avoids per-PE complete dim=1 partition, which exploded to
    //     16(PE)*16(cyclic)=256 banks = +240 BRAM. cyclic factor=16 keeps 16 banks (= canonical)
    //     so WRITE_OFM's 16-consecutive-channel read stays conflict-free.
    // [RP] flat RPP*960: rows SHARE the 16 cyclic banks at different depths (120 deep for RPP=2)
    //      instead of 2x16=32 shallow banks -> BRAM width-bound, deeper costs nothing (256->128).
    data_256_t y_cache[RPP*960];
#pragma HLS BIND_STORAGE variable=y_cache type=ram_2p impl=bram
#pragma HLS ARRAY_PARTITION variable=y_cache cyclic factor=16 dim=1

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
    for (int r = 0; r < H; r += RPP) {
#if !defined(__SYNTHESIS__)
        printf("  [Mode %d] Rows %d..%d/%d\n", mode, r+1, r+RPP, H); fflush(stdout);
#endif

        float sum_acc[RPP][PEs], sumsq_acc[RPP][PEs];   // init + accumulate inside uek_reduce_consumer
#pragma HLS ARRAY_PARTITION variable=sum_acc complete dim=0
#pragma HLS ARRAY_PARTITION variable=sumsq_acc complete dim=0

        // STEP 1: Load (Optimized for II=1). [RP] window for RPP output rows = input [r-1 .. r+RPP].
        int lc = (r == 0) ? (RPP + 2) : RPP;
        for (int i = 0; i < lc; i++) {
            int h = (r == 0) ? (i - 1) : (r + 1 + i);
            int h_c = (h < 0) ? -h : ((h >= H) ? (H<<1)-h-2 : h);
            int slot = (h + 1) & 3;
            for (int w = 0; w < W; w++) {
                int ddr_base = flat_idx(0, h_c, w, 0, H, W, MODEL_C) / 16;
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

        // STEP 2: Conv (Shared MACs) — DATAFLOW: MAC producer || reduce/stats consumer.
        // [A] producer flattens co*step*ci -> one II=1 pipeline so the MAC fill is paid ONCE
        //     per row. [B] reduce8+bias+LayerNorm-stats moved to a concurrent consumer that
        //     iterates p sequentially (1 reused 7-adder tree, not 16x in-pipeline) -> ~18K LUT
        //     saved vs in-pipeline reduce, fits the [D]-stacked LUT budget. Stream carries the
        //     raw 8-slot psum per co (PEs*8 fp16). Reduce + per-PE-over-co stats order kept
        //     identical to canonical -> BIT-IDENTICAL.
        {
#pragma HLS DATAFLOW
            hls::stream<ap_uint<PEs*128*RPP> > pstrm;
#pragma HLS STREAM variable=pstrm depth=4
#pragma HLS BIND_STORAGE variable=pstrm type=fifo impl=srl
            uek_mac_producer<PEs, T>(W_ptr, x_buf, CI_PAD, CI_W, r, pstrm);
            uek_reduce_consumer<PEs, T>(pstrm, b_buf, y_cache, sum_acc, sumsq_acc);
        }

        // STEP 3: Out  ([RP] one PASS_B per output row in the pass; rows share nothing here)
        for (int rr=0; rr<RPP; rr++) {
            int out_row = r + rr;
            PASS_B: for (int p=0; p<PEs; p++) {
                float mean_f = sum_acc[rr][p] / 960.0f;
                float var_f  = sumsq_acc[rr][p] * pre_div_sq;
                float denom  = var_f - mean_f * mean_f * (960.0f/959.0f) + (float)epsilon;
                T inv = (T)(1.0f / my_sqrt_f(denom));
                int out_px_base = (out_row * W + p) * 60;
                WRITE_OFM: for (int j=0; j<60; j++) {
#pragma HLS PIPELINE II=1
                    data_256_t out_w=0, sk_w=(is_l2)?skip_buf[out_px_base+j]:(data_256_t)0;
                    for (int k = 0; k < 16; k++) {
#pragma HLS UNROLL
                        T val=bits_to_half<T>(y_cache[rr*960 + j*16+k].range(16*p+15,16*p)), g=bits_to_half<T>(g_buf[j].range(16*k+15,16*k)), be=bits_to_half<T>(be_buf[j].range(16*k+15,16*k));
                        T norm = (val-(T)mean_f)*inv*g + be;
                        T act = is_cbi ? norm : (is_l1 ? ((norm<(T)0)?(T)0:norm) : (norm+bits_to_half<T>(sk_w.range(16*k+15,16*k))));
                        out_w.range(16*k+15,16*k) = half_to_bits(act);
                    }
                    Out_ptr[out_px_base+j] = out_w;
                    if (is_cbi) global_buf[out_px_base+j] = out_w;
                }
            }
        }
    }
}
