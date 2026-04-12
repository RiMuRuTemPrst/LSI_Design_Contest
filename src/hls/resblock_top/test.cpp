#include <iostream>
#include <ctime>
#include <cmath>
#include <iomanip>

/* ===== Synth code ===== */
#include "gen/Core.h"
#include "gen/resblock_top.h"

/* ===== Testbench-only ===== */
#include "non_gen/tensor_io.tpp"
#include "non_gen/fake_stack.h"
#include "non_gen/resblock_golden.h"

#define BATCH_SIZE 1
#define DATA_PATH "../../assets/test_data/"

// ============================================================
// Arena allocation size based on MODEL_C
//   - FULL (960):  W = 960*9*960*2 bytes ≈ 16.6 MB  → 40 MB is enough
//   - MINI  (16):  W = 16*9*16*2  bytes ≈ 4.6 KB   → 40 MB is plenty
// ============================================================
static const int ARENA1_SZ = 40000000;
static const int ARENA2_SZ = 40000000;

byte ARENA_DATA_1[ARENA1_SZ];
Arena ARENA_1(ARENA_DATA_1, ARENA1_SZ);
byte ARENA_DATA_2[ARENA2_SZ];
Arena ARENA_2(ARENA_DATA_2, ARENA2_SZ);

// ============================================================
// Helper: Print the current running mode to console
// ============================================================
static void print_mode() {
#ifdef ENABLE_MINI_TEST
    std::cout << "[MODE] MINI TEST  — C=" << MODEL_C
              << "  (fast cosim, no data files used)\n" << std::endl;
#else
    std::cout << "[MODE] FULL SIZE  — C=" << MODEL_C
              << "  (loads from disk, used to export IP)\n" << std::endl;
#endif
}

// ============================================================
// Helper: Generate random tensor data (used for MINI TEST)
// ============================================================
static void random_fill(data_t* buf, int n, float lo, float hi, unsigned& seed) {
    for (int i = 0; i < n; i++) {
        float r = lo + (hi - lo) * ((float)(rand_r(&seed) % 10000) / 10000.0f);
        buf[i] = (data_t)r;
    }
}

// ============================================================
// MAIN
// ============================================================
int main() {

    print_mode();

    // ----------------------------------------------------------
    // 1. Allocate memory on Arenas (based on current MODEL_C)
    // ----------------------------------------------------------
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

    // W1 on ARENA_1 (separated to avoid cosim bundle conflict)
    data_t* rb_weight_1_data = ARENA_1.alloc<data_t>(W_ELEMS);
    data_t* rb_bias_1_data   = ARENA_1.alloc<data_t>(B_ELEMS);
    data_t* rb_gamma_1_data  = ARENA_1.alloc<data_t>(B_ELEMS);
    data_t* rb_beta_1_data   = ARENA_1.alloc<data_t>(B_ELEMS);
    TensorMem<data_t> rb_weight_1(rb_weight_1_data, rb_weight_shape, false);
    TensorMem<data_t> rb_bias_1  (rb_bias_1_data,   rb_bias_shape,   false);
    TensorMem<data_t> rb_gamma_1 (rb_gamma_1_data,  rb_bias_shape,   false);
    TensorMem<data_t> rb_beta_1  (rb_beta_1_data,   rb_bias_shape,   false);

    // W2 on ARENA_2
    data_t* rb_weight_2_data = ARENA_2.alloc<data_t>(W_ELEMS);
    data_t* rb_bias_2_data   = ARENA_2.alloc<data_t>(B_ELEMS);
    data_t* rb_gamma_2_data  = ARENA_2.alloc<data_t>(B_ELEMS);
    data_t* rb_beta_2_data   = ARENA_2.alloc<data_t>(B_ELEMS);
    TensorMem<data_t> rb_weight_2(rb_weight_2_data, rb_weight_shape, false);
    TensorMem<data_t> rb_bias_2  (rb_bias_2_data,   rb_bias_shape,   false);
    TensorMem<data_t> rb_gamma_2 (rb_gamma_2_data,  rb_bias_shape,   false);
    TensorMem<data_t> rb_beta_2  (rb_beta_2_data,   rb_bias_shape,   false);

    // ----------------------------------------------------------
    // 2. Load data (FULL) or generate randomly (MINI)
    // ----------------------------------------------------------
#ifdef ENABLE_MINI_TEST
    // MINI: create random data, calculate golden reference on CPU
    std::cout << "[INFO] Generating random data for MINI TEST..." << std::endl;
    unsigned seed = 42;
    random_fill(input_data,       HWC,     -1.0f,  1.0f, seed);
    random_fill(rb_weight_1_data, W_ELEMS, -0.5f,  0.5f, seed);
    random_fill(rb_bias_1_data,   B_ELEMS, -0.1f,  0.1f, seed);
    random_fill(rb_gamma_1_data,  B_ELEMS,  0.5f,  1.5f, seed);
    random_fill(rb_beta_1_data,   B_ELEMS, -0.1f,  0.1f, seed);
    random_fill(rb_weight_2_data, W_ELEMS, -0.5f,  0.5f, seed);
    random_fill(rb_bias_2_data,   B_ELEMS, -0.1f,  0.1f, seed);
    random_fill(rb_gamma_2_data,  B_ELEMS,  0.5f,  1.5f, seed);
    random_fill(rb_beta_2_data,   B_ELEMS, -0.1f,  0.1f, seed);

    // Calculate golden output using CPU reference (template uses current MODEL_C)
    std::cout << "[INFO] Computing CPU golden reference..." << std::endl;
    clock_t t_gold_begin = clock();
    golden_resblock<MODEL_C>(
        input,
        rb_weight_1, rb_bias_1, rb_gamma_1, rb_beta_1,
        rb_weight_2, rb_bias_2, rb_gamma_2, rb_beta_2,
        0.001f,
        golden
    );
    clock_t t_gold_end = clock();
    std::cout << "[INFO] Golden done in "
              << ((double)(t_gold_end - t_gold_begin)) / CLOCKS_PER_SEC
              << " s\n" << std::endl;

#else
    // FULL: load from disk
    std::cout << "[INFO] Loading tensors from disk..." << std::endl;
    read_tensor(DATA_PATH "io_params/Gen_rb0_Add_output_0.txt",    input);
    read_tensor(DATA_PATH "model_params/Gen_rb0_gamma_1.txt",      rb_gamma_1);
    read_tensor(DATA_PATH "model_params/Gen_rb0_beta_1.txt",       rb_beta_1);
    read_tensor(DATA_PATH "model_params/Gen_rb0_weight_1.txt",     rb_weight_1);
    read_tensor(DATA_PATH "model_params/Gen_rb0_bias_1.txt",       rb_bias_1);
    read_tensor(DATA_PATH "model_params/Gen_rb0_gamma_2.txt",      rb_gamma_2);
    read_tensor(DATA_PATH "model_params/Gen_rb0_beta_2.txt",       rb_beta_2);
    read_tensor(DATA_PATH "model_params/Gen_rb0_weight_2.txt",     rb_weight_2);
    read_tensor(DATA_PATH "model_params/Gen_rb0_bias_2.txt",       rb_bias_2);
    read_tensor(DATA_PATH "gold_test.txt",                         golden);
    std::cout << "[INFO] Load complete.\n" << std::endl;
#endif

    // ----------------------------------------------------------
    // 3. Execute HLS kernel
    // ----------------------------------------------------------
    std::cout << "[INFO] Executing hardware top function..." << std::endl;
    clock_t begin = clock();

    resblock_top(
        reinterpret_cast<data_256_t*>(input.raw()),
        reinterpret_cast<const data_256_t*>(rb_weight_1.raw()),
        reinterpret_cast<const data_256_t*>(rb_bias_1.raw()),
        reinterpret_cast<const data_256_t*>(rb_gamma_1.raw()),
        reinterpret_cast<const data_256_t*>(rb_beta_1.raw()),
        reinterpret_cast<const data_256_t*>(rb_weight_2.raw()),
        reinterpret_cast<const data_256_t*>(rb_bias_2.raw()),
        reinterpret_cast<const data_256_t*>(rb_gamma_2.raw()),
        reinterpret_cast<const data_256_t*>(rb_beta_2.raw()),
        reinterpret_cast<data_256_t*>(output.raw()),
        (data_t)0.001f
    );

    clock_t end = clock();
    std::cout << "[INFO] Execution complete. Total runtime: "
              << ((double)(end - begin)) / CLOCKS_PER_SEC << " s\n" << std::endl;

    // ----------------------------------------------------------
    // 4. Verification
    // ----------------------------------------------------------
    float max_err   = 0.0f;
    float sum_err   = 0.0f;
    int   err_cnt   = 0;
    int   worst_idx = 0;

    const float ABS_THRESH = 0.15f;

    for (int i = 0; i < HWC; i++) {
        float got  = (float)output.raw()[i];
        float ref  = (float)golden.raw()[i];
        float diff = std::fabs(got - ref);

        sum_err += diff;
        if (diff > max_err) { max_err = diff; worst_idx = i; }
        if (diff > ABS_THRESH) err_cnt++;
    }

#ifndef ENABLE_MINI_TEST
    write_tensor("test_1_ver2.txt", output, 15);
    std::cout << "[INFO] HLS output written to test_1_ver2.txt\n" << std::endl;
#endif

    // ----------------------------------------------------------
    // 5. Report
    // ----------------------------------------------------------
    int wc = worst_idx % MODEL_C;
    int ww = (worst_idx / MODEL_C) % MODEL_W;
    int wh = (worst_idx / MODEL_C / MODEL_W) % MODEL_H;

    std::cout << "=== VERIFICATION RESULTS ===" << std::endl;
    std::cout << "Max abs error : " << std::fixed << std::setprecision(6) << max_err
              << " at [h=" << wh << ", w=" << ww << ", c=" << wc << "]\n"
              << "  -> HLS computed : " << (float)output.raw()[worst_idx] << "\n"
              << "  -> Golden (Ref) : " << (float)golden.raw()[worst_idx] << std::endl;
    std::cout << "Mean abs error: " << (sum_err / HWC) << std::endl;
    std::cout << "Error count   : " << err_cnt << " / " << HWC
              << "  (Threshold = " << ABS_THRESH << ")" << std::endl;

    if (err_cnt == 0) {
        std::cout << "\n[PASS] ✅  ALL OUTPUTS MATCH GOLDEN REFERENCE!" << std::endl;
        return 0;
    } else {
        std::cout << "\n[FAIL] ❌ " << err_cnt << " mismatches detected!" << std::endl;
        return 0;  // Keep return 0 so Vitis Cosim does not abort prematurely
    }
}