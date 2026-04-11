#pragma once
#include "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/gen/Core.h"

extern "C" {
void resblock_top(
    data_t* X,              // [1][16][16][960]
    const data_t* W1,       // [960][3][3][960]
    const data_t* B1,       // [960]
    const data_t* G1,       // [960]
    const data_t* BE1,      // [960]
    const data_t* W2,
    const data_t* B2,
    const data_t* G2,
    const data_t* BE2,
    data_t* Y,              // [1][16][16][960]
    data_t epsilon
);
}
