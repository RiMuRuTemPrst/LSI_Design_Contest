#include "..\include\Core.h"
#include "..\src\Arithmetic.cc"
#include "..\src\class_tensor.cpp"
#include "..\src\Conv.cc"
#include "..\src\ConvTranspose.cc"
#include "..\src\Manipulation.cc"
#include "..\src\Reduce.cc"
#include "..\src\Unary.cc"
#include <fstream>
#include <iomanip>

template<typename T>
TensorMem<T>* read_tensor(const char* file_path, const Shape &shape) {
    std::ifstream file(file_path);
    TensorMem<T>* tens = new TensorMem<T>(shape);
    int size = shape.N* shape.H* shape.W* shape.C;
    T val;
    int i = 0;
    while (file >> val) {
        tens->raw()[i++] = val;
    }
    return tens;
}
template<typename T>
void write_tensor(const char* file_path, TensorMem<T> &X) {
    std::ofstream file(file_path);
    int size = X.shape.N* X.shape.H* X.shape.W* X.shape.C;
    float val;
    for (int i = 0; i < size; i++) {
        val = X.raw()[i];
        file << std::fixed << std::setprecision(6) << val << std::endl;
    }
    file.close();
}
TensorMem<float>* forward_block(TensorMem<float>* input) {
    TensorMem<float>* cur;

    TensorMem<float>* conv_trans_w = read_tensor<float>("block_weight\\conv_transpose_w.txt", {60, 3, 3, 120});
    TensorMem<float>* conv_trans_b = read_tensor<float>("block_weight\\conv_transpose_b.txt", {1, 1, 1, 60});
    TensorMem<float>* conv_trans_y = ConvTranspose({{1, 1}, 1, {3, 3}, {1, 1}, {1, 1, 1, 1}, {2, 2}}, input, conv_trans_w, conv_trans_b);
    delete conv_trans_w;
    delete conv_trans_b;

    TensorMem<float>* reducemean_y00 = ReduceMean(*conv_trans_y, H_AXIS);
    TensorMem<float>* reducemean_y01 = ReduceMean(*reducemean_y00, W_AXIS);
    TensorMem<float>* sub_y0 = Sub(*conv_trans_y, *reducemean_y01);
    delete reducemean_y00;
    delete reducemean_y01;

    TensorMem<float>* mul_y0 = Mul(*sub_y0, *sub_y0);
    TensorMem<float>* reducemean_y10 = ReduceMean(*mul_y0, H_AXIS);
    TensorMem<float>* reducemean_y11 = ReduceMean(*reducemean_y10, W_AXIS);
    delete reducemean_y10;
    delete mul_y0;

    TensorMem<int64_t>* shape_y0 = Shapeof(*conv_trans_y);
    delete conv_trans_y;
    TensorMem<int64_t>* gather_y0 = Gather(*shape_y0, (int[]){3}, 1, C_AXIS);
    delete shape_y0;
    TensorMem<float>* cast_y0 = Cast<float>(*gather_y0);
    delete gather_y0;

    Mul(*reducemean_y11, *cast_y0, *reducemean_y11);
    TensorMem<float>* mul_y1 = reducemean_y11;
    cur = new TensorMem<float>(new float[1]{1}, {1, 1, 1, 1}, true);
    Sub(*cast_y0, *cur, *cur);
    delete cast_y0;
    Div(*mul_y1, *cur, *mul_y1);
    delete cur;
    cur = mul_y1;

    TensorMem<float>* epsilon = new TensorMem<float>(new float[1]{0.001}, {1, 1, 1, 1}, true);
    Add(*cur, *epsilon, *cur);
    Sqrt(*cur, *cur);
    epsilon->raw()[0] = 1;
    Div(*epsilon, *cur, *cur);
    delete epsilon;

    Mul(*sub_y0, *cur, *sub_y0);
    delete cur;
    cur = sub_y0;

    TensorMem<float>* norm_w = read_tensor<float>("block_weight\\inst_norm_scale.txt", {1, 1, 1, 60});
    TensorMem<float>* norm_b = read_tensor<float>("block_weight\\inst_norm_bias.txt", {1, 1, 1, 60});
    Mul(*norm_w, *cur, *cur);
    Add(*norm_b, *cur, *cur);
    delete norm_w;
    delete norm_b;
    Relu(*cur, *cur);

    TensorMem<int64_t>* constofshape_y = Constant_of_shape<int64_t>({1, 1, 1, 4}, 0);
    TensorMem<int64_t>* const1 = new TensorMem<int64_t>(new int64_t[4]{3, 3, 3, 3}, {1, 1, 1, 4}, true);
    TensorMem<int64_t>* concat_x[2] = {const1, constofshape_y};
    TensorMem<int64_t>* concat_y = Concat(concat_x, 2, C_AXIS);
    delete const1;
    delete constofshape_y;

    TensorMem<int64_t>* reshape_y0 = Reshape(*concat_y, {4, 1, 1, 2});
    TensorMem<int64_t>* slice_y = Slice(*reshape_y0, {3, 0, 0, 0}, {-1, 1, 1, 2}, (int[]){-1, 1, 1, 1});
    TensorMem<int64_t>* transpose_y = Transpose(*slice_y, (int[]){3, 1, 2, 0});
    TensorMem<int64_t>* reshape_y1 = Reshape(*transpose_y, {1, 1, 1, 8});
    TensorMem<int>* cast_y1 = Cast<int>(*reshape_y1);
    TensorMem<int>* temp = Gather(*cast_y1, (int[]){0, 2, 3, 1, 4, 6, 7, 5}, 8, C_AXIS);
    delete reshape_y0;
    delete slice_y;
    delete transpose_y;
    delete reshape_y1;
    delete cast_y1;

    TensorMem<float>* pad_y = Pad<float>(*cur, temp->raw(), "reflect", 0);
    delete cur;
    delete temp;

    TensorMem<float>* conv_w = read_tensor<float>("block_weight\\conv_w.txt", {3, 7, 7, 60});
    TensorMem<float>* conv_b = read_tensor<float>("block_weight\\conv_b.txt", {1, 1, 1, 3});
    TensorMem<float>* conv_y = Conv({{1, 1}, 1, {7, 7}, {0, 0, 0, 0}, {1, 1}}, pad_y, conv_w, conv_b);
    delete pad_y;
    delete conv_w;
    delete conv_b;

    return conv_y;
}
int main() {
    TensorMem<float>* inp = read_tensor<float>("block_weight\\input_data.txt", {1, 32, 32, 120}), *output;
    output = forward_block(inp);
    write_tensor("block_weight\\output_test.txt", *output);
    delete inp;
    delete output;
    // std::ofstream file("block_weight\\output_test.txt");
    // file.clear();
    // file.close();
}