// ============================================================
// Universal Engine V2 — 16x16 Verification
// ============================================================
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cstring>
#include <ctime>
#include <cstdlib>
using namespace std;

#include "gen/Core.h"
#include "gen/fusion_core.h"

// ============================================================
// PACK / UNPACK helpers
// ============================================================

static data_t f2h(float v) { return (data_t)v; }
static float  h2f(data_t v) { return (float)v; }

static void pack(const float* src, data_256_t* dst, int n_elems) {
    int n_words = (n_elems + 15) / 16;
    for (int i = 0; i < n_words; i++) {
        data_256_t word = 0;
        for (int k = 0; k < 16; k++) {
            if (i*16 + k < n_elems) {
                data_t h = f2h(src[i*16 + k]);
                uint16_t bits; memcpy(&bits, &h, 2);
                word.range(16*k+15, 16*k) = bits;
            }
        }
        dst[i] = word;
    }
}

static void unpack(const data_256_t* src, float* dst, int n_elems) {
    for (int i = 0; i < (n_elems + 15) / 16; i++) {
        for (int k = 0; k < 16; k++) {
            if (i*16 + k < n_elems) {
                uint16_t bits = (uint16_t)src[i].range(16*k+15, 16*k).to_uint();
                data_t h; memcpy(&h, &bits, 2);
                dst[i*16 + k] = h2f(h);
            }
        }
    }
}

static void pack_cbi_input(const float* src, data_256_t* dst, int HW, int C_IN, int CI_PAD) {
    int ci_words = CI_PAD / 16;
    for (int pos = 0; pos < HW; pos++) {
        for (int i = 0; i < ci_words; i++) {
            data_256_t word = 0;
            for (int k = 0; k < 16; k++) {
                int ch = i*16 + k;
                if (ch < C_IN) {
                    data_t h = f2h(src[pos*C_IN + ch]);
                    uint16_t bits; memcpy(&bits, &h, 2);
                    word.range(16*k+15, 16*k) = bits;
                }
            }
            dst[pos * ci_words + i] = word;
        }
    }
}

static void pack_cbi_weights(const float* src, data_256_t* dst, int CO, int C_IN, int CI_PAD) {
    int ci_words = CI_PAD / 16;
    for (int co = 0; co < CO; co++) {
        for (int ks = 0; ks < 9; ks++) {
            for (int i = 0; i < ci_words; i++) {
                data_256_t word = 0;
                for (int k = 0; k < 16; k++) {
                    int ci = i*16 + k;
                    if (ci < C_IN) {
                        data_t h = f2h(src[(co*9 + ks)*C_IN + ci]);
                        uint16_t bits; memcpy(&bits, &h, 2);
                        word.range(16*k+15, 16*k) = bits;
                    }
                }
                dst[(co*9 + ks)*ci_words + i] = word;
            }
        }
    }
}

static void pack_cbi_inorm(const float* gamma, const float* beta, data_256_t* g_buf, data_256_t* be_buf, int C_IN, int CI_PAD) {
    int ci_words = CI_PAD / 16;
    for (int i = 0; i < ci_words; i++) {
        data_256_t gw = 0, bw = 0;
        for (int k = 0; k < 16; k++) {
            int ch = i*16 + k;
            float gv = (ch < C_IN) ? gamma[ch] : 1.0f;
            float bv = (ch < C_IN) ? beta[ch]  : 0.0f;
            data_t gh = f2h(gv); uint16_t gb; memcpy(&gb, &gh, 2);
            data_t bh = f2h(bv); uint16_t bb; memcpy(&bb, &bh, 2);
            gw.range(16*k+15, 16*k) = gb; bw.range(16*k+15, 16*k) = bb;
        }
        g_buf[i] = gw; be_buf[i] = bw;
    }
}

// ============================================================
// GOLDEN REFERENCE (float32)
// ============================================================

static inline int reflect(int i, int max) {
    if (i < 0) return -i; if (i >= max) return 2*max - i - 2; return i;
}

static void golden_cn(const float* x, const float* g, const float* b, float eps, float* y, int HW, int C) {
    for (int pos = 0; pos < HW; pos++) {
        const float* xp = x + pos*C; float* yp = y + pos*C;
        float sum = 0, sumsq = 0;
        for (int c = 0; c < C; c++) { sum += xp[c]; sumsq += xp[c]*xp[c]; }
        float mean = sum / (float)C;
        float var = sumsq/(float)(C-1) - mean*mean*(float)C/(float)(C-1) + eps;
        float invstd = 1.0f / sqrtf(var);
        for (int c = 0; c < C; c++) yp[c] = (xp[c] - mean) * invstd * g[c] + b[c];
    }
}

static void golden_conv3x3(const float* x, const float* w, const float* b, float* y, int H, int Wsz, int CI, int CO) {
    for (int h = 0; h < H; h++) {
        for (int ww = 0; ww < Wsz; ww++) {
            for (int co = 0; co < CO; co++) {
                float acc = b[co];
                for (int kh = 0; kh < 3; kh++) {
                    for (int kw = 0; kw < 3; kw++) {
                        int ih = reflect(h - 1 + kh, H); int iw = reflect(ww - 1 + kw, Wsz);
                        const float* xp = x + (ih*Wsz + iw)*CI;
                        const float* wp = w + (co*9 + kh*3 + kw)*CI;
                        for (int ci = 0; ci < CI; ci++) acc += xp[ci] * wp[ci];
                    }
                }
                y[(h*Wsz + ww)*CO + co] = acc;
            }
        }
    }
}

static void golden_cbi(const float* X, const float* W, const float* B, const float* G0, const float* BE0, const float* G3, const float* BE3, float eps, float* Y, int H, int Wsz, int C_IN, int CO) {
    int HW = H * Wsz; float* Xn = new float[HW * C_IN]; float* Z = new float[HW * CO];
    golden_cn(X, G0, BE0, eps, Xn, HW, C_IN);
    golden_conv3x3(Xn, W, B, Z, H, Wsz, C_IN, CO);
    golden_cn(Z, G3, BE3, eps, Y, HW, CO);
    delete[] Xn; delete[] Z;
}

static void golden_rb(const float* X, const float* W1, const float* B1, const float* G1, const float* BE1, const float* W2, const float* B2, const float* G2, const float* BE2, float eps, float* Y, int H, int Wsz, int C) {
    int HW = H * Wsz, HWC = HW * C; float* skip = new float[HWC]; memcpy(skip, X, HWC*sizeof(float));
    float* Z1 = new float[HWC]; float* Z2 = new float[HWC];
    golden_conv3x3(X, W1, B1, Z1, H, Wsz, C, C);
    golden_cn(Z1, G1, BE1, eps, Z1, HW, C);
    for (int i = 0; i < HWC; i++) if (Z1[i] < 0) Z1[i] = 0;
    golden_conv3x3(Z1, W2, B2, Z2, H, Wsz, C, C);
    golden_cn(Z2, G2, BE2, eps, Z2, HW, C);
    for (int i = 0; i < HWC; i++) Y[i] = Z2[i] + skip[i];
    delete[] skip; delete[] Z1; delete[] Z2;
}

struct CmpResult { float max_err, mean_err; int err_cnt; int worst_idx; };
static CmpResult compare(const float* got, const float* ref, int n, float tol) {
    CmpResult r = {0, 0, 0, 0};
    for (int i = 0; i < n; i++) {
        float d = fabsf(got[i] - ref[i]); r.mean_err += d;
        if (d > r.max_err) { r.max_err = d; r.worst_idx = i; }
        if (d > tol) r.err_cnt++;
    }
    r.mean_err /= n; return r;
}

static void rand_fill(float* buf, int n, float lo, float hi, unsigned& seed) {
    for (int i = 0; i < n; i++) buf[i] = lo + (hi - lo) * (float)(rand_r(&seed) % 100000) / 100000.0f;
}

int main() {
    cout << "======================================" << endl;
    cout << " UNIVERSAL ENGINE V2 — 16x16 CSIM" << endl;
    cout << "======================================" << endl;

    const int H=MODEL_H, W=MODEL_W, HW=H*W;
    constexpr int C_CBI=220, CI_PAD=224, C_OUT=960, C_RB=960;
    constexpr int CI_WORDS = CI_PAD/16, CO_WORDS = C_OUT/16;
    constexpr float EPS = 0.001f, TOL = 0.15f;
    unsigned seed = 42; int all_pass = 1;

    // --- TEST 1: CBI ---
    cout << "\n[TEST 1/2] CBI MODE" << endl;
    vector<float> cbi_x(HW*C_CBI), cbi_w(C_OUT*9*C_CBI), cbi_b(C_OUT), cbi_g0(C_CBI), cbi_be0(C_CBI), cbi_g3(C_OUT), cbi_be3(C_OUT), cbi_ref(HW*C_OUT), cbi_got(HW*C_OUT);
    rand_fill(cbi_x.data(), HW*C_CBI, -1.0f, 1.0f, seed); rand_fill(cbi_w.data(), C_OUT*9*C_CBI, -0.1f, 0.1f, seed); rand_fill(cbi_b.data(), C_OUT, -0.1f, 0.1f, seed);
    rand_fill(cbi_g0.data(), C_CBI, 0.5f, 1.5f, seed); rand_fill(cbi_be0.data(), C_CBI, -0.1f, 0.1f, seed); rand_fill(cbi_g3.data(), C_OUT, 0.5f, 1.5f, seed); rand_fill(cbi_be3.data(), C_OUT, -0.1f, 0.1f, seed);

    golden_cbi(cbi_x.data(), cbi_w.data(), cbi_b.data(), cbi_g0.data(), cbi_be0.data(), cbi_g3.data(), cbi_be3.data(), EPS, cbi_ref.data(), H, W, C_CBI, C_OUT);

    vector<data_256_t> cbi_X_hls(HW*CI_WORDS), cbi_W_hls(C_OUT*9*CI_WORDS), cbi_B_hls(CO_WORDS), cbi_G_hls(CO_WORDS), cbi_BE_hls(CO_WORDS), cbi_GIN_hls(CI_WORDS), cbi_BEIN_hls(CI_WORDS), cbi_Y_hls(HW*CO_WORDS);
    pack_cbi_input(cbi_x.data(), cbi_X_hls.data(), HW, C_CBI, CI_PAD); pack_cbi_weights(cbi_w.data(), cbi_W_hls.data(), C_OUT, C_CBI, CI_PAD);
    pack(cbi_b.data(), cbi_B_hls.data(), C_OUT); pack(cbi_g3.data(), cbi_G_hls.data(), C_OUT); pack(cbi_be3.data(), cbi_BE_hls.data(), C_OUT);
    pack_cbi_inorm(cbi_g0.data(), cbi_be0.data(), cbi_GIN_hls.data(), cbi_BEIN_hls.data(), C_CBI, CI_PAD);

    fusion_core_top(cbi_X_hls.data(), cbi_W_hls.data(), cbi_B_hls.data(), cbi_G_hls.data(), cbi_BE_hls.data(), cbi_GIN_hls.data(), cbi_BEIN_hls.data(), cbi_Y_hls.data(), (data_t)EPS, MODE_CBI);
    unpack(cbi_Y_hls.data(), cbi_got.data(), HW*C_OUT);
    CmpResult r1 = compare(cbi_got.data(), cbi_ref.data(), HW*C_OUT, TOL);
    cout << "  CBI Result: " << (r1.err_cnt==0?"PASS":"FAIL") << " (MaxErr=" << r1.max_err << ")" << endl;
    if(r1.err_cnt>0) all_pass=0;

    // --- TEST 2: ResBlock (L1 + L2) ---
    cout << "\n[TEST 2/2] RESBLOCK MODE" << endl;
    int HWC=HW*C_RB, WE=C_RB*9*C_RB;
    vector<float> rb_x(HWC), rb_w1(WE), rb_b1(C_RB), rb_g1(C_RB), rb_be1(C_RB), rb_w2(WE), rb_b2(C_RB), rb_g2(C_RB), rb_be2(C_RB), rb_ref(HWC), rb_got(HWC);
    rand_fill(rb_x.data(), HWC, -1.0f, 1.0f, seed); rand_fill(rb_w1.data(), WE, -0.05f, 0.05f, seed); rand_fill(rb_b1.data(), C_RB, -0.1f, 0.1f, seed); rand_fill(rb_g1.data(), C_RB, 0.5f, 1.5f, seed); rand_fill(rb_be1.data(), C_RB, -0.1f, 0.1f, seed);
    rand_fill(rb_w2.data(), WE, -0.05f, 0.05f, seed); rand_fill(rb_b2.data(), C_RB, -0.1f, 0.1f, seed); rand_fill(rb_g2.data(), C_RB, 0.5f, 1.5f, seed); rand_fill(rb_be2.data(), C_RB, -0.1f, 0.1f, seed);

    golden_rb(rb_x.data(), rb_w1.data(), rb_b1.data(), rb_g1.data(), rb_be1.data(), rb_w2.data(), rb_b2.data(), rb_g2.data(), rb_be2.data(), EPS, rb_ref.data(), H, W, C_RB);

    vector<data_256_t> rb_X_hls(HW*CO_WORDS), rb_W1_hls(C_RB*9*CO_WORDS), rb_B1_hls(CO_WORDS), rb_G1_hls(CO_WORDS), rb_BE1_hls(CO_WORDS), rb_W2_hls(C_RB*9*CO_WORDS), rb_B2_hls(CO_WORDS), rb_G2_hls(CO_WORDS), rb_BE2_hls(CO_WORDS), rb_Y_hls(HW*CO_WORDS), rb_Z_hls(HW*CO_WORDS);
    pack(rb_x.data(), rb_X_hls.data(), HWC); pack(rb_w1.data(), rb_W1_hls.data(), WE); pack(rb_b1.data(), rb_B1_hls.data(), C_RB); pack(rb_g1.data(), rb_G1_hls.data(), C_RB); pack(rb_be1.data(), rb_BE1_hls.data(), C_RB);
    pack(rb_w2.data(), rb_W2_hls.data(), WE); pack(rb_b2.data(), rb_B2_hls.data(), C_RB); pack(rb_g2.data(), rb_G2_hls.data(), C_RB); pack(rb_be2.data(), rb_BE2_hls.data(), C_RB);

    cout << "  Running L1 (Output to rb_Z_hls)..." << endl;
    fusion_core_top(rb_X_hls.data(), rb_W1_hls.data(), rb_B1_hls.data(), rb_G1_hls.data(), rb_BE1_hls.data(), NULL, NULL, rb_Z_hls.data(), (data_t)EPS, MODE_RB_L1);
    cout << "  Running L2 (Input from rb_Z_hls)..." << endl;
    fusion_core_top(rb_Z_hls.data(), rb_W2_hls.data(), rb_B2_hls.data(), rb_G2_hls.data(), rb_BE2_hls.data(), NULL, NULL, rb_Y_hls.data(), (data_t)EPS, MODE_RB_L2);

    unpack(rb_Y_hls.data(), rb_got.data(), HWC);
    CmpResult r2 = compare(rb_got.data(), rb_ref.data(), HWC, TOL);
    cout << "  RB Result : " << (r2.err_cnt==0?"PASS":"FAIL") << " (MaxErr=" << r2.max_err << ")" << endl;
    if(r2.err_cnt>0) all_pass=0;

    cout << "\n======================================" << endl;
    cout << (all_pass ? " RESULT: ALL TESTS PASSED" : " RESULT: SOME TESTS FAILED") << endl;
    cout << "======================================" << endl;
    return 0;
}
