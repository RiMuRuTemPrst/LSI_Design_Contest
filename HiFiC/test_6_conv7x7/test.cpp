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

    float* input_data = ARENA_2.alloc<float>(256*256*60);
    TensorMem<float> input(input_data, {1, 256, 256, 60}, false);
    float* output_data = ARENA_2.alloc<float>(256*256*3);
    TensorMem<float> output(output_data, {1, 256, 256, 3}, false);
    for (int i = 0; i < 256*256*3; i++) 
        output.raw()[i] = 0;

    Shape weight_shape(3, 7, 7, 60);
    Shape bias_shape(1, 1, 1, 3);
    float* weight_1_data = ARENA_1.alloc<float>(3* 7* 7* 60);
    TensorMem<float> weight_1(weight_1_data, weight_shape, false);
    float* bias_1_data = ARENA_1.alloc<float>(3);
    TensorMem<float> bias_1(bias_1_data, bias_shape, false);


    read_tensor("..\\io_params\\Gen_ucb4_Relu_output_0.txt", input);
    read_tensor("..\\model_params\\Gen_cbo_weight.txt", weight_1);
    read_tensor("..\\model_params\\Gen_cbo_bias.txt", bias_1);

    begin = clock();

    
    Conv_7x7<12, 1, 60, 3, 256, 256, 7, 7, 256, 256>(input, weight_1, bias_1, output);
    end = clock();

    std::cout << "Arena_1 max size: " << ARENA_1.max_runtime_size << "\n" 
                << "Arena_2 max size: " << ARENA_2.max_runtime_size << "\n"
                << "Total runtime: " << ((double) (end - begin)) / CLOCKS_PER_SEC << " s\n";

    write_tensor("test.txt", output, 15);

}