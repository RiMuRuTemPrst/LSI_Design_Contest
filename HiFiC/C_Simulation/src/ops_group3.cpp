#include "ops_group3.h"

// =======================================================
// Identity
// =======================================================
void OpsGroup3::identity(Tensor4D& out, const Tensor4D& in) {
    int size = in.N * in.H * in.W * in.C;
    for (int i = 0; i < size; i++) {
        out.raw()[i] = in.raw()[i];
    }
}

// =======================================================
// Reshape (copy data only)
// =======================================================
void OpsGroup3::reshape(Tensor4D& out, const Tensor4D& in) {
    int total = in.N * in.H * in.W * in.C;
    for (int i = 0; i < total; i++) {
        out.raw()[i] = in.raw()[i];
    }
}

// =======================================================
// Transpose (swap H-W)
// =======================================================
void OpsGroup3::transpose(Tensor4D& out, const Tensor4D& in) {
    for (int n = 0; n < in.N; n++)
    for (int h = 0; h < in.H; h++)
    for (int w = 0; w < in.W; w++)
    for (int c = 0; c < in.C; c++) {
        out.set(n, w, h, c, in.get(n, h, w, c));
    }
}

// =======================================================
// Pad
// =======================================================
void OpsGroup3::pad(Tensor4D& out, const Tensor4D& in,
                    int pad_top, int pad_bottom,
                    int pad_left, int pad_right)
{
    for (int n = 0; n < out.N; n++)
    for (int h = 0; h < out.H; h++)
    for (int w = 0; w < out.W; w++)
    for (int c = 0; c < out.C; c++)
    {
        int ih = h - pad_top;
        int iw = w - pad_left;

        data_t v = 0;
        if (ih >= 0 && ih < in.H && iw >= 0 && iw < in.W)
            v = in.get(n, ih, iw, c);

        out.set(n, h, w, c, v);
    }
}

// =======================================================
// Concat (axis = C only for simplicity)
// =======================================================
void OpsGroup3::concat(Tensor4D& out, Tensor4D** inputs, int num_inputs, int axis)
{
    int offset = 0;

    for (int k = 0; k < num_inputs; k++)
    {
        Tensor4D& t = *inputs[k];

        for (int n = 0; n < t.N; n++)
        for (int h = 0; h < t.H; h++)
        for (int w = 0; w < t.W; w++)
        for (int c = 0; c < t.C; c++)
        {
            out.set(n, h, w, c + offset, t.get(n,h,w,c));
        }

        offset += t.C;
    }
}

// =======================================================
// Constant (copy input buffer directly)
// =======================================================
void OpsGroup3::constant(data_t* out, const data_t* in, int size)
{
    for (int i = 0; i < size; i++)
        out[i] = in[i];
}

// =======================================================
// ConstantOfShape (fill with value)
// =======================================================
void OpsGroup3::constant_of_shape(data_t* out, int size, data_t val)
{
    for (int i = 0; i < size; i++)
        out[i] = val;
}

// =======================================================
// Shape (output = [N,H,W,C])
// =======================================================
void OpsGroup3::shape(int out_shape[4], const Tensor4D& in)
{
    out_shape[0] = in.N;
    out_shape[1] = in.H;
    out_shape[2] = in.W;
    out_shape[3] = in.C;
}

// =======================================================
// Slice (copy region)
// NOTE: simple version: full copy same shape
// =======================================================
void OpsGroup3::slice(Tensor4D& out, const Tensor4D& in,
                      int n0,int n1,
                      int h0,int h1,
                      int w0,int w1,
                      int c0,int c1)
{
    int on = 0;
    for (int n = n0; n < n1; n++, on++)
    {
        int oh = 0;
        for (int h = h0; h < h1; h++, oh++)
        {
            int ow = 0;
            for (int w = w0; w < w1; w++, ow++)
            {
                int oc = 0;
                for (int c = c0; c < c1; c++, oc++)
                {
                    out.set(on, oh, ow, oc, in.get(n,h,w,c));
                }
            }
        }
    }
}

// =======================================================
// Gather (simple axis=0 only)
// =======================================================
void OpsGroup3::gather(Tensor4D& out, const Tensor4D& in,
                       const int* indices, int idx_count, int axis)
{
    for (int i = 0; i < idx_count; i++)
    {
        int id = indices[i];

        for (int h = 0; h < in.H; h++)
        for (int w = 0; w < in.W; w++)
        for (int c = 0; c < in.C; c++)
        {
            out.set(i, h, w, c, in.get(id, h, w, c));
        }
    }
}
