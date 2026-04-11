#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <hls_half.h>

void mul_fp16_hls(half *a,
                  half *b,
                  half *p);

int main()
{
    half a, b, p_hw;
    float p_sw;

    printf("=== HLS FP16 MULTIPLIER TB ===\n");

    a = half(1.5f);
    b = half(2.0f);
    mul_fp16_hls(&a, &b, &p_hw);
    p_sw = 1.5f * 2.0f;

    printf("Test1: %f * %f = %f\n",
           (float)a, (float)b, (float)p_hw);
    assert(fabs((float)p_hw - p_sw) < 1e-2);

    a = half(-3.25f);
    b = half(4.0f);
    mul_fp16_hls(&a, &b, &p_hw);
    p_sw = -3.25f * 4.0f;

    printf("Test2: %f * %f = %f\n",
           (float)a, (float)b, (float)p_hw);
    assert(fabs((float)p_hw - p_sw) < 1e-2);

    printf("=== ALL TESTS PASSED ===\n");
    return 0;
}
