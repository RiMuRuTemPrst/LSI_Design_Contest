#include "Core.h"


void Add(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y) {
    assert(X1.shape == X2.shape && X2.shape == Y.shape);
    int size = X1.shape.N* X1.shape.H* X1.shape.W* X1.shape.C;
    for (int i = 0; i < size; ++i) 
        Y.raw()[i] = X1.raw()[i] + X2.raw()[i];
}
TensorMem<float>* Add(TensorMem<float> &X1, TensorMem<float> &X2) {
    TensorMem<float>* Y = new TensorMem<float>(X1.shape);
    Add(X1, X2, *Y);
    return Y;
}

void Sub(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y) {
    assert(X1.shape == X2.shape && X2.shape == Y.shape);
    int size = X1.shape.N* X1.shape.H* X1.shape.W* X1.shape.C;
    for (int i = 0; i < size; ++i) 
        Y.raw()[i] = X1.raw()[i] - X2.raw()[i];
}
TensorMem<float>* Sub(TensorMem<float> &X1, TensorMem<float> &X2) {
    TensorMem<float>* Y = new TensorMem<float>(X1.shape);
    Sub(X1, X2, *Y);
    return Y;
}

void Mul(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y) {
    assert(X1.shape == X2.shape && X2.shape == Y.shape);
    int size = X1.shape.N* X1.shape.H* X1.shape.W* X1.shape.C;
    for (int i = 0; i < size; ++i) 
        Y.raw()[i] = X1.raw()[i]* X2.raw()[i];
}
TensorMem<float>* Mul(TensorMem<float> &X1, TensorMem<float> &X2) {
    TensorMem<float>* Y = new TensorMem<float>(X1.shape);
    Mul(X1, X2, *Y);
    return Y;
}

void Div(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y) {
    assert(X1.shape == X2.shape && X2.shape == Y.shape);
    int size = X1.shape.N* X1.shape.H* X1.shape.W* X1.shape.C;
    for (int i = 0; i < size; ++i) {
        float div = X2.raw()[i];
        if (std::abs(div) < 1e-7) div = std::copysign(1e-7, div);
        Y.raw()[i] = X1.raw()[i] / div;
    }
}
TensorMem<float>* Div(TensorMem<float> &X1, TensorMem<float> &X2) {
    TensorMem<float>* Y = new TensorMem<float>(X1.shape);
    Div(X1, X2, *Y);
    return Y;
}
