#include "Core.h"


void Concat(TensorMem<int64_t>** X, TensorMem<int64_t> &Y, int num_inputs, int axis) {
    assert(axis >= 0 && axis <= 3);
    int n_offset = 0, h_offset = 0, w_offset = 0, c_offset = 0;
    int* offset, offs_del;
    switch (axis) {
        case N_AXIS: offset = &n_offset; break;
        case H_AXIS: offset = &h_offset; break;
        case W_AXIS: offset = &w_offset; break;
        case C_AXIS: offset = &c_offset;
    }

    for (int k = 0; k < num_inputs; k++) {
        TensorMem<int64_t>& t = *X[k];
        int N = t.shape.N, H = t.shape.H, W = t.shape.W, C = t.shape.C;
        switch (axis) {
            case N_AXIS: offs_del = N; break;
            case H_AXIS: offs_del = H; break;
            case W_AXIS: offs_del = W; break;
            case C_AXIS: offs_del = C;
        }

        for (int n = 0; n < N; n++)
        for (int h = 0; h < H; h++)
        for (int w = 0; w < W; w++)
        for (int c = 0; c < C; c++) {
            Y.at(n + n_offset, h + h_offset, w + w_offset, c + c_offset) = t.get(n, h, w, c);
        }
        *offset += offs_del;
    }
}
TensorMem<int64_t>* Concat(TensorMem<int64_t>** X, int num_inputs, int axis) {
    TensorMem<int64_t>* Y;
    Shape shape;
    if (axis == N_AXIS) 
        for (int i = 0; i < num_inputs; i++) shape.N += X[i]->shape.N;
    else shape.N = X[0]->shape.N;
    if (axis == H_AXIS) 
        for (int i = 0; i < num_inputs; i++) shape.H += X[i]->shape.H;
    else shape.H = X[0]->shape.H;
    if (axis == W_AXIS) 
        for (int i = 0; i < num_inputs; i++) shape.W += X[i]->shape.W;
    else shape.W = X[0]->shape.W;
    if (axis == C_AXIS) 
        for (int i = 0; i < num_inputs; i++) shape.C += X[i]->shape.C;
    else shape.C = X[0]->shape.C;

    Y = new TensorMem<int64_t>(shape);
    Concat(X, *Y, num_inputs, axis);
    return Y;
}

void Gather(TensorMem<int64_t> &X, TensorMem<int64_t> &Y, const int* indices, int idx_count, int axis) {
    assert(axis >= 0 && axis <= 3);
    int N = X.shape.N, H = X.shape.H, W = X.shape.W, C = X.shape.C;
    int n, h, w, c, id, max_idx;
    int* ind_id, *nn = &n, *hh = &h, *ww = &w, *cc = &c;
    switch (axis) {
        case N_AXIS: max_idx = N; N = idx_count; ind_id = &n; nn = &id; break;
        case H_AXIS: max_idx = H; H = idx_count; ind_id = &h; hh = &id; break;
        case W_AXIS: max_idx = W; W = idx_count; ind_id = &w; ww = &id; break;
        case C_AXIS: max_idx = C; C = idx_count; ind_id = &c; cc = &id;
    }

    for (n = 0; n < N; n++) 
    for (h = 0; h < H; h++)
    for (w = 0; w < W; w++)
    for (c = 0; c < C; c++) {
        id = indices[*ind_id];
        assert(id < max_idx);
        Y.at(n, h, w, c) = X.get(*nn, *hh, *ww, *cc);
    }
}
TensorMem<int64_t>* Gather(TensorMem<int64_t> &X, const int* indices, int idx_count, int axis) {
    TensorMem<int64_t>* Y;
    Shape shape;
    if (axis == N_AXIS) shape.N = idx_count;
    else shape.N = X.shape.N;
    if (axis == H_AXIS) shape.H = idx_count;
    else shape.H = X.shape.H;
    if (axis == W_AXIS) shape.W = idx_count;
    else shape.W = X.shape.W;
    if (axis == C_AXIS) shape.C = idx_count;
    else shape.C = X.shape.C;

    Y = new TensorMem<int64_t>(shape);
    Gather(X, *Y, indices, idx_count, axis);
    return Y;
}

void Pad(TensorMem<float> &X, TensorMem<float> &Y, int pad_top, int pad_left, int pad_bottom, int pad_right) {
    assert(pad_top >= 0 && pad_left >= 0);
    int YN = Y.shape.N, YH = Y.shape.H, YW = Y.shape.W, YC = Y.shape.C;
    int XH = X.shape.H, XW = X.shape.W;
    for (int n = 0; n < YN; n++)
    for (int h = 0; h < YH; h++)
    for (int w = 0; w < YW; w++)
    for (int c = 0; c < YC; c++) {
        int ih = h - pad_top;
        int iw = w - pad_left;

        float v = 0;
        if (ih >= 0 && ih < XH && iw >= 0 && iw < XW)
            v = X.get(n, ih, iw, c);

        Y.at(n, h, w, c) = v;
    }
}
TensorMem<float>* Pad(TensorMem<float> &X, int pad_top, int pad_left, int pad_bottom, int pad_right) {
    assert(pad_right >= 0 && pad_bottom >= 0);
    TensorMem<float>* Y;
    Shape shape = {X.shape.N, X.shape.H + pad_top + pad_bottom, X.shape.W + pad_left + pad_right, X.shape.C};
    Y = new TensorMem<float>(shape);
    Pad(X, *Y, pad_top, pad_left, pad_bottom, pad_right);
    return Y;
}

template <typename T>
void Reshape(TensorMem<T> &X, TensorMem<T> &Y) {
    int size = X.shape.N* X.shape.H* X.shape.W* X.shape.C;
    int check_size = Y.shape.N* Y.shape.H* Y.shape.W* Y.shape.C;
    assert(size == check_size);
    for (int i = 0; i < size; i++) 
        Y.raw()[i] = X.raw()[i];
}
template <typename T>
TensorMem<T>* Reshape(TensorMem<T> &X, const Shape &shape) {
    TensorMem<float>* Y = new TensorMem<float>(shape);
    Reshape(X, *Y);
    return Y;
}

template <typename T>
TensorMem<int64_t>* Shapeof(TensorMem<T> &X) {
    return new TensorMem<int64_t>(new int64_t[4]{X.shape.N, X.shape.H, X.shape.W, X.shape.C}, {1, 1, 1, 4}, true);
}

void Slice(TensorMem<int64_t> &X, TensorMem<int64_t> &Y, Shape &pos_0, Shape &pos_1) {
    for (int n = pos_0.N, on = 0; n < pos_1.N; n++, on++) 
    for (int h = pos_0.H, oh = 0; h < pos_1.H; h++, oh++) 
    for (int w = pos_0.W, ow = 0; w < pos_1.W; w++, ow++) 
    for (int c = pos_0.C, oc = 0; c < pos_1.C; c++, oc++) 
        Y.at(on, oh, ow, oc) = X.get(n, h, w, c);
}
TensorMem<int64_t>* Slice(TensorMem<int64_t> &X, Shape &pos_0, Shape &pos_1) {
    TensorMem<int64_t>* Y;
    Shape shape;
    shape.N = pos_1.N - pos_0.N; assert(shape.N > 0);
    shape.H = pos_1.H - pos_0.H; assert(shape.H > 0);
    shape.W = pos_1.W - pos_0.W; assert(shape.W > 0);
    shape.C = pos_1.C - pos_0.C; assert(shape.C > 0);
    Y = new TensorMem<int64_t>(shape);
    Slice(X, *Y, pos_0, pos_1);
    return Y;
}

void Transpose(TensorMem<int64_t> &X, TensorMem<int64_t> &Y, int perm[]) {
    int N = X.shape.N, H = X.shape.H, W = X.shape.W, C = X.shape.C;
    int n, h, w, c;
    int* per[4] = {NULL, NULL, NULL, NULL};
    for (int i = 0; i < 4; i++) 
        switch (perm[i]) {
            case N_AXIS: per[i] = &n; break;
            case H_AXIS: per[i] = &h; break;
            case W_AXIS: per[i] = &w; break;
            case C_AXIS: per[i] = &c;
        }
    for (n = 0; n < N; n++)
    for (h = 0; h < H; h++)
    for (w = 0; w < W; w++)
    for (c = 0; c < C; c++) 
        Y.at(*per[0], *per[1], *per[2], *per[3]) = X.get(n, h, w, c);
}
TensorMem<int64_t>* Transpose(TensorMem<int64_t> &X, int perm[]) {
    TensorMem<int64_t>* Y;
    Shape shape;
    int x_shape[4] = {X.shape.N, X.shape.H, X.shape.W, X.shape.C};
    shape.N = x_shape[perm[0]];
    shape.H = x_shape[perm[1]];
    shape.W = x_shape[perm[2]];
    shape.C = x_shape[perm[3]];
    Y = new TensorMem<int64_t>(shape);
    Transpose(X, *Y, perm);
    return Y;
}


