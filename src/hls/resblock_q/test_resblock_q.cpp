// ============================================================
// int16 quantized ResBlock — full-size CSIM (16x16x960)
// Reads int data from test_7_quantize/, runs L1 then L2, compares golden output.txt.
// Scalar quant params replicate the (fixed) reference main(): residual+M-swap correct.
// Params are read as EXACT integers (not float) to avoid the reference's float-read loss.
// ============================================================
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <cstdint>
using namespace std;

#include "gen/Core.h"
#include "gen/resblock_q.h"

static const char* Q = "/home/rimurutempest/Code/LSI_Design_Contest/test_7_quantize/";

template<typename T>
static bool read_ints(const char* path, T* dst, int N) {
    ifstream f(path);
    if (!f.is_open()) { cerr << "Cannot open: " << path << "\n"; return false; }
    long long v;
    for (int i = 0; i < N; i++) { if (!(f >> v)) { cerr << "Read err " << i << " " << path << "\n"; return false; } dst[i] = (T)v; }
    return true;
}
static void pack_i16(const int16_t* src, data_256_t* dst, int n) {
    int nw = (n + 15) / 16;
    for (int i = 0; i < nw; i++) {
        data_256_t w = 0;
        for (int k = 0; k < 16; k++) if (i*16+k < n) w.range(16*k+15,16*k) = ap_uint<16>((uint16_t)src[i*16+k]);
        dst[i] = w;
    }
}
static void unpack_i16(const data_256_t* src, int* dst, int n) {
    for (int i = 0; i < (n+15)/16; i++) for (int k = 0; k < 16; k++) if (i*16+k < n)
        dst[i*16+k] = (short)(uint16_t)src[i].range(16*k+15,16*k).to_uint();
}
template<typename T>
static void float_to_mult_shift(float scale, int bw, T* m, int8_t* s) {
    if (scale == 0.0f) { *m = 0; *s = 0; return; }
    int sign = (scale > 0.0f) ? 1 : -1; scale = fabsf(scale);
    int exp = 0; float mant = frexpf(scale, &exp);
    int32_t mul = (int32_t)(mant * (float)(1LL << (bw - 1))) * sign;
    *m = (T)mul; *s = (int8_t)((bw - 1) - exp);
}

int main() {
    cout << "==============================================\n int16 ResBlock CSIM (full 16x16x960)\n==============================================\n";
    const int H=16, W=16, HW=256, C=960;
    const int HWC = HW*C;          // 245760
    const int WE  = C*9*C;         // 8294400
    const int CO_W = C/16;         // 60
    const int HW_W = HW*CO_W;      // 15360
    const int WE_W = WE/16;        // 518400
    char p[600];

    // ---- scalar quant params (reference constants) ----
    float x_scale=0.00029286922654137015f; int x_zp=-516;
    float y_scale=0.0002957680553663522f;  int y_zp=-1837;
    float bias_1_scale=6.3509935088745806e-9f, bias_2_scale=2.396622011957561e-9f;
    float c1_scale=0.004122796934098005f;  int c1_zp=689;
    float n1_scale=0.00010767392086563632f; int n1_zp=-32768;
    float c2_scale=0.0019970976281911135f;  int c2_zp=-12033;
    float n2_scale=0.0002132747322320938f;  int n2_zp=-11043;

    int16_t reci_C_mult; int8_t reci_C_shift; float_to_mult_shift<int16_t>(1.0f/960.0f, 15, &reci_C_mult, &reci_C_shift);
    int16_t m_conv_1; int8_t s_conv_1; float_to_mult_shift<int16_t>(bias_1_scale/c1_scale, 15, &m_conv_1, &s_conv_1);
    int16_t m_conv_2; int8_t s_conv_2; float_to_mult_shift<int16_t>(bias_2_scale/c2_scale, 15, &m_conv_2, &s_conv_2);
    int eps1 = (int)lround(0.001 / ((double)c1_scale*c1_scale));
    int eps2 = (int)lround(0.001 / ((double)c2_scale*c2_scale));
    int32_t m_res_1, m_res_2; int8_t s_res_1, s_res_2;
    float_to_mult_shift<int32_t>(x_scale/y_scale, 31, &m_res_1, &s_res_1);
    float_to_mult_shift<int32_t>(n2_scale/y_scale, 31, &m_res_2, &s_res_2);
    double rz = (double)y_zp - (double)x_scale/y_scale*x_zp - (double)n2_scale/y_scale*n2_zp;
    int res_zp = (int)rz; if (res_zp > 32767) res_zp = 32767; if (res_zp < -32768) res_zp = -32768;

    cout << "  reci_C=("<<reci_C_mult<<","<<(int)reci_C_shift<<") m_conv1=("<<m_conv_1<<","<<(int)s_conv_1<<")"
         << " m_conv2=("<<m_conv_2<<","<<(int)s_conv_2<<") eps1="<<eps1<<" eps2="<<eps2<<"\n"
         << "  m_res1=("<<m_res_1<<","<<(int)s_res_1<<") m_res2=("<<m_res_2<<","<<(int)s_res_2<<") res_zp="<<res_zp<<"\n";

    // ---- load int data (exact) ----
    vector<int16_t> in16(HWC), w1_16(WE), w2_16(WE);
    vector<int32_t> b1(C), g1(C), be1(C), b2(C), g2(C), be2(C);
    vector<int8_t>  gs1(C), gs2(C);
    vector<int>     gold(HWC);
#define RDQ(sub,file,dst,n) do{ sprintf(p,"%s%s/%s",Q,sub,file); if(!read_ints(p,dst,n)) return 1; }while(0)
    RDQ("io_params_q","input.txt",   in16.data(), HWC);
    RDQ("model_params_q","conv1_w.txt", w1_16.data(), WE);
    RDQ("model_params_q","conv1_b.txt", b1.data(),  C);
    RDQ("model_params_q","norm1_gm.txt", g1.data(), C);
    RDQ("model_params_q","norm1_gs.txt", gs1.data(),C);
    RDQ("model_params_q","norm1_b.txt", be1.data(), C);
    RDQ("model_params_q","conv2_w.txt", w2_16.data(), WE);
    RDQ("model_params_q","conv2_b.txt", b2.data(),  C);
    RDQ("model_params_q","norm2_gm.txt", g2.data(), C);
    RDQ("model_params_q","norm2_gs.txt", gs2.data(),C);
    RDQ("model_params_q","norm2_b.txt", be2.data(), C);
    RDQ("io_params_q","output.txt",  gold.data(), HWC);

    vector<data_256_t> X(HW_W), W1(WE_W), W2(WE_W), Z(HW_W), Y(HW_W);
    pack_i16(in16.data(), X.data(), HWC);
    pack_i16(w1_16.data(), W1.data(), WE);
    pack_i16(w2_16.data(), W2.data(), WE);

    cout << "  Running L1 (conv1+norm1)...\n";
    resblock_q_top(X.data(), W1.data(), b1.data(), g1.data(), gs1.data(), be1.data(), Z.data(),
        m_conv_1, s_conv_1, x_zp, c1_zp, reci_C_mult, reci_C_shift, eps1,
        0,0,0,0,0, MODE_RB_L1);
    cout << "  Running L2 (conv2+norm2+add)...\n";
    resblock_q_top(Z.data(), W2.data(), b2.data(), g2.data(), gs2.data(), be2.data(), Y.data(),
        m_conv_2, s_conv_2, n1_zp, c2_zp, reci_C_mult, reci_C_shift, eps2,
        m_res_2, s_res_2, m_res_1, s_res_1, res_zp, MODE_RB_L2);   // M_path=conv-path=m_res_2, M_skip=residual=m_res_1

    vector<int> got(HWC); unpack_i16(Y.data(), got.data(), HWC);
    long long maxd=0, mism=0; double sse=0;
    for (int i = 0; i < HWC; i++) { long long d = labs((long long)got[i]-gold[i]); if (d>maxd) maxd=d; sse += (double)d*d; if (d) mism++; }
    double rmse = sqrt(sse/HWC);
    double Ys = 0.0002957680553663522;
    cout << "  first 8 (got/gold):";
    for (int i=0;i<8;i++) cout << " ("<<got[i]<<"/"<<gold[i]<<")";
    cout << "\n  vs golden: max|d|="<<maxd<<" (real "<<maxd*Ys<<")  rmse="<<rmse<<" (real "<<rmse*Ys<<")  mism="<<mism<<"/"<<HWC<<"\n";
    bool pass = (rmse <= 80.0 && maxd <= 4100);   // reference: max 4051, rmse 79
    cout << "==============================================\n" << (pass ? "RESULT: PASS" : "RESULT: FAIL") << "\n";
    return pass ? 0 : 1;
}
