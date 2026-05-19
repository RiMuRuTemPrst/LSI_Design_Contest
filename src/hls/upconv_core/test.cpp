#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <climits>
#include <cstdlib>
#include "gen/upconv_core.h"

using namespace std;

// Derive data root relative to this source file — works on any machine.
// Uses realpath() to resolve __FILE__ (may be relative) to absolute path.
// test.cpp lives at <repo>/src/hls/upconv_core/test.cpp → 3 dirs up = repo root.
static std::string get_data_root() {
    char resolved[PATH_MAX];
    if (!realpath(__FILE__, resolved))
        strncpy(resolved, __FILE__, PATH_MAX - 1);
    std::string f(resolved);
    size_t p = f.rfind('/');
    if (p != std::string::npos) f = f.substr(0, p); // strip filename
    for (int i = 0; i < 3; i++) {                   // go up: upconv_core → hls → src → repo
        size_t q = f.rfind('/');
        if (q == std::string::npos) break;
        f = f.substr(0, q);
    }
    return f + "/assets/test_data/";
}
static const std::string DATA_STR = get_data_root();
static const char* DATA = DATA_STR.c_str();

static bool read_floats(const char* path, float* dst, int N) {
    ifstream f(path);
    if (!f.is_open()) { cerr << "Cannot open: " << path << "\n"; return false; }
    for (int i = 0; i < N; i++) {
        if (!(f >> dst[i])) { cerr << "Read error at " << i << " in " << path << "\n"; return false; }
    }
    return true;
}

static uint16_t f2h(float f) {
    data_t h = (data_t)f; uint16_t bits; memcpy(&bits, &h, 2); return bits;
}
static float h2f(uint16_t bits) {
    data_t h; memcpy(&h, &bits, 2); return (float)h;
}

// Pack: src[n_pixels][n_ch] → dst[n_pixels * ch_words], with zero-pad beyond n_ch
static void pack_stride(const float* src, data_256_t* dst, int n_pixels, int n_ch) {
    int ch_words = (n_ch + 15) / 16;
    for (int p = 0; p < n_pixels; p++) {
        for (int cw = 0; cw < ch_words; cw++) {
            data_256_t word = 0;
            for (int l = 0; l < 16; l++) {
                int c = cw * 16 + l;
                if (c < n_ch)
                    word.range(16*l+15, 16*l) = f2h(src[p * n_ch + c]);
            }
            dst[p * ch_words + cw] = word;
        }
    }
}

// Pack weights [CO, 9, CI] → dst[CO * 9 * ci_words], zero-pad beyond CI
static void pack_weights(const float* src, data_256_t* dst, int co, int ci) {
    int ci_words = (ci + 15) / 16;
    for (int o = 0; o < co; o++) {
        for (int k = 0; k < 9; k++) {
            for (int cw = 0; cw < ci_words; cw++) {
                data_256_t word = 0;
                for (int l = 0; l < 16; l++) {
                    int c = cw * 16 + l;
                    if (c < ci)
                        word.range(16*l+15, 16*l) = f2h(src[(o * 9 + k) * ci + c]);
                }
                dst[(o * 9 + k) * ci_words + cw] = word;
            }
        }
    }
}

// Pack flat params (bias/gamma/beta) with no stride issue
static void pack_flat(const float* src, data_256_t* dst, int n) {
    int nw = (n + 15) / 16;
    for (int i = 0; i < nw; i++) {
        data_256_t word = 0;
        for (int l = 0; l < 16; l++) {
            if (i*16 + l < n)
                word.range(16*l+15, 16*l) = f2h(src[i*16 + l]);
        }
        dst[i] = word;
    }
}

// Unpack: src[n_pixels * co_words] → dst[n_pixels][co], ignoring pad channels
static void unpack_stride(const data_256_t* src, float* dst, int n_pixels, int co) {
    int co_words = (co + 15) / 16;
    for (int p = 0; p < n_pixels; p++) {
        for (int cw = 0; cw < co_words; cw++) {
            data_256_t word = src[p * co_words + cw];
            for (int l = 0; l < 16; l++) {
                int c = cw * 16 + l;
                if (c < co)
                    dst[p * co + c] = h2f((uint16_t)word.range(16*l+15, 16*l).to_uint());
            }
        }
    }
}

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

static int run_ucb(int mode, const char* label,
                   int hi, int wi, int ci, int co,
                   const char* x_file, const char* w_file,
                   const char* b_file, const char* g_file, const char* be_file,
                   const char* gold_file, float tol)
{
    int ho = hi*2, wo_out = wi*2;
    int nx = hi*wi*ci, nw = co*9*ci, ny = ho*wo_out*co;
    int ci_words = (ci + 15) / 16;
    int co_words = (co + 15) / 16;
    // Buffer sizes match what HLS kernel actually reads/writes
    int wx = hi * wi * ci_words;
    int ww = co * 9 * ci_words;
    int wy = ho * wo_out * co_words;
    int wc = co_words;

    cout << "\n[" << label << "] " << hi << "x" << wi << "x" << ci
         << " -> " << ho << "x" << wo_out << "x" << co
         << "  (wx=" << wx << " ww=" << ww << " wy=" << wy << ")\n";

    vector<float> x(nx), w(nw), b(co), g(co), be(co), gold(ny);

    char path[512];
    sprintf(path, "%sio_params/%s",    DATA, x_file);
    if (!read_floats(path, x.data(),    nx))  return 1;
    sprintf(path, "%smodel_params/%s", DATA, w_file);
    if (!read_floats(path, w.data(),    nw))  return 1;
    sprintf(path, "%smodel_params/%s", DATA, b_file);
    if (!read_floats(path, b.data(),    co))  return 1;
    sprintf(path, "%smodel_params/%s", DATA, g_file);
    if (!read_floats(path, g.data(),    co))  return 1;
    sprintf(path, "%smodel_params/%s", DATA, be_file);
    if (!read_floats(path, be.data(),   co))  return 1;
    sprintf(path, "%sio_params/%s",    DATA, gold_file);
    if (!read_floats(path, gold.data(), ny))  return 1;

    vector<data_256_t> X_hls(wx), W_hls(ww), B_hls(wc), G_hls(wc), BE_hls(wc), Y_hls(wy);

    pack_stride(x.data(), X_hls.data(), hi*wi, ci);
    pack_weights(w.data(), W_hls.data(), co, ci);
    pack_flat(b.data(),  B_hls.data(), co);
    pack_flat(g.data(),  G_hls.data(), co);
    pack_flat(be.data(), BE_hls.data(), co);

    cout << "  Running HLS...\n";
    upconv_core_top(X_hls.data(), W_hls.data(), B_hls.data(),
                    G_hls.data(), BE_hls.data(), Y_hls.data(),
                    (data_t)1e-5f, mode);

    vector<float> got(ny);
    unpack_stride(Y_hls.data(), got.data(), ho*wo_out, co);

    CmpResult r = compare(got.data(), gold.data(), ny, tol);
    cout << "  " << (r.mismatch == 0 ? "PASS" : "FAIL")
         << "  max_err=" << r.max_err
         << "  rmse=" << r.rmse
         << "  mismatch=" << r.mismatch << "/" << ny << "\n";
    return (r.mismatch == 0) ? 0 : 1;
}

int main() {
    cout << "======================================\n";
    cout << " UPCONV CORE — REAL-DATA CSIM\n";
    cout << "======================================\n";

    const float TOL = 0.5f;
    int all_pass = 1;

    all_pass &= (run_ucb(MODE_UCB_0, "UCB_0 (960->480)",
                         16, 16, 960, 480,
                         "Gen_global_add_output.txt",
                         "Gen_ucb1_weight.txt", "Gen_ucb1_bias.txt",
                         "Gen_ucb1_gamma.txt",  "Gen_ucb1_beta.txt",
                         "Gen_ucb1_output.txt", TOL) == 0);

    all_pass &= (run_ucb(MODE_UCB_1, "UCB_1 (480->240)",
                         32, 32, 480, 240,
                         "Gen_ucb1_output.txt",
                         "Gen_ucb2_weight.txt", "Gen_ucb2_bias.txt",
                         "Gen_ucb2_gamma.txt",  "Gen_ucb2_beta.txt",
                         "Gen_ucb2_output.txt", TOL) == 0);

    all_pass &= (run_ucb(MODE_UCB_2, "UCB_2 (240->120)",
                         64, 64, 240, 120,
                         "Gen_ucb2_output.txt",
                         "Gen_ucb3_weight.txt", "Gen_ucb3_bias.txt",
                         "Gen_ucb3_gamma.txt",  "Gen_ucb3_beta.txt",
                         "Gen_ucb3_output.txt", TOL) == 0);

    all_pass &= (run_ucb(MODE_UCB_3, "UCB_3 (120->60)",
                         128, 128, 120, 60,
                         "Gen_ucb3_output.txt",
                         "Gen_ucb4_weight.txt", "Gen_ucb4_bias.txt",
                         "Gen_ucb4_gamma.txt",  "Gen_ucb4_beta.txt",
                         "Gen_ucb4_output.txt", TOL) == 0);

    cout << "\n======================================\n";
    cout << (all_pass ? "RESULT: ALL PASS" : "RESULT: SOME FAIL") << "\n";
    cout << "======================================\n";
    return all_pass ? 0 : 1;
}
