#include <iostream>
#include <ctime>
#include "non_gen\\Core.h"
#include "non_gen\\fake_stack.h"
#include "non_gen\\tensor_io.tpp"


#define BATCH_SIZE 1

byte ARENA_DATA_1[20000000];
Arena ARENA_1(ARENA_DATA_1, 20000000);
byte ARENA_DATA_2[20000000];
Arena ARENA_2(ARENA_DATA_2, 20000000);



int main() {
    clock_t begin, end, mid;

    float* input_data = ARENA_2.alloc<float>(16*16*960);
    TensorMem<float> input(input_data, {1, 16, 16, 960}, false);
    float* output_data = ARENA_2.alloc<float>(32*32*480);
    TensorMem<float> output(output_data, {1, 32, 32, 480}, false);
    for (int i = 0; i < 32*32*480; i++) 
        output.raw()[i] = 0;

    Shape weight_shape(480, 3, 3, 960);
    Shape bias_shape(1, 1, 1, 480);
    float* weight_1_data = ARENA_1.alloc<float>(960* 3* 3* 480);
    TensorMem<float> weight_1(weight_1_data, weight_shape, false);
    float* bias_1_data = ARENA_1.alloc<float>(480);
    TensorMem<float> bias_1(bias_1_data, bias_shape, false);


    read_tensor("io_params\\Gen_Add_output_0.txt", input);
    read_tensor("..\\model_params\\Gen_ucb1_weight.txt", weight_1);
    read_tensor("..\\model_params\\Gen_ucb1_bias.txt", bias_1);

    // float* input_data = ARENA_2.alloc<float>(2*2*2);
    // TensorMem<float> input(input_data, {1, 2, 2, 2}, false);
    // for (int i = 0; i < 4*4*4; i++) 
    //     input.raw()[i] = 0;
    // input_data[0] = input_data[7] = 1;
    // float* output_data = ARENA_2.alloc<float>(4*4*4);
    // TensorMem<float> output(output_data, {1, 4, 4, 4}, false);
    // for (int i = 0; i < 4*4*4; i++) 
    //     output.raw()[i] = 0;

    // Shape weight_shape(4, 3, 3, 2);
    // Shape bias_shape(1, 1, 1, 4);
    // float* weight_1_data = ARENA_1.alloc<float>(4* 3* 3* 2);
    // TensorMem<float> weight_1(weight_1_data, weight_shape, false);
    // for (int i = 0; i < 18; i+=2) {
    //     weight_1.raw()[i] = 1;
    //     weight_1.raw()[i + 1] = 2;
    // }
    // for (int i = 0; i < 18; i+=2) {
    //     weight_1.raw()[18+i] = 1;
    //     weight_1.raw()[18+i + 1] = -1;
    // }
    // for (int i = 0; i < 18; i+=2) {
    //     weight_1.raw()[36+i] = 0;
    //     weight_1.raw()[36+i + 1] = 1;
    // }
    // for (int i = 0; i < 18; i+=2) {
    //     weight_1.raw()[54+i] = 2;
    //     weight_1.raw()[54+i + 1] = 0;
    // }
    // float* bias_1_data = ARENA_1.alloc<float>(4);
    // TensorMem<float> bias_1(bias_1_data, bias_shape, false);
    // for (int i = 0; i < 4; i++) 
    //     bias_1.raw()[i] = 0;

    begin = clock();

    //ConvTranspose_2<float>({1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 2, 2}, &input, &weight_1, &bias_1, &output);
    ConvTranspose<16, 1, 960, 480, 16, 16, 3, 3, 32, 32>(input.raw(), weight_1.raw(), bias_1.raw(), output.raw());
    //ConvTranspose<1, 1, 2, 4, 2, 2, 3, 3, 4, 4>(&input, &weight_1, &bias_1, &output);
    end = clock();

    std::cout << "Arena_1 max size: " << ARENA_1.max_runtime_size << "\n" 
                << "Arena_2 max size: " << ARENA_2.max_runtime_size << "\n"
                << "Total runtime: " << ((double) (end - begin)) / CLOCKS_PER_SEC << " s\n";

    write_tensor("test.txt", output, 15);

}