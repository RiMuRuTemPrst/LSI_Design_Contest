// End-to-end CSIM for full_generator_opt5 — UCB + Conv77 stages
// Input:  Gen_global_add_output.txt  (skips Fusion, verifies UCB offset fix)
// Golden: Gen_ucb4_output.txt  (after all 4 UCBs)
//         Gen_conv77_output.txt (after Conv77)

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <cstdio>

#include "Core.h"           // data_256_t, data_t, DDR_PTR, DDR_CONST_PTR
#include "upconv_core.h"    // MODE_UCB_0..3
#include "Hls_Layers_UpConv.tpp"
#include "Hls_Layers_Conv77.tpp"

using namespace std;

static const char* DATA = "/home/rimurutempest/Code/LSI_Design_Contest/assets/test_data/";

// Opt5 weight layout — must match generator_top.cpp
static const int W_OFF[4] = { 0, 259200, 324000, 340200 };
//  UCB_0: 480*9*60=259200, UCB_1: 240*9*30=64800, UCB_2: 120*9*15=16200, UCB_3: 60*9*8=4320
static const int W_TOTAL   = 344520;  // total W_upconv words

// P_upconv layout: [B0(30), G0(30), BE0(30), B1(15), G1(15), BE1(15), B2(8), G2(8), BE2(8), B3(4), G3(4), BE3(4)]
static const int B_OFF[4]  = {  0,  90, 135, 159 };
static const int G_OFF[4]  = { 30, 105, 143, 163 };
static const int BE_OFF[4] = { 60, 120, 151, 167 };
static const int P_TOTAL   = 171;     // total P_upconv words

// ---- utilities ----

static bool read_floats(const char* path, vector<float>& dst, int N) {
    ifstream f(path);
    if (!f.is_open()) { cerr << "Cannot open: " << path << "\n"; return false; }
    dst.resize(N);
    for (int i = 0; i < N; i++)
        if (!(f >> dst[i])) { cerr << "Read error at " << i << "\n"; return false; }
    return true;
}

static uint16_t f2h(float v) { data_t h = (data_t)v; uint16_t b; memcpy(&b, &h, 2); return b; }
static float    h2f(uint16_t b) { data_t h; memcpy(&h, &b, 2); return (float)h; }

// Pack tensor [n_pixels][n_ch] → data_256_t words (NHWC, 16 fp16 per word)
static void pack_stride(const float* src, data_256_t* dst, int n_pixels, int n_ch) {
    int cw = (n_ch + 15) / 16;
    for (int p = 0; p < n_pixels; p++)
        for (int i = 0; i < cw; i++) {
            data_256_t w = 0;
            for (int l = 0; l < 16; l++)
                if (i*16+l < n_ch) w.range(16*l+15,16*l) = f2h(src[p*n_ch + i*16+l]);
            dst[p*cw + i] = w;
        }
}

// Unpack data_256_t words → float tensor
static void unpack_stride(const data_256_t* src, float* dst, int n_pixels, int n_ch) {
    int cw = (n_ch + 15) / 16;
    for (int p = 0; p < n_pixels; p++)
        for (int i = 0; i < cw; i++) {
            data_256_t w = src[p*cw + i];
            for (int l = 0; l < 16; l++)
                if (i*16+l < n_ch) dst[p*n_ch + i*16+l] = h2f(w.range(16*l+15,16*l).to_uint());
        }
}

// Pack ConvTranspose weights [CO][K=9][CI] → data_256_t
static void pack_conv_weights(const float* src, data_256_t* dst, int co, int ci) {
    int ciw = (ci + 15) / 16;
    for (int o = 0; o < co; o++)
        for (int k = 0; k < 9; k++)
            for (int i = 0; i < ciw; i++) {
                data_256_t w = 0;
                for (int l = 0; l < 16; l++)
                    if (i*16+l < ci) w.range(16*l+15,16*l) = f2h(src[(o*9+k)*ci + i*16+l]);
                dst[(o*9+k)*ciw + i] = w;
            }
}

// Pack flat param array
static void pack_flat(const float* src, data_256_t* dst, int n) {
    int nw = (n + 15) / 16;
    for (int i = 0; i < nw; i++) {
        data_256_t w = 0;
        for (int l = 0; l < 16; l++)
            if (i*16+l < n) w.range(16*l+15,16*l) = f2h(src[i*16+l]);
        dst[i] = w;
    }
}

struct CmpResult { float max_err; double rmse; int mismatch; };
static CmpResult compare(const float* got, const float* ref, int n, float tol) {
    CmpResult r = {0,0,0};
    double ss = 0;
    for (int i = 0; i < n; i++) {
        float d = fabsf(got[i] - ref[i]);
        if (d > r.max_err) r.max_err = d;
        ss += (double)d*d;
        if (d > tol) r.mismatch++;
    }
    r.rmse = sqrt(ss / n);
    return r;
}

// ---- UCB runner — single UCB with SEPARATE in/out to avoid Y aliasing ----
// Bug: if X_in == Y_out (same buffer), ho=2*hi writes to Y before hi+1 is loaded.
// Fix: caller does ping-pong between two Y buffers across UCBs.

static void run_one_ucb(
    DDR_CONST_PTR     X_in,          // input (read-only, must NOT alias Y_out)
    DDR_PTR           Y_out,         // output (separate buffer)
    const data_256_t* W_upconv,
    const data_256_t* P_upconv,
    data_t            epsilon,
    int               m              // UCB index 0..3
) {
    static data_256_t x_buf[2 * 128 * 60];

    const int h_in_arr[4]  = {16,  32,  64,  128};
    const int w_in_arr[4]  = {16,  32,  64,  128};
    const int c_in_arr[4]  = {960, 480, 240, 120};
    const int c_out_arr[4] = {480, 240, 120,  60};

    int h_in  = h_in_arr[m];
    int w_in  = w_in_arr[m];
    int c_in  = c_in_arr[m];
    int c_out = c_out_arr[m];
    int ci_words = (c_in + 15) / 16;

    const data_256_t* W_ucb  = W_upconv + W_OFF[m];
    const data_256_t* B_ucb  = P_upconv + B_OFF[m];
    const data_256_t* G_ucb  = P_upconv + G_OFF[m];
    const data_256_t* BE_ucb = P_upconv + BE_OFF[m];

    for (int wi = 0; wi < w_in; wi++)
        for (int ciw = 0; ciw < ci_words; ciw++)
            x_buf[(0 * w_in + wi) * ci_words + ciw] =
                X_in[(0 * w_in + wi) * ci_words + ciw];

    UpConv_Fused_Row<12>(x_buf, W_ucb, B_ucb, G_ucb, BE_ucb,
                         Y_out, epsilon, h_in, w_in, c_in, c_out, 0);

    for (int hi = 1; hi < h_in; hi++) {
        int slot = hi % 2;
        for (int wi = 0; wi < w_in; wi++)
            for (int ciw = 0; ciw < ci_words; ciw++)
                x_buf[(slot * w_in + wi) * ci_words + ciw] =
                    X_in[(hi * w_in + wi) * ci_words + ciw];

        UpConv_Fused_Row<12>(x_buf, W_ucb, B_ucb, G_ucb, BE_ucb,
                             Y_out, epsilon, h_in, w_in, c_in, c_out, 2*hi-1);
        UpConv_Fused_Row<12>(x_buf, W_ucb, B_ucb, G_ucb, BE_ucb,
                             Y_out, epsilon, h_in, w_in, c_in, c_out, 2*hi);
    }
    UpConv_Fused_Row<12>(x_buf, W_ucb, B_ucb, G_ucb, BE_ucb,
                         Y_out, epsilon, h_in, w_in, c_in, c_out, 2*h_in-1);

    printf("  [UCB_%d] done  (%dx%d\xc3\x97%d \xe2\x86\x92 %dx%d\xc3\x97%d)\n",
           m, h_in, w_in, c_in, 2*h_in, 2*w_in, c_out);
}

// ---- main ----

int main() {
    printf("============================================================\n");
    printf(" full_generator_opt5 — UCB + Conv77 E2E CSIM\n");
    printf(" Input:  Gen_global_add_output.txt (16x16x960)\n");
    printf(" Golden: Gen_ucb4_output.txt  (256x256x60)\n");
    printf("         Gen_conv77_output.txt (256x256x3)\n");
    printf("============================================================\n\n");

    char path[512];
    int all_pass = 1;

    // ===== Build W_upconv and P_upconv =====
    printf("[LOAD] Assembling W_upconv (%d words) and P_upconv (%d words)...\n",
           W_TOTAL, P_TOTAL);

    vector<data_256_t> W_upconv(W_TOTAL, 0);
    vector<data_256_t> P_upconv(P_TOTAL, 0);

    // (file index k+1 = UCB_k in code, e.g. Gen_ucb1_* = UCB_0 with C_IN=960)
    const int co_arr[4] = {480, 240, 120, 60};
    const int ci_arr[4] = {960, 480, 240, 120};
    const char* w_files[4] = {"Gen_ucb1_weight.txt","Gen_ucb2_weight.txt",
                               "Gen_ucb3_weight.txt","Gen_ucb4_weight.txt"};
    const char* b_files[4] = {"Gen_ucb1_bias.txt","Gen_ucb2_bias.txt",
                               "Gen_ucb3_bias.txt","Gen_ucb4_bias.txt"};
    const char* g_files[4] = {"Gen_ucb1_gamma.txt","Gen_ucb2_gamma.txt",
                               "Gen_ucb3_gamma.txt","Gen_ucb4_gamma.txt"};
    const char* be_files[4]= {"Gen_ucb1_beta.txt","Gen_ucb2_beta.txt",
                               "Gen_ucb3_beta.txt","Gen_ucb4_beta.txt"};

    for (int m = 0; m < 4; m++) {
        int co = co_arr[m], ci = ci_arr[m];
        int cwo = (co + 15) / 16;
        vector<float> wf, bf, gf, bef;

        sprintf(path, "%smodel_params/%s", DATA, w_files[m]);
        printf("  Loading %s (%d floats)...\n", w_files[m], co*9*ci);
        if (!read_floats(path, wf, co*9*ci)) return 1;
        sprintf(path, "%smodel_params/%s", DATA, b_files[m]);
        if (!read_floats(path, bf, co)) return 1;
        sprintf(path, "%smodel_params/%s", DATA, g_files[m]);
        if (!read_floats(path, gf, co)) return 1;
        sprintf(path, "%smodel_params/%s", DATA, be_files[m]);
        if (!read_floats(path, bef, co)) return 1;

        pack_conv_weights(wf.data(), W_upconv.data() + W_OFF[m], co, ci);
        pack_flat(bf.data(),  P_upconv.data() + B_OFF[m],  co);
        pack_flat(gf.data(),  P_upconv.data() + G_OFF[m],  co);
        pack_flat(bef.data(), P_upconv.data() + BE_OFF[m], co);
    }

    // ===== Load X = Gen_global_add_output.txt =====
    printf("\n[LOAD] Loading X = Gen_global_add_output.txt (16x16x960)...\n");
    vector<float> x_f;
    sprintf(path, "%sio_params/Gen_global_add_output.txt", DATA);
    if (!read_floats(path, x_f, 16*16*960)) return 1;

    int x_words = 16 * 16 * 60;  // 60 = ceil(960/16)
    vector<data_256_t> X_hls(x_words);
    pack_stride(x_f.data(), X_hls.data(), 16*16, 960);

    // ===== Two Y buffers for ping-pong (avoid in-place aliasing) =====
    // UCB_0→Y_a, UCB_1→Y_b, UCB_2→Y_a, UCB_3→Y_b; final output in Y_b
    int y_words = 256 * 256 * 4;  // 4 = ceil(60/16), max size for UCB_3 output
    vector<data_256_t> Y_a(y_words, 0), Y_b(y_words, 0);
    data_256_t* ping_pong[2] = {Y_a.data(), Y_b.data()};

    // ===== Run UCB_0 through UCB_3 with separate in/out =====
    printf("\n[RUN] Running all 4 UCBs with opt5 weight layout (ping-pong Y)...\n");
    run_one_ucb((DDR_CONST_PTR)X_hls.data(), (DDR_PTR)ping_pong[0],
                W_upconv.data(), P_upconv.data(), (data_t)1e-5f, 0);
    for (int m = 1; m < 4; m++)
        run_one_ucb((DDR_CONST_PTR)ping_pong[(m-1)&1], (DDR_PTR)ping_pong[m&1],
                    W_upconv.data(), P_upconv.data(), (data_t)1e-5f, m);
    data_256_t* Y_final = ping_pong[3 & 1];  // UCB_3 writes to ping_pong[1] = Y_b

    // ===== Check UCB output vs Gen_ucb4_output.txt =====
    printf("\n[CHECK] UCB output vs Gen_ucb4_output.txt (256x256x60)...\n");
    int ny = 256 * 256 * 60;
    vector<float> got_ucb(ny), gold_ucb;
    sprintf(path, "%sio_params/Gen_ucb4_output.txt", DATA);
    if (!read_floats(path, gold_ucb, ny)) return 1;
    unpack_stride(Y_final, got_ucb.data(), 256*256, 60);
    CmpResult r_ucb = compare(got_ucb.data(), gold_ucb.data(), ny, 0.5f);
    printf("  UCB: %s  max_err=%.4f  rmse=%.6f  mismatch=%d/%d\n",
           r_ucb.mismatch == 0 ? "PASS" : "FAIL",
           r_ucb.max_err, r_ucb.rmse, r_ucb.mismatch, ny);
    all_pass &= (r_ucb.mismatch == 0);

    // ===== Load Conv77 weights =====
    printf("\n[LOAD] Loading Conv77 weights (3x7x7x60)...\n");
    vector<float> wc_f, bc_f;
    sprintf(path, "%smodel_params/Gen_cbo_weight.txt", DATA);
    if (!read_floats(path, wc_f, 3*7*7*60)) return 1;
    sprintf(path, "%smodel_params/Gen_cbo_bias.txt", DATA);
    if (!read_floats(path, bc_f, 3)) return 1;

    // Pack Conv77 weights: [CO=3][H_R=7][W_R=7] × CI_PAD=64 → 588 words
    // File layout: [CO][H_R][W_R][CI=60], padded to CI_PAD=64 in HLS
    int wc_words = 3 * 7 * 7 * 4;  // 4 = CI_PAD/16 = 64/16
    vector<data_256_t> W_conv77(wc_words, 0);
    for (int co = 0; co < 3; co++)
        for (int hf = 0; hf < 7; hf++)
            for (int wf = 0; wf < 7; wf++)
                for (int cw = 0; cw < 4; cw++) {
                    data_256_t word = 0;
                    for (int l = 0; l < 16; l++) {
                        int ci = cw*16 + l;
                        if (ci < 60)
                            word.range(16*l+15, 16*l) =
                                f2h(wc_f[((co*7 + hf)*7 + wf)*60 + ci]);
                    }
                    W_conv77[((co*7 + hf)*7 + wf)*4 + cw] = word;
                }

    // Bias: 1 word (3 channels packed in bits[47:0])
    vector<data_256_t> B_conv77(1, 0);
    for (int l = 0; l < 3; l++)
        B_conv77[0].range(16*l+15, 16*l) = f2h(bc_f[l]);

    // ===== Z buffer: 256×256 words (1 pixel/word, bits[47:0]=3×fp16) =====
    vector<data_256_t> Z_hls(256 * 256, 0);

    // ===== Run Conv77 =====
    printf("\n[RUN] Running Conv77_Kernel<8,8,1,60,3,256,256,7,7,256,256>...\n");
    Conv77_Kernel<8, 8, 1, 60, 3, 256, 256, 7, 7, 256, 256>(
        Y_final, W_conv77.data(), B_conv77.data(), Z_hls.data()
    );

    // ===== Check Conv77 output vs Gen_conv77_output.txt =====
    printf("\n[CHECK] Conv77 output vs Gen_conv77_output.txt (256x256x3)...\n");
    int nz = 256 * 256 * 3;
    vector<float> got_z(nz), gold_z;
    sprintf(path, "%sio_params/Gen_conv77_output.txt", DATA);
    if (!read_floats(path, gold_z, nz)) return 1;
    // Unpack Z: each word has 3 channels in bits[47:0]
    for (int i = 0; i < 256*256; i++)
        for (int c = 0; c < 3; c++)
            got_z[i*3 + c] = h2f(Z_hls[i].range(16*c+15, 16*c).to_uint());
    // HLS applies Clip(0,1); clip reference too before comparison
    for (int i = 0; i < nz; i++)
        gold_z[i] = gold_z[i] < 0.0f ? 0.0f : (gold_z[i] > 1.0f ? 1.0f : gold_z[i]);
    CmpResult r_z = compare(got_z.data(), gold_z.data(), nz, 1.0f);
    printf("  Conv77: %s  max_err=%.4f  rmse=%.6f  mismatch=%d/%d\n",
           r_z.mismatch == 0 ? "PASS" : "FAIL",
           r_z.max_err, r_z.rmse, r_z.mismatch, nz);
    all_pass &= (r_z.mismatch == 0);

    printf("\n============================================================\n");
    printf(" RESULT: %s\n", all_pass ? "ALL PASS" : "SOME FAIL");
    printf("============================================================\n");
    return all_pass ? 0 : 1;
}
