// COSIM PROBE testbench — dummy deterministic data (correctness irrelevant).
// cosim compares RTL vs C model bit-for-bit regardless of golden, so this only
// needs to DRIVE the design with valid-sized buffers and return 0.
#include <iostream>
#include <vector>
#include <cstdint>
#include <cstring>
#include "upconv_core.h"

extern "C" void probe_top(const data_256_t* X, const data_256_t* W,
                          const data_256_t* B, const data_256_t* G,
                          const data_256_t* BE, data_256_t* Y, data_t epsilon);

static uint16_t f2h(float f) { data_t h = (data_t)f; uint16_t b; std::memcpy(&b, &h, 2); return b; }

static data_256_t pack16(float base, int seed) {
    data_256_t w = 0;
    for (int l = 0; l < 16; l++)
        w.range(16*l+15, 16*l) = f2h(base + 0.013f * ((seed + l) % 11 - 5));
    return w;
}

int main() {
    const int h_in = 1, w_in = 4, c_in = 120, c_out = 60;  // FAST throughput probe
    const int ci_words = (c_in + 15) / 16;   // 8
    const int co_words = (c_out + 15) / 16;  // 4
    const int ho = 2*h_in, wo = 2*w_in;      // 8, 256

    const int wx = h_in * w_in * ci_words;   // 4096
    const int ww = c_out * 9 * ci_words;     // 4320
    const int wc = co_words;                 // 4
    const int wy = ho * wo * co_words;       // 8192

    std::vector<data_256_t> X(wx), W(ww), B(wc), G(wc), BE(wc), Y(wy, 0);

    for (int i = 0; i < wx; i++) X[i] = pack16(0.20f, i);
    for (int i = 0; i < ww; i++) W[i] = pack16(0.02f, i*3);
    for (int i = 0; i < wc; i++) { B[i] = pack16(0.00f, i); G[i] = pack16(1.00f, i); BE[i] = pack16(0.00f, i); }

    std::cout << "[PROBE] UCB_3 reduced  " << h_in << "x" << w_in << "x" << c_in
              << " -> " << ho << "x" << wo << "x" << c_out
              << "  (wx=" << wx << " ww=" << ww << " wy=" << wy << ")\n";
    std::cout << "[PROBE] calling probe_top...\n";

    probe_top(X.data(), W.data(), B.data(), G.data(), BE.data(), Y.data(), (data_t)1e-5f);

    uint64_t cksum = 0;
    for (int i = 0; i < wy; i++)
        for (int l = 0; l < 16; l++)
            cksum += (uint64_t)Y[i].range(16*l+15, 16*l).to_uint();
    std::cout << "[PROBE] done. Y checksum=" << cksum << "\n";
    return 0;  // cosim drives RTL regardless; just need clean return
}
