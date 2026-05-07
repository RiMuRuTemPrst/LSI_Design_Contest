#ifndef TENSOR_MEM_H
#define TENSOR_MEM_H

#include <ap_int.h>
#include <hls_half.h>

typedef half data_t;
typedef ap_uint<256> data_256_t;

typedef data_256_t*       DDR_PTR;
typedef const data_256_t* DDR_CONST_PTR;

#endif
