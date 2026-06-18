#pragma once
#include "Core.h"

// ============================================================
// int16 quantized ResBlock — single conv+norm engine (UEK), called twice.
//
// Modes:
//   1: MODE_RB_L1  Conv3x3 -> requant -> Norm  (saves residual=input to skip_buf,
//                  output = norm1, feeds conv2. NO ReLU — matches int16 reference.)
//   2: MODE_RB_L2  Conv3x3 -> requant -> Norm -> quantized Add with skip(residual)
//
// One engine, two calls (like the fp16 fusion core). Quant scalars passed per call.
//   conv:  accum = sum_taps (x - x_zp)*w ; +bias ; q = (accum*M_conv)>>N_conv + conv_zp
//   norm:  mean/var (integer, reci_C) -> rsqrt(var+eps) -> (x-mean)*comb>>sh + beta
//   add (L2 only): y = clamp(res_zp + (norm2*M_path)>>N_path + (skip*M_skip)>>N_skip)
//          NOTE per the fixed reference: conv-path(norm2) uses n2_scale/y_scale (M_path),
//          residual(skip) uses x_scale/y_scale (M_skip).
// ============================================================

#define MODE_RB_L1  1
#define MODE_RB_L2  2

extern "C" {
void resblock_q_top(
    data_256_t*       X,    // int16 activation in  (16x16x960 packed)  [residual src in L1]
    const data_256_t* W,    // int16 weights        (960*3*3*960 packed)
    const int32_t*    B,    // int32 bias       [960]
    const int32_t*    G,    // int32 gamma-mult [960]
    const int8_t*     GS,   // int8  gamma-shift[960]
    const int32_t*    BE,   // int32 beta       [960]
    data_256_t*       Y,    // int16 activation out (16x16x960 packed)
    int M_conv, int N_conv, int x_zp, int conv_zp,   // conv requantize
    int reci_C_mult, int reci_C_shift, int epsilon,  // norm stats
    int M_path, int N_path, int M_skip, int N_skip, int res_zp,  // L2 add (unused in L1)
    int mode
);
}
