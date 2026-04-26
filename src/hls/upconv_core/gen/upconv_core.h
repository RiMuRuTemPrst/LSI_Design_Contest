#pragma once
#include "Core.h"

// Mode definitions for 4 UpConv blocks
#define MODE_UCB_0  0   // ConvTranspose + Norm + ReLU  [960→480, 16×16→32×32]
#define MODE_UCB_1  1   // ConvTranspose + Norm + ReLU  [480→240, 32×32→64×64]
#define MODE_UCB_2  2   // ConvTranspose + Norm + ReLU  [240→120, 64×64→128×128]
#define MODE_UCB_3  3   // ConvTranspose + Norm + ReLU  [120→60,  128×128→256×256]
#define MODE_UCB_TEST 4 // [16→16, 16×16→32×32]

extern "C" {
void upconv_core_top(
    DDR_CONST_PTR X,      // data_in
    const data_256_t* W,  // weights (ConvTranspose)
    const data_256_t* B,  // bias (Norm)
    const data_256_t* G,  // gamma (Norm)
    const data_256_t* BE, // beta (Norm)
    DDR_PTR       Y,      // data_out
    data_t        epsilon,
    int           mode
);
}
