#include <iostream>
#include <ctime>
#include "../core/Core.h"
#include "../core/fake_stack.h"
#include "../core/tensor_io.tpp"

#define BATCH_SIZE 1

byte ARENA_DATA_1[20000000];
Arena ARENA_1(ARENA_DATA_1, 20000000);
byte ARENA_DATA_2[20000000];
Arena ARENA_2(ARENA_DATA_2, 20000000);


template<int C_IN, int H_IN, int W_IN>
void conv_block_i(TensorMem<_Float16> &input, TensorMem<_Float16> &output, 
                    TensorMem<_Float16> &weight, TensorMem<_Float16> &bias, 
                    TensorMem<_Float16> &gamma, TensorMem<_Float16> &beta, _Float16 epsilon) {
    int Cast_output_0[8] = {0, 1, 0, 0, 0, 0, 1, 0};

    Shape Pad_output_0_shape(BATCH_SIZE, H_IN + 1, W_IN + 1, C_IN);
    _Float16* Pad_output_0_data = ARENA_2.alloc<_Float16>(BATCH_SIZE* (H_IN + 1)* (W_IN + 1)* C_IN);
    TensorMem<_Float16> Pad_output_0(Pad_output_0_data, Pad_output_0_shape, false);

    Pad<_Float16>(input, Pad_output_0, Cast_output_0, "reflect", 0);


    Conv_Attributes conv_att(1, 1, 1, 3, 3, 0, 0, 0, 0, 2, 2);
    Conv(conv_att, &Pad_output_0, &weight, &bias, &input);
    ARENA_2.pop();

    
    Shape reducemean_shape(BATCH_SIZE, H_IN / 2, W_IN / 2, 1);
    _Float16* rdm_data = ARENA_1.alloc<_Float16>(BATCH_SIZE* H_IN* W_IN / 4);
    TensorMem<_Float16> reducemean(rdm_data, reducemean_shape, false);

    Norm<_Float16>(input, output, gamma, beta, epsilon, C_AXIS, reducemean);
    ARENA_1.pop();

    Relu(output, output);
}

void encoder(TensorMem<_Float16> &input, TensorMem<_Float16> &output) {
    _Float16 e = (_Float16) 0.001;

    // conv block 1
    int Cast_output_0[8] = {0, 3, 3, 0, 0, 3, 3, 0};

    Shape Pad_output_0_shape(BATCH_SIZE, 262, 262, 3);
    _Float16* Pad_output_0_data = ARENA_1.alloc<_Float16>(BATCH_SIZE* 262* 262* 3);
    TensorMem<_Float16> Pad_output_0(Pad_output_0_data, Pad_output_0_shape, false);

    Pad<_Float16>(input, Pad_output_0, Cast_output_0, "reflect", 0);


    Shape weight_1_shape(60, 7, 7, 3);
    _Float16* weight_1_data = ARENA_1.alloc<_Float16>(60* 7* 7* 3);
    TensorMem<_Float16> weight_1(weight_1_data, weight_1_shape, false);

    Shape bias_1_shape(1, 1, 1, 60);
    _Float16* bias_1_data = ARENA_1.alloc<_Float16>(60);
    TensorMem<_Float16> bias_1(bias_1_data, bias_1_shape, false);

    Shape conv_1_output_shape(BATCH_SIZE, 256, 256, 60);
    _Float16* conv_1_output_data = ARENA_2.alloc<_Float16>(BATCH_SIZE* 256* 256* 60);
    TensorMem<_Float16> conv_1_output(conv_1_output_data, conv_1_output_shape, false);

    _Float16* gamma_1_data = ARENA_2.alloc<_Float16>(60);
    TensorMem<_Float16> gamma_1(gamma_1_data, bias_1_shape, false);

    _Float16* beta_1_data = ARENA_2.alloc<_Float16>(60);
    TensorMem<_Float16> beta_1(beta_1_data, bias_1_shape, false);

    Conv_Attributes conv_1_att(1, 1, 1, 7, 7, 0, 0, 0, 0, 1, 1);
    Conv(conv_1_att, &Pad_output_0, &weight_1, &bias_1, &conv_1_output);
    ARENA_1.pop();
    ARENA_1.pop();
    ARENA_1.pop();

    _Float16* norm_1_output_data = ARENA_1.alloc<_Float16>(BATCH_SIZE* 256* 256* 60);
    TensorMem<_Float16> norm_1_output(norm_1_output_data, conv_1_output_shape, false);

    Shape rdm_1_shape(BATCH_SIZE, 256, 256, 1);
    _Float16* rdm_1_data = ARENA_1.alloc<_Float16>(BATCH_SIZE* 256* 256);
    TensorMem<_Float16> rdm_1(norm_1_output_data, conv_1_output_shape, false);

    Norm(conv_1_output, norm_1_output, gamma_1, beta_1, e, C_AXIS, rdm_1);
    ARENA_2.pop();
    ARENA_2.pop();
    ARENA_2.pop();
    ARENA_1.pop();

    Relu(norm_1_output, norm_1_output);
    

    // conv block 2
    Shape weight_2_shape(120, 3, 3, 60);
    _Float16* weight_2_data = ARENA_1.alloc<_Float16>(120* 3* 3* 60);
    TensorMem<_Float16> weight_2(weight_2_data, weight_2_shape, false);

    Shape bias_2_shape(1, 1, 1, 120);
    _Float16* bias_2_data = ARENA_1.alloc<_Float16>(120);
    TensorMem<_Float16> bias_2(bias_2_data, bias_2_shape, false);

    Shape norm_2_output_shape(BATCH_SIZE, 128, 128, 120);
    _Float16* norm_2_output_data = ARENA_2.alloc<_Float16>(BATCH_SIZE* 128* 128* 120);
    TensorMem<_Float16> norm_2_output(norm_2_output_data, norm_2_output_shape, false);

    _Float16* gamma_2_data = ARENA_2.alloc<_Float16>(120);
    TensorMem<_Float16> gamma_2(gamma_2_data, bias_2_shape, false);

    _Float16* beta_2_data = ARENA_2.alloc<_Float16>(120);
    TensorMem<_Float16> beta_2(beta_2_data, bias_2_shape, false);

    conv_block_i<60, 256, 256>(norm_1_output, norm_2_output, weight_2, bias_2, gamma_2, beta_2, e);
    ARENA_1.pop();
    ARENA_1.pop();
    ARENA_1.pop();
    ARENA_2.pop();
    ARENA_2.pop();
    

    // conv block 3
    Shape weight_3_shape(240, 3, 3, 120);
    _Float16* weight_3_data = ARENA_2.alloc<_Float16>(240* 3* 3* 120);
    TensorMem<_Float16> weight_3(weight_3_data, weight_3_shape, false);

    Shape bias_3_shape(1, 1, 1, 240);
    _Float16* bias_3_data = ARENA_2.alloc<_Float16>(240);
    TensorMem<_Float16> bias_3(bias_3_data, bias_3_shape, false);

    Shape norm_3_output_shape(BATCH_SIZE, 64, 64, 240);
    _Float16* norm_3_output_data = ARENA_1.alloc<_Float16>(BATCH_SIZE* 64* 64* 240);
    TensorMem<_Float16> norm_3_output(norm_3_output_data, norm_3_output_shape, false);

    _Float16* gamma_3_data = ARENA_1.alloc<_Float16>(240);
    TensorMem<_Float16> gamma_3(gamma_3_data, bias_3_shape, false);

    _Float16* beta_3_data = ARENA_1.alloc<_Float16>(240);
    TensorMem<_Float16> beta_3(beta_3_data, bias_3_shape, false);

    conv_block_i<120, 128, 128>(norm_2_output, norm_3_output, weight_3, bias_3, gamma_3, beta_3, e);
    ARENA_1.pop();
    ARENA_1.pop();
    ARENA_2.pop();
    ARENA_2.pop();
    ARENA_2.pop();


    // conv block 4
    Shape weight_4_shape(480, 3, 3, 240);
    _Float16* weight_4_data = ARENA_1.alloc<_Float16>(480* 3* 3* 240);
    TensorMem<_Float16> weight_4(weight_4_data, weight_4_shape, false);

    Shape bias_4_shape(1, 1, 1, 480);
    _Float16* bias_4_data = ARENA_1.alloc<_Float16>(480);
    TensorMem<_Float16> bias_4(bias_4_data, bias_4_shape, false);

    Shape norm_4_output_shape(BATCH_SIZE, 32, 32, 480);
    _Float16* norm_4_output_data = ARENA_2.alloc<_Float16>(BATCH_SIZE* 32* 32* 480);
    TensorMem<_Float16> norm_4_output(norm_4_output_data, norm_4_output_shape, false);

    _Float16* gamma_4_data = ARENA_2.alloc<_Float16>(480);
    TensorMem<_Float16> gamma_4(gamma_4_data, bias_4_shape, false);

    _Float16* beta_4_data = ARENA_2.alloc<_Float16>(480);
    TensorMem<_Float16> beta_4(beta_4_data, bias_4_shape, false);

    conv_block_i<240, 64, 64>(norm_3_output, norm_4_output, weight_4, bias_4, gamma_4, beta_4, e);
    ARENA_1.pop();
    ARENA_1.pop();
    ARENA_1.pop();
    ARENA_2.pop();
    ARENA_2.pop();


    // conv block 5
    Shape weight_5_shape(960, 3, 3, 480);
    _Float16* weight_5_data = ARENA_2.alloc<_Float16>(960* 3* 3* 480);
    TensorMem<_Float16> weight_5(weight_5_data, weight_5_shape, false);

    Shape bias_5_shape(1, 1, 1, 960);
    _Float16* bias_5_data = ARENA_2.alloc<_Float16>(960);
    TensorMem<_Float16> bias_5(bias_5_data, bias_5_shape, false);

    Shape norm_5_output_shape(BATCH_SIZE, 16, 16, 960);
    _Float16* norm_5_output_data = ARENA_1.alloc<_Float16>(BATCH_SIZE* 16* 16* 960);
    TensorMem<_Float16> norm_5_output(norm_5_output_data, norm_5_output_shape, false);

    _Float16* gamma_5_data = ARENA_1.alloc<_Float16>(960);
    TensorMem<_Float16> gamma_5(gamma_5_data, bias_5_shape, false);

    _Float16* beta_5_data = ARENA_1.alloc<_Float16>(960);
    TensorMem<_Float16> beta_5(beta_5_data, bias_5_shape, false);

    conv_block_i<480, 32, 32>(norm_4_output, norm_5_output, weight_5, bias_5, gamma_5, beta_5, e);
    ARENA_1.pop();
    ARENA_1.pop();
    ARENA_2.pop();
    ARENA_2.pop();
    ARENA_2.pop();


    // conv block out
    int Cast_output_1[8] = {0, 1, 1, 0, 0, 1, 1, 0};

    Shape Pad_output_1_shape(BATCH_SIZE, 18, 18, 960);
    _Float16* Pad_output_1_data = ARENA_2.alloc<_Float16>(BATCH_SIZE* 18* 18* 960);
    TensorMem<_Float16> Pad_output_1(Pad_output_1_data, Pad_output_1_shape, false);

    Pad<_Float16>(norm_5_output, Pad_output_1, Cast_output_1, "reflect", 0);
    ARENA_1.pop();

    Shape weight_o_shape(220, 3, 3, 960);
    _Float16* weight_o_data = ARENA_1.alloc<_Float16>(220* 3* 3* 960);
    TensorMem<_Float16> weight_o(weight_o_data, weight_o_shape, false);

    Shape bias_o_shape(1, 1, 1, 220);
    _Float16* bias_o_data = ARENA_1.alloc<_Float16>(220);
    TensorMem<_Float16> bias_o(bias_o_data, bias_o_shape, false);

    Conv_Attributes conv_2_att(1, 1, 1, 3, 3, 0, 0, 0, 0, 1, 1);
    Conv(conv_2_att, &Pad_output_1, &weight_o, &bias_o, &output);
    ARENA_1.pop();
    ARENA_1.pop();
    ARENA_2.pop();
}

void hyperprior(TensorMem<_Float16> &input, TensorMem<_Float16> &output) {
    // analysis
    Shape weight_1_shape(320, 3, 3, 220);
    _Float16* weight_1_data = ARENA_1.alloc<_Float16>(320* 3* 3* 220);
    TensorMem<_Float16> weight_1(weight_1_data, weight_1_shape, false);

    Shape bias_1_shape(1, 1, 1, 320);
    _Float16* bias_1_data = ARENA_1.alloc<_Float16>(320);
    TensorMem<_Float16> bias_1(bias_1_data, bias_1_shape, false);

    Shape conv_1_output_shape(BATCH_SIZE, 16, 16, 320);
    _Float16* conv_1_output_data = ARENA_2.alloc<_Float16>(BATCH_SIZE* 16* 16* 320);
    TensorMem<_Float16> conv_1_output(conv_1_output_data, conv_1_output_shape, false);

    read_tensor("", weight_1);
    read_tensor("", bias_1);

    Conv_Attributes conv_1_att(1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1);
    Conv(conv_1_att, &input, &weight_1, &bias_1, &conv_1_output);
    ARENA_1.pop();
    ARENA_1.pop();


    Relu(conv_1_output, conv_1_output);


    int Cast_output_0[8] = {0, 2, 2, 0, 0, 2, 2, 0};

    Shape Pad_1_output_shape(BATCH_SIZE, 20, 20, 320);
    _Float16* Pad_1_output_data = ARENA_1.alloc<_Float16>(BATCH_SIZE* 20* 20* 320);
    TensorMem<_Float16> Pad_1_output(Pad_1_output_data, Pad_1_output_shape, false);

    Pad<_Float16>(conv_1_output, Pad_1_output, Cast_output_0, "reflect", 0);
    ARENA_2.pop();


    Shape weight_2_shape(320, 5, 5, 320);
    _Float16* weight_2_data = ARENA_1.alloc<_Float16>(320* 5* 5* 320);
    TensorMem<_Float16> weight_2(weight_2_data, weight_2_shape, false);

    _Float16* bias_2_data = ARENA_1.alloc<_Float16>(320);
    TensorMem<_Float16> bias_2(bias_2_data, bias_1_shape, false);

    Shape conv_2_output_shape(BATCH_SIZE, 8, 8, 320);
    _Float16* conv_2_output_data = ARENA_2.alloc<_Float16>(BATCH_SIZE* 8* 8* 320);
    TensorMem<_Float16> conv_2_output(conv_2_output_data, conv_2_output_shape, false);

    read_tensor("", weight_2);
    read_tensor("", bias_2);

    Conv_Attributes conv_2_att(1, 1, 1, 5, 5, 0, 0, 0, 0, 2, 2);
    Conv(conv_2_att, &Pad_1_output, &weight_2, &bias_2, &conv_2_output);
    ARENA_1.pop();
    ARENA_1.pop();
    ARENA_1.pop();


    Relu(conv_2_output, conv_2_output);


    Shape Pad_2_output_shape(BATCH_SIZE, 12, 12, 320);
    _Float16* Pad_2_output_data = ARENA_1.alloc<_Float16>(BATCH_SIZE* 12* 12* 320);
    TensorMem<_Float16> Pad_2_output(Pad_2_output_data, Pad_2_output_shape, false);

    Pad<_Float16>(conv_2_output, Pad_2_output, Cast_output_0, "reflect", 0);
    ARENA_2.pop();


    _Float16* weight_3_data = ARENA_1.alloc<_Float16>(320* 5* 5* 320);
    TensorMem<_Float16> weight_3(weight_3_data, weight_2_shape, false);

    _Float16* bias_3_data = ARENA_1.alloc<_Float16>(320);
    TensorMem<_Float16> bias_3(bias_3_data, bias_1_shape, false);

    Shape conv_3_output_shape(BATCH_SIZE, 4, 4, 320);
    _Float16* conv_3_output_data = ARENA_2.alloc<_Float16>(BATCH_SIZE* 4* 4* 320);
    TensorMem<_Float16> conv_3_output(conv_3_output_data, conv_3_output_shape, false);

    read_tensor("", weight_3);
    read_tensor("", bias_3);

    Conv(conv_2_att, &Pad_2_output, &weight_3, &bias_3, &conv_3_output);
    ARENA_1.pop();
    ARENA_1.pop();
    ARENA_1.pop();


    Round(conv_3_output, conv_3_output);


    // synthesis
    _Float16* weight_4_data = ARENA_2.alloc<_Float16>(320* 5* 5* 320);
    TensorMem<_Float16> weight_4(weight_4_data, weight_2_shape, false);

    _Float16* bias_4_data = ARENA_2.alloc<_Float16>(320);
    TensorMem<_Float16> bias_4(bias_4_data, bias_1_shape, false);

    Shape conv_4_output_shape(BATCH_SIZE, 8, 8, 320);
    _Float16* conv_4_output_data = ARENA_1.alloc<_Float16>(BATCH_SIZE* 8* 8* 320);
    TensorMem<_Float16> conv_4_output(conv_4_output_data, conv_4_output_shape, false);

    read_tensor("", weight_4);
    read_tensor("", bias_4);

    ConvTranspose_Attributes conv_4_att(1, 1, 1, 5, 5, 1, 1, 2, 2, 2, 2, 2, 2);
    ConvTranspose(conv_4_att, &conv_3_output, &weight_4, &bias_4, &conv_4_output);
    ARENA_2.pop();
    ARENA_2.pop();
    ARENA_2.pop();


    Relu(conv_4_output, conv_4_output);


    _Float16* weight_5_data = ARENA_1.alloc<_Float16>(320* 5* 5* 320);
    TensorMem<_Float16> weight_5(weight_5_data, weight_2_shape, false);

    _Float16* bias_5_data = ARENA_1.alloc<_Float16>(320);
    TensorMem<_Float16> bias_5(bias_5_data, bias_1_shape, false);

    Shape conv_5_output_shape(BATCH_SIZE, 16, 16, 320);
    _Float16* conv_5_output_data = ARENA_2.alloc<_Float16>(BATCH_SIZE* 16* 16* 320);
    TensorMem<_Float16> conv_5_output(conv_5_output_data, conv_5_output_shape, false);

    read_tensor("", weight_5);
    read_tensor("", bias_5);

    ConvTranspose(conv_4_att, &conv_4_output, &weight_5, &bias_5, &conv_5_output);
    ARENA_1.pop();
    ARENA_1.pop();
    ARENA_1.pop();


    Relu(conv_5_output, conv_5_output);


    Shape weight_6_shape(220, 3, 3, 320);
    _Float16* weight_6_data = ARENA_2.alloc<_Float16>(220* 3* 3* 320);
    TensorMem<_Float16> weight_6(weight_6_data, weight_6_shape, false);

    Shape bias_6_shape(1, 1, 1, 220);
    _Float16* bias_6_data = ARENA_2.alloc<_Float16>(220);
    TensorMem<_Float16> bias_6(bias_6_data, bias_6_shape, false);

    Shape conv_6_output_shape(BATCH_SIZE, 16, 16, 220);
    _Float16* conv_6_output_data = ARENA_1.alloc<_Float16>(BATCH_SIZE* 16* 16* 220);
    TensorMem<_Float16> conv_6_output(conv_6_output_data, conv_6_output_shape, false);

    read_tensor("", weight_6);
    read_tensor("", bias_6);

    ConvTranspose_Attributes conv_6_att(1, 1, 1, 3, 3, 0, 0, 1, 1, 1, 1, 1, 1);
    ConvTranspose(conv_6_att, &conv_5_output, &weight_6, &bias_6, &conv_6_output);
    ARENA_2.pop();
    ARENA_2.pop();
    ARENA_2.pop();


    Sub(input, conv_6_output, output);
    Round(output, output);
    Add(conv_6_output, output, output);
    ARENA_1.pop();
}
