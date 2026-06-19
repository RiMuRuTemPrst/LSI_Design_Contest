// MAC cost micro-benchmark: 1 op/kernel, return-by-value (ap_return port, no memory iface).
// Đo DSP/LUT/FF của: nhân, cộng, và MAC (a*b+c) cho fp32/fp16/fp8/int16/int8.
#include <ap_int.h>
#include "hls_half.h"

// ---- fp8 E4M3 multiply -> half (copy từ Hls_Layers_Fp8.tpp) ----
static inline half fp8_mul_to_half(ap_uint<8> a, ap_uint<8> b) {
#pragma HLS INLINE
    ap_uint<1> sa=a[7], sb=b[7];
    ap_uint<4> ea=a.range(6,3), eb=b.range(6,3);
    ap_uint<3> ma=a.range(2,0), mb=b.range(2,0);
    bool az=(ea==0), bz=(eb==0);
    ap_uint<4> Ma = ap_uint<4>((ap_uint<4>(0x8)) | ma);
    ap_uint<4> Mb = ap_uint<4>((ap_uint<4>(0x8)) | mb);
    ap_uint<8> prod = Ma * Mb;
    ap_uint<1> sr = sa ^ sb;
    ap_uint<16> bits;
    if (az || bz) { bits = ap_uint<16>(sr) << 15; }
    else {
        ap_uint<1> hi = prod[7];
        int lz = hi ? 7 : 6;
        int e16 = lz + (int)ea + (int)eb - 5;
        if (e16 < 1)  e16 = 1;
        if (e16 > 30) e16 = 30;
        ap_uint<8> frac = hi ? ap_uint<8>(prod - 128) : ap_uint<8>(prod - 64);
        ap_uint<10> m16 = hi ? ap_uint<10>(frac << 3) : ap_uint<10>(frac << 4);
        bits = (ap_uint<16>(sr) << 15) | (ap_uint<16>(e16 & 0x1F) << 10) | ap_uint<16>(m16);
    }
    union { unsigned short i; half f; } u; u.i = bits.to_uint(); return u.f;
}

// ===================== MULTIPLY ONLY =====================
float       mul_fp32 (float a, float b)                 { return a * b; }
float       mul_fp16 (half a, half b)                   { return (float)a * (float)b; }
half        mul_fp8  (ap_uint<8> a, ap_uint<8> b)       { return fp8_mul_to_half(a, b); }
ap_int<32>  mul_int16(ap_int<16> a, ap_int<16> b)       { return (ap_int<32>)a * (ap_int<32>)b; }
ap_int<16>  mul_int8 (ap_int<8>  a, ap_int<8>  b)       { return (ap_int<16>)a * (ap_int<16>)b; }

// ===================== ADD ONLY =====================
float       add_fp32 (float a, float b)                 { return a + b; }
float       add_fp16 (half a, half b)                   { return (float)a + (float)b; }
half        add_fp8  (ap_uint<8> a, ap_uint<8> b)       { return (float)(fp8_mul_to_half(a, 1) + fp8_mul_to_half(b, 1)); }
ap_int<32>  add_int32(ap_int<32> a, ap_int<32> b)       { return a + b; }
ap_int<16>  add_int16(ap_int<16> a, ap_int<16> b)       { return (ap_int<16>)(a + b); }

// ===================== MAC: a*b + c =====================
float       mac_fp32 (float a, float b, float c)                  { return a * b + c; }
float       mac_fp16 (half a, half b, float c)                    { return (float)a * (float)b + c; }
half        mac_fp8  (ap_uint<8> a, ap_uint<8> b, half c)         { return (half)(fp8_mul_to_half(a, b) + c); }
ap_int<32>  mac_int16(ap_int<16> a, ap_int<16> b, ap_int<32> c)   { return (ap_int<32>)a * (ap_int<32>)b + c; }
ap_int<32>  mac_int8 (ap_int<8>  a, ap_int<8>  b, ap_int<32> c)   { return (ap_int<32>)a * (ap_int<32>)b + c; }
