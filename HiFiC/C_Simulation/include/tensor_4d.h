#ifndef TENSOR_4D_H
#define TENSOR_4D_H

#include "tensor_mem.h"
#include <cstring>   // memcpy

class Tensor4D {
private:
    data_t* data;
    bool own_memory;

public:
    int N, H, W, C;

    // Default ctor
    Tensor4D() : data(nullptr), N(0), H(0), W(0), C(0), own_memory(false) {}

    // Allocate owned memory
    Tensor4D(int n, int h, int w, int c)
        : N(n), H(h), W(w), C(c), own_memory(true)
    {
        int total = N * H * W * C;
        data = new data_t[total];
    }

    // NO RAW POINTER VERSION ANYMORE — REMOVE UNSAFE WRAP

    // Destructor
    ~Tensor4D() {
        if (own_memory && data) {
            delete[] data;
        }
    }

    inline int index(int n,int h,int w,int c) const {
        return ((n * H + h) * W + w) * C + c;
    }

    inline data_t get(int n,int h,int w,int c) const {
        return data[index(n,h,w,c)];
    }

    inline void set(int n,int h,int w,int c,data_t v) {
        data[index(n,h,w,c)] = v;
    }

    inline data_t* raw() { return data; }
    inline const data_t* raw() const { return data; }

    inline int size() const { return N * H * W * C; }

    // Load data into owned memory
    inline void load(const data_t* buf) {
        std::memcpy(data, buf, sizeof(data_t) * size());
    }
};

#endif
