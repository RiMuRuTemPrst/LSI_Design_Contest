#include "tensor_mem.h"

template<typename T>
TensorMem<T>::TensorMem(T* data, Shape shape)
    : m_data(data), m_shape(shape)
{
    m_size = shape.N * shape.H * shape.W * shape.C;
}

template<typename T>
int TensorMem<T>::get_index(int n,int h,int w,int c) {
    return n*(m_shape.H*m_shape.W*m_shape.C)
            + h*(m_shape.W*m_shape.C)
            + w*(m_shape.C)
            + c;
}

template<typename T>
T TensorMem<T>::read_element(int n,int h,int w,int c) {
    return m_data[get_index(n,h,w,c)];
}

template<typename T>
void TensorMem<T>::write_element(int n,int h,int w,int c,T v) {
    m_data[get_index(n,h,w,c)] = v;
}

template<typename T>
void TensorMem<T>::load_tile_to_stream(
        int n,int h0,int w0,int hs,int ws,
        MyStream<T>& out)
{
    for(int h=0;h<hs;h++)
        for(int w=0;w<ws;w++)
            for(int c=0;c<m_shape.C;c++)
                out.write(
                    read_element(n,h0+h,w0+w,c)
                );
}

template<typename T>
void TensorMem<T>::store_stream_to_mem(
        int n,int h0,int w0,int hs,int ws,
        MyStream<T>& in)
{
    for(int h=0;h<hs;h++)
        for(int w=0;w<ws;w++)
            for(int c=0;c<m_shape.C;c++)
                if(!in.empty())
                    write_element(n,h0+h,w0+w,c,in.read());
}

template<typename T>
Shape TensorMem<T>::get_shape() const {
    return m_shape;
}

template class TensorMem<float>;

