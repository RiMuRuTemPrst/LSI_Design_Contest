#include <iostream>
#include <ctime>

// Ch? c?n s?a 1 dÃ²ng include d??i khi ??i mÃ¡y:
#include "/home/rimurutempest/Code/LSI_Design_Contest/HLS/convtranspose/src/non_gen/Core.h"
// #include "/home/qanh_k67/Project/GAN_HLS/HLS/Conv_Transpose_layer/src/non_gen/Core.h"

// PATH_FAKE_STACK_H vÃ  PATH_TENSOR_IO ?Ã£ ???c define trong Core.h
#include PATH_FAKE_STACK_H
#include PATH_TENSOR_IO

#define BATCH_SIZE 1

byte ARENA_DATA_1[25000000];
Arena ARENA_1(ARENA_DATA_1, 20000000);
byte ARENA_DATA_2[20000000];
Arena ARENA_2(ARENA_DATA_2, 20000000);

int main() {
    clock_t begin, end;

    data_t* input_data = ARENA_2.alloc<data_t>(16 * 16 * 960);
    TensorMem<data_t> input(input_data, {1, 16, 16, 960}, false);

    data_t* output_data = ARENA_2.alloc<data_t>(32 * 32 * 480);
    TensorMem<data_t> output(output_data, {1, 32, 32, 480}, false);
    for (int i = 0; i < 32 * 32 * 480; i++)
        output.raw()[i] = 0;

    Shape weight_shape(480, 3, 3, 960);
    Shape bias_shape(1, 1, 1, 480);

    data_t* weight_1_data = ARENA_1.alloc<data_t>(960 * 3 * 3 * 480);
    TensorMem<data_t> weight_1(weight_1_data, weight_shape, false);

    data_t* bias_1_data = ARENA_1.alloc<data_t>(480);
    TensorMem<data_t> bias_1(bias_1_data, bias_shape, false);

    read_tensor(PATH_IO_PARAMS "/Gen_Add_output_0.txt", input);
    read_tensor(PATH_IO_PARAMS "/Gen_ucb1_weight.txt",  weight_1);
    read_tensor(PATH_IO_PARAMS "/Gen_ucb1_bias.txt",    bias_1);

    begin = clock();
    ConvTranspose<16, 1, 960, 480, 16, 16, 3, 3, 32, 32, data_t>(
        input.raw(), weight_1.raw(), bias_1.raw(), output.raw()
    );
    end = clock();

    std::cout << "Arena_1 max size: " << ARENA_1.max_runtime_size << "\n"
              << "Arena_2 max size: " << ARENA_2.max_runtime_size << "\n"
              << "Total runtime: " << ((double)(end - begin)) / CLOCKS_PER_SEC << " s\n";

    write_tensor(PATH_IO_PARAMS "/output_test.txt", output, 15);

    return 0;
}