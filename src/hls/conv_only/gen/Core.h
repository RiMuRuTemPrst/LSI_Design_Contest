#ifndef TENSOR_MEM_H
#define TENSOR_MEM_H

// =========================================================================
// RUN MODE CONFIGURATION
// =========================================================================
// Uncomment the line below to run MINI TEST (16 channels)
// Comment it out to run FULL SIZE (960 channels)
#ifndef ENABLE_MINI_TEST
#define ENABLE_MINI_TEST  0
#endif

// =========================================================================
// AUTOMATIC PARAMETER DEFINITIONS
// =========================================================================
#if ENABLE_MINI_TEST
    #define MODEL_H      16
    #define MODEL_W      16
    #define MODEL_C      16
    #define MODEL_BLOCK  16

    #define DEPTH_X_VAL  4096
    #define DEPTH_W_VAL  2304
    #define DEPTH_B_VAL  16
#else
    #define MODEL_H      16
    #define MODEL_W      16
    #define MODEL_C      960
    #define MODEL_BLOCK  120

    #define DEPTH_X_VAL  245760
    #define DEPTH_W_VAL  8294400
    #define DEPTH_B_VAL  960
#endif

// =========================================================================
// WIDE ACCESS: 256-bit packing (matches ConvTranspose pattern)
// 256-bit / 16-bit(half) = 16 FP16 elements per beat
// =========================================================================
#define PACK_256  16

// Depth calculated in 256-bit words for m_axi pragma
#define DEPTH_X_WIDE   (DEPTH_X_VAL / PACK_256)
#define DEPTH_W_WIDE   (DEPTH_W_VAL / PACK_256)
#define DEPTH_B_WIDE   (DEPTH_B_VAL / PACK_256)

// PSUM_PACKED: 128-bit words, one per output element (N×H×W×C)
#define DEPTH_PSUM_PACKED  (MODEL_H * MODEL_W * MODEL_C)

// =========================================================================
// LIBRARIES
// =========================================================================
#include <hls_half.h>
#include <hls_stream.h>
#include <ap_int.h>

typedef half data_t;

// =========================================================================
// 256-BIT UNIVERSAL TYPE
// Available in both __SYNTHESIS__ and simulation.
// Represents one AXI beat = 16 × FP16 values packed into 256 bits.
// All inter-block on-chip data transfers use this type.
// =========================================================================
typedef ap_uint<256> data_256_t;

// =========================================================================
// DDR pointer types: ap_uint<256>*
// =========================================================================
#define DDR_PTR        data_256_t*
#define DDR_CONST_PTR  const data_256_t*


#if defined(__SYNTHESIS__)
    #define HLS_INLINE_PRAGMA _Pragma("HLS INLINE")
#else
    #define HLS_INLINE_PRAGMA
    #include <iostream>
    #include <vector>
    #include <cmath>
    #include <cassert>
#endif

// Shape class to store tensor dimensions (N, H, W, C)
struct Shape {
    int N, H, W, C;
    Shape() : N(0), H(0), W(0), C(0) {}
    Shape(int n, int h, int w, int c) : N(n), H(h), W(w), C(c) {}
    bool operator==(const Shape &o) const { return N==o.N && H==o.H && W==o.W && C==o.C; }
};

// =========================================================================
// CLASS TENSOR_MEM: A lightweight wrapper for tensor data and its shape
// =========================================================================
template <typename T>
class TensorMem {
private:
    T* data;
    bool own_memory;

    // Helper to calculate flattened index for (n, h, w, c)
    inline int index(int n, int h, int w, int c) const {
        #pragma HLS INLINE
        return ((n * shape.H + h) * shape.W + w) * shape.C + c;
    }
public:
    Shape shape;
 
    // Default constructor
    TensorMem() : data(nullptr), own_memory(false) {
        #pragma HLS INLINE
    }
 
    // Constructor wrapping an existing pointer
    TensorMem(T* data, const Shape &shape, bool own)
        : data(data), shape(shape), own_memory(own) {
        #pragma HLS INLINE
    }
 
    // Constructor allocating new memory (Simulation only)
    TensorMem(const Shape &shape) : shape(shape), own_memory(true) {
        #ifndef __SYNTHESIS__
        data = new T[shape.N * shape.H * shape.W * shape.C];
        #else
        data = nullptr;
        #endif
    }
 
    // Destructor
    ~TensorMem() {
        #ifndef __SYNTHESIS__
        if (own_memory && data) {
            delete[] data;
            data = nullptr;
        }
        #endif
    }
 
    // Get pointer to element at specific location
    inline T* raw_at(int n, int h, int w, int c) {
        #pragma HLS INLINE
        return &data[index(n, h, w, c)];
    }
 
    // Get raw data pointer
    inline T* raw() {
        #pragma HLS INLINE
        return data;
    }
 
    // Get raw data pointer (const version)
    inline const T* raw() const {
        #pragma HLS INLINE
        return data;
    }
 
    // Return value at (n, h, w, c)
    inline T get(int n, int h, int w, int c) {
        #pragma HLS INLINE
        return data[index(n, h, w, c)];
    }
 
    // Return reference to value at (n, h, w, c)
    inline T& at(int n, int h, int w, int c) {
        #pragma HLS INLINE
        return data[index(n, h, w, c)];
    }
};

#include "Hls_Layers.tpp"

#endif
