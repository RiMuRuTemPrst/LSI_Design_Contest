#include "Core.h"


template <typename T>
void Identity(const TensorMem<T> &X, TensorMem<T> &Y) {
    assert(X.shape == Y.shape);
    int size = X.shape.N* X.shape.H* X.shape.W* X.shape.C;
    for (int i = 0; i < size; i++) 
        Y.raw()[i] = X.raw()[i];
}
template <typename T>
TensorMem<T>* Identity(const TensorMem<T> &X) {
    TensorMem<T>* Y = new TensorMem<T>(X.shape);
    Identity(X, *Y);
    return Y;
}

template <typename T>
void Constant(T* X, TensorMem<T> &Y, int size) {
    for (int i = 0; i < size; i++)
        Y.raw()[i] = X[i];
}

void Constant_of_shape(TensorMem<int64_t> &Y, int64_t val) {
    int size = Y.shape.N* Y.shape.H* Y.shape.W* Y.shape.C;
    for (int i = 0; i < size; i++)
        Y.raw()[i] = val;
}
TensorMem<int64_t>* Constant_of_shape(const Shape &shape, int64_t val) {
    TensorMem<int64_t>* Y = new TensorMem<int64_t>(shape);
    Constant_of_shape(*Y, val);
    return Y;
}

template <typename T_IN, typename T_OUT>
void Cast(TensorMem<T_IN> &X, TensorMem<T_OUT> &Y) {
    assert(X.shape == Y.shape);
    int size = X.shape.N* X.shape.H* X.shape.W* X.shape.C;
    for (int i = 0; i < size; ++i) 
        Y.raw()[i] = static_cast<T_OUT>(X.raw()[i]);
}
template <typename T_OUT, typename T_IN>
auto Cast(TensorMem<T_IN> &X) -> TensorMem<T_OUT>* {
    TensorMem<T_OUT>* Y = new TensorMem<T_OUT>(X.shape);
    Cast(X, *Y);
    return Y;
}

template <typename T>
void Relu(TensorMem<T> &X, TensorMem<T> &Y) {
    assert(X.shape == Y.shape);
    int size = X.shape.N* X.shape.H* X.shape.W* X.shape.C;
    for (int i = 0; i < size; ++i) {
        T val = X.raw()[i];
        Y.raw()[i] = val > 0 ? val : 0;
    }
}
template <typename T>
TensorMem<T>* Relu(TensorMem<T> &X) {
    TensorMem<T>* Y = new TensorMem<T>(X.shape);
    Relu(X, *Y);
    return Y;
}

void Sqrt(TensorMem<float> &X, TensorMem<float> &Y) {
    assert(X.shape == Y.shape);
    int size = X.shape.N* X.shape.H* X.shape.W* X.shape.C;
    for (int i = 0; i < size; ++i) 
        Y.raw()[i] = std::sqrt(X.raw()[i]);
}
TensorMem<float>* Sqrt(TensorMem<float> &X) {
    TensorMem<float>* Y = new TensorMem<float>(X.shape);
    Sqrt(X, *Y);
    return Y;
}

template <typename T>
void Floor(TensorMem<T> &X, TensorMem<T> &Y) {
    assert(X.shape == Y.shape);
    int size = X.shape.N* X.shape.H* X.shape.W* X.shape.C;
    for (int i = 0; i < size; ++i) 
        Y.raw()[i] = std::floor(X.raw()[i]);
}
template <typename T>
TensorMem<T>* Floor(TensorMem<T> &X) {
    TensorMem<T>* Y = new TensorMem<T>(X.shape);
    Floor(X, *Y);
    return Y;
}

