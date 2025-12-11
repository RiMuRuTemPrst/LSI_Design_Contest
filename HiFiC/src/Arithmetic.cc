#include "Core.h"


void Add(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y) {
    assert(X1.shape == Y.shape || X2.shape == Y.shape);
    int dn1, dn2, dh1, dh2, dw1, dw2, dc1, dc2;
    dn1 = dn2 = dh1 = dh2 = dw1 = dw2 = dc1 = dc2 = 1;
    int n1, n2, h1, h2, w1, w2, c1, c2;
    int* yn = &n1, *yh = &h1, *yw = &w1, *yc = &c1;
    if (X1.shape.N != X2.shape.N) {
        if (X1.shape.N == 1) dn1--, yn = &n2;
        else if (X2.shape.N == 1) dn2--;
        else assert(0 && "Error: Inappropriate N-axis size !!");
    }
    if (X1.shape.H != X2.shape.H) {
        if (X1.shape.H == 1) dh1--, yh = &h2;
        else if (X2.shape.H == 1) dh2--;
        else assert(0 && "Error: Inappropriate H-axis size !!");
    }
    if (X1.shape.W != X2.shape.W) {
        if (X1.shape.W == 1) dw1--, yw = &w2;
        else if (X2.shape.W == 1) dw2--;
        else assert(0 && "Error: Inappropriate W-axis size !!");
    }
    if (X1.shape.C != X2.shape.C) {
        if (X1.shape.C == 1) dc1--, yc = &c2;
        else if (X2.shape.C == 1) dc2--;
        else assert(0 && "Error: Inappropriate C-axis size !!");
    }
    for (n1 = 0, n2 = 0; n1 < Y.shape.N && n2 < Y.shape.N; n1 += dn1, n2 += dn2)
    for (h1 = 0, h2 = 0; h1 < Y.shape.H && h2 < Y.shape.H; h1 += dh1, h2 += dh2)
    for (w1 = 0, w2 = 0; w1 < Y.shape.W && w2 < Y.shape.W; w1 += dw1, w2 += dw2)
    for (c1 = 0, c2 = 0; c1 < Y.shape.C && c2 < Y.shape.C; c1 += dc1, c2 += dc2) 
        Y.at(*yn, *yh, *yw, *yc) = X1.get(n1, h1, w1, c1) + X2.get(n2, h2, w2, c2);
}
TensorMem<float>* Add(TensorMem<float> &X1, TensorMem<float> &X2) {
    TensorMem<float>* Y = new TensorMem<float>(X1.shape);
    Add(X1, X2, *Y);
    return Y;
}

void Sub(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y) {
    assert(X1.shape == Y.shape || X2.shape == Y.shape);
    int dn1, dn2, dh1, dh2, dw1, dw2, dc1, dc2;
    dn1 = dn2 = dh1 = dh2 = dw1 = dw2 = dc1 = dc2 = 1;
    int n1, n2, h1, h2, w1, w2, c1, c2;
    int* yn = &n1, *yh = &h1, *yw = &w1, *yc = &c1;
    if (X1.shape.N != X2.shape.N) {
        if (X1.shape.N == 1) dn1--, yn = &n2;
        else if (X2.shape.N == 1) dn2--;
        else assert(0 && "Error: Inappropriate N-axis size !!");
    }
    if (X1.shape.H != X2.shape.H) {
        if (X1.shape.H == 1) dh1--, yh = &h2;
        else if (X2.shape.H == 1) dh2--;
        else assert(0 && "Error: Inappropriate H-axis size !!");
    }
    if (X1.shape.W != X2.shape.W) {
        if (X1.shape.W == 1) dw1--, yw = &w2;
        else if (X2.shape.W == 1) dw2--;
        else assert(0 && "Error: Inappropriate W-axis size !!");
    }
    if (X1.shape.C != X2.shape.C) {
        if (X1.shape.C == 1) dc1--, yc = &c2;
        else if (X2.shape.C == 1) dc2--;
        else assert(0 && "Error: Inappropriate C-axis size !!");
    }
    for (n1 = 0, n2 = 0; n1 < Y.shape.N && n2 < Y.shape.N; n1 += dn1, n2 += dn2)
    for (h1 = 0, h2 = 0; h1 < Y.shape.H && h2 < Y.shape.H; h1 += dh1, h2 += dh2)
    for (w1 = 0, w2 = 0; w1 < Y.shape.W && w2 < Y.shape.W; w1 += dw1, w2 += dw2)
    for (c1 = 0, c2 = 0; c1 < Y.shape.C && c2 < Y.shape.C; c1 += dc1, c2 += dc2) 
        Y.at(*yn, *yh, *yw, *yc) = X1.get(n1, h1, w1, c1) - X2.get(n2, h2, w2, c2);
}
TensorMem<float>* Sub(TensorMem<float> &X1, TensorMem<float> &X2) {
    TensorMem<float>* Y = new TensorMem<float>(X1.shape);
    Sub(X1, X2, *Y);
    return Y;
}

void Mul(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y) {
    assert(X1.shape == Y.shape || X2.shape == Y.shape);
    int dn1, dn2, dh1, dh2, dw1, dw2, dc1, dc2;
    dn1 = dn2 = dh1 = dh2 = dw1 = dw2 = dc1 = dc2 = 1;
    int n1, n2, h1, h2, w1, w2, c1, c2;
    int* yn = &n1, *yh = &h1, *yw = &w1, *yc = &c1;
    if (X1.shape.N != X2.shape.N) {
        if (X1.shape.N == 1) dn1--, yn = &n2;
        else if (X2.shape.N == 1) dn2--;
        else assert(0 && "Error: Inappropriate N-axis size !!");
    }
    if (X1.shape.H != X2.shape.H) {
        if (X1.shape.H == 1) dh1--, yh = &h2;
        else if (X2.shape.H == 1) dh2--;
        else assert(0 && "Error: Inappropriate H-axis size !!");
    }
    if (X1.shape.W != X2.shape.W) {
        if (X1.shape.W == 1) dw1--, yw = &w2;
        else if (X2.shape.W == 1) dw2--;
        else assert(0 && "Error: Inappropriate W-axis size !!");
    }
    if (X1.shape.C != X2.shape.C) {
        if (X1.shape.C == 1) dc1--, yc = &c2;
        else if (X2.shape.C == 1) dc2--;
        else assert(0 && "Error: Inappropriate C-axis size !!");
    }
    for (n1 = 0, n2 = 0; n1 < Y.shape.N && n2 < Y.shape.N; n1 += dn1, n2 += dn2)
    for (h1 = 0, h2 = 0; h1 < Y.shape.H && h2 < Y.shape.H; h1 += dh1, h2 += dh2)
    for (w1 = 0, w2 = 0; w1 < Y.shape.W && w2 < Y.shape.W; w1 += dw1, w2 += dw2)
    for (c1 = 0, c2 = 0; c1 < Y.shape.C && c2 < Y.shape.C; c1 += dc1, c2 += dc2) 
        Y.at(*yn, *yh, *yw, *yc) = X1.get(n1, h1, w1, c1)* X2.get(n2, h2, w2, c2);
}
TensorMem<float>* Mul(TensorMem<float> &X1, TensorMem<float> &X2) {
    TensorMem<float>* Y = new TensorMem<float>(X1.shape);
    Mul(X1, X2, *Y);
    return Y;
}

void Div(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y) {
    assert(X1.shape == Y.shape || X2.shape == Y.shape);
    int dn1, dn2, dh1, dh2, dw1, dw2, dc1, dc2;
    dn1 = dn2 = dh1 = dh2 = dw1 = dw2 = dc1 = dc2 = 1;
    int n1, n2, h1, h2, w1, w2, c1, c2;
    int* yn = &n1, *yh = &h1, *yw = &w1, *yc = &c1;
    if (X1.shape.N != X2.shape.N) {
        if (X1.shape.N == 1) dn1--, yn = &n2;
        else if (X2.shape.N == 1) dn2--;
        else assert(0 && "Error: Inappropriate N-axis size !!");
    }
    if (X1.shape.H != X2.shape.H) {
        if (X1.shape.H == 1) dh1--, yh = &h2;
        else if (X2.shape.H == 1) dh2--;
        else assert(0 && "Error: Inappropriate H-axis size !!");
    }
    if (X1.shape.W != X2.shape.W) {
        if (X1.shape.W == 1) dw1--, yw = &w2;
        else if (X2.shape.W == 1) dw2--;
        else assert(0 && "Error: Inappropriate W-axis size !!");
    }
    if (X1.shape.C != X2.shape.C) {
        if (X1.shape.C == 1) dc1--, yc = &c2;
        else if (X2.shape.C == 1) dc2--;
        else assert(0 && "Error: Inappropriate C-axis size !!");
    }
    for (n1 = 0, n2 = 0; n1 < Y.shape.N && n2 < Y.shape.N; n1 += dn1, n2 += dn2)
    for (h1 = 0, h2 = 0; h1 < Y.shape.H && h2 < Y.shape.H; h1 += dh1, h2 += dh2)
    for (w1 = 0, w2 = 0; w1 < Y.shape.W && w2 < Y.shape.W; w1 += dw1, w2 += dw2)
    for (c1 = 0, c2 = 0; c1 < Y.shape.C && c2 < Y.shape.C; c1 += dc1, c2 += dc2) {
        float val = X2.get(n2, h2, w2, c2);
        if (std::abs(val) < 1e-7) val = std::copysign(1e-7, val);
        Y.at(*yn, *yh, *yw, *yc) = X1.get(n1, h1, w1, c1) / val;
    }
}
TensorMem<float>* Div(TensorMem<float> &X1, TensorMem<float> &X2) {
    TensorMem<float>* Y = new TensorMem<float>(X1.shape);
    Div(X1, X2, *Y);
    return Y;
}

