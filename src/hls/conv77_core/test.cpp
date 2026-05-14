#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <chrono>
#include "gen/conv77_core.h"

using namespace std;

static uint16_t f2h(float f) {
    data_t h = (data_t)f;
    uint16_t bits;
    memcpy(&bits, &h, 2);
    return bits;
}

static float h2f(uint16_t bits) {
    data_t h;
    memcpy(&h, &bits, 2);
    return (float)h;
}

static bool load_floats(const char* path, vector<float>& out, int expected) {
    ifstream fin(path);
    if (!fin.is_open()) {
        cerr << "ERROR: cannot open " << path << "\n";
        return false;
    }
    out.resize(expected);
    for (int i = 0; i < expected; i++) {
        if (!(fin >> out[i])) {
            cerr << "ERROR: premature EOF at " << path << " (got " << i << "/" << expected << ")\n";
            return false;
        }
    }
    return true;
}

int main() {
    const int H = 256, W = 256, C_IN = 60, C_OUT = 3;
    const int CI_WORDS = 4;  // ceil(60/16)
    const int KR = 7;
    const int N_X  = H * W * C_IN;       // 3,932,160
    const int N_W  = C_OUT * KR * KR * C_IN; // 8,820
    const int N_B  = C_OUT;               // 3
    const int N_Y  = H * W * C_OUT;      // 196,608

    // CWD when Vitis HLS CSIM runs = prj/solution1/csim/build/ (7 levels from project root)
    const char* X_PATH    = "../../../../../../../assets/test_data/io_params/Gen_ucb4_output.txt";
    const char* W_PATH    = "../../../../../../../assets/test_data/model_params/Gen_cbo_weight.txt";
    const char* B_PATH    = "../../../../../../../assets/test_data/model_params/Gen_cbo_bias.txt";
    const char* GOLD_PATH = "../../../../../../../assets/test_data/io_params/Gen_conv77_output.txt";

    cout << "Loading test data...\n";

    vector<float> x_f, w_f, b_f;
    if (!load_floats(X_PATH, x_f, N_X)) return 1;
    if (!load_floats(W_PATH, w_f, N_W)) return 1;
    if (!load_floats(B_PATH, b_f, N_B)) return 1;
    cout << "  Data loaded.\n";

    // Pack X: [H][W][C_IN] NHWC -> X_hls[(h*W+w)*CI_WORDS+ci_w]
    vector<data_256_t> X_hls(H * W * CI_WORDS, 0);
    for (int h = 0; h < H; h++)
        for (int w_idx = 0; w_idx < W; w_idx++)
            for (int ci_w = 0; ci_w < CI_WORDS; ci_w++) {
                data_256_t word = 0;
                for (int l = 0; l < 16; l++) {
                    int c = ci_w * 16 + l;
                    float v = (c < C_IN) ? x_f[(h * W + w_idx) * C_IN + c] : 0.0f;
                    word.range(16*l+15, 16*l) = f2h(v);
                }
                X_hls[(h * W + w_idx) * CI_WORDS + ci_w] = word;
            }

    // Pack W: [C_OUT][kh][kw][C_IN] -> W_hls[(co*49+k)*CI_WORDS+ci_w]
    vector<data_256_t> W_hls(C_OUT * 49 * CI_WORDS, 0);
    for (int co = 0; co < C_OUT; co++)
        for (int kh = 0; kh < KR; kh++)
            for (int kw = 0; kw < KR; kw++) {
                int k = kh * KR + kw;
                for (int ci_w = 0; ci_w < CI_WORDS; ci_w++) {
                    data_256_t word = 0;
                    for (int l = 0; l < 16; l++) {
                        int c = ci_w * 16 + l;
                        float v = (c < C_IN)
                            ? w_f[((co * KR + kh) * KR + kw) * C_IN + c]
                            : 0.0f;
                        word.range(16*l+15, 16*l) = f2h(v);
                    }
                    W_hls[(co * 49 + k) * CI_WORDS + ci_w] = word;
                }
            }

    // Pack B: 3 bias values packed in 1 word
    vector<data_256_t> B_hls(1, 0);
    for (int co = 0; co < C_OUT; co++)
        B_hls[0].range(16*co+15, 16*co) = f2h(b_f[co]);

    vector<data_256_t> Y_hls(H * W, 0);

    cout << "Running CSIM...\n";
    auto t0 = chrono::high_resolution_clock::now();
    conv77_core_top(X_hls.data(), W_hls.data(), B_hls.data(), Y_hls.data());
    auto t1 = chrono::high_resolution_clock::now();
    cout << "  CSIM time: " << chrono::duration<double>(t1 - t0).count() << " s\n";

    // Unpack output: [H][W][C_OUT]
    vector<float> y_got(N_Y, 0.0f);
    for (int h = 0; h < H; h++)
        for (int w_idx = 0; w_idx < W; w_idx++) {
            data_256_t word = Y_hls[h * W + w_idx];
            for (int co = 0; co < C_OUT; co++)
                y_got[(h * W + w_idx) * C_OUT + co] =
                    h2f(word.range(16*co+15, 16*co).to_uint());
        }

    // Verify against gold reference
    ifstream gold(GOLD_PATH);
    if (!gold.is_open()) {
        cout << "WARN: gold file not found, skipping verification\n";
        return 0;
    }

    const float TOL = 0.5f;
    int mismatch = 0;
    float max_err = 0.0f;
    double sum_sq = 0.0;
    for (int i = 0; i < N_Y; i++) {
        float ref;
        gold >> ref;
        float err = fabsf(y_got[i] - ref);
        if (err > max_err) max_err = err;
        sum_sq += (double)err * err;
        if (err > TOL) mismatch++;
    }
    float rmse = (float)sqrt(sum_sq / N_Y);

    cout << "max_err=" << max_err << "  rmse=" << rmse
         << "  mismatch(>" << TOL << ")=" << mismatch << "/" << N_Y << "\n";

    if (mismatch > 0) {
        cerr << "RESULT: FAIL\n";
        return 1;
    }
    cout << "RESULT: PASS\n";
    return 0;
}
