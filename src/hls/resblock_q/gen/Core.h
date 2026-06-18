#ifndef RESBLOCK_Q_CORE_H
#define RESBLOCK_Q_CORE_H

// =========================================================================
// int16 quantized ResBlock — FULL SIZE 16x16x960 (HiFiC Generator latent)
// =========================================================================
#define MODEL_H 16
#define MODEL_W 16
#define MODEL_C 960

// data_256_t packs 16 x int16 per 256-bit AXI word. No wide ap_uint stream here
// (int16 design drops the fp16 dataflow split), but keep the guard harmless.
#ifndef AP_INT_MAX_W
#define AP_INT_MAX_W 2048
#endif
#include <ap_int.h>
#include <stdint.h>

typedef int16_t       data_t;      // int16 activation / weight
typedef ap_uint<256>  data_256_t;  // 16 x int16 packed

typedef data_256_t*       DDR_PTR;
typedef const data_256_t* DDR_CONST_PTR;

#endif
