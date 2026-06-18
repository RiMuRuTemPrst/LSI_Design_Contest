#ifndef TENSOR_MEM_H
#define TENSOR_MEM_H

// =========================================================================
// RUN MODE CONFIGURATION
// =========================================================================
// FULL SIZE for Generator Latent Space (16x16x960)
#define MODEL_H 16
#define MODEL_W 16
#define MODEL_C 960

// =========================================================================
// AUTOMATIC PARAMETER DEFINITIONS
// =========================================================================
#define DEPTH_X       (1 * MODEL_H * MODEL_W * MODEL_C)
#define DEPTH_W       (MODEL_C * 3 * 3 * MODEL_C)
#define DEPTH_B       (MODEL_C)

#define PACK_256      16
#define DEPTH_X_WIDE  (DEPTH_X / PACK_256)
#define DEPTH_W_WIDE  (DEPTH_W / PACK_256)
#define DEPTH_B_WIDE  (DEPTH_B / PACK_256)

// [A]+[B] Fusion dataflow streams a per-co psum word of ap_uint<PEs*128>=2048 bits.
// csim's software ap_int model caps at 1024 unless AP_INT_MAX_W is raised before <ap_int.h>.
// Synth is unaffected; this only fixes the C-sim build.
#ifndef AP_INT_MAX_W
#define AP_INT_MAX_W 4096
#endif
#include <ap_int.h>
#include <hls_half.h>

typedef half data_t;
typedef ap_uint<256> data_256_t;

typedef data_256_t*       DDR_PTR;
typedef const data_256_t* DDR_CONST_PTR;

#endif
