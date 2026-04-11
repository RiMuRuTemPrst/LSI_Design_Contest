#include <iostream>
#include <fstream>
#include <iomanip>
#include <inttypes.h>
#include "..\include\Core.h"


template<typename T>
TensorMem<T>* read_tensor(const char* file_path, const Shape &shape) {
    std::ifstream file(file_path);
    TensorMem<T>* tens = new TensorMem<T>(shape);
    int size = shape.N* shape.H* shape.W* shape.C;
    float val;
    int i = 0;
    while (file >> val) {
        tens->raw()[i++] = static_cast<T>(val);
    }
    return tens;
}
template<typename T>
void read_tensor(const char* file_path, TensorMem<T> &tens) {
    std::ifstream file(file_path);
    int size = tens.shape.N* tens.shape.H* tens.shape.W* tens.shape.C;
    float val;
    int i = 0;
    while (file >> val && i < size) {
        tens.raw()[i++] = static_cast<T>(val);
    }
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

void first_block(TensorMem<_Float16> &input, TensorMem<_Float16> &gamma, TensorMem<_Float16> &beta, TensorMem<_Float16> &output) {
    _Float16 prepad_data[16* 16* 220];
    TensorMem<_Float16> prepad(prepad_data, input.shape, false);
    Shape reducemean_shape = {1, 16, 16, 1};
    _Float16 rdm_data[16* 16];
    TensorMem<_Float16> reducemean(rdm_data, reducemean_shape, false);
    Norm<_Float16>(input, prepad, gamma, beta, (_Float16)0.001, C_AXIS, reducemean);
    Identity(prepad, output);
    // int prepad_params[8] = {0, 3, 3, 0, 0, 3, 3, 0};
    // Pad<_Float16>(prepad, output, prepad_params, "reflect", 0);
}

int main() {
    Shape real_input_shape = {1, 220, 16, 16};
    Shape input_shape = {1, 16, 16, 220};
    _Float16 real_input_data[220* 16* 16];
    TensorMem<_Float16> real_input(real_input_data, real_input_shape, false);
    _Float16 input_data[220* 16* 16];
    TensorMem<_Float16> input(input_data, input_shape, false);
    int trans_input[4] = {0, 2, 3, 1};
    read_tensor("Gen_cbi_cbi0_ReduceMean_1.txt", real_input);
    Transpose(real_input, input, trans_input);

    Shape gamma_shape = {1, 1, 1, 220};
    _Float16 gamma_data[220];
    _Float16 beta_data[220];
    TensorMem<_Float16> gamma(gamma_data, gamma_shape, false);
    TensorMem<_Float16> beta(beta_data, gamma_shape, false);
    read_tensor("Gen_cbi_cbi0_gamma.txt", gamma);
    read_tensor("Gen_cbi_cbi0_beta.txt", beta);

    _Float16 output_data[220* 16* 16];
    _Float16 real_output_data[220* 16* 16];
    TensorMem<_Float16> output(output_data, input_shape, false);
    TensorMem<_Float16> real_output(real_output_data, real_input_shape, false);

    first_block(input, gamma, beta, output);
    int trans_output[4] = {0, 3, 1, 2};
    Transpose(output, real_output, trans_output);
    write_tensor("test_output.txt", real_output);
    
    return 0;
}
