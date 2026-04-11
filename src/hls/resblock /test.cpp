// #include <iostream>
// #include <ctime>

// /* ===== Synth code ===== */
// #include "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/gen/Core.h"
// #include "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/gen/resblock_top.h"

// /* ===== Testbench-only ===== */
// #include "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/non_gen/fake_stack.h"
// #include "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/non_gen/tensor_io.tpp"

// #define BATCH_SIZE 1

// // ============================================================
// // Arena
// // ============================================================
// byte ARENA_DATA_1[40000000];
// Arena ARENA_1(ARENA_DATA_1, 40000000);

// byte ARENA_DATA_2[40000000];
// Arena ARENA_2(ARENA_DATA_2, 40000000);

// // ============================================================
// // >>> TOP FUNCTION PROTOTYPE (BẮT BUỘC CHO COSIM)
// // ============================================================



// // ============================================================
// // Main
// // ============================================================
// int main() {

//     clock_t begin, end;

//     // --------------------------
//     // Input / Output tensors
//     // --------------------------
//     float* input_data = ARENA_2.alloc<float>(16 * 16 * 960);
//     TensorMem<float> input(input_data, {1, 16, 16, 960}, false);

//     float* output_data = ARENA_2.alloc<float>(16 * 16 * 960);
//     TensorMem<float> output(output_data, {1, 16, 16, 960}, false);

//     // --------------------------
//     // Weights / Bias / Norm params
//     // --------------------------
//     Shape rb_weight_shape(960, 3, 3, 960);
//     Shape rb_bias_shape(1, 1, 1, 960);

//     float* rb_weight_1_data = ARENA_1.alloc<float>(960 * 3 * 3 * 960);
//     TensorMem<float> rb_weight_1(rb_weight_1_data, rb_weight_shape, false);

//     float* rb_bias_1_data = ARENA_1.alloc<float>(960);
//     TensorMem<float> rb_bias_1(rb_bias_1_data, rb_bias_shape, false);

//     float* rb_gamma_1_data = ARENA_1.alloc<float>(960);
//     TensorMem<float> rb_gamma_1(rb_gamma_1_data, rb_bias_shape, false);

//     float* rb_beta_1_data = ARENA_1.alloc<float>(960);
//     TensorMem<float> rb_beta_1(rb_beta_1_data, rb_bias_shape, false);

//     float* rb_weight_2_data = ARENA_2.alloc<float>(960 * 3 * 3 * 960);
//     TensorMem<float> rb_weight_2(rb_weight_2_data, rb_weight_shape, false);

//     float* rb_bias_2_data = ARENA_2.alloc<float>(960);
//     TensorMem<float> rb_bias_2(rb_bias_2_data, rb_bias_shape, false);

//     float* rb_gamma_2_data = ARENA_2.alloc<float>(960);
//     TensorMem<float> rb_gamma_2(rb_gamma_2_data, rb_bias_shape, false);

//     float* rb_beta_2_data = ARENA_2.alloc<float>(960);
//     TensorMem<float> rb_beta_2(rb_beta_2_data, rb_bias_shape, false);

//     // --------------------------
//     // Load data
//     // --------------------------
//     read_tensor(
//         "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/io_params/Gen_rb0_Add_output_0.txt",
//         input
//     );

//     read_tensor(
//         "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/model_params/Gen_rb0_gamma_1.txt",
//         rb_gamma_1
//     );
//     read_tensor(
//         "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/model_params/Gen_rb0_beta_1.txt",
//         rb_beta_1
//     );
//     read_tensor(
//         "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/model_params/Gen_rb0_weight_1.txt",
//         rb_weight_1
//     );
//     read_tensor(
//         "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/model_params/Gen_rb0_bias_1.txt",
//         rb_bias_1
//     );

//     read_tensor(
//         "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/model_params/Gen_rb0_gamma_2.txt",
//         rb_gamma_2
//     );
//     read_tensor(
//         "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/model_params/Gen_rb0_beta_2.txt",
//         rb_beta_2
//     );
//     read_tensor(
//         "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/model_params/Gen_rb0_weight_2.txt",
//         rb_weight_2
//     );
//     read_tensor(
//         "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/model_params/Gen_rb0_bias_2.txt",
//         rb_bias_2
//     );

//     // --------------------------
//     // Call TOP FUNCTION (IMPORTANT)
//     // --------------------------
//     begin = clock();

//     resblock_top(
//         input.raw(),
//         rb_weight_1.raw(), rb_bias_1.raw(), rb_gamma_1.raw(), rb_beta_1.raw(),
//         rb_weight_2.raw(), rb_bias_2.raw(), rb_gamma_2.raw(), rb_beta_2.raw(),
//         output.raw(),
//         (data_t)0.001
//     );

//     end = clock();

//     std::cout << "C-sim runtime: "
//               << ((double)(end - begin)) / CLOCKS_PER_SEC
//               << " s\n";

//     // --------------------------
//     // Dump output
//     // --------------------------
//     write_tensor(
//         "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/test_1_ver2.txt",
//         output,
//         15
//     );

//     return 0;
// }
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

#include "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/gen/Core.h"
#include "/home/rimurutempest/Code/LSI_Design_Contest/HiFiC/GAN_HLS/gen/resblock_top.h"

static const int H = 16;
static const int W = 16;
static const int C_CH = 960;

static const int X_SIZE = 1 * H * W * C_CH;    
static const int W_SIZE = C_CH * 3 * 3 * C_CH; 
static const int V_SIZE = C_CH;                

int main() {
    printf("Starting C-Simulation Testbench (FP16 Mode)...\n");

    // Dùng new(std::nothrow) để tránh exception nếu hết RAM
    data_t* X   = new(std::nothrow) data_t[X_SIZE];
    data_t* Y   = new(std::nothrow) data_t[X_SIZE];
    data_t* W1  = new(std::nothrow) data_t[W_SIZE];
    data_t* W2  = new(std::nothrow) data_t[W_SIZE];
    data_t* B1  = new(std::nothrow) data_t[V_SIZE];
    data_t* B2  = new(std::nothrow) data_t[V_SIZE];
    data_t* G1  = new(std::nothrow) data_t[V_SIZE];
    data_t* G2  = new(std::nothrow) data_t[V_SIZE];
    data_t* BE1 = new(std::nothrow) data_t[V_SIZE];
    data_t* BE2 = new(std::nothrow) data_t[V_SIZE];

    if (!X||!Y||!W1||!W2||!B1||!B2||!G1||!G2||!BE1||!BE2) {
        printf("ALLOC FAIL\n");
        return 1;
    }

    // Khởi tạo dữ liệu (Ép kiểu sang data_t/half)
    // Lưu ý: data_t(0.01) gọi constructor của lớp half
    for (int i = 0; i < X_SIZE; i++) X[i] = data_t(0.01);
    
    // W1, W2 khởi tạo nhỏ
    for (int i = 0; i < W_SIZE; i++) { 
        W1[i] = data_t(0.001); 
        W2[i] = data_t(0.001); 
    }

    for (int i = 0; i < V_SIZE; i++) {
        B1[i] = data_t(0);  B2[i] = data_t(0);
        G1[i] = data_t(1);  G2[i] = data_t(1);
        BE1[i] = data_t(0); BE2[i] = data_t(0);
    }

    // Gọi hàm top
    resblock_top(
        X,
        W1, B1, G1, BE1,
        W2, B2, G2, BE2,
        Y,
        data_t(0.001) // epsilon
    );

    printf("COSIM TB finished OK\n");

    delete[] X; delete[] Y; delete[] W1; delete[] W2;
    delete[] B1; delete[] B2; delete[] G1; delete[] G2; delete[] BE1; delete[] BE2;
    return 0;
}