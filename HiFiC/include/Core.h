#ifndef TENSOR_MEM_H
#define TENSOR_MEM_H


#if defined(USE_HLS) || defined(__SYNTHESIS__)
    // --- MÔI TRƯỜNG HLS (Hardware) ---
    #define HLS_INLINE_PRAGMA _Pragma("HLS INLINE")
    #define HLS_PIPELINE_PRAGMA _Pragma("HLS PIPELINE II=1")

    #include <hls_stream.h>
    #include <ap_axi_sdata.h>
    #include <ap_int.h>
    #include <ap_fixed.h>

    // Dùng ap_fixed cho phần cứng để tối ưu tài nguyên
    typedef ap_fixed<16, 8> data_t;

    // Alias stream của HLS
    template<typename T>
    using MyStream = hls::stream<T>;

#else
    // --- MÔI TRƯỜNG PURE C++ (Software Simulation) ---
    #define HLS_INLINE_PRAGMA
    #define HLS_PIPELINE_PRAGMA

    #include <iostream>
    #include <vector>
    #include <queue>
    #include <cmath>
    #include <cassert>
    #include <inttypes.h>

    typedef float data_t;

    template<typename T>
    class MyStream {
    private:
        std::queue<T> q;
    public:
        void write(const T& val) { q.push(val); }
        T read() { 
            if(q.empty()) return 0; 
            T val = q.front(); 
            q.pop(); 
            return val; 
        }
        bool empty() { return q.empty(); }
        bool full() { return false; }
    };

#endif

#define N_AXIS 0
#define H_AXIS 1
#define W_AXIS 2
#define C_AXIS 3

struct Shape {
    int N; // Batch size
    int H; // Height
    int W; // Width
    int C; // Channels
    /**
     * @brief New Shape object with full 0-sizes
     * 
     */
    Shape() : N(0), H(0), W(0), C(0) {}
    /**
     * @brief New Shape object
     * 
     * @param n Batchsize
     * @param h Height
     * @param w Width
     * @param c Channels
     */
    Shape(int n, int h, int w, int c) : N(n), H(h), W(w), C(c) {}

    bool operator==(const Shape &other) const {
        return N == other.N && H == other.H && W == other.W && C == other.C;
    }
};

template <typename T>
class TensorMem {
private:
    T* data;
    bool own_memory;

    inline int index(int n, int h, int w, int c);

public:
    Shape shape;

    /**
     * @brief New Tensor with no mem
     */
    TensorMem();
    /**
     * @brief New Tensor
     * 
     * @param data 
     * @param shape {N, H, W, C}
     * @param is_own_memory 
     */
    TensorMem(T* data, const Shape &shape, bool own_memory);
    /**
     * @brief New Tensor with own full 0s mem
     * 
     * @param shape {N, H, W, C}
     */
    TensorMem(const Shape &shape);
    ~TensorMem();

    /**
     * @brief print data of tensor
     * 
     */
    void print();

    /**
     * @brief get tensor value at position
     * 
     * @param n 
     * @param h 
     * @param w 
     * @param c 
     * @return (T) value 
     */
    inline T get(int n, int h, int w, int c);

    /**
     * @brief get tensor reference at position
     * 
     * @param n 
     * @param h 
     * @param w 
     * @param c 
     * @return (T) reference 
     */
    inline T &at(int n, int h, int w, int c);

    /**
     * @brief raw data
     * 
     * @return T* 
     */
    inline T* raw() { return data; }
    inline const T* raw() const { return data; }

    void load_tile_to_stream(int n, int h_start, int w_start, int h_size, int w_size, MyStream<T>& out_stream);
    void store_stream_to_mem(int n, int h_start, int w_start, int h_size, int w_size, MyStream<T>& in_stream);
};



struct Conv_Attributes {
    int dilations[2];          // [a channel filter spacing]
    int group;
    int kernel_shape[2];       // [H, W]
    int pads[4];               // [top, left, bottom, right]
    int strides[2];            // [H, W]
};
struct ConvTranspose_Attributes {
    int dilations[2];          // [a channel filter spacing]
    int group;
    int kernel_shape[2];       // [H, W]
    int output_padding[2];     // [bottom, right]
    int pads[4];               // [top, left, bottom, right]
    int strides[2];            // [H, W]
};

/**
 * @brief 
 * 
 * @param attributes parameters of conv -- {[0], [1], [2], [3], [4]}
 * @param attributes[0] -- {dilations_h, dilations_w}
 * @param attributes[1] -- group
 * @param attributes[2] -- {kernel_size_h, kernel_size_w}
 * @param attributes[3] -- {pad_top, pad_left, pad_bottom, pad_right}
 * @param attributes[4] -- {stride_h, stride_w}
 * 
 * @param X [Batch, H, W, Cin]
 * @param W [Cout,  H, W, Cin]
 * @param B [  1,   1, 1, Cout]
 * @return TensorMem<float>* -- [Batch, H, W, Cout]
 */
TensorMem<float>* Conv(const Conv_Attributes &attributes, TensorMem<float>* X, TensorMem<float>* W, TensorMem<float>* B);
/**
 * @brief 
 * 
 * @param attributes parameters of convtranspose -- {[0], [1], [2], [3], [4], [5]}
 * @param attributes[0] -- {dilations_h, dilations_w}
 * @param attributes[1] -- group
 * @param attributes[2] -- {kernel_size_h, kernel_size_w}
 * @param attributes[3] -- {output_pad_h, output_pad_w}
 * @param attributes[4] -- {pad_top, pad_left, pad_bottom, pad_right}
 * @param attributes[5] -- {stride_h, stride_w}
 * 
 * @param X [Batch, H, W, Cin]
 * @param W [Cout,  H, W, Cin]
 * @param B [  1,   1, 1, Cout]
 * @return TensorMem<float>* -- [Batch, H, W, Cout]
 */
TensorMem<float>* ConvTranspose(const ConvTranspose_Attributes &attributes, TensorMem<float>* X, TensorMem<float>* W, TensorMem<float>* B);

/**
 * @brief Concat X(s) into reference parameter Y
 * 
 * @param X {X1, X2, ..., Xn}
 * @param Y reference to res place
 * @param num_inputs n
 * @param axis -- N_AXIS || H_AXIS || W_AXIS || C_AXIS
 */
void Concat(TensorMem<int64_t>** X, TensorMem<int64_t> &Y, int num_inputs, int axis);
/**
 * @brief Concat X(s) and return res Y
 * 
 * @param X {X1, X2, ..., Xn}
 * @param num_inputs n
 * @param axis -- N_AXIS || H_AXIS || W_AXIS || C_AXIS
 * @return TensorMem<int64_t>*
 */
TensorMem<int64_t>* Concat(TensorMem<int64_t>** X, int num_inputs, int axis);

/**
 * @brief gather tensors chosen by indices[i] follow an axis
 * 
 * @param X tensor to choose X[ii]
 * @param Y reference to res place
 * @param indices {i1, i2, ..., in}
 * @param idx_count n
 * @param axis -- N_AXIS || H_AXIS || W_AXIS || C_AXIS
 */
void Gather(TensorMem<int64_t> &X, TensorMem<int64_t> &Y, const int* indices, int idx_count, int axis);
/**
 * @brief gather tensors chosen by indices[i] follow an axis
 * 
 * @param X tensor to choose X[ii]
 * @param indices {i1, i2, ..., in}
 * @param idx_count n
 * @param axis -- N_AXIS || H_AXIS || W_AXIS || C_AXIS
 * @return TensorMem<int64_t>*
 */
TensorMem<int64_t>* Gather(TensorMem<int64_t> &X, const int* indices, int idx_count, int axis);

/**
 * @brief copy X data to Y
 * 
 * @tparam T 
 * @param X 
 * @param Y = X
 */
template <typename T>
void Identity(const TensorMem<T> &X, TensorMem<T> &Y);
/**
 * @brief return a copy of X
 * 
 * @tparam T 
 * @param X 
 * @return TensorMem<T>* 
 */
template <typename T>
TensorMem<T>* Identity(const TensorMem<T> &X);

/**
 * @brief add 0s around X to Y
 * 
 * @param X 
 * @param Y 
 * [0 0 0]
 * [0 X 0]
 * [0 0 0]
 * @param pad_top 
 * @param pad_left 
 * @param pad_bottom 
 * @param pad_right 
 */
void Pad(TensorMem<float> &X, TensorMem<float> &Y, int pad_top, int pad_left, int pad_bottom, int pad_right);
/**
 * @brief return X added 0s around
 * 
 * @param X 
 * @param pad_top 
 * @param pad_left 
 * @param pad_bottom 
 * @param pad_right 
 * @return TensorMem<float>* 
 */
TensorMem<float>* Pad(TensorMem<float> &X, int pad_top, int pad_left, int pad_bottom, int pad_right);

/**
 * @brief copy X data to reshaped reference Y
 * 
 * @param X 
 * @param Y 
 */
void Reshape(TensorMem<float> &X, TensorMem<float> &Y);
/**
 * @brief reshape X
 * 
 * @param X 
 * @param shape 
 * @return TensorMem<float>* 
 */
TensorMem<float>* Reshape(TensorMem<float> &X, Shape shape);

/**
 * @brief return shape of X
 * 
 * @param X 
 * @return TensorMem<int64_t>* [N, H, W, C]
 */
template <typename T>
TensorMem<int64_t>* Shapeof(TensorMem<T> &X);

/**
 * @brief slice part of X from pos_0 to pos_1 to Y
 * 
 * @param X 
 * @param Y 
 * @param pos_0 [N0, H0, W0, C0]
 * @param pos_1 [N1, H1, W1, C1]
 */
void Slice(TensorMem<int64_t> &X, TensorMem<int64_t> &Y, Shape &pos_0, Shape &pos_1);
/**
 * @brief return a slice part of X from pos_0 to pos_1
 * 
 * @param X 
 * @param pos_0 [N0, H0, W0, C0]
 * @param pos_1 [N1, H1, W1, C1]
 * @return TensorMem<int64_t>* 
 */
TensorMem<int64_t>* Slice(TensorMem<int64_t> &X, Shape &pos_0, Shape &pos_1);

/**
 * @brief fill X permuted to Y
 * 
 * @param X 
 * @param Y 
 * @param perm [H_AXIS, C_AXIS, W_AXIS, N_AXIS] or any order else
 */
void Transpose(TensorMem<int64_t> &X, TensorMem<int64_t> &Y, int perm[]);
/**
 * @brief return X permuted
 * 
 * @param X 
 * @param perm [H_AXIS, C_AXIS, W_AXIS, N_AXIS] or any order else
 * @return TensorMem<int64_t>* 
 */
TensorMem<int64_t>* Transpose(TensorMem<int64_t> &X, int perm[]);

/**
 * @brief load const data node
 * 
 * @tparam T 
 * @param X 
 * @param Y = X
 * @param size 
 */
template <typename T>
void Constant(T* X, TensorMem<T> &Y, int size);
/**
 * @brief write a value full to tensor
 * 
 * @param Y 
 * @param val 
 */
void Constant_of_shape(TensorMem<int64_t> &Y, int64_t val);
/**
 * @brief return a tensor with full value
 * 
 * @param shape 
 * @param val 
 * @return TensorMem<int64_t>* 
 */
TensorMem<int64_t>* Constant_of_shape(Shape &shape, int64_t val);

/**
 * @brief load cast data from X to Y
 * 
 * @tparam T_IN 
 * @tparam T_OUT 
 * @param X 
 * @param Y = Cast<type_t>(X)
 */
template <typename T_IN, typename T_OUT>
void Cast(TensorMem<T_IN> &X, TensorMem<T_OUT> &Y);
/**
 * @brief return cast tensor from X
 * 
 * @tparam T_OUT 
 * @tparam T_IN 
 * @param X 
 * @return TensorMem<T_OUT>* Cast<type_t>(X)
 */
template <typename T_OUT, typename T_IN>
auto Cast(TensorMem<T_IN> &X) -> TensorMem<T_OUT>*;

/**
 * @brief load relu of X to Y
 * @brief ( can self apply )
 * 
 * @tparam T 
 * @param X 
 * @param Y = relu(X)
 */
template <typename T>
void Relu(TensorMem<T> &X, TensorMem<T> &Y);
/**
 * @brief return relu of X
 * 
 * @tparam T 
 * @param X 
 * @return TensorMem<T>* 
 */
template <typename T>
TensorMem<T>* Relu(TensorMem<T> &X);

/**
 * @brief load sqrt of X to Y
 * @brief ( can self apply )
 * 
 * @param X 
 * @param Y 
 */
void Sqrt(TensorMem<float> &X, TensorMem<float> &Y);
/**
 * @brief return sqrt of X
 * 
 * @param X 
 * @return TensorMem<float>* 
 */
TensorMem<float>* Sqrt(TensorMem<float> &X);

/**
 * @brief load sum X1, X2 to Y
 * @brief ( can self apply )
 * 
 * @param X1 
 * @param X2 
 * @param Y 
 */
void Add(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y);
/**
 * @brief return sum of X1, X2
 * 
 * @param X1 
 * @param X2 
 * @return TensorMem<float>* 
 */
TensorMem<float>* Add(TensorMem<float> &X1, TensorMem<float> &X2);
/**
 * @brief load X1 - X2 to Y
 * @brief ( can self apply )
 * 
 * @param X1 
 * @param X2 
 * @param Y 
 */
void Sub(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y);
/**
 * @brief return X1 - X2
 * 
 * @param X1 
 * @param X2 
 * @return TensorMem<float>* 
 */
TensorMem<float>* Sub(TensorMem<float> &X1, TensorMem<float> &X2);
/**
 * @brief load X1 elewise mul X2 to Y
 * @brief ( can self apply )
 * 
 * @param X1 
 * @param X2 
 * @param Y 
 */
void Mul(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y);
/**
 * @brief return elewise mul of X1, X2
 * 
 * @param X1 
 * @param X2 
 * @return TensorMem<float>* 
 */
TensorMem<float>* Mul(TensorMem<float> &X1, TensorMem<float> &X2);
/**
 * @brief load X1 elewise divide X2 to Y 
 * 
 * @param X1 
 * @param X2 
 * @param Y 
 */
void Div(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y);
/**
 * @brief return elewise divide of X1, X2
 * 
 * @param X1 
 * @param X2 
 * @return TensorMem<float>* 
 */
TensorMem<float>* Div(TensorMem<float> &X1, TensorMem<float> &X2);

/**
 * @brief load floor data X to Y
 * @brief ( can self apply )
 * 
 * @tparam T 
 * @param X 
 * @param Y 
 */
template <typename T>
void Floor(TensorMem<T> &X, TensorMem<T> &Y);
/**
 * @brief return floor of X
 * 
 * @tparam T 
 * @param X 
 * @return TensorMem<T>* 
 */
template <typename T>
TensorMem<T>* Floor(TensorMem<T> &X);

/**
 * @brief load a mean-1_axis tensor of X to Y
 * 
 * @tparam T 
 * @param X 
 * @param Y 
 * @param axis -- N_AXIS || H_AXIS || W_AXIS || C_AXIS
 */
template <typename T>
void ReduceMean(TensorMem<T> &X, TensorMem<T> &Y, int axis);
/**
 * @brief return a mean-1_axis of X
 * 
 * @tparam T 
 * @param X 
 * @param axis -- N_AXIS || H_AXIS || W_AXIS || C_AXIS
 * @return TensorMem<T>* 
 */
template <typename T>
TensorMem<T>* ReduceMean(TensorMem<T> &X, int axis);

/**
 * @brief load a product-1_axis tensor of X to Y
 * 
 * @tparam T 
 * @param X 
 * @param Y 
 * @param axis -- N_AXIS || H_AXIS || W_AXIS || C_AXIS
 */
template <typename T>
void ReduceProd(TensorMem<T> &X, TensorMem<T> &Y, int axis);
/**
 * @brief return a product-1_axis of X
 * 
 * @tparam T 
 * @param X 
 * @param axis -- N_AXIS || H_AXIS || W_AXIS || C_AXIS
 * @return TensorMem<T>* 
 */
template <typename T>
TensorMem<T>* ReduceProd(TensorMem<T> &X, int axis);

#endif 





