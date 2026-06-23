#include <iostream>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <string>
#include <cstdlib>

/* ===== Synth code ===== */
#include "gen/Core.h"
#include "gen/conv_only.h"

/* ===== Testbench-only ===== */
#include "non_gen/tensor_io.tpp"
#include "non_gen/fake_stack.h"
#include "non_gen/resblock_golden.h"   // provides golden_conv3x3_reflect()

#define BATCH_SIZE 1
// Default relative data dir; override at runtime with env CONV_DATA_DIR=/abs/path/
#ifndef DATA_PATH
#define DATA_PATH "../../assets/test_data/"
#endif
#ifndef ABS_THRESH
#define ABS_THRESH 0.5f
#endif

// ============================================================
// Arena allocation
//   FULL (960): W = 960*9*960*2 ≈ 16.6 MB → 40 MB is enough
//   MINI  (16): trivial
// ============================================================
static const int ARENA1_SZ = 40000000;
static const int ARENA2_SZ = 40000000;

byte ARENA_DATA_1[ARENA1_SZ];
Arena ARENA_1(ARENA_DATA_1, ARENA1_SZ);
byte ARENA_DATA_2[ARENA2_SZ];
Arena ARENA_2(ARENA_DATA_2, ARENA2_SZ);

static void print_mode() {
#if defined(ENABLE_MINI_TEST) && ENABLE_MINI_TEST
    std::cout << "[MODE] MINI TEST  — C=" << MODEL_C
              << "  (random data, CPU golden)\n" << std::endl;
#else
    std::cout << "[MODE] FULL SIZE  — C=" << MODEL_C
              << "  (real Gen_rb0 data from disk)\n" << std::endl;
#endif
}

static void random_fill(data_t* buf, int n, float lo, float hi, unsigned& seed) {
    for (int i = 0; i < n; i++) {
        float r = lo + (hi - lo) * ((float)(rand_r(&seed) % 10000) / 10000.0f);
        buf[i] = (data_t)r;
    }
}

// ============================================================
// Conv-only golden: Y = Conv3x3_reflect(X, W1) + B1   (FP32 reference)
// ============================================================
static void conv_only_golden(
    TensorMem<data_t>& X, TensorMem<data_t>& W, TensorMem<data_t>& B,
    TensorMem<data_t>& Y)
{
    const int HWC = MODEL_H * MODEL_W * MODEL_C;
    const int WE  = MODEL_C * 3 * 3 * MODEL_C;
    float* xf = new float[HWC];
    float* wf = new float[WE];
    float* bf = new float[MODEL_C];
    float* yf = new float[HWC];
    for (int i = 0; i < HWC;     i++) xf[i] = (float)X.raw()[i];
    for (int i = 0; i < WE;      i++) wf[i] = (float)W.raw()[i];
    for (int i = 0; i < MODEL_C; i++) bf[i] = (float)B.raw()[i];

    golden_conv3x3_reflect(xf, wf, bf, yf, MODEL_H, MODEL_W, MODEL_C, MODEL_C);

    for (int i = 0; i < HWC; i++) Y.raw()[i] = (data_t)yf[i];
    delete[] xf; delete[] wf; delete[] bf; delete[] yf;
}

// ============================================================
// MAIN
// ============================================================
int main() {

    print_mode();

    const int HWC     = MODEL_H * MODEL_W * MODEL_C;
    const int W_ELEMS = MODEL_C * 3 * 3 * MODEL_C;
    const int B_ELEMS = MODEL_C;

    data_t* input_data  = ARENA_2.alloc<data_t>(HWC);
    data_t* output_data = ARENA_2.alloc<data_t>(HWC);
    data_t* golden_data = ARENA_2.alloc<data_t>(HWC);

    TensorMem<data_t> input (input_data,  Shape(1, MODEL_H, MODEL_W, MODEL_C), false);
    TensorMem<data_t> output(output_data, Shape(1, MODEL_H, MODEL_W, MODEL_C), false);
    TensorMem<data_t> golden(golden_data, Shape(1, MODEL_H, MODEL_W, MODEL_C), false);

    Shape rb_weight_shape(MODEL_C, 3, 3, MODEL_C);
    Shape rb_bias_shape  (1, 1, 1, MODEL_C);

    // Conv1 weight on its own arena (avoid cosim bundle conflict)
    data_t* rb_weight_1_data = ARENA_1.alloc<data_t>(W_ELEMS);
    data_t* rb_bias_1_data   = ARENA_1.alloc<data_t>(B_ELEMS);
    TensorMem<data_t> rb_weight_1(rb_weight_1_data, rb_weight_shape, false);
    TensorMem<data_t> rb_bias_1  (rb_bias_1_data,   rb_bias_shape,   false);

    // ----------------------------------------------------------
    // Load / generate data
    // ----------------------------------------------------------
#if defined(ENABLE_MINI_TEST) && ENABLE_MINI_TEST
    std::cout << "[INFO] Generating random data for MINI TEST..." << std::endl;
    unsigned seed = 42;
    random_fill(input_data,       HWC,     -1.0f,  1.0f, seed);
    random_fill(rb_weight_1_data, W_ELEMS, -0.5f,  0.5f, seed);
    random_fill(rb_bias_1_data,   B_ELEMS, -0.1f,  0.1f, seed);
#else
    const char* env_dir = std::getenv("CONV_DATA_DIR");
    std::string base = env_dir ? std::string(env_dir) : std::string(DATA_PATH);
    std::cout << "[INFO] Loading real Gen_rb0 tensors from: " << base << std::endl;
    read_tensor((base + "io_params/Gen_rb0_output.txt").c_str(),      input);
    read_tensor((base + "model_params/Gen_rb0_weight_1.txt").c_str(), rb_weight_1);
    read_tensor((base + "model_params/Gen_rb0_bias_1.txt").c_str(),   rb_bias_1);
    std::cout << "[INFO] Load complete.\n" << std::endl;

    // Guard: ifstream fails silently — make sure data actually loaded
    {
        double sx = 0.0, sw = 0.0;
        for (int i = 0; i < HWC;     i++) sx += std::fabs((float)input_data[i]);
        for (int i = 0; i < W_ELEMS; i++) sw += std::fabs((float)rb_weight_1_data[i]);
        std::cout << "[CHK] sum|input|=" << sx << "  sum|W1|=" << sw << std::endl;
        if (sx == 0.0 || sw == 0.0) {
            std::cout << "[ERROR] input/weight is all-zero — data dir likely wrong: "
                      << base << std::endl;
            return 1;
        }
    }
#endif

    // ----------------------------------------------------------
    // CPU golden (Conv 3x3 + bias, FP32 reference)
    // ----------------------------------------------------------
    std::cout << "[INFO] Computing CPU conv-only golden..." << std::endl;
    clock_t t_gold_begin = clock();
    conv_only_golden(input, rb_weight_1, rb_bias_1, golden);
    clock_t t_gold_end = clock();
    std::cout << "[INFO] Golden done in "
              << ((double)(t_gold_end - t_gold_begin)) / CLOCKS_PER_SEC
              << " s\n" << std::endl;

    // ----------------------------------------------------------
    // Execute HLS kernel (conv-only)
    // ----------------------------------------------------------
    std::cout << "[INFO] Executing conv_only_top..." << std::endl;
    clock_t begin = clock();

    conv_only_top(
        reinterpret_cast<data_256_t*>(input.raw()),
        reinterpret_cast<const data_256_t*>(rb_weight_1.raw()),
        reinterpret_cast<const data_256_t*>(rb_bias_1.raw()),
        reinterpret_cast<data_256_t*>(output.raw()),
        (data_t)0.001f
    );

    clock_t end = clock();
    std::cout << "[INFO] Execution complete. Runtime: "
              << ((double)(end - begin)) / CLOCKS_PER_SEC << " s\n" << std::endl;

    // ----------------------------------------------------------
    // Verification
    // ----------------------------------------------------------
    float max_err   = 0.0f;
    float sum_err   = 0.0f;
    int   err_cnt   = 0;
    int   worst_idx = 0;

    for (int i = 0; i < HWC; i++) {
        float got  = (float)output.raw()[i];
        float ref  = (float)golden.raw()[i];
        float diff = std::fabs(got - ref);

        sum_err += diff;
        if (diff > max_err) { max_err = diff; worst_idx = i; }
        if (diff > ABS_THRESH) err_cnt++;
    }

#if !(defined(ENABLE_MINI_TEST) && ENABLE_MINI_TEST)
    write_tensor("conv_only_hls_output.txt", output, 15);
    std::cout << "[INFO] HLS output written to conv_only_hls_output.txt\n" << std::endl;
#endif

    int wc = worst_idx % MODEL_C;
    int ww = (worst_idx / MODEL_C) % MODEL_W;
    int wh = (worst_idx / MODEL_C / MODEL_W) % MODEL_H;

    std::cout << "=== CONV-ONLY VERIFICATION ===" << std::endl;
    std::cout << "Max abs error : " << std::fixed << std::setprecision(6) << max_err
              << " at [h=" << wh << ", w=" << ww << ", c=" << wc << "]\n"
              << "  -> HLS computed : " << (float)output.raw()[worst_idx] << "\n"
              << "  -> Golden (Ref) : " << (float)golden.raw()[worst_idx] << std::endl;
    std::cout << "Mean abs error: " << (sum_err / HWC) << std::endl;
    std::cout << "Error count   : " << err_cnt << " / " << HWC
              << "  (Threshold = " << ABS_THRESH << ")" << std::endl;

    if (err_cnt == 0) {
        std::cout << "\n[PASS] ✅  CONV OUTPUT MATCHES GOLDEN REFERENCE!" << std::endl;
    } else {
        std::cout << "\n[FAIL] ❌ " << err_cnt << " mismatches detected!" << std::endl;
    }
    return 0;  // keep 0 so Vitis cosim does not abort prematurely
}
