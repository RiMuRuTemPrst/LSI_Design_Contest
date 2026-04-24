#pragma once
#include "Core.h"

// ============================================================
// Fusion Core — single IP core for Generator blocks
//
// Modes:
//   0: MODE_CBI   (Norm-on-load -> Conv -> Norm -> Bypass)
//   1: MODE_RB_L1 (Plain load -> Conv -> Norm -> ReLU)
//   2: MODE_RB_L2 (Plain load -> Conv -> Norm -> Add)
// ============================================================

#define MODE_CBI   0
#define MODE_RB_L1 1
#define MODE_RB_L2 2

extern "C" {
void fusion_core_top(
    data_256_t*       X,      // data_inout
    const data_256_t* W,      // weights
    const data_256_t* B,      // bias
    const data_256_t* G,      // gamma
    const data_256_t* BE,     // beta
    const data_256_t* G_IN,   // cbi_in_gamma
    const data_256_t* BE_IN,  // cbi_in_beta
    data_256_t*       Y,      // output
    data_t            epsilon,
    int               mode
);
}
