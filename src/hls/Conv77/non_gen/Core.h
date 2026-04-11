#ifndef TENSOR_MEM_H
#define TENSOR_MEM_H


#if defined(USE_HLS) || defined(__SYNTHESIS__)
    // --- MÔI TRƯỜNG HLS (Hardware) ---
    #define HLS_INLINE_PRAGMA _Pragma("HLS INLINE")

    #include <hls_stream.h>
    #include <ap_axi_sdata.h>
    #include <ap_int.h>
    #include <ap_fixed.h>

    // Dùng ap_fixed cho phần cứng để tối ưu tài nguyên
    typedef ap_fixed<16, 8> data_t;

    // Alias stream của HLS
    template<typename T>
    using MyStream = hls::stream<T>;

#else
    // --- MÔI TRƯỜNG PURE C++ (Software Simulation) ---
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
    int N; // Batch size
    int H; // Height
    int W; // Width
    int C; // Channels
    /**
     * @brief New Shape object with full 0-sizes
     * 
     */
    Shape() : N(0), H(0), W(0), C(0) {}
    /**
     * @brief New Shape object
     * 
     * @param n Batchsize
     * @param h Height
     * @param w Width
     * @param c Channels
     */
    Shape(int n, int h, int w, int c) : N(n), H(h), W(w), C(c) {}

    bool operator==(const Shape &other) const {
        return N == other.N && H == other.H && W == other.W && C == other.C;
    }
    bool operator>=(const Shape &other) const {
        return N >= other.N && H >= other.H && W >= other.W && C >= other.C;
    }
};

template <typename T>
class TensorMem {
private:
    T* data;
    bool own_memory;
    inline int index(int n, int h, int w, int c);

public:
    Shape shape;

    /**
     * @brief New Tensor with no mem
     */
    TensorMem();
    /**
     * @brief New Tensor
     * 
     * @param data 
     * @param shape {N, H, W, C}
     * @param is_own_memory 
     */
    TensorMem(T* data, const Shape &shape, bool own_memory);
    /**
     * @brief New Tensor with own full 0s mem
     * 
     * @param shape {N, H, W, C}
     */
    TensorMem(const Shape &shape);
    ~TensorMem();

    /**
     * @brief get tensor value at position
     * 
     * @param n 
     * @param h 
     * @param w 
     * @param c 
     * @return (T) value 
     */
    inline T get(int n, int h, int w, int c);

    /**
     * @brief get tensor reference at position
     * 
     * @param n 
     * @param h 
     * @param w 
     * @param c 
     * @return (T) reference 
     */
    inline T &at(int n, int h, int w, int c);

    /**
     * @brief raw data
     * 
     * @return T* 
     */
    inline T* raw() { return data; }
    /**
     * @brief raw data
     * 
     * @return const T* 
     */
    inline const T* raw() const { return data; }
    inline T* raw_at(int n, int h, int w, int c);
};

#include "class_tensor.tpp"

#endif
