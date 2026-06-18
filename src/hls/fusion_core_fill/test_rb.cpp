// ============================================================
// Fusion Core — RB-only CSIM (verifies [A]+[B] PASS_A flatten)
// Skips CBI (pre-existing test-harness input-stride bug) + GlobalAdd.
// ResBlock-0 (CI_PAD=960) is the dominant PASS_A path.
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

static bool read_floats(const char* path, float* dst, int N) {
    ifstream f(path);
    if (!f.is_open()) { cerr << "Cannot open: " << path << "\n"; return false; }
    for (int i = 0; i < N; i++) if (!(f >> dst[i])) { cerr << "Read err " << i << " " << path << "\n"; return false; }
    return true;
}
static data_t f2h(float v) { return (data_t)v; }
static float  h2f(data_t v) { return (float)v; }

static void pack(const float* src, data_256_t* dst, int n_elems) {
    int n_words = (n_elems + 15) / 16;
    for (int i = 0; i < n_words; i++) {
        data_256_t word = 0;
        for (int k = 0; k < 16; k++) if (i*16+k < n_elems) {
            data_t h = f2h(src[i*16+k]); uint16_t b; memcpy(&b,&h,2); word.range(16*k+15,16*k)=b;
        }
        dst[i] = word;
    }
}
static void unpack(const data_256_t* src, float* dst, int n_elems) {
    for (int i = 0; i < (n_elems+15)/16; i++) for (int k = 0; k < 16; k++) if (i*16+k < n_elems) {
        uint16_t b = (uint16_t)src[i].range(16*k+15,16*k).to_uint(); data_t h; memcpy(&h,&b,2); dst[i*16+k]=h2f(h);
    }
}
struct CmpResult { float max_err; double rmse; int mismatch; };
static CmpResult compare(const float* got, const float* ref, int n, float tol) {
    CmpResult r = {0.0f, 0.0, 0}; double s = 0.0;
    for (int i = 0; i < n; i++) { float d = fabsf(got[i]-ref[i]); if (d>r.max_err) r.max_err=d; s+=(double)d*d; if (d>tol) r.mismatch++; }
    r.rmse = sqrt(s/n); return r;
}

int main() {
    cout << "======================================\n FUSION CORE — RB-ONLY CSIM ([A]+[B])\n======================================\n";
    const int H = MODEL_H, W = MODEL_W, HW = H * W;
    constexpr int C_OUT = 960, C_RB = 960, CO_WORDS = C_OUT / 16;
    constexpr float EPS = 0.001f, TOL = 0.15f;
    char path[512];
    int HWC = HW * C_RB, WE = C_RB * 9 * C_RB;

    vector<float> rb_x(HWC), rb_w1(WE), rb_b1(C_RB), rb_g1(C_RB), rb_be1(C_RB);
    vector<float> rb_w2(WE), rb_b2(C_RB), rb_g2(C_RB), rb_be2(C_RB), rb_gold(HWC);

    sprintf(path, "%sio_params/Gen_cbi_output.txt", DATA);   if (!read_floats(path, rb_x.data(), HWC)) return 1;
    sprintf(path, "%smodel_params/Gen_rb0_weight_1.txt", DATA); if (!read_floats(path, rb_w1.data(), WE)) return 1;
    sprintf(path, "%smodel_params/Gen_rb0_bias_1.txt", DATA);   if (!read_floats(path, rb_b1.data(), C_RB)) return 1;
    sprintf(path, "%smodel_params/Gen_rb0_gamma_1.txt", DATA);  if (!read_floats(path, rb_g1.data(), C_RB)) return 1;
    sprintf(path, "%smodel_params/Gen_rb0_beta_1.txt", DATA);   if (!read_floats(path, rb_be1.data(), C_RB)) return 1;
    sprintf(path, "%smodel_params/Gen_rb0_weight_2.txt", DATA); if (!read_floats(path, rb_w2.data(), WE)) return 1;
    sprintf(path, "%smodel_params/Gen_rb0_bias_2.txt", DATA);   if (!read_floats(path, rb_b2.data(), C_RB)) return 1;
    sprintf(path, "%smodel_params/Gen_rb0_gamma_2.txt", DATA);  if (!read_floats(path, rb_g2.data(), C_RB)) return 1;
    sprintf(path, "%smodel_params/Gen_rb0_beta_2.txt", DATA);   if (!read_floats(path, rb_be2.data(), C_RB)) return 1;
    sprintf(path, "%sio_params/Gen_rb0_output.txt", DATA);      if (!read_floats(path, rb_gold.data(), HWC)) return 1;

    vector<data_256_t> rb_X(HW*CO_WORDS), rb_W1(C_RB*9*CO_WORDS), rb_B1(CO_WORDS), rb_G1(CO_WORDS), rb_BE1(CO_WORDS);
    vector<data_256_t> rb_W2(C_RB*9*CO_WORDS), rb_B2(CO_WORDS), rb_G2(CO_WORDS), rb_BE2(CO_WORDS);
    vector<data_256_t> rb_Z(HW*CO_WORDS), rb_Y(HW*CO_WORDS);

    pack(rb_x.data(), rb_X.data(), HWC);
    pack(rb_w1.data(), rb_W1.data(), WE); pack(rb_b1.data(), rb_B1.data(), C_RB);
    pack(rb_g1.data(), rb_G1.data(), C_RB); pack(rb_be1.data(), rb_BE1.data(), C_RB);
    pack(rb_w2.data(), rb_W2.data(), WE); pack(rb_b2.data(), rb_B2.data(), C_RB);
    pack(rb_g2.data(), rb_G2.data(), C_RB); pack(rb_be2.data(), rb_BE2.data(), C_RB);

    cout << "  Running HLS RB_L1...\n";
    fusion_core_top(rb_X.data(), rb_W1.data(), rb_B1.data(), rb_G1.data(), rb_BE1.data(), NULL, NULL, rb_Z.data(), (data_t)EPS, MODE_RB_L1);
    cout << "  Running HLS RB_L2...\n";
    fusion_core_top(rb_Z.data(), rb_W2.data(), rb_B2.data(), rb_G2.data(), rb_BE2.data(), NULL, NULL, rb_Y.data(), (data_t)EPS, MODE_RB_L2);

    vector<float> got(HWC);
    unpack(rb_Y.data(), got.data(), HWC);
    CmpResult r = compare(got.data(), rb_gold.data(), HWC, TOL);
    cout << "  RB0: " << (r.mismatch==0 ? "PASS" : "FAIL")
         << "  max_err=" << r.max_err << "  rmse=" << r.rmse
         << "  mismatch=" << r.mismatch << "/" << HWC << "\n";
    cout << "======================================\n" << (r.mismatch==0 ? "RESULT: PASS" : "RESULT: FAIL") << "\n";
    return r.mismatch==0 ? 0 : 1;
}
