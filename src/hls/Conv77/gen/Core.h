#pragma once
// Minimal Core.h for standalone Conv77 HLS compilation
// Provides data_256_t type consistent with full_generator's fusion_core/gen/Core.h
#include "ap_int.h"
#include "ap_fixed.h"

typedef ap_uint<256> data_256_t;
