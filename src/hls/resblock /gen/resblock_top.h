#pragma once
#include "Core.h"

extern "C" {
void resblock_top(
    data_256_t* X,              // [1][16][16][960] - read+write (intermediate results)
    const data_256_t* W1,             // [960][3][3][960] - Weight block 1
    const data_256_t* B1,             // [960] - Bias block 1
    const data_256_t* G1,             // [960] - Gamma block 1
    const data_256_t* BE1,            // [960] - Beta block 1
    const data_256_t* W2,             // Weight block 2
    const data_256_t* B2,             // Bias block 2
    const data_256_t* G2,             // Gamma block 2
    const data_256_t* BE2,            // Beta block 2
    data_256_t* Y,              // [1][16][16][960] - Final Output
    data_t epsilon                // Small value for numerical stability
);
}
