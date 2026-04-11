#ifndef TENSOR_MEM_H
#define TENSOR_MEM_H

// --- BẮT ĐẦU SỬA ---

// Luôn include thư viện HLS để hỗ trợ kiểu 'half'
// Vitis HLS hỗ trợ thư viện này cả trong C-Simulation
#include <hls_half.h>
#include <hls_stream.h>
#include <ap_int.h>

// BẮT BUỘC: Luôn dùng half cho data_t để đồng bộ giữa TB và Kernel
typedef half data_t;

// Định nghĩa macro xử lý pragma
#if defined(__SYNTHESIS__)
    #define HLS_INLINE_PRAGMA _Pragma("HLS INLINE")
#else
    #define HLS_INLINE_PRAGMA
#endif

// Class Stream (giữ nguyên logic của bạn nhưng dùng hls::stream cho đơn giản)
// Trong simulation, hls::stream vẫn hoạt động bình thường giống std::queue
template<typename T>
using MyStream = hls::stream<T>;

#if !defined(__SYNTHESIS__)
    // Các thư viện C++ chuẩn cần cho Testbench
    #include <iostream>
    #include <vector>
    #include <queue>
    #include <cmath>
    #include <cassert>
    #include <inttypes.h>
#endif

// --- KẾT THÚC SỬA (Phần Shape và TensorMem phía dưới giữ nguyên) ---

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

#include "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/gen/class_tensor.tpp"
// Lưu ý: Hard_op.tpp đã sửa thêm 'static' ở câu trả lời trước, hãy giữ nguyên file đó
#include "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/gen/Hard_op.tpp"

#endif