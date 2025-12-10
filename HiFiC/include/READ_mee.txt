Instruction:

Tensor:
        Shape() : N(0), H(0), W(0), C(0)
        Shape(int n, int h, int w, int c) : N(n), H(h), W(w), C(c)

        template <typename T>
        class TensorMem {
            Shape shape;
            TensorMem();
            TensorMem(T* data, const Shape &shape, bool own_memory);
            TensorMem(const Shape &shape);
            ~TensorMem();

            void print();

            inline T get(int n, int h, int w, int c);
            inline T &at(int n, int h, int w, int c);

            inline T* raw() { return data; }
            inline const T* raw() const { return data; }

            void load_tile_to_stream(int n, int h_start, int w_start, int h_size, int w_size, MyStream<T>& out_stream);
            void store_stream_to_mem(int n, int h_start, int w_start, int h_size, int w_size, MyStream<T>& in_stream);
        };



Convolution:
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

    Conv:
        TensorMem<float>* Conv(Conv_Attributes &attributes, TensorMem<float>* X, TensorMem<float>* W, TensorMem<float>* B);
        @example: Y = Conv({{1, 1}, 1, {3, 3}, {0, 0, 0, 0}, {1, 1}}, X, W, B)

    ConvTranspose:
        TensorMem<float>* ConvTranspose(ConvTranspose_Attributes &attributes, TensorMem<float>* X, TensorMem<float>* W, TensorMem<float>* B);
        @example: Y = Conv({{1, 1}, 1, {3, 3}, {0, 0, 0, 0}, {1, 1}}, X, W, B)



Mem mainpulation:
    Concat:
            void Concat(TensorMem<int64_t>** X, TensorMem<int64_t> &Y, int num_inputs, int axis);
            TensorMem<int64_t>* Concat(TensorMem<int64_t>** X, int num_inputs, int axis);

    Gather:
            void Gather(TensorMem<int64_t> &X, TensorMem<int64_t> &Y, const int* indices, int idx_count, int axis);
            TensorMem<int64_t>* Gather(TensorMem<int64_t> &X, const int* indices, int idx_count, int axis);

    Identity:
            void Identity(const TensorMem<T> &X, TensorMem<T> &Y);
            TensorMem<T>* Identity(const TensorMem<T> &X);

    Pad:
            void Pad(TensorMem<float> &X, TensorMem<float> &Y, int pad_top, int pad_left, int pad_bottom, int pad_right);
            TensorMem<float>* Pad(TensorMem<float> &X, int pad_top, int pad_left, int pad_bottom, int pad_right);

    Reshape:
            void Reshape(TensorMem<float> &X, TensorMem<float> &Y);
            TensorMem<float>* Reshape(TensorMem<float> &X, Shape shape);

    Shapeof:
            Shape Shapeof(TensorMem<float> &X);

    Slice:
            void Slice(TensorMem<int64_t> &X, TensorMem<int64_t> &Y, Shape &pos_0, Shape &pos_1);
            TensorMem<int64_t>* Slice(TensorMem<int64_t> &X, Shape &pos_0, Shape &pos_1);

    Transpose:
            void Transpose(TensorMem<int64_t> &X, TensorMem<int64_t> &Y, int perm[]);
            TensorMem<int64_t>* Transpose(TensorMem<int64_t> &X, int perm[]);



Constant:
            void Constant(T* X, TensorMem<T> &Y, int size);
            void Constant_of_shape(TensorMem<int64_t> &Y, int64_t val);
            TensorMem<int64_t>* Constant_of_shape(Shape &shape, int64_t val);



Unary:
    Cast:
        void Cast(TensorMem<T_IN> &X, TensorMem<T_OUT> &Y);
        auto Cast(TensorMem<T_IN> &X) -> TensorMem<T_OUT>*;

    Relu:
        void Relu(TensorMem<T> &X, TensorMem<T> &Y);
        TensorMem<T>* Relu(TensorMem<T> &X);

    Sqrt:
        void Sqrt(TensorMem<float> &X, TensorMem<float> &Y);
        TensorMem<float>* Sqrt(TensorMem<float> &X);

    Floor:
        void Floor(TensorMem<T> &X, TensorMem<T> &Y);
        TensorMem<T>* Floor(TensorMem<T> &X);



Arithmatic:
    Add:
        void Add(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y);
        TensorMem<float>* Add(TensorMem<float> &X1, TensorMem<float> &X2);

    Sub:
        void Sub(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y);
        TensorMem<float>* Sub(TensorMem<float> &X1, TensorMem<float> &X2);

    Mul:
        void Mul(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y);
        TensorMem<float>* Mul(TensorMem<float> &X1, TensorMem<float> &X2);

    Div:
        void Div(TensorMem<float> &X1, TensorMem<float> &X2, TensorMem<float> &Y);
        TensorMem<float>* Div(TensorMem<float> &X1, TensorMem<float> &X2);
