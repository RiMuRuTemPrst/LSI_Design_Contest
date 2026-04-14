#include <iostream>
#include <vector>
#include <cmath>
#include "Core.h"

extern "C" {
void ChannelNorm_IP(
    data_256_t* X,
    const data_256_t* G,
    const data_256_t* B,
    data_t epsilon,
    data_256_t* Y
);
}

int main() {
    const int C = MODEL_C;
    const int H = MODEL_H; 
    const int W = MODEL_W;
    const int N_beats = (H * W * C) / 16;
    const int C_words = C / 16;

    std::vector<data_256_t> X(N_beats);
    std::vector<data_256_t> G(C_words);
    std::vector<data_256_t> B(C_words);
    std::vector<data_256_t> Y(N_beats);
    data_t epsilon = (data_t)0.001f;

    std::vector<float> g_float(C);
    std::vector<float> b_float(C);
    std::vector<float> x_full(H * W * C);

    // Initialize parameters
    for (int c = 0; c < C; c++) {
        g_float[c] = (float)(rand() % 20 + 5) / 10.0f;
        b_float[c] = (float)(rand() % 10) / 10.0f;
    }
    for (int w = 0; w < C_words; w++) {
        data_256_t wg = 0, wb = 0;
        for (int k = 0; k < 16; k++) {
            wg.range(16*k+15, 16*k) = half_to_bits((data_t)g_float[w*16+k]);
            wb.range(16*k+15, 16*k) = half_to_bits((data_t)b_float[w*16+k]);
        }
        G[w] = wg;
        B[w] = wb;
    }

    // Initialize input data
    for (int i = 0; i < H * W * C; i++) {
        x_full[i] = (float)(rand() % 100) / 10.0f;
    }
    for (int i = 0; i < N_beats; i++) {
        data_256_t wx = 0;
        for (int k = 0; k < 16; k++) {
            wx.range(16*k+15, 16*k) = half_to_bits((data_t)x_full[i*16+k]);
        }
        X[i] = wx;
    }

    // Run IP
    ChannelNorm_IP(X.data(), G.data(), B.data(), epsilon, Y.data());

    // Verify first pixel
    float sum = 0, sumsq = 0;
    for (int c = 0; c < C; c++) {
        sum += x_full[c];
        sumsq += x_full[c] * x_full[c];
    }
    float mean = sum / (float)C;
    float num = (float)C;
    float adj = num / (num - 1.0f);
    float var_refined = (sumsq / (num - 1.0f)) - (mean * mean * adj);
    float inv_std = 1.0f / std::sqrt(var_refined + (float)epsilon);

    int errors = 0;
    for (int c = 0; c < C; c++) {
        data_t y_val = bits_to_half<data_t>(Y[c/16].range(16*(c%16)+15, 16*(c%16)));
        float expected = (x_full[c] - mean) * inv_std * g_float[c] + b_float[c];
        
        float diff = std::abs((float)y_val - expected);
        if (diff > 0.1f) {
            errors++;
            if (errors < 5) {
                std::cout << "Error at p0, c" << c << ": Out=" << (float)y_val 
                          << ", Expected=" << expected << std::endl;
            }
        }
    }

    if (errors == 0) {
        std::cout << "ChannelNorm C-Sim: PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "ChannelNorm C-Sim: FAILED with " << errors << " errors in first pixel." << std::endl;
        return 1;
    }
}
