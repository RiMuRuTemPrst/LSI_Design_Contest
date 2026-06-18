#include <iostream>
#include <stdint.h>
#include "..\include\Core.h"
#include "..\include\fake_stack.h"
#include "..\src\tensor_io.tpp"


#define BATCH_SIZE 1

byte ARENA_DATA_1[20000000];
Arena ARENA_1(ARENA_DATA_1, 20000000);
byte ARENA_DATA_2[20];
Arena ARENA_2(ARENA_DATA_2, 20);



int8_t differentiate(int32_t x) {
    if (x >= 1840700270)
        return 16;
    if (x >= 460175068)
        return 15;
    if (x >= 115043767)
        return 14;
    if (x >= 28760942)
        return 13;
    if (x >= 7190236)
        return 12;
    if (x >= 1797559)
        return 11;
    if (x >= 449390)
        return 10;
    if (x >= 112348)
        return 9;
    if (x >= 28087)
        return 8;
    if (x >= 7022)
        return 7;
    if (x >= 1756)
        return 6;
    if (x >= 439)
        return 5;
    if (x >= 110)
        return 4;
    if (x >= 28)
        return 3;
    if (x >= 7)
        return 2;
    if (x >= 2)
        return 1;
    return 0;
}
void reci_sqrt_int(int32_t x, int32_t* m, int8_t* s) {
    int64_t mult;
    int8_t first_shift = differentiate(x);
    int64_t _3_mul_2_pow_2s = 3LL << (first_shift << 1);

    // first loop
    // m0 = 1  ->  m1 = (3*2^2s - x) >> (s + 1)
    mult = (_3_mul_2_pow_2s - x) >> (first_shift + 1);

    // mid loop
    // mi+1 = mi * (3*2^2s - x*mi^2 >> 2s) >> (2s + 1)
    for (int i = 0; i < 2; i++) {
        mult = mult * (_3_mul_2_pow_2s - (x * (mult * mult >> first_shift) >> first_shift)) >> ((first_shift << 1) + 1);
    }

    // last loop
    mult = mult * (_3_mul_2_pow_2s - (x * (mult * mult >> first_shift) >> first_shift));
    int8_t final_shift = (first_shift << 2) + 1;
    int8_t check = final_shift - 32;
    if (check > 0) {
        mult = mult >> check;
        final_shift = 32;
    }

    *m = (int32_t) mult;
    *s = final_shift;
}
void Norm_vector_quantize(int16_t* X, int16_t* Y, 
                            int32_t* gamma, int8_t* gamma_shift, 
                            int32_t* beta, int16_t epsilon, 
                            int n, int16_t reci_n_mult, int8_t reci_n_shift) {
    int32_t sum_x = 0;
    int64_t sum_x_sq = 0;

    for (int i = 0; i < n; i++) {
        sum_x += X[i];
        sum_x_sq += (int32_t) X[i]* X[i];
    }

    int32_t mean_x = (int32_t) (((int64_t) sum_x * reci_n_mult) >> reci_n_shift);
    int64_t mean_x_sq = (sum_x_sq * reci_n_mult) >> reci_n_shift;

    int32_t var = (int32_t) (mean_x_sq - (int64_t) mean_x* mean_x);
    int32_t mult_inv;
    int8_t shift_inv;
    reci_sqrt_int(var + epsilon, &mult_inv, &shift_inv);

    for (int i = 0; i < n; i++) {
        int32_t mult_combined = (int32_t) ((int64_t) mult_inv* gamma[i] >> (shift_inv - 4));
        int8_t shift_combined = gamma_shift[i] + 4;

        int64_t val = (int64_t) ((int32_t) X[i] - mean_x) * mult_combined;
        if (shift_combined >= 0) 
            val >>= shift_combined;
        else 
            val <<= -shift_combined;

        Y[i] = clamp<int16_t>(val + beta[i]);
    }
}
template <int C>
void Channel_Norm_quantize(TensorMem<int16_t> &X, TensorMem<int16_t> &Y, 
                            TensorMem<int32_t> &gamma, TensorMem<int8_t> &gamma_shift, 
                            TensorMem<int32_t> &beta, int16_t epsilon, 
                            int16_t reci_C_mult, int8_t reci_C_shift) {
    for (int n = 0; n < X.shape.N; n++)
    for (int h = 0; h < X.shape.H; h++)
    for (int w = 0; w < X.shape.W; w++) {
        Norm_vector_quantize(X.raw_at(n, h, w, 0), Y.raw_at(n, h, w, 0), 
                                gamma.raw(), gamma_shift.raw(), beta.raw(), epsilon, 
                                C, reci_C_mult, reci_C_shift);
    }
}

template <typename T>
static int64_t _mul_quantize_multiplier(int64_t x, T M, int8_t shift) {
    int64_t prod = x* M;
    if (shift > 0) 
        // round(a / d) = (a + d/2) / d
        prod = (prod + (1LL << (shift - 1))) >> shift;
    else if (shift < 0) 
        prod <<= (-shift);
    return prod;
}

static int64_t _conv_pointq(const Conv_Attributes &att, TensorMem<int16_t>* X, TensorMem<int16_t>* W, 
                            int16_t x_zero_point, Shape &x_pos, Shape &w_pos) {
    int64_t y_point = 0;
    Shape x_shape = X->shape;
    int w_height = w_pos.H;
    int w_width = w_pos.W;

    int i, j, e, f;
    for (i = e = 0; i < w_height; i++, e += att.dilations[0]) 
    for (j = f = 0; j < w_width; j++, f += att.dilations[1]) {
        int h = x_pos.H + e, w = x_pos.W + f;
        if (h < 0) 
            h = -h;
        else if (h >= x_shape.H) 
            h = 2* x_shape.H - h - 2;
        if (w < 0) 
            w = -w;
        else if (w >= x_shape.W) 
            w = 2* x_shape.W - w - 2;
        int16_t x = X->get(x_pos.N, h, w, x_pos.C);
        int16_t weight = W->get(w_pos.N, i, j, w_pos.C);
        y_point += (int64_t) ((int32_t) x * weight) - (int32_t) x_zero_point * weight;
    }
    return y_point;
}
void Conv_quantize(const Conv_Attributes &attributes, 
                    TensorMem<int16_t>* X, TensorMem<int16_t>* W, TensorMem<int32_t>* B, TensorMem<int16_t>* Y, 
                    int16_t M, int8_t N, int16_t x_zero_point, int16_t y_zero_point) {

    static_assert(is_integral<int16_t>::value, "Error: Inappropriate type for QLConv !!");
    static_assert(is_integral<int16_t>::value, "Error: Inappropriate type for QLConv !!");
    static_assert(is_integral<int16_t>::value, "Error: Inappropriate type for QLConv !!");
    assert(X && W && B);
    Shape x_shape = X->shape;
    Shape w_shape = W->shape;
    Shape b_shape = B->shape;
    Shape y_shape = Y->shape;

    if (x_shape.C % attributes.group || w_shape.N % attributes.group || x_shape.C / attributes.group != w_shape.C
        || attributes.kernel_shape[0] != w_shape.H || attributes.kernel_shape[1] != w_shape.W) 
        assert(0 && "Error: Unappropriate input sizes for QLConv !!\n");
    
    if (y_shape.H != (x_shape.H - (w_shape.H - 1)* attributes.dilations[0] 
        + attributes.pads[0] + attributes.pads[2] - 1) / attributes.strides[0] + 1 
        || y_shape.W != (x_shape.W - (w_shape.W - 1)* attributes.dilations[1] 
        + attributes.pads[1] + attributes.pads[3] - 1) / attributes.strides[1] + 1 
        || y_shape.N != x_shape.N || y_shape.C != w_shape.N)
        assert(0 && "Error: Unappropriate output sizes for QLConv !!\n");

    int i, j, l, m, n;
    int start_height = -attributes.pads[0], start_width = -attributes.pads[1];
    int batch = x_shape.N, C_in = w_shape.C, C_out = w_shape.N / attributes.group;
    b_shape.N = b_shape.H = b_shape.W = 0;
    for (i = 0; i < batch; i++) {
        x_shape.N = y_shape.N = i;
        for (l = 0; l < C_out; l++) {
            y_shape.C = w_shape.N = b_shape.C = l;
            int k, h, e, f;
            for (k = e = 0; k < y_shape.H; k++, e += attributes.strides[0]) 
            for (h = f = 0; h < y_shape.W; h++, f += attributes.strides[1]) {

                // conv
                int64_t accum = 0;
                x_shape.H = start_height + e, x_shape.W = start_width + f;
                for (m = 0; m < C_in; m++) {
                    x_shape.C = w_shape.C = m;
                    accum += _conv_pointq(attributes, X, W, x_zero_point, x_shape, w_shape);
                }
                accum += B->get(b_shape.N, b_shape.H, b_shape.W, b_shape.C);

                // requantize
                accum = _mul_quantize_multiplier<int16_t>(accum, M, N);
                Y->at(y_shape.N, k, h, y_shape.C) 
                    = clamp<int16_t>(y_zero_point + accum);
            }
        }
    }
}


void Add_quantize(TensorMem<int16_t> &X1, TensorMem<int16_t> &X2, TensorMem<int16_t> &Y, 
                    int32_t M1, int8_t N1, int32_t M2, int8_t N2, 
                    int16_t merge_zero_point) {

    static_assert(is_integral<int16_t>::value, "Error: Inappropriate type for QLAdd !!");
    assert(X1.shape == Y.shape || X2.shape == Y.shape);

    int dn1, dn2, dh1, dh2, dw1, dw2, dc1, dc2;
    dn1 = dn2 = dh1 = dh2 = dw1 = dw2 = dc1 = dc2 = 1;
    int n1, n2, h1, h2, w1, w2, c1, c2;
    int* yn = &n1, *yh = &h1, *yw = &w1, *yc = &c1;
    if (X1.shape.N != X2.shape.N) {
        if (X1.shape.N == 1) dn1--, yn = &n2;
        else if (X2.shape.N == 1) dn2--;
        else assert(0 && "Error: Inappropriate N-axis size !!");
    }
    if (X1.shape.H != X2.shape.H) {
        if (X1.shape.H == 1) dh1--, yh = &h2;
        else if (X2.shape.H == 1) dh2--;
        else assert(0 && "Error: Inappropriate H-axis size !!");
    }
    if (X1.shape.W != X2.shape.W) {
        if (X1.shape.W == 1) dw1--, yw = &w2;
        else if (X2.shape.W == 1) dw2--;
        else assert(0 && "Error: Inappropriate W-axis size !!");
    }
    if (X1.shape.C != X2.shape.C) {
        if (X1.shape.C == 1) dc1--, yc = &c2;
        else if (X2.shape.C == 1) dc2--;
        else assert(0 && "Error: Inappropriate C-axis size !!");
    }

    // scale_1 /= scale_y;
    // scale_2 /= scale_y;
    // int M1, M2;
    // int N1, N2;
    // cast_scale_to_M_pow_2_n(scale_1, M1, N1);
    // cast_scale_to_M_pow_2_n(scale_2, M2, N2);

    for (n1 = 0, n2 = 0; n1 < Y.shape.N && n2 < Y.shape.N; n1 += dn1, n2 += dn2)
    for (h1 = 0, h2 = 0; h1 < Y.shape.H && h2 < Y.shape.H; h1 += dh1, h2 += dh2)
    for (w1 = 0, w2 = 0; w1 < Y.shape.W && w2 < Y.shape.W; w1 += dw1, w2 += dw2)
    for (c1 = 0, c2 = 0; c1 < Y.shape.C && c2 < Y.shape.C; c1 += dc1, c2 += dc2) {
        int64_t x1 = (int64_t) X1.get(n1, h1, w1, c1);
        int64_t x2 = (int64_t) X2.get(n2, h2, w2, c2);
        x1 = _mul_quantize_multiplier<int32_t>(x1, M1, N1);
        x2 = _mul_quantize_multiplier<int32_t>(x2, M2, N2);
        Y.at(*yn, *yh, *yw, *yc) = clamp<int16_t>((int32_t) (merge_zero_point + x1 + x2));
    }
}

template <typename T>
void float_to_mult_shift(float scale, int8_t bit_width, T* m, int8_t* s) {
    if (scale == 0.0f) {
        *m = 0;
        *s = 0;        
        return ;
    }

    int sign = (scale > 0.0f) ? 1 : -1;
    scale = std::abs(scale);

    int exponent = 0;
    // std::frexp trong C++ trả về mantissa và nhận vào con trỏ để gán exponent
    float mantissa = std::frexp(scale, &exponent);

    // Sử dụng 1LL (long long) để dịch bit an toàn, tránh tràn số nếu bit_width lớn
    // Ép kiểu (1LL << ...) về float để nhân, sau đó ép kiểu toàn bộ về int32_t (tương đương int() trong Python)
    int32_t multiplier = static_cast<int32_t>(mantissa * static_cast<float>(1LL << (bit_width - 1))) * sign;
    int8_t shift = (int8_t) (bit_width - 1) - exponent;

    *m = (T) multiplier;
    *s = shift;
}
int main() {
    clock_t start, end;
    start = clock();

    // outer calculation
    int16_t reci_C_mult;
    int8_t reci_C_shift;
    float_to_mult_shift<int16_t>(1.0 / 960, 15, &reci_C_mult, &reci_C_shift);

    float x_scale = 0.00029286922654137015;
    int16_t x_zp = -516;

    float y_scale = 0.0002957680553663522;
    int16_t y_zp = -1837;

    float weight_1_scale = 0.000021685424144379795;
    float bias_1_scale = 6.3509935088745806e-9;

    float weight_2_scale = 0.000022258147510001436;
    float bias_2_scale = 2.396622011957561e-9;

    float gamma_1_scale = 0.000015961695680744015;
    float beta_1_scale = 0.000009176691492029931;
    int16_t gamma_1_zp = -32768;
    int16_t beta_1_zp = 26874;

    float gamma_2_scale = 0.000026696541681303643;
    float beta_2_scale = 0.000005774956207460491;
    int16_t gamma_2_zp = -32758;
    int16_t beta_2_zp = 4774;

    float c1_scale = 0.004122796934098005;
    int16_t c1_zp = 689;
    float n1_scale = 0.00010767392086563632;
    int16_t n1_zp = -32768;
    float c2_scale = 0.0019970976281911135;
    int16_t c2_zp = -12033;
    float n2_scale = 0.0002132747322320938;
    int16_t n2_zp = -11043;


    TensorMem<int16_t> x(ARENA_1.alloc<int16_t>(256* 960), {1, 16, 16, 960}, false);
    TensorMem<int16_t> residual(ARENA_1.alloc<int16_t>(256* 960), {1, 16, 16, 960}, false);

    TensorMem<int16_t> weight_1(ARENA_1.alloc<int16_t>(960*960*9), {960, 3, 3, 960}, false);
    TensorMem<int32_t> bias_1(ARENA_1.alloc<int32_t>(960), {1, 1, 1, 960}, false);
    int16_t m_conv_1;
    int8_t s_conv_1;
    float_to_mult_shift<int16_t>(bias_1_scale / c1_scale, 15, &m_conv_1, &s_conv_1);

    TensorMem<int32_t> gamma_1(ARENA_1.alloc<int32_t>(960), {1, 1, 1, 960}, false);
    TensorMem<int8_t> gamma_shift_1(ARENA_1.alloc<int8_t>(960), {1, 1, 1, 960}, false);
    TensorMem<int32_t> beta_1(ARENA_1.alloc<int32_t>(960), {1, 1, 1, 960}, false);
    int16_t epsilon_1 = (int16_t) round(0.001 / (c1_scale* c1_scale));

    TensorMem<int16_t> weight_2(ARENA_1.alloc<int16_t>(960*960*9), {960, 3, 3, 960}, false);
    TensorMem<int32_t> bias_2(ARENA_1.alloc<int32_t>(960), {1, 1, 1, 960}, false);
    int16_t m_conv_2;
    int8_t s_conv_2;
    float_to_mult_shift<int16_t>(bias_2_scale / c2_scale, 15, &m_conv_2, &s_conv_2);

    TensorMem<int32_t> gamma_2(ARENA_1.alloc<int32_t>(960), {1, 1, 1, 960}, false);
    TensorMem<int8_t> gamma_shift_2(ARENA_1.alloc<int8_t>(960), {1, 1, 1, 960}, false);
    TensorMem<int32_t> beta_2(ARENA_1.alloc<int32_t>(960), {1, 1, 1, 960}, false);
    int16_t epsilon_2 = (int16_t) round(0.001 / (c2_scale* c2_scale));

    int32_t m_res_1, m_res_2;
    int8_t s_res_1, s_res_2;
    float_to_mult_shift<int32_t>(x_scale / y_scale, 31, &m_res_1, &s_res_1);
    float_to_mult_shift<int32_t>(n2_scale / y_scale, 31, &m_res_2, &s_res_2);
    int16_t res_zp = clamp<int16_t>((int32_t) (y_zp - x_scale / y_scale * x_zp - n2_scale / y_scale * n2_zp));


    read_tensor("io_params_q\\input.txt", x);

    read_tensor("model_params_q\\conv1_w.txt", weight_1);
    read_tensor("model_params_q\\conv1_b.txt", bias_1);
    read_tensor("model_params_q\\norm1_gm.txt", gamma_1);
    read_tensor("model_params_q\\norm1_gs.txt", gamma_shift_1);
    read_tensor("model_params_q\\norm1_b.txt", beta_1);

    read_tensor("model_params_q\\conv2_w.txt", weight_2);
    read_tensor("model_params_q\\conv2_b.txt", bias_2);
    read_tensor("model_params_q\\norm2_gm.txt", gamma_2);
    read_tensor("model_params_q\\norm2_gs.txt", gamma_shift_2);
    read_tensor("model_params_q\\norm2_b.txt", beta_2);

    TensorMem<int16_t> y(ARENA_1.alloc<int16_t>(256* 960), {1, 16, 16, 960}, false);

    Identity(x, residual);   // FIX: snapshot residual = input AFTER reads (was before read_tensor -> residual=0, skip lost)

    // resblock
    Conv_quantize({1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1}, &x, &weight_1, &bias_1, &y, m_conv_1, s_conv_1, x_zp, c1_zp);
    Channel_Norm_quantize<960>(y, y, gamma_1, gamma_shift_1, beta_1, epsilon_1, reci_C_mult, reci_C_shift);

    Conv_quantize({1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1}, &y, &weight_2, &bias_2, &x, m_conv_2, s_conv_2, n1_zp, c2_zp);
    Channel_Norm_quantize<960>(x, x, gamma_2, gamma_shift_2, beta_2, epsilon_2, reci_C_mult, reci_C_shift);
    Add_quantize(x, residual, y, m_res_2, s_res_2, m_res_1, s_res_1, res_zp);   // FIX: conv-path(x)<->n2_scale=m_res_2, residual<->x_scale=m_res_1 (were swapped)


    write_tensor("io_params_q\\test_output.txt", y, 0);

    end = clock();
    std::cout << "Arena_1 max size: " << ARENA_1.max_runtime_size << "\n" 
                << "Arena_2 max size: " << ARENA_2.max_runtime_size << "\n"
                << "Total runtime: " << ((double) (end - start)) / CLOCKS_PER_SEC << " s\n";

    return 0;
}
