#pragma once
#include "Core.h"

extern "C" {
void conv77_core_top(
    DDR_CONST_PTR     X,   // input  256×256×60  packed as [H][W][CI_WORDS=4]
    const data_256_t* W,   // weights [C_OUT=3][49][CI_WORDS=4]
    const data_256_t* B,   // bias — 3 half values in bits [47:0] of word 0
    DDR_PTR           Y    // output [H][W][1] — 3 half values in bits [47:0] per pixel
);
}
