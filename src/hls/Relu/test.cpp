#include <iostream>
#include <vector>
#include <cmath>
#include "Core.h"

extern "C" {
void Relu_IP(data_256_t* X, data_256_t* Y, int N);
}

int main() {
    const int N = 100; // 100 beats of 256-bit
    std::vector<data_256_t> X(N);
    std::vector<data_256_t> Y(N);

    // Initialize with random values (some negative)
    for (int i = 0; i < N; i++) {
        data_256_t word = 0;
        for (int k = 0; k < 16; k++) {
            float val = (float)(rand() % 200 - 100) / 10.0f;
            data_t h_val = (data_t)val;
            word.range(16*k+15, 16*k) = half_to_bits(h_val);
        }
        X[i] = word;
    }

    // Run IP
    Relu_IP(X.data(), Y.data(), N);

    // Verify
    int errors = 0;
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < 16; k++) {
            ap_uint<16> x_bits = X[i].range(16*k+15, 16*k);
            ap_uint<16> y_bits = Y[i].range(16*k+15, 16*k);
            data_t x_val = bits_to_half<data_t>(x_bits);
            data_t y_val = bits_to_half<data_t>(y_bits);
            
            data_t expected = (x_val < (data_t)0) ? (data_t)0 : x_val;
            if (y_val != expected) {
                errors++;
                if (errors < 10) {
                    std::cout << "Error at [" << i << "][" << k << "]: In=" << (float)x_val 
                              << ", Out=" << (float)y_val << ", Expected=" << (float)expected << std::endl;
                }
            }
        }
    }

    if (errors == 0) {
        std::cout << "Relu C-Sim: PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "Relu C-Sim: FAILED with " << errors << " errors." << std::endl;
        return 1;
    }
}
