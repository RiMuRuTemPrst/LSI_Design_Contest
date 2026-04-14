#include <iostream>
#include <vector>
#include <cmath>
#include "Core.h"

extern "C" {
void Add_IP(data_256_t* X1, data_256_t* X2, data_256_t* Y, int N);
}

int main() {
    const int N = 100;
    std::vector<data_256_t> X1(N);
    std::vector<data_256_t> X2(N);
    std::vector<data_256_t> Y(N);

    for (int i = 0; i < N; i++) {
        data_256_t w1 = 0, w2 = 0;
        for (int k = 0; k < 16; k++) {
            data_t v1 = (data_t)((float)(rand() % 100) / 10.0f);
            data_t v2 = (data_t)((float)(rand() % 100) / 10.0f);
            w1.range(16*k+15, 16*k) = half_to_bits(v1);
            w2.range(16*k+15, 16*k) = half_to_bits(v2);
        }
        X1[i] = w1;
        X2[i] = w2;
    }

    Add_IP(X1.data(), X2.data(), Y.data(), N);

    int errors = 0;
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < 16; k++) {
            data_t v1 = bits_to_half<data_t>(X1[i].range(16*k+15, 16*k));
            data_t v2 = bits_to_half<data_t>(X2[i].range(16*k+15, 16*k));
            data_t vy = bits_to_half<data_t>(Y[i].range(16*k+15, 16*k));
            data_t expected = v1 + v2;
            
            float diff = std::abs((float)vy - (float)expected);
            if (diff > 0.01f) {
                errors++;
                if (errors < 10) {
                    std::cout << "Error at [" << i << "][" << k << "]: " << (float)v1 << " + " << (float)v2 
                              << " = " << (float)vy << " (Expected " << (float)expected << ")" << std::endl;
                }
            }
        }
    }

    if (errors == 0) {
        std::cout << "Add C-Sim: PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "Add C-Sim: FAILED with " << errors << " errors." << std::endl;
        return 1;
    }
}
