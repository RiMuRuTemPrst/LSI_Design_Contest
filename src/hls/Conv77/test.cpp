#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <vector>
using namespace std;

#include "gen/Core.h"  // data_256_t = ap_uint<256>

// Forward declaration (defined in Hard_op_3.cpp → Hls_Layers_Conv77.tpp)
void HW_Conv7x7(const data_256_t* X, const data_256_t* W,
                const data_256_t* B, data_256_t* Z);

// ============================================================
// BIT HELPERS (software simulation)
// ============================================================
static uint16_t float_to_half_bits(float v) {
    half h = (half)v;
    uint16_t bits;
    memcpy(&bits, &h, 2);
    return bits;
}

static float half_bits_to_float(uint16_t bits) {
    half h;
    memcpy(&h, &bits, 2);
    return (float)h;
}

// ============================================================
// PACK: (n_pixels, C_IN) float → (n_pixels * CI_WORDS) data_256_t
// Channel-padded: C_IN values per pixel → C_PAD (next mult of 16), zeros for ci>=C_IN
// Word layout: bits[k*16+15 : k*16] = half(src[ci]) where k = ci%16
// ============================================================
static void pack_channel_padded(const float* src, data_256_t* dst,
                                 int n_pixels, int C_IN, int C_PAD) {
    int CI_WORDS = (C_PAD + 15) / 16;
    for (int pix = 0; pix < n_pixels; pix++) {
        for (int ciw = 0; ciw < CI_WORDS; ciw++) {
            data_256_t word = 0;
            for (int k = 0; k < 16; k++) {
                int ci = ciw * 16 + k;
                if (ci < C_IN) {
                    uint16_t bits = float_to_half_bits(src[pix * C_IN + ci]);
                    word.range(k*16+15, k*16) = bits;
                }
            }
            dst[pix * CI_WORDS + ciw] = word;
        }
    }
}

// Pack bias: C_OUT values → 1 word, bits[co*16+15:co*16] = half(B[co])
static void pack_bias(const float* src, data_256_t* dst, int C_OUT) {
    data_256_t word = 0;
    for (int co = 0; co < C_OUT; co++) {
        uint16_t bits = float_to_half_bits(src[co]);
        word.range(co*16+15, co*16) = bits;
    }
    dst[0] = word;
}

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

int main() {
    const int H = 256, W_IMG = 256;
    const int C_IN = 60, C_OUT = 3, H_R = 7, W_R = 7;
    const int C_PAD = 64, CI_WORDS = 4;  // C_PAD/16 = 4
    const int N_PIX  = H * W_IMG;        // 65536
    const int W_VALS = C_OUT * H_R * W_R * C_IN;  // 8820

    vector<float> X_f(N_PIX * C_IN);
    vector<float> W_f(W_VALS);
    vector<float> B_f(C_OUT);
    vector<float> gold(N_PIX * C_OUT);

    // X: 262144 words (N_PIX * CI_WORDS)
    vector<data_256_t> X_p(N_PIX * CI_WORDS);
    // W: 588 words (C_OUT*H_R*W_R * CI_WORDS)
    vector<data_256_t> W_p(C_OUT * H_R * W_R * CI_WORDS);
    vector<data_256_t> B_p(1);
    // Z: 65536 words (1 pixel/word)
    vector<data_256_t> Z_p(N_PIX, data_256_t(0));

    cout << "Loading data...\n";
    // Vitis HLS csim copies tb files flat into csim/build/ — use bare filenames
    if (!read_floats("Gen_ucb4_Relu_output_0.txt",  X_f.data(), N_PIX * C_IN))  return 1;
    if (!read_floats("Gen_cbo_weight.txt",           W_f.data(), W_VALS))        return 1;
    if (!read_floats("Gen_cbo_bias.txt",             B_f.data(), C_OUT))         return 1;
    if (!read_floats("Gen_cbo_Conv_output_0.txt",    gold.data(), N_PIX * C_OUT)) return 1;

    // Pack X: NHWC (H,W,C_IN) → channel-padded (N_PIX, CI_WORDS)
    pack_channel_padded(X_f.data(), X_p.data(), N_PIX, C_IN, C_PAD);

    // Pack W: (C_OUT*H_R*W_R, C_IN) → channel-padded
    pack_channel_padded(W_f.data(), W_p.data(), C_OUT * H_R * W_R, C_IN, C_PAD);

    // Pack B: 3 values → 1 word
    pack_bias(B_f.data(), B_p.data(), C_OUT);

    cout << "Running HW_Conv7x7...\n";
    HW_Conv7x7(X_p.data(), W_p.data(), B_p.data(), Z_p.data());

    // Unpack Z and verify: Z[h*W+w].bits[co*16+15:co*16] = half(output[h][w][co])
    const float TOL = 1.0f;  // ap_fixed<16,8> over 2940 MACs: max_err ~0.5
    int mismatch = 0;
    float max_err = 0.0f;
    double sum_sq_err = 0.0;
    const int OUT_SIZE = N_PIX * C_OUT;

    for (int h = 0; h < H; h++) {
        for (int w = 0; w < W_IMG; w++) {
            data_256_t z_word = Z_p[h * W_IMG + w];
            for (int co = 0; co < C_OUT; co++) {
                uint16_t bits = (uint16_t)z_word.range(co*16+15, co*16).to_uint();
                float out_val  = half_bits_to_float(bits);
                float gold_val = gold[(h * W_IMG + w) * C_OUT + co];
                float err = fabs(out_val - gold_val);
                if (err > max_err) max_err = err;
                sum_sq_err += (double)err * err;
                if (err > TOL) mismatch++;
            }
        }
    }

    float rmse = (float)sqrt(sum_sq_err / OUT_SIZE);
    cout << "Verification: max_err=" << max_err
         << "  rmse=" << rmse
         << "  mismatch(>" << TOL << ")=" << mismatch << "/" << OUT_SIZE << "\n";

    if (mismatch > 0) {
        cerr << "RESULT: FAIL\n";
        return 1;
    }
    cout << "RESULT: PASS\n";
    return 0;
}
