#pragma once
// Minimal Core.h for standalone Conv77 HLS compilation
#include "ap_int.h"
#include "ap_fixed.h"
#include "hls_half.h"   // required for half type in synthesis (fusion_core/Core.h does the same)

typedef ap_uint<256> data_256_t;
