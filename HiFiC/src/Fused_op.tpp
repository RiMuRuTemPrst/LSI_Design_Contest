#include "..\include\Core.h"
#include <cassert>


template<typename T>
void Relu_Pad_reflect_hw(TensorMem<T> &X) {
    int XN = X.shape.N, XH = X.shape.H, XW = X.shape.W, XC = X.shape.C;
    XH--, XW--;
    for (int n = 0; n < XN; n++) 
    for (int c = 0; c < XC; c++) {
        for (int h = 1; h < XH; h++)
        for (int w = 1; w < XW; w++) {
            T &x = X.at(n, h, w, c);
            if (x < T(0)) x = T(0);
        }
        X.at(n, 0, 0, c) = X.get(n, 2, 2, c);
        X.at(n, 0, XW, c) = X.get(n, 2, XW - 2, c);
        X.at(n, XH, 0, c) = X.get(n, XH - 2, 2, c);
        X.at(n, XH, XW, c) = X.get(n, XH - 2, XW - 2, c);
        for (int w = 1; w < XW; w++) {
            X.at(n, 0, w, c) = X.get(n, 2, w, c);
            X.at(n, XH, w, c) = X.get(n, XH - 2, w, c);
        }
        for (int h = 1; h < XH; h++) {
            X.at(n, h, 0, c) = X.get(n, h, 2, c);
            X.at(n, h, XW, c) = X.get(n, h, XW - 2, c);
        }
    }
}
template<int Size, typename T>
void Relu_Pad_reflect_hw(TensorMem<T> &X) {
    int XN = X.shape.N, XH = X.shape.H - Size, XW = X.shape.W - Size, XC = X.shape.C;
    for (int n = 0; n < XN; n++) 
    for (int c = 0; c < XC; c++) {
        for (int h = Size; h < XH; h++)
        for (int w = Size; w < XW; w++) {
            T &x = X.at(n, h, w, c);
            if (x < T(0)) x = T(0);
        }
        for (int h = 0; h < Size; h++) 
        for (int w = 0; w < Size; w++) {
            X.at(n, h, w, c) = X.get(n, Size + Size - h, Size + Size - w, c);
            X.at(n, h, XW + w, c) = X.get(n, Size + Size - h, XW - w - 2, c);
            X.at(n, XH + h, w, c) = X.get(n, XH - h - 2, Size + Size - w, c);
            X.at(n, XH + h, XW + w, c) = X.get(n, XH - h - 2, XW - w - 2, c);
        }
        for (int h = 0; h < Size; h++) 
        for (int w = Size; w < XW; w++) {
            X.at(n, h, w, c) = X.get(n, Size + Size - h, w, c);
            X.at(n, XH + h, w, c) = X.get(n, XH - h - 2, w, c);
        }
        for (int w = 0; w < Size; w++) 
        for (int h = Size; h < XH; h++) {
            X.at(n, h, w, c) = X.get(n, h, Size + Size - w, c);
            X.at(n, h, XW + w, c) = X.get(n, h, XW - w - 2, c);
        }
    }
}

template<typename T>
void Add_Pad_reflect_hw(TensorMem<T> &X1, TensorMem<T> &X2, TensorMem<T> &Y) {
    int YN = Y.shape.N, YH = Y.shape.H, YW = Y.shape.W, YC = Y.shape.C;
    YH--, YW--;
    for (int n = 0; n < YN; n++) 
    for (int c = 0; c < YC; c++) {
        for (int h = 1; h < YH; h++)
        for (int w = 1; w < YW; w++) 
            Y.at(n, h, w, c) = X1.get(n, h, w, c) + X2.get(n, h, w, c);
        
        Y.at(n, 0, 0, c) = Y.get(n, 2, 2, c);
        Y.at(n, 0, YW, c) = Y.get(n, 2, YW - 2, c);
        Y.at(n, YH, 0, c) = Y.get(n, YH - 2, 2, c);
        Y.at(n, YH, YW, c) = Y.get(n, YH - 2, YW - 2, c);
        for (int w = 1; w < YW; w++) {
            Y.at(n, 0, w, c) = Y.get(n, 2, w, c);
            Y.at(n, YH, w, c) = Y.get(n, XH - 2, w, c);
        }
        for (int h = 1; h < YH; h++) {
            Y.at(n, h, 0, c) = Y.get(n, h, 2, c);
            Y.at(n, h, YW, c) = Y.get(n, h, YW - 2, c);
        }
    }
}
