#pragma once
#include "Core.h"

extern "C" {
void conv_only_top(
    data_256_t* X,              // [1][16][16][960] - input activation (read only)
    const data_256_t* W1,       // [960][3][3][960] - Conv1 weights
    const data_256_t* B1,       // [960]            - Conv1 bias
    data_256_t* Y,              // [1][16][16][960] - raw Conv1 + bias output
    data_t epsilon              // kept for interface parity (unused in conv-only)
);
}
