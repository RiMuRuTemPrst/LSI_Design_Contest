#ifndef TENSOR_MEM_H
#define TENSOR_MEM_H

// ============================================================
//  Chọn môi trường: uncomment 1 trong 2 dòng dưới
// ============================================================
#define ENV_LOCAL
// #define ENV_SERVER

// ============================================================
//  Path shortcuts (dùng trong test.cpp)
// ============================================================
#ifdef ENV_LOCAL
    #define PATH_IO_PARAMS "/home/rimurutempest/Code/LSI_Design_Contest/HLS/convtranspose/src/io_params"
    #define PATH_FAKE_STACK_H "/home/rimurutempest/Code/LSI_Design_Contest/HLS/convtranspose/src/non_gen/fake_stack.h"
    #define PATH_TENSOR_IO "/home/rimurutempest/Code/LSI_Design_Contest/HLS/convtranspose/src/non_gen/tensor_io.tpp"
#else
    #define PATH_IO_PARAMS "/home/qanh_k67/Project/GAN_HLS/HLS/Conv_Transpose_layer/src/io_params"
    #define PATH_FAKE_STACK_H "/home/qanh_k67/Project/GAN_HLS/HLS/Conv_Transpose_layer/src/non_gen/fake_stack.h"
    #define PATH_TENSOR_IO "/home/qanh_k67/Project/GAN_HLS/HLS/Conv_Transpose_layer/src/non_gen/tensor_io.tpp"
#endif

// ============================================================

#if defined(USE_HLS) || defined(__SYNTHESIS__)
    #define HLS_INLINE_PRAGMA _Pragma("HLS INLINE")
    #include <hls_stream.h>
    #include <ap_axi_sdata.h>
    #include <ap_int.h>
    #include <ap_fixed.h>
    typedef ap_fixed<16, 8> data_t;
    template<typename T>
    using MyStream = hls::stream<T>;
#else
    #define HLS_INLINE_PRAGMA
    #include <iostream>
    #include <vector>
    #include <queue>
    #include <cmath>
    #include <cassert>
    #include <inttypes.h>
    typedef float data_t;
    template<typename T>
    class MyStream {
    private:
        std::queue<T> q;
    public:
        void write(const T& val) { q.push(val); }
        T read() {
            if(q.empty()) return 0;
            T val = q.front();
            q.pop();
            return val;
        }
        bool empty() { return q.empty(); }
        bool full() { return false; }
    };
#endif

struct Shape {
    int N, H, W, C;
    Shape() : N(0), H(0), W(0), C(0) {}
    Shape(int n, int h, int w, int c) : N(n), H(h), W(w), C(c) {}
    bool operator==(const Shape &o) const { return N==o.N && H==o.H && W==o.W && C==o.C; }
    bool operator>=(const Shape &o) const { return N>=o.N && H>=o.H && W>=o.W && C>=o.C; }
};

template <typename T>
class TensorMem {
private:
    T* data;
    bool own_memory;
    inline int index(int n, int h, int w, int c);
public:
    Shape shape;
    TensorMem();
    TensorMem(T* data, const Shape &shape, bool own_memory);
    TensorMem(const Shape &shape);
    ~TensorMem();
    inline T get(int n, int h, int w, int c);
    inline T &at(int n, int h, int w, int c);
    inline T* raw() { return data; }
    inline const T* raw() const { return data; }
    inline T* raw_at(int n, int h, int w, int c);
};

#ifdef ENV_LOCAL
    #include "/home/rimurutempest/Code/LSI_Design_Contest/HLS/convtranspose/src/non_gen/class_tensor.tpp"
    #include "/home/rimurutempest/Code/LSI_Design_Contest/HLS/convtranspose/src/gen/Hard_op_2.tpp"
#else
    #include "/home/qanh_k67/Project/GAN_HLS/HLS/Conv_Transpose_layer/src/non_gen/class_tensor.tpp"
    #include "/home/qanh_k67/Project/GAN_HLS/HLS/Conv_Transpose_layer/src/gen/Hard_op_2.tpp"
#endif

#endif // TENSOR_MEM_H