#include "..\include\Core.h"


template <typename T>
void Norm(TensorMem<T> &X, TensorMem<T> &Y, TensorMem<T> &gamma, TensorMem<T> &beta, T epsilon, int axis, 
            TensorMem<T> &extra_mem_size_X_reduced_axis) {
    Shape temp_shape = {1, 1, 1, 1};
    TensorMem<T> temp(temp_shape);
    switch (axis) {
        case N_AXIS: temp.raw()[0] = X.shape.N - 1; break;
        case C_AXIS: temp.raw()[0] = X.shape.C - 1; break;
        case H_AXIS: temp.raw()[0] = X.shape.H - 1; break;
        case W_AXIS: temp.raw()[0] = X.shape.W - 1;
    }
    assert(temp.raw()[0] > 0);
    temp.raw()[0] = static_cast<T>(sqrt(temp.raw()[0]));

    ReduceMean(X, extra_mem_size_X_reduced_axis, axis);
    Sub(X, extra_mem_size_X_reduced_axis, X);
    Div(X, temp, Y);
    Mul(Y, Y, Y);
    ReduceSum(Y, extra_mem_size_X_reduced_axis, axis);
    temp.raw()[0] = epsilon;
    Add(extra_mem_size_X_reduced_axis, temp, extra_mem_size_X_reduced_axis);
    Sqrt(extra_mem_size_X_reduced_axis, extra_mem_size_X_reduced_axis);
    Div(X, extra_mem_size_X_reduced_axis, X);

    Mul(gamma, X, X);
    Add(X, beta, Y);
}
template <typename T>
void Norm(TensorMem<T> &X, TensorMem<T> &Y, TensorMem<T> &gamma, TensorMem<T> &beta, T epsilon, int axis, 
            TensorMem<T> &mean, TensorMem<T> &var, TensorMem<T> &std) {
    Shape temp_shape = {1, 1, 1, 1};
    TensorMem<T> temp(temp_shape);
    switch (axis) {
        case N_AXIS: temp.raw()[0] = X.shape.N - 1; break;
        case C_AXIS: temp.raw()[0] = X.shape.C - 1; break;
        case H_AXIS: temp.raw()[0] = X.shape.H - 1; break;
        case W_AXIS: temp.raw()[0] = X.shape.W - 1;
    }
    assert(temp.raw()[0] > 0);

    ReduceMean(X, mean, axis);
    Sub(X, mean, X);
    Mul(X, X, Y);
    ReduceSum(Y, var, axis);
    Div(var, temp, var);
    temp.raw()[0] = epsilon;
    Add(var, temp, var);
    Sqrt(var, std);
    Div(X, std, X);

    Mul(gamma, X, X);
    Add(X, beta, Y);
}
