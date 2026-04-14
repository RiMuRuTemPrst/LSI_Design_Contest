#include <cassert>
#include <ap_int.h>

inline int get_index(int n, int h, int w, int c, int H, int W, int C) {
    #pragma HLS INLINE
    return ((n * H + h) * W + w) * C + c;
}

template <int PEs, int BATCH,
          int C_IN, int C_OUT,
          int H_IN, int W_IN,
          int H_R,  int W_R,
          int H_OUT, int W_OUT,
          typename T>
#ifdef __SYNTHESIS__
void ConvTranspose_Kernel(ap_uint<64>* X, ap_uint<64>* W_ptr, ap_uint<64>* B, ap_uint<64>* Y) {
#else
void ConvTranspose_Kernel(T* X, T* W_ptr, T* B, T* Y) {
#endif
    constexpr int S       = 2;
    constexpr int PAD     = 1;
    constexpr int PACK    = 4;
    // OPT-1: LANE 8→16. Halves Mac_loop trip count (120→60), same URAM banks (64).
    // Valid for Layer 0-2 (C_IN=960/480/240 all div by 16). NOT for Layer 3 (C_IN=120).
    // syn.top=HW_ConvTranspose_0 so only Layer 0 is synthesized.
    constexpr int LANE    = 16;
    constexpr int TILE_C  = 8;
    constexpr int N_TILES = C_OUT / TILE_C;
    constexpr int KK      = H_R * W_R;  // 9

    // x_buf: toàn bộ X on-chip URAM
    // cyclic factor=LANE=16 → 16 banks × 4 URAM/bank = 64 URAM (không đổi vs LANE=8)
    static T x_buf[H_IN][W_IN][C_IN];
    #pragma HLS ARRAY_PARTITION variable=x_buf cyclic factor=LANE dim=3
    #pragma HLS BIND_STORAGE variable=x_buf type=ram_t2p impl=uram

    T w_tile[TILE_C][KK][C_IN];
    #pragma HLS ARRAY_PARTITION variable=w_tile complete dim=1
    #pragma HLS ARRAY_PARTITION variable=w_tile complete dim=2
    #pragma HLS BIND_STORAGE variable=w_tile type=ram_s2p impl=lutram

    // OPT-2: b_buf cyclic partition TILE_C=8 → loại bỏ BRAM port conflict
    // khi IOM_pass3 đọc 8 giá trị b_buf song song sau khi unroll tc
    T b_buf[C_OUT];
    #pragma HLS BIND_STORAGE variable=b_buf type=ram_t2p impl=bram
    #pragma HLS ARRAY_PARTITION variable=b_buf cyclic factor=TILE_C dim=1

    // OPT-3: row_buff complete dim=1 (W_IN=16) → convert BRAM → 256 FF registers
    // Loại bỏ BRAM address decode latency trong IOM_pass3
    static T col_buff[TILE_C][H_R][W_R - S];
    #pragma HLS ARRAY_PARTITION variable=col_buff complete dim=1
    #pragma HLS ARRAY_PARTITION variable=col_buff complete dim=2
    #pragma HLS ARRAY_PARTITION variable=col_buff complete dim=3

    static T row_buff[W_IN][TILE_C][H_R - S][S];
    #pragma HLS ARRAY_PARTITION variable=row_buff complete dim=1
    #pragma HLS ARRAY_PARTITION variable=row_buff complete dim=2
    #pragma HLS ARRAY_PARTITION variable=row_buff complete dim=3
    #pragma HLS ARRAY_PARTITION variable=row_buff complete dim=4

    T acc[TILE_C][KK][LANE];
    #pragma HLS ARRAY_PARTITION variable=acc complete dim=0

    T sum_buf[TILE_C][KK];
    #pragma HLS ARRAY_PARTITION variable=sum_buf complete dim=0

    T out_col_buf[TILE_C][KK];
    #pragma HLS ARRAY_PARTITION variable=out_col_buf complete dim=0

    // OPT-4: out_final buffer lưu (or_ + bias) trước khi packed-write Y
    T out_final[TILE_C][KK];
    #pragma HLS ARRAY_PARTITION variable=out_final complete dim=0

    // Load bias
    Load_bias:
    for (int i = 0; i < C_OUT; i += PACK) {
        #pragma HLS PIPELINE II=1
#ifdef __SYNTHESIS__
        ap_uint<64> raw = B[i / PACK];
        for (int p = 0; p < PACK; p++) {
            #pragma HLS UNROLL
            b_buf[i + p].range(15, 0) = raw.range(16*p+15, 16*p);
        }
#else
        for (int p = 0; p < PACK; p++)
            b_buf[i + p] = B[i + p];
#endif
    }

    Batch_loop:
    for (int n = 0; n < BATCH; n++) {

        // Preload X vào URAM — 1 lần/batch
        // LANE=16: LANE/PACK=4 unrolled AXI reads/cycle (vs 2 trước)
        Load_X:
        for (int hi = 0; hi < H_IN; hi++) {
            for (int wi2 = 0; wi2 < W_IN; wi2++) {
                int x_base_flat = n * H_IN * W_IN * C_IN
                                + hi * W_IN * C_IN
                                + wi2 * C_IN;
                for (int ci = 0; ci < C_IN; ci += LANE) {
                    #pragma HLS PIPELINE II=1
                    #pragma HLS DEPENDENCE variable=x_buf inter false
                    #pragma HLS DEPENDENCE variable=x_buf intra false
#ifdef __SYNTHESIS__
                    int x_base = x_base_flat / PACK;
                    for (int p = 0; p < LANE / PACK; p++) {
                        #pragma HLS UNROLL
                        ap_uint<64> raw = X[x_base + ci/PACK + p];
                        for (int l = 0; l < PACK; l++) {
                            #pragma HLS UNROLL
                            x_buf[hi][wi2][ci + p*PACK + l].range(15, 0) =
                                raw.range(16*l+15, 16*l);
                        }
                    }
#else
                    for (int l = 0; l < LANE; l++)
                        x_buf[hi][wi2][ci + l] = X[x_base_flat + ci + l];
#endif
                }
            }
        }

        Tile_loop:
        for (int tile = 0; tile < N_TILES; tile++) {
            int co_base = tile * TILE_C;

            // Load_w: 1 lần/tile
            Load_w:
            for (int tc = 0; tc < TILE_C; tc++) {
                int co = co_base + tc;
                int w_co_base = co * KK * C_IN;
                for (int kk = 0; kk < KK; kk++) {
                    int w_kk_base = w_co_base + kk * C_IN;
                    for (int ci = 0; ci < C_IN; ci += LANE) {
                        #pragma HLS PIPELINE II=1
                        #pragma HLS DEPENDENCE variable=w_tile inter false
                        #pragma HLS DEPENDENCE variable=w_tile intra false
#ifdef __SYNTHESIS__
                        for (int p = 0; p < LANE / PACK; p++) {
                            #pragma HLS UNROLL
                            ap_uint<64> raw = W_ptr[(w_kk_base + ci + p*PACK) / PACK];
                            for (int l = 0; l < PACK; l++) {
                                #pragma HLS UNROLL
                                w_tile[tc][kk][ci + p*PACK + l].range(15, 0) =
                                    raw.range(16*l+15, 16*l);
                            }
                        }
#else
                        for (int l = 0; l < LANE; l++)
                            w_tile[tc][kk][ci + l] = W_ptr[w_kk_base + ci + l];
#endif
                    }
                }
            }

            Layer_loop:
            for (int hi = 0; hi < H_IN; hi++) {
                for (int wi = 0; wi < W_IN; wi++) {
                    int base_h = hi * S - PAD;
                    int base_w = wi * S - PAD;

                    // Reset acc
                    Reset_acc:
                    for (int tc = 0; tc < TILE_C; tc++) {
                        #pragma HLS UNROLL
                        for (int kk = 0; kk < KK; kk++) {
                            #pragma HLS UNROLL
                            for (int l = 0; l < LANE; l++) {
                                #pragma HLS UNROLL
                                acc[tc][kk][l] = (T)0;
                            }
                        }
                    }

                    // Mac_loop: LANE=16 → 60 iterations (vs 120), DSP 576→1152
                    Mac_loop:
                    for (int ci = 0; ci < C_IN; ci += LANE) {
                        #pragma HLS PIPELINE II=1
                        T xv[LANE];
                        #pragma HLS ARRAY_PARTITION variable=xv complete dim=1
                        for (int l = 0; l < LANE; l++) {
                            #pragma HLS UNROLL
                            xv[l] = x_buf[hi][wi][ci + l];
                        }
                        for (int tc = 0; tc < TILE_C; tc++) {
                            #pragma HLS UNROLL
                            for (int kk = 0; kk < KK; kk++) {
                                #pragma HLS UNROLL
                                for (int l = 0; l < LANE; l++) {
                                    #pragma HLS UNROLL
                                    T wv = w_tile[tc][kk][ci + l];
                                    T pv = xv[l] * wv;
                                    #pragma HLS BIND_OP variable=pv op=fmul impl=dsp
                                    acc[tc][kk][l] += pv;
                                }
                            }
                        }
                    }

                    // IOM Pass 1: Reduce LANE partial products → sum_buf
                    IOM_pass1:
                    for (int tc = 0; tc < TILE_C; tc++) {
                        #pragma HLS UNROLL
                        for (int kk = 0; kk < KK; kk++) {
                            #pragma HLS UNROLL
                            T s = (T)0;
                            for (int l = 0; l < LANE; l++) {
                                #pragma HLS UNROLL
                                s += acc[tc][kk][l];
                            }
                            sum_buf[tc][kk] = s;
                        }
                    }

                    // IOM Pass 2: Column overlap (col_buff hoàn toàn trong registers)
                    IOM_pass2:
                    for (int tc = 0; tc < TILE_C; tc++) {
                        #pragma HLS UNROLL
                        for (int kk = 0; kk < KK; kk++) {
                            #pragma HLS UNROLL
                            int hf = kk / W_R;
                            int wf = kk % W_R;
                            T s = sum_buf[tc][kk];
                            T oc;
                            if (wf < W_R - S) {
                                if (wi == 0) oc = s;
                                else oc = s + col_buff[tc][hf][wf];
                            } else {
                                oc = s;
                            }
                            if (wf >= S)
                                col_buff[tc][hf][wf - S] = oc;
                            out_col_buf[tc][kk] = oc;
                        }
                    }

                    // IOM Pass 3: Row overlap + tích luỹ bias → out_final
                    // row_buff hoàn toàn trong FF (dim=1 complete) → zero BRAM latency
                    // b_buf cyclic partition → TILE_C reads song song
                    IOM_pass3:
                    for (int tc = 0; tc < TILE_C; tc++) {
                        #pragma HLS UNROLL
                        int co = co_base + tc;
                        for (int kk = 0; kk < KK; kk++) {
                            #pragma HLS UNROLL
                            int hf = kk / W_R;
                            int wf = kk % W_R;
                            T oc = out_col_buf[tc][kk];
                            T or_;
                            if (hf < H_R - S) {
                                if (hi == 0) or_ = oc;
                                else {
                                    int _wf;
                                    if (wf < S) _wf = wf;
                                    else _wf = 0;
                                    or_ = oc + row_buff[wi][tc][hf][_wf];
                                }
                            } else {
                                or_ = oc;
                            }
                            if (hf >= S && wf < S)
                                row_buff[wi][tc][hf - S][wf] = or_;
                            out_final[tc][kk] = or_ + b_buf[co];
                        }
                    }

                    // OPT-4: Write Y dạng packed ap_uint<64> (4 fp16/word) trong synthesis
                    // Giảm AXI write transactions từ ~32 → ~8 per interior pixel (~4× ít overhead)
                    // SW sim: giữ nguyên scalar write, interface test.cpp không đổi
                    Write_Y:
                    for (int kk = 0; kk < KK; kk++) {
                        #pragma HLS UNROLL
                        int hf = kk / W_R;
                        int wf = kk % W_R;
                        bool h_final = (hf < S) || (hi == H_IN - 1);
                        bool w_final = (wf < S) || (wi == W_IN - 1);
                        if (h_final && w_final) {
                            int h_out = base_h + hf;
                            int w_out = base_w + wf;
                            if (h_out >= 0 && h_out < H_OUT &&
                                w_out >= 0 && w_out < W_OUT) {
#ifdef __SYNTHESIS__
                                // co_base = tile*TILE_C divisible by PACK → address aligned
                                // C_OUT divisible by PACK → no partial word at boundary
                                int y_packed_base =
                                    (n * H_OUT * W_OUT + h_out * W_OUT + w_out)
                                    * (C_OUT / PACK) + co_base / PACK;
                                for (int pw = 0; pw < TILE_C / PACK; pw++) {
                                    #pragma HLS UNROLL
                                    ap_uint<64> packed = 0;
                                    for (int p = 0; p < PACK; p++) {
                                        #pragma HLS UNROLL
                                        packed.range(16*p+15, 16*p) =
                                            out_final[pw*PACK + p][kk].range(15, 0);
                                    }
                                    Y[y_packed_base + pw] = packed;
                                }
#else
                                for (int tc_w = 0; tc_w < TILE_C; tc_w++) {
                                    int co_w = co_base + tc_w;
                                    int y_idx = get_index(n, h_out, w_out, co_w,
                                                          H_OUT, W_OUT, C_OUT);
                                    Y[y_idx] = out_final[tc_w][kk];
                                }
#endif
                            }
                        }
                    }

                }  // wi
            }  // hi
        }  // Tile_loop
    }  // Batch_loop
}
