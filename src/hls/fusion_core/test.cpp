// ============================================================
// Fusion Core — Real-Data End-to-End CSIM
// Verifies CBI, ResBlock-0, and GlobalAdd against golden files
// ============================================================
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
using namespace std;

#include "gen/Core.h"
#include "gen/fusion_core.h"

static const char* DATA = "/home/rimurutempest/Code/LSI_Design_Contest/assets/test_data/";

// ============================================================
// IO helpers
// ============================================================
static bool read_floats(const char* path, float* dst, int N) {
    ifstream f(path);
    if (!f.is_open()) { cerr << "Cannot open: " << path << "\n"; return false; }
    for (int i = 0; i < N; i++) {
        if (!(f >> dst[i])) {
            cerr << "Read error at " << i << " in " << path << "\n";
            return false;
        }
    }
    return true;
}

// ============================================================
// Pack / Unpack helpers
// ============================================================
static data_t f2h(float v) { return (data_t)v; }
static float  h2f(data_t v) { return (float)v; }

// Flat pack: src[0..n_elems-1] → dst words
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

// CBI input pack: NHWC (n_pixels, C_IN) with padding to CI_PAD
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

// CBI weight pack: src is [CO, 9, CI] (pre-transposed from OIHW)
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

// CBI input norm: gamma/beta packed with padding
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
// Comparison
// ============================================================
struct CmpResult { float max_err; double rmse; int mismatch; };

static CmpResult compare(const float* got, const float* ref, int n, float tol) {
    CmpResult r = {0.0f, 0.0, 0};
    double sumsq = 0.0;
    for (int i = 0; i < n; i++) {
        float d = fabsf(got[i] - ref[i]);
        if (d > r.max_err) r.max_err = d;
        sumsq += (double)d * d;
        if (d > tol) r.mismatch++;
    }
    r.rmse = sqrt(sumsq / n);
    return r;
}

int main() {
    cout << "======================================\n";
    cout << " FUSION CORE — REAL-DATA CSIM\n";
    cout << "======================================\n";

    const int H = MODEL_H, W = MODEL_W, HW = H * W;
    constexpr int C_CBI = 220, CI_PAD = 224, C_OUT = 960, C_RB = 960;
    constexpr int CI_WORDS = CI_PAD / 16, CO_WORDS = C_OUT / 16;
    constexpr float EPS = 0.001f, TOL = 0.15f;

    char path[512];
    int all_pass = 1;

    // =========================================================
    // TEST 1: CBI — real input + real weights vs golden
    // =========================================================
    cout << "\n[TEST 1/3] CBI (220->960)\n";
    cout << "  Loading data...\n";

    vector<float> cbi_x(HW * C_CBI), cbi_w(C_OUT * 9 * C_CBI), cbi_b(C_OUT);
    vector<float> cbi_g0(C_CBI), cbi_be0(C_CBI), cbi_g3(C_OUT), cbi_be3(C_OUT);
    vector<float> cbi_gold(HW * C_OUT);

    sprintf(path, "%sio_params/Gen_input.txt", DATA);
    if (!read_floats(path, cbi_x.data(),   HW * C_CBI))       return 1;
    sprintf(path, "%smodel_params/Gen_cbi_cbi2_weight.txt", DATA);
    if (!read_floats(path, cbi_w.data(),   C_OUT * 9 * C_CBI)) return 1;
    sprintf(path, "%smodel_params/Gen_cbi_cbi2_bias.txt", DATA);
    if (!read_floats(path, cbi_b.data(),   C_OUT))             return 1;
    sprintf(path, "%smodel_params/Gen_cbi_cbi0_gamma.txt", DATA);
    if (!read_floats(path, cbi_g0.data(),  C_CBI))             return 1;
    sprintf(path, "%smodel_params/Gen_cbi_cbi0_beta.txt", DATA);
    if (!read_floats(path, cbi_be0.data(), C_CBI))             return 1;
    sprintf(path, "%smodel_params/Gen_cbi_cbi3_gamma.txt", DATA);
    if (!read_floats(path, cbi_g3.data(),  C_OUT))             return 1;
    sprintf(path, "%smodel_params/Gen_cbi_cbi3_beta.txt", DATA);
    if (!read_floats(path, cbi_be3.data(), C_OUT))             return 1;
    sprintf(path, "%sio_params/Gen_cbi_output.txt", DATA);
    if (!read_floats(path, cbi_gold.data(), HW * C_OUT))       return 1;

    vector<data_256_t> cbi_X_hls(HW * CI_WORDS), cbi_W_hls(C_OUT * 9 * CI_WORDS);
    vector<data_256_t> cbi_B_hls(CO_WORDS), cbi_G_hls(CO_WORDS), cbi_BE_hls(CO_WORDS);
    vector<data_256_t> cbi_GIN_hls(CI_WORDS), cbi_BEIN_hls(CI_WORDS), cbi_Y_hls(HW * CO_WORDS);

    pack_cbi_input(cbi_x.data(),  cbi_X_hls.data(),   HW, C_CBI, CI_PAD);
    pack_cbi_weights(cbi_w.data(), cbi_W_hls.data(),  C_OUT, C_CBI, CI_PAD);
    pack(cbi_b.data(),   cbi_B_hls.data(),   C_OUT);
    pack(cbi_g3.data(),  cbi_G_hls.data(),   C_OUT);
    pack(cbi_be3.data(), cbi_BE_hls.data(),  C_OUT);
    pack_cbi_inorm(cbi_g0.data(), cbi_be0.data(), cbi_GIN_hls.data(), cbi_BEIN_hls.data(), C_CBI, CI_PAD);

    cout << "  Running HLS CBI...\n";
    fusion_core_top(cbi_X_hls.data(), cbi_W_hls.data(), cbi_B_hls.data(),
                    cbi_G_hls.data(), cbi_BE_hls.data(),
                    cbi_GIN_hls.data(), cbi_BEIN_hls.data(),
                    cbi_Y_hls.data(), (data_t)EPS, MODE_CBI);

    vector<float> cbi_got(HW * C_OUT);
    unpack(cbi_Y_hls.data(), cbi_got.data(), HW * C_OUT);
    CmpResult r1 = compare(cbi_got.data(), cbi_gold.data(), HW * C_OUT, TOL);
    cout << "  CBI: " << (r1.mismatch == 0 ? "PASS" : "FAIL")
         << "  max_err=" << r1.max_err
         << "  rmse=" << r1.rmse
         << "  mismatch=" << r1.mismatch << "/" << HW * C_OUT << "\n";
    if (r1.mismatch > 0) all_pass = 0;

    // =========================================================
    // TEST 2: ResBlock 0 — real weights vs golden
    // (global_buf filled from TEST 1 CBI above)
    // =========================================================
    cout << "\n[TEST 2/3] ResBlock-0 (L1+L2, 960->960)\n";
    cout << "  Loading data...\n";

    int HWC = HW * C_RB, WE = C_RB * 9 * C_RB;
    vector<float> rb_x(HWC), rb_w1(WE), rb_b1(C_RB), rb_g1(C_RB), rb_be1(C_RB);
    vector<float> rb_w2(WE), rb_b2(C_RB), rb_g2(C_RB), rb_be2(C_RB);
    vector<float> rb_gold(HWC);

    sprintf(path, "%sio_params/Gen_cbi_output.txt", DATA);
    if (!read_floats(path, rb_x.data(),   HWC)) return 1;
    sprintf(path, "%smodel_params/Gen_rb0_weight_1.txt", DATA);
    if (!read_floats(path, rb_w1.data(),  WE))  return 1;
    sprintf(path, "%smodel_params/Gen_rb0_bias_1.txt", DATA);
    if (!read_floats(path, rb_b1.data(),  C_RB)) return 1;
    sprintf(path, "%smodel_params/Gen_rb0_gamma_1.txt", DATA);
    if (!read_floats(path, rb_g1.data(),  C_RB)) return 1;
    sprintf(path, "%smodel_params/Gen_rb0_beta_1.txt", DATA);
    if (!read_floats(path, rb_be1.data(), C_RB)) return 1;
    sprintf(path, "%smodel_params/Gen_rb0_weight_2.txt", DATA);
    if (!read_floats(path, rb_w2.data(),  WE))  return 1;
    sprintf(path, "%smodel_params/Gen_rb0_bias_2.txt", DATA);
    if (!read_floats(path, rb_b2.data(),  C_RB)) return 1;
    sprintf(path, "%smodel_params/Gen_rb0_gamma_2.txt", DATA);
    if (!read_floats(path, rb_g2.data(),  C_RB)) return 1;
    sprintf(path, "%smodel_params/Gen_rb0_beta_2.txt", DATA);
    if (!read_floats(path, rb_be2.data(), C_RB)) return 1;
    sprintf(path, "%sio_params/Gen_rb0_output.txt", DATA);
    if (!read_floats(path, rb_gold.data(), HWC)) return 1;

    vector<data_256_t> rb_X_hls(HW * CO_WORDS),  rb_W1_hls(C_RB * 9 * CO_WORDS);
    vector<data_256_t> rb_B1_hls(CO_WORDS), rb_G1_hls(CO_WORDS), rb_BE1_hls(CO_WORDS);
    vector<data_256_t> rb_W2_hls(C_RB * 9 * CO_WORDS);
    vector<data_256_t> rb_B2_hls(CO_WORDS), rb_G2_hls(CO_WORDS), rb_BE2_hls(CO_WORDS);
    vector<data_256_t> rb_Z_hls(HW * CO_WORDS), rb_Y_hls(HW * CO_WORDS);

    pack(rb_x.data(),   rb_X_hls.data(),  HWC);
    pack(rb_w1.data(),  rb_W1_hls.data(), WE);
    pack(rb_b1.data(),  rb_B1_hls.data(), C_RB);
    pack(rb_g1.data(),  rb_G1_hls.data(), C_RB);
    pack(rb_be1.data(), rb_BE1_hls.data(), C_RB);
    pack(rb_w2.data(),  rb_W2_hls.data(), WE);
    pack(rb_b2.data(),  rb_B2_hls.data(), C_RB);
    pack(rb_g2.data(),  rb_G2_hls.data(), C_RB);
    pack(rb_be2.data(), rb_BE2_hls.data(), C_RB);

    cout << "  Running HLS RB_L1...\n";
    fusion_core_top(rb_X_hls.data(), rb_W1_hls.data(), rb_B1_hls.data(),
                    rb_G1_hls.data(), rb_BE1_hls.data(), NULL, NULL,
                    rb_Z_hls.data(), (data_t)EPS, MODE_RB_L1);
    cout << "  Running HLS RB_L2...\n";
    fusion_core_top(rb_Z_hls.data(), rb_W2_hls.data(), rb_B2_hls.data(),
                    rb_G2_hls.data(), rb_BE2_hls.data(), NULL, NULL,
                    rb_Y_hls.data(), (data_t)EPS, MODE_RB_L2);

    vector<float> rb_got(HWC);
    unpack(rb_Y_hls.data(), rb_got.data(), HWC);
    CmpResult r2 = compare(rb_got.data(), rb_gold.data(), HWC, TOL);
    cout << "  RB0:  " << (r2.mismatch == 0 ? "PASS" : "FAIL")
         << "  max_err=" << r2.max_err
         << "  rmse=" << r2.rmse
         << "  mismatch=" << r2.mismatch << "/" << HWC << "\n";
    if (r2.mismatch > 0) all_pass = 0;

    // =========================================================
    // TEST 3: GlobalAdd — rb8 output + CBI skip vs golden
    // global_buf was filled by TEST 1 (CBI), matches Gen_cbi_output.txt
    // =========================================================
    cout << "\n[TEST 3/3] GlobalAdd\n";
    cout << "  Loading data...\n";

    vector<float> ga_x(HW * C_OUT), ga_gold(HW * C_OUT);

    sprintf(path, "%sio_params/Gen_rb8_output.txt", DATA);
    if (!read_floats(path, ga_x.data(),    HW * C_OUT)) return 1;
    sprintf(path, "%sio_params/Gen_global_add_output.txt", DATA);
    if (!read_floats(path, ga_gold.data(), HW * C_OUT)) return 1;

    vector<data_256_t> ga_X_hls(HW * CO_WORDS), ga_Y_hls(HW * CO_WORDS);
    pack(ga_x.data(), ga_X_hls.data(), HW * C_OUT);

    cout << "  Running HLS GlobalAdd...\n";
    fusion_core_top(ga_X_hls.data(), NULL, NULL, NULL, NULL, NULL, NULL,
                    ga_Y_hls.data(), (data_t)EPS, MODE_GLOBAL_ADD);

    vector<float> ga_got(HW * C_OUT);
    unpack(ga_Y_hls.data(), ga_got.data(), HW * C_OUT);
    CmpResult r3 = compare(ga_got.data(), ga_gold.data(), HW * C_OUT, TOL);
    cout << "  GA:   " << (r3.mismatch == 0 ? "PASS" : "FAIL")
         << "  max_err=" << r3.max_err
         << "  rmse=" << r3.rmse
         << "  mismatch=" << r3.mismatch << "/" << HW * C_OUT << "\n";
    if (r3.mismatch > 0) all_pass = 0;

    // =========================================================
    cout << "\n======================================\n";
    cout << (all_pass ? "RESULT: ALL PASS" : "RESULT: SOME FAIL") << "\n";
    cout << "======================================\n";
    return all_pass ? 0 : 1;
}
