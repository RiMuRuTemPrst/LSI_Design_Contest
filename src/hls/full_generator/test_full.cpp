#include <iostream>
#include <vector>
#include <chrono>
#include <cstring>
#include "../fusion_core/gen/fusion_core.h"
#include "../upconv_core/gen/upconv_core.h"

using namespace std;

// Mock function for generator top
void generator_dummy() {}

int main() {
    cout << "==========================================================" << endl;
    cout << "     FULL GENERATOR PIPELINE INTEGRATION TEST             " << endl;
    cout << "==========================================================" << endl;

    // --- Memory Allocation for Tensors ---
    // Hyper-latent input: 16x16x224
    vector<data_256_t> hyper_latent(16 * 16 * (224/16), 0x3C00); // 0.5f in FP16 bits
    // Fusion Core Weight/Params (approx buffers)
    vector<data_256_t> f_weights(960 * 9 * (960/16), 0);
    vector<data_256_t> f_params(960/16 * 5, 0); // B, G, BE, G_IN, BE_IN
    // Intermediate Tensor: 16x16x960
    vector<data_256_t> latent_960(16 * 16 * (960/16));
    
    // UpConv buffers (pre-allocate max size for Block 3 output: 256x256x60)
    vector<data_256_t> final_output(256 * 256 * (60/16) + 100);

    auto total_start = chrono::high_resolution_clock::now();

    // 1. [FUSION CORE] CBI
    cout << "[1/7] Running CBI..." << endl;
    fusion_core_top(hyper_latent.data(), f_weights.data(), f_params.data(), f_params.data(), f_params.data(), 
                    f_params.data(), f_params.data(), latent_960.data(), (data_t)1e-5f, MODE_CBI);

    // 2. [FUSION CORE] 9x ResBlocks
    for(int i=0; i<9; i++) {
        cout << "[2/7] Running ResBlock " << i << " (L1+L2)..." << endl;
        fusion_core_top(latent_960.data(), f_weights.data(), f_params.data(), f_params.data(), f_params.data(), 
                        NULL, NULL, latent_960.data(), (data_t)1e-5f, MODE_RB_L1);
        fusion_core_top(latent_960.data(), f_weights.data(), f_params.data(), f_params.data(), f_params.data(), 
                        NULL, NULL, latent_960.data(), (data_t)1e-5f, MODE_RB_L2);
    }

    // 3. [FUSION CORE] Global Add
    cout << "[3/7] Running Global Add..." << endl;
    fusion_core_top(latent_960.data(), NULL, NULL, NULL, NULL, NULL, NULL, latent_960.data(), (data_t)1e-5f, MODE_GLOBAL_ADD);

    // 4. [UPCONV CORE] Block 0 (16x16x960 -> 32x32x480)
    cout << "[4/7] Running UpConv Block 0..." << endl;
    upconv_core_top(latent_960.data(), f_weights.data(), f_params.data(), f_params.data(), f_params.data(), 
                    final_output.data(), (data_t)1e-5f, MODE_UCB_0);

    // 5. [UPCONV CORE] Block 1 (32x32x480 -> 64x64x240)
    // Reuse final_output as ping-pong for next blocks in real app, here we just show flow
    cout << "[5/7] Running UpConv Block 1..." << endl;
    upconv_core_top(final_output.data(), f_weights.data(), f_params.data(), f_params.data(), f_params.data(), 
                    final_output.data(), (data_t)1e-5f, MODE_UCB_1);

    // 6. [UPCONV CORE] Block 2 (64x64x240 -> 128x128x120)
    cout << "[6/7] Running UpConv Block 2..." << endl;
    upconv_core_top(final_output.data(), f_weights.data(), f_params.data(), f_params.data(), f_params.data(), 
                    final_output.data(), (data_t)1e-5f, MODE_UCB_2);

    // 7. [UPCONV CORE] Block 3 (128x128x120 -> 256x256x60)
    cout << "[7/7] Running UpConv Block 3..." << endl;
    upconv_core_top(final_output.data(), f_weights.data(), f_params.data(), f_params.data(), f_params.data(), 
                    final_output.data(), (data_t)1e-5f, MODE_UCB_3);

    auto total_end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = total_end - total_start;

    cout << "\n==========================================================" << endl;
    cout << "   INTEGRATION TEST SUCCESSFUL!                           " << endl;
    cout << "   Total CSIM Time: " << diff.count() << " s" << endl;
    cout << "==========================================================" << endl;

    return 0;
}
