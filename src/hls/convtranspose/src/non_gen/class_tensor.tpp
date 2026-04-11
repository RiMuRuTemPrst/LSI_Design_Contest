#include "Core.h"



template <typename T>
TensorMem<T>::TensorMem() : data(nullptr), own_memory(false) { HLS_INLINE_PRAGMA }
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
    HLS_INLINE_PRAGMA
    if (own_memory && data)
        delete[] data;
}
template <typename T>
inline int TensorMem<T>::index(int n, int h, int w, int c) {
    HLS_INLINE_PRAGMA
    return ((n* shape.H + h)* shape.W + w)* shape.C + c;
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
inline T* TensorMem<T>::raw_at(int n, int h, int w, int c) {
    HLS_INLINE_PRAGMA
    return &data[index(n, h, w, c)];
}
