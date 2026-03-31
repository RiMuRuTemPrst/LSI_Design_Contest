#include <iostream>
#include <ctime>
#include "gen\\Core.h"
#include "non_gen\\fake_stack.h"
#include "non_gen\\tensor_io.tpp"

#define BATCH_SIZE 1

byte ARENA_DATA_1[40000000];
Arena ARENA_1(ARENA_DATA_1, 40000000);
byte ARENA_DATA_2[40000000];
Arena ARENA_2(ARENA_DATA_2, 40000000);


template <typename T>
void Identity(const TensorMem<T> &X, TensorMem<T> &Y) {
    assert(X.shape == Y.shape);
    int size = X.shape.N* X.shape.H* X.shape.W* X.shape.C;
    for (int i = 0; i < size; i++) 
        Y.raw()[i] = X.raw()[i];
}


int main() {
    Shape input_shape(1, 256, 256, 3);

    clock_t begin, end, mid;

    float* input_data = ARENA_2.alloc<float>(16*16*960);
    TensorMem<float> input(input_data, {1, 16, 16, 960}, false);
    float* skip_data = ARENA_2.alloc<float>(16*16*960);
    TensorMem<float> skip(skip_data, {1, 16, 16, 960}, false);
    float* output_data = ARENA_2.alloc<float>(16*16*960);
    TensorMem<float> output(output_data, {1, 16, 16, 960}, false);

    Shape rb_weight_shape(960, 3, 3, 960);
    Shape rb_bias_shape(1, 1, 1, 960);
    float* rb_weight_1_data = ARENA_1.alloc<float>(960* 3* 3* 960);
    TensorMem<float> rb_weight_1(rb_weight_1_data, rb_weight_shape, false);
    float* rb_bias_1_data = ARENA_1.alloc<float>(960);
    TensorMem<float> rb_bias_1(rb_bias_1_data, rb_bias_shape, false);

    float* rb_gamma_1_data = ARENA_1.alloc<float>(960);
    TensorMem<float> rb_gamma_1(rb_gamma_1_data, rb_bias_shape, false);
    float* rb_beta_1_data = ARENA_1.alloc<float>(960);
    TensorMem<float> rb_beta_1(rb_beta_1_data, rb_bias_shape, false);

    float* rb_weight_2_data = ARENA_2.alloc<float>(960* 3* 3* 960);
    TensorMem<float> rb_weight_2(rb_weight_2_data, rb_weight_shape, false);
    float* rb_bias_2_data = ARENA_2.alloc<float>(960);
    TensorMem<float> rb_bias_2(rb_bias_2_data, rb_bias_shape, false);

    float* rb_gamma_2_data = ARENA_2.alloc<float>(960);
    TensorMem<float> rb_gamma_2(rb_gamma_2_data, rb_bias_shape, false);
    float* rb_beta_2_data = ARENA_2.alloc<float>(960);
    TensorMem<float> rb_beta_2(rb_beta_2_data, rb_bias_shape, false);

    read_tensor("..\\io_params\\Gen_rb0_Add_output_0.txt", input);
    read_tensor("..\\model_params\\Gen_rb0_gamma_1.txt", rb_gamma_1);
    read_tensor("..\\model_params\\Gen_rb0_beta_1.txt", rb_beta_1);
    read_tensor("..\\model_params\\Gen_rb0_weight_1.txt", rb_weight_1);
    read_tensor("..\\model_params\\Gen_rb0_bias_1.txt", rb_bias_1);
    read_tensor("..\\model_params\\Gen_rb0_gamma_2.txt", rb_gamma_2);
    read_tensor("..\\model_params\\Gen_rb0_beta_2.txt", rb_beta_2);
    read_tensor("..\\model_params\\Gen_rb0_weight_2.txt", rb_weight_2);
    read_tensor("..\\model_params\\Gen_rb0_bias_2.txt", rb_bias_2);

    begin = clock();

    Identity(input, skip);
    Resblock___Pad_ref_Conv_11133111111_CNorm_Relu___<16, 960, 960, 1>(input, 
                            rb_weight_1, rb_bias_1, /*rb_gamma_1, rb_beta_1, 
                            rb_weight_2, rb_bias_2, rb_gamma_2, rb_beta_2, (float) 0.001,*/ output);
    Channel_Norm_rb<16, 1, 960, 16, 16>(output, rb_gamma_1, rb_beta_1, (float) 0.001, input);
    Relu_rb<1, 960, 16, 16>(input);

    Resblock___Pad_ref_Conv_11133111111_CNorm_Relu___<16, 960, 960, 1>(input, 
                            rb_weight_2, rb_bias_2, output);
    Channel_Norm_rb<16, 1, 960, 16, 16>(output, rb_gamma_2, rb_beta_2, (float) 0.001, output);
    Add_rb<1, 960, 16, 16>(output, skip, output);
    end = clock();

    std::cout << "Total runtime: " << ((double) (end - begin)) / CLOCKS_PER_SEC << " s\n";

    write_tensor("test_1.txt", output, 15);

}