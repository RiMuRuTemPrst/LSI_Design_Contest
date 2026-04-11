#include <stdint.h>
#include <hls_half.h>   // half (FP16)

// =================================================
// Top function: FP16 x FP16 -> FP16
// =================================================
void mul_fp16_hls(half *a,
                  half *b,
                  half *p)
{
#pragma HLS INTERFACE s_axilite port=a bundle=CTRL
#pragma HLS INTERFACE s_axilite port=b bundle=CTRL
#pragma HLS INTERFACE s_axilite port=p bundle=CTRL
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL

#pragma HLS PIPELINE II=1

    half va = *a;
    half vb = *b;
    half vp = va * vb;

    *p = vp;
}
