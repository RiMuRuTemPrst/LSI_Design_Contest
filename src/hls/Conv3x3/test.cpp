#include <iostream>
#include <vector>
#include <cmath>
#include "Core.h"

extern "C" {
void Conv3x3_IP(
    data_256_t* X,
    const data_256_t* W,
    const data_256_t* B,
    data_t epsilon,
    data_256_t* Y
);
}

int main() {
    const int CI = MODEL_C;
    const int CO = MODEL_C;
    const int H = MODEL_H;
    const int W = MODEL_W;
    const int KH = 3;
    const int KW = 3;

    std::vector<data_256_t> X((H * W * CI) / 16);
    std::vector<data_256_t> Weight((CO * KH * KW * CI) / 16);
    std::vector<data_256_t> Bias(CO / 16);
    std::vector<data_256_t> Y((H * W * CO) / 16);

    // Initialize with something predictable
    for (size_t i = 0; i < X.size(); i++) X[i] = 0;
    for (size_t i = 0; i < Weight.size(); i++) Weight[i] = 0;
    for (size_t i = 0; i < Bias.size(); i++) Bias[i] = 0;

    // Set one weight and one input to test
    // W[co=0, kh=1, kw=1, ci=0] = 1.0
    // Weight is (CO, KH, KW, CI)
    // idx = (((co * KH + kh) * KW + kw) * CI + ci) / 16
    int w_idx = (((0 * 3 + 1) * 3 + 1) * CI + 0) / 16;
    Weight[w_idx].range(15, 0) = half_to_bits((data_t)1.0f);

    // X[h=1, w=1, ci=0] = 5.0
    int x_idx = ((1 * W + 1) * CI + 0) / 16;
    X[x_idx].range(15, 0) = half_to_bits((data_t)5.0f);

    // Run IP
    Conv3x3_IP(X.data(), Weight.data(), Bias.data(), (data_t)0.001f, Y.data());

    // Verify Y[h=1, w=1, co=0] should be 5.0 (since weight at 1,1 is 1.0)
    int y_idx = ((1 * W + 1) * CO + 0) / 16;
    data_t y_val = bits_to_half<data_t>(Y[y_idx].range(15, 0));

    std::cout << "Conv3x3 Test: Y[1,1,0] = " << (float)y_val << std::endl;

    if (std::abs((float)y_val - 5.0f) < 0.1f) {
        std::cout << "Conv3x3 C-Sim: PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "Conv3x3 C-Sim: FAILED!" << std::endl;
        return 1;
    }
}
