#include <ap_int.h>

/*
 * HLS IP: 32-bit Adder
 * AXI4-Lite interface
 */
void adder32(ap_uint<32> a,
             ap_uint<32> b,
             ap_uint<32> *sum)
{
#pragma HLS INTERFACE s_axilite port=a     bundle=CTRL
#pragma HLS INTERFACE s_axilite port=b     bundle=CTRL
#pragma HLS INTERFACE s_axilite port=sum   bundle=CTRL
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL

    *sum = a + b;
}

