#include "Core.h"



template <typename T>
TensorMem<T>::TensorMem() : data(NULL), own_memory(false) { HLS_INLINE_PRAGMA }
template <typename T>
TensorMem<T>::TensorMem(T* data, const Shape &shape, bool own_memory) 
            : data(data), shape(shape), own_memory(own_memory) { HLS_INLINE_PRAGMA }
template <typename T>
TensorMem<T>::TensorMem(const Shape &shape) : shape(shape), own_memory(true) {
    HLS_INLINE_PRAGMA
    data = new T[shape.N* shape.H* shape.W* shape.C]{};
}
template <typename T>
TensorMem<T>::~TensorMem() {
    if (own_memory && data)
        delete[] data;
}
template <typename T>
inline int TensorMem<T>::index(int n, int h, int w, int c) {
    HLS_INLINE_PRAGMA
    return ((n* shape.H + h)* shape.W + w)* shape.C + c;
}
template <typename T>
void TensorMem<T>::print() {
    int size = shape.N* shape.H* shape.W* shape.C;
    for (int i = 0; i < size; i++) std::cout << data[i] << " ";
    std::cout << "\n";
}
template <typename T>
inline T TensorMem<T>::get(int n, int h, int w, int c) {
    HLS_INLINE_PRAGMA
    return data[index(n, h, w, c)];
}
template <typename T>
inline T &TensorMem<T>::at(int n, int h, int w, int c) {
    HLS_INLINE_PRAGMA
    return data[index(n, h, w, c)];
}
template <typename T>
void TensorMem<T>::load_tile_to_stream(int n, int h_start, int w_start, int h_size, int w_size, MyStream<T>& out_stream) {
    // Vòng lặp tính toán
    for (int h = 0; h < h_size; ++h) 
        for (int w = 0; w < w_size; ++w) 
            for (int c = 0; c < shape.C; ++c) {
                
                // Chỉ bật PIPELINE khi chạy HLS
                HLS_PIPELINE_PRAGMA

                // Logic tính toán vị trí bộ nhớ
                int idx = index(n, h_start + h, w_start + w, c);
                T val = data[idx];
                out_stream.write(val);
            }
}
template <typename T>
void TensorMem<T>::store_stream_to_mem(int n, int h_start, int w_start, int h_size, int w_size, MyStream<T>& in_stream) {
    for (int h = 0; h < h_size; ++h) 
        for (int w = 0; w < w_size; ++w) 
            for (int c = 0; c < shape.C; ++c) {
                
                HLS_PIPELINE_PRAGMA

                int idx = index(n, h_start + h, w_start + w, c);
                if (!in_stream.empty()) {
                    T val = in_stream.read();
                    data[idx] = val;
                }
            }
}
