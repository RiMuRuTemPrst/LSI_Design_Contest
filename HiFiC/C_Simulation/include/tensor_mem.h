#ifndef TENSOR_MEM_H
#define TENSOR_MEM_H

#if defined(USE_HLS) || defined(__SYNTHESIS__)
    #include <hls_stream.h>
    #include <ap_int.h>
    #include <ap_fixed.h>

    typedef ap_fixed<16, 8> data_t;

    template<typename T>
    using MyStream = hls::stream<T>;

#else
    #include <iostream>
    #include <queue>
    #include <cassert>

    typedef float data_t;

    template<typename T>
    class MyStream {
    private:
        std::queue<T> q;
    public:
        void write(const T& val) { q.push(val); }
        T read() { T v = q.front(); q.pop(); return v; }
        bool empty() const { return q.empty(); }
    };
#endif

struct Shape {
    int N, H, W, C;
};

template<typename T>
class TensorMem {
private:
    T* m_data;
    Shape m_shape;
    int m_size;

public:
    TensorMem(T* data, Shape shape);
    int get_index(int n,int h,int w,int c);

    T read_element(int n,int h,int w,int c);
    void write_element(int n,int h,int w,int c,T val);

    void load_tile_to_stream(int n,int h0,int w0,int hs,int ws,
                             MyStream<T>& out);

    void store_stream_to_mem(int n,int h0,int w0,int hs,int ws,
                             MyStream<T>& in);

    Shape get_shape() const;
};

#endif
