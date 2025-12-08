#ifndef OPS_GROUP3_H
#define OPS_GROUP3_H

#include "tensor_4d.h"

class OpsGroup3 {
public:

    static void identity(Tensor4D& out, const Tensor4D& in);
    static void reshape(Tensor4D& out, const Tensor4D& in);
    static void transpose(Tensor4D& out, const Tensor4D& in);

    static void pad(Tensor4D& out, const Tensor4D& in,
                    int pad_top, int pad_bottom,
                    int pad_left, int pad_right);

    static void concat(Tensor4D& out, Tensor4D** inputs, int num_inputs, int axis);

    static void constant(data_t* out, const data_t* in, int size);
    static void constant_of_shape(data_t* out, int size, data_t val);

    static void gather(Tensor4D& out, const Tensor4D& in,
                       const int* indices, int idx_count, int axis);

    static void shape(int out_shape[4], const Tensor4D& in);

    static void slice(Tensor4D& out, const Tensor4D& in,
                      int n0,int n1,
                      int h0,int h1,
                      int w0,int w1,
                      int c0,int c1);
};

#endif
