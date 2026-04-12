#pragma once
// ============================================================
// resblock_golden.h
// Golden reference implementation of ResBlock on CPU
// Computes expected output for HLS kernel validation
// Automatically uses MODEL_C, MODEL_H, MODEL_W from Core.h
// Supports both MINI_TEST (16 channels) and FULL_SIZE (960 channels)
// ============================================================
#include "../gen/Core.h"
#include <cmath>

// ============================================================
// 1. Reflect Padding Helper
// Returns reflected index if input index falls outside bounds [0, size)
// Example: size=5, idx=-1 → returns 1; idx=5 → returns 3
// ============================================================
static inline int reflect_pad(int idx, int size) {
    if (idx < 0)    return -idx;
    if (idx >= size) return 2 * size - idx - 2;
    return idx;
}

// ============================================================
// 2. 3x3 Convolution with Reflect Padding (NHWC Layout)
// Performs standard 2D convolution with boundary reflection
// Input:  X   [1, H, W, C_in]   - single batch, spatial dims, input channels
// Weights: W   [C_out, 3, 3, C_in] - co-located layout (matches Hard_op)
// Bias:    B   [C_out]            - per-output-channel bias
// Output: Y   [1, H, W, C_out]   - single batch, spatial dims, output channels
// ============================================================
static void golden_conv3x3_reflect(
    const float* X,     // Input tensor (spatial: H × W, channels: C_in)
    const float* W,     // Weight tensor (output ch: C_out, kernel: 3×3, input ch: C_in)
    const float* B,     // Bias vector (length C_out)
    float*       Y,     // Output tensor (spatial: H × W, channels: C_out)
    int H,              // Height of spatial dimensions
    int W_sz,           // Width of spatial dimensions
    int C_in,           // Number of input channels
    int C_out           // Number of output channels
) {
    for (int oh = 0; oh < H; oh++) {
        for (int ow = 0; ow < W_sz; ow++) {
            for (int co = 0; co < C_out; co++) {
                float acc = B[co];
                for (int kh = 0; kh < 3; kh++) {
                    for (int kw = 0; kw < 3; kw++) {
                        int ih = reflect_pad(oh - 1 + kh, H);
                        int iw = reflect_pad(ow - 1 + kw, W_sz);
                        const float* x_ptr = X + (ih * W_sz + iw) * C_in;
                        const float* w_ptr = W + ((co * 3 + kh) * 3 + kw) * C_in;
                        for (int ci = 0; ci < C_in; ci++) {
                            acc += x_ptr[ci] * w_ptr[ci];
                        }
                    }
                }
                Y[(oh * W_sz + ow) * C_out + co] = acc;
            }
        }
    }
}

// ============================================================
// 3. Channel Normalization (per-sample, across all channels)
// Computes batch normalization statistic per spatial location (H × W)
// Formula (matches Hard_op implementation):
//   num              = C                             (number of channels)
//   adjustment       = C / (C-1)                    (Bessel correction factor)
//   var_biased       = Σ(y·pre_div)²               (pre_div = 1/sqrt(C-1))
//   mean             = Σ(y) / C                    (per-location mean)
//   var              = var_biased - mean²·adjustment + εpsilon  (adjusted variance)
//   x_norm           = (y - mean) / sqrt(var)      (normalized activation)
//   out              = gamma·x_norm + beta         (affine transform)
// ============================================================
static void golden_channel_norm(
    const float* X,      // Input tensor [H × W × C] (normalized in-place into Y)
    const float* gamma,  // Scale parameter per channel [C]
    const float* beta,   // Shift parameter per channel [C]
    float        epsilon, // Small constant to prevent division by zero
    float*       Y,      // Output tensor [H × W × C] (normalized activation)
    int H,               // Height
    int W_sz,            // Width
    int C                // Channels
) {
    const float adjustment = (float)C / (float)(C - 1);
    const float pre_div    = 1.0f / std::sqrt((float)(C - 1));

    for (int h = 0; h < H; h++) {
        for (int w = 0; w < W_sz; w++) {
            const float* xp = X + (h * W_sz + w) * C;
            float*       yp = Y + (h * W_sz + w) * C;

            // Compute statistics: mean and scaled variance
            // Strategy: compute Σ(y·pre_div)² to match Hard_op's FP16 precision model
            float sum   = 0.0f;  // Accumulator for mean: Σ(y)
            float sumsq = 0.0f;  // Accumulator for variance: Σ((y·pre_div)²)
            for (int c = 0; c < C; c++) {
                float v  = xp[c];
                sum   += v;
                float vn = v * pre_div;  // Scale down before squaring (FP16 precision)
                sumsq += vn * vn;
            }
            float mean   = sum / (float)C;
            float var    = sumsq - mean * mean * adjustment + epsilon;
            float invstd = 1.0f / std::sqrt(var);

            // Apply affine transformation: y_out = gamma·(x-mean)/std + beta
            for (int c = 0; c < C; c++) {
                float weight = gamma[c] * invstd;
                float bias   = beta[c] - mean * weight;
                yp[c] = xp[c] * weight + bias;
            }
        }
    }
}

// ============================================================
// 4. Rectified Linear Unit (ReLU) - In-Place
// Applies ReLU activation: max(0, x) to entire tensor
// ============================================================

static void golden_relu(float* X, int total) {
    for (int i = 0; i < total; i++) {
        if (X[i] < 0.0f) X[i] = 0.0f;
    }
}

// ============================================================
// 5. Element-Wise Addition - In-Place
// Adds skip connection: Y += skip (residual pathway)
// ============================================================
static void golden_add(float* Y, const float* skip, int total) {
    for (int i = 0; i < total; i++) {
        Y[i] += skip[i];
    }
}

// ============================================================
// 6. Complete Golden ResBlock Implementation
// Computes reference output for HLS ResBlock kernel validation
//
// Data Flow:
//   skip ← Input copy (for residual pathway)
//   Z1   ← Conv3x3(Input, W1, B1) with reflect padding
//   Z1   ← ChannelNorm(Z1, gamma1, beta1, ε)
//   Z1   ← ReLU(Z1)
//   Z2   ← Conv3x3(Z1, W2, B2) with reflect padding
//   Z2   ← ChannelNorm(Z2, gamma2, beta2, ε)
//   Output ← Z2 + skip (residual addition)
//
// Template Parameter:
//   C_val allows independent calls without forcing MODEL_C
//   Useful for unit-testing with smaller channel counts
// ============================================================
template<int C_val = MODEL_C>
static void golden_resblock(
    TensorMem<data_t>& X,           // Input activation
    TensorMem<data_t>& W1,          // First conv weight
    TensorMem<data_t>& B1,          // First conv bias
    TensorMem<data_t>& gamma1,      // First norm scale
    TensorMem<data_t>& beta1,       // First norm shift
    TensorMem<data_t>& W2,          // Second conv weight
    TensorMem<data_t>& B2,          // Second conv bias
    TensorMem<data_t>& gamma2,      // Second norm scale
    TensorMem<data_t>& beta2,       // Second norm shift
    float              epsilon,      // Normalization epsilon
    TensorMem<data_t>& Y            // Output activation
) {
    constexpr int H    = MODEL_H;
    constexpr int W_sz = MODEL_W;
    constexpr int C    = C_val;
    const int     HWC  = H * W_sz * C;

    // Allocate working buffers in float32 (avoids FP16 precision loss from accumulation)
    float* x_f   = new float[HWC];
    float* w1_f  = new float[C * 9 * C];
    float* b1_f  = new float[C];
    float* g1_f  = new float[C];
    float* be1_f = new float[C];
    float* w2_f  = new float[C * 9 * C];
    float* b2_f  = new float[C];
    float* g2_f  = new float[C];
    float* be2_f = new float[C];
    float* z1_f  = new float[HWC];  // Conv1 output → ChannelNorm1 → ReLU output
    float* z2_f  = new float[HWC];  // Conv2 output → ChannelNorm2 output

    // Convert from half (FP16) to float (FP32) for higher precision calculations
    for (int i = 0; i < HWC;     i++) x_f[i]   = (float)X.raw()[i];
    for (int i = 0; i < C*9*C;   i++) w1_f[i]  = (float)W1.raw()[i];
    for (int i = 0; i < C;       i++) b1_f[i]  = (float)B1.raw()[i];
    for (int i = 0; i < C;       i++) g1_f[i]  = (float)gamma1.raw()[i];
    for (int i = 0; i < C;       i++) be1_f[i] = (float)beta1.raw()[i];
    for (int i = 0; i < C*9*C;   i++) w2_f[i]  = (float)W2.raw()[i];
    for (int i = 0; i < C;       i++) b2_f[i]  = (float)B2.raw()[i];
    for (int i = 0; i < C;       i++) g2_f[i]  = (float)gamma2.raw()[i];
    for (int i = 0; i < C;       i++) be2_f[i] = (float)beta2.raw()[i];

    // ===== Block 1: Conv1 → ChannelNorm1 → ReLU =====
    golden_conv3x3_reflect(x_f,  w1_f, b1_f, z1_f, H, W_sz, C, C);
    golden_channel_norm  (z1_f, g1_f, be1_f, epsilon, z1_f, H, W_sz, C);
    golden_relu           (z1_f, HWC);

    // ===== Block 2: Conv2 → ChannelNorm2 =====
    golden_conv3x3_reflect(z1_f, w2_f, b2_f, z2_f, H, W_sz, C, C);
    golden_channel_norm  (z2_f, g2_f, be2_f, epsilon, z2_f, H, W_sz, C);

    // ===== Residual Pathway: Y = z2 + skip(input) =====
    golden_add(z2_f, x_f, HWC);

    // Convert output from float (FP32) back to half (FP16) and store in TensorMem
    for (int i = 0; i < HWC; i++) Y.raw()[i] = (data_t)z2_f[i];

    // Free temporary working buffers
    delete[] x_f;
    delete[] w1_f;  delete[] b1_f;  delete[] g1_f;  delete[] be1_f;
    delete[] w2_f;  delete[] b2_f;  delete[] g2_f;  delete[] be2_f;
    delete[] z1_f;
    delete[] z2_f;
}