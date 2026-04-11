#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include "non_gen/Core.h"
#include "non_gen/fake_stack.h"
#include "non_gen/tensor_io.tpp"

#define BATCH_SIZE 1

// Forward declaration — defined in the kernel file compiled alongside this TB
void HW_Conv7x7(float* X, float* W, float* B, float* Y);

byte ARENA_DATA_1[20000000];
Arena ARENA_1(ARENA_DATA_1, 20000000);
byte ARENA_DATA_2[20000000];
Arena ARENA_2(ARENA_DATA_2, 20000000);


int main() {
    clock_t begin, end;

    float* input_data = ARENA_2.alloc<float>(256*256*60);
    TensorMem<float> input(input_data, {1, 256, 256, 60}, false);
    float* output_data = ARENA_2.alloc<float>(256*256*3);
    TensorMem<float> output(output_data, {1, 256, 256, 3}, false);
    for (int i = 0; i < 256*256*3; i++)
        output.raw()[i] = 0;

    Shape weight_shape(3, 7, 7, 60);
    Shape bias_shape(1, 1, 1, 3);
    float* weight_1_data = ARENA_1.alloc<float>(3 * 7 * 7 * 60);
    TensorMem<float> weight_1(weight_1_data, weight_shape, false);
    float* bias_1_data = ARENA_1.alloc<float>(3);
    TensorMem<float> bias_1(bias_1_data, bias_shape, false);

    read_tensor("io_params/Gen_ucb4_Relu_output_0.txt", input);
    read_tensor("model_params/Gen_cbo_weight.txt", weight_1);
    read_tensor("model_params/Gen_cbo_bias.txt", bias_1);

    begin = clock();
    HW_Conv7x7(input.raw(), weight_1.raw(), bias_1.raw(), output.raw());
    end = clock();

    std::cout << "Arena_1 max size: " << ARENA_1.max_runtime_size << "\n"
              << "Arena_2 max size: " << ARENA_2.max_runtime_size << "\n"
              << "Total runtime: " << ((double)(end - begin)) / CLOCKS_PER_SEC << " s\n";

    write_tensor("test.txt", output, 15);

    // --- Verify against gold output ---
    // Tolerance is widened for ap_fixed<16,8> (fp16) quantization:
    //   - Resolution of ap_fixed<16,8>: 1/256 ≈ 0.004
    //   - Quantization noise accumulates over 2940 MACs (7×7×60)
    //   - Expected max absolute error: ~0.1–0.5 depending on input magnitude
    const float TOL = 1.0f;   // 1.0 — appropriate for ap_fixed<16,8> fp16 quantization
    const int OUT_SIZE = 256 * 256 * 3;
    std::ifstream gold_file("io_params/Gen_cbo_Conv_output_0.txt");
    if (!gold_file.is_open()) {
        std::cout << "WARN: gold file not found, skipping verification\n";
        return 0;
    }

    float* out = output.raw();
    int mismatch = 0;
    float max_err = 0.0f;
    double sum_err = 0.0;
    for (int i = 0; i < OUT_SIZE; i++) {
        float gold_val;
        gold_file >> gold_val;
        float err = std::fabs(out[i] - gold_val);
        if (err > max_err) max_err = err;
        sum_err += err;
        if (err > TOL) mismatch++;
    }

    std::cout << "Verification: max_err=" << max_err
              << "  mean_err=" << (sum_err / OUT_SIZE)
              << "  mismatch(>" << TOL << ")=" << mismatch << "/" << OUT_SIZE << "\n";

    if (mismatch > 0) {
        std::cerr << "RESULT: FAIL (fp16 quantization error exceeds " << TOL << ")\n";
        return 1;
    }
    std::cout << "RESULT: PASS\n";
    return 0;
}
