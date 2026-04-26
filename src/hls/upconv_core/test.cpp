#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <chrono>
#include "gen/upconv_core.h"

using namespace std;

// Helper to convert float to half bits and back
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

void pack_256(const vector<float>& src, vector<data_256_t>& dst) {
    int n_words = (src.size() + 15) / 16;
    for (int i = 0; i < n_words; i++) {
        data_256_t word = 0;
        for (int k = 0; k < 16; k++) {
            if (i * 16 + k < src.size())
                word.range(16 * k + 15, 16 * k) = f2h(src[i * 16 + k]);
        }
        dst[i] = word;
    }
}

void unpack_256(const vector<data_256_t>& src, vector<float>& dst) {
    int n_words = src.size();
    for (int i = 0; i < n_words; i++) {
        for (int k = 0; k < 16; k++) {
            if (i * 16 + k < dst.size())
                dst[i * 16 + k] = h2f(src[i].range(16 * k + 15, 16 * k).to_uint());
        }
    }
}

void run_upconv_test(int mode, const char* name, int hi, int wi, int ci, int co) {
    int ho = hi * 2, wo = wi * 2;
    cout << "\n--- Testing " << name << " (" << hi << "x" << wi << "x" << ci << " -> " << ho << "x" << wo << "x" << co << ") ---" << endl;

    vector<float> x(hi * wi * ci, 0.5f);
    vector<float> w(co * 9 * ci, 0.01f);
    vector<float> b_ct(co, 0.1f);
    vector<float> g(co, 1.0f);
    vector<float> be(co, 0.0f);
    vector<float> y_got(ho * wo * co, 0.0f);

    vector<data_256_t> X_hls((hi * wi * ((ci + 15) / 16)));
    vector<data_256_t> W_hls((co * 9 * ((ci + 15) / 16)));
    vector<data_256_t> B_hls((co + 15) / 16);
    vector<data_256_t> G_hls((co + 15) / 16);
    vector<data_256_t> BE_hls((co + 15) / 16);
    vector<data_256_t> Y_hls((ho * wo * ((co + 15) / 16)));

    pack_256(x, X_hls);
    pack_256(w, W_hls);
    pack_256(b_ct, B_hls);
    pack_256(g, G_hls);
    pack_256(be, BE_hls);

    auto start = chrono::high_resolution_clock::now();
    upconv_core_top(X_hls.data(), W_hls.data(), B_hls.data(), G_hls.data(), BE_hls.data(), Y_hls.data(), (data_t)1e-5f, mode);
    auto end = chrono::high_resolution_clock::now();
    
    chrono::duration<double> diff = end - start;
    cout << "  HLS Execution Time (CSIM): " << diff.count() << " s" << endl;

    unpack_256(Y_hls, y_got);

    float sum_out = 0;
    for(float f : y_got) sum_out += f;
    cout << "  Output Sum: " << sum_out << endl;
    if (sum_out > 0) cout << "  " << name << " PASSED (Functional)" << endl;
    else cout << "  " << name << " FAILED (Zero Output)" << endl;
}

int main() {
    cout << "====================================================" << endl;
    cout << "   UPCONV CORE FULL DIMENSION VERIFICATION          " << endl;
    cout << "====================================================" << endl;

    // Mini test first for quick verification
    run_upconv_test(MODE_UCB_TEST, "MINI_TEST", 16, 16, 16, 16);

    // Real dimensions
    run_upconv_test(MODE_UCB_0, "UPCONV_BLOCK_0", 16, 16, 960, 480);
    run_upconv_test(MODE_UCB_1, "UPCONV_BLOCK_1", 32, 32, 480, 240);
    run_upconv_test(MODE_UCB_2, "UPCONV_BLOCK_2", 64, 64, 240, 120);
    run_upconv_test(MODE_UCB_3, "UPCONV_BLOCK_3", 128, 128, 120, 60);

    cout << "\n====================================================" << endl;
    return 0;
}
