#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <sstream>
#include <numeric>

#include "tensor_4d.h"
#include "ops_group3.h"

namespace fs = std::filesystem;

// ============================================================
// Helper Functions (Read/Write/Tensor creation)
// ============================================================
std::vector<float> read_txt(const std::string& path) {
    std::vector<float> data;
    std::ifstream f(path);
    float v;
    while (f >> v) data.push_back(v);
    return data;
}

std::vector<int> read_shape(const std::string& path) {
    std::vector<int> shape;
    std::ifstream f(path);
    if (!f.is_open()) return {};
    int v;
    while (f >> v) shape.push_back(v);
    return shape;
}

Tensor4D make_tensor4d(const std::vector<float>& buf, const std::vector<int>& shape) {
    int N = 1, H = 1, W = 1, C = 1;
    if (!shape.empty()) {
        if (shape.size() == 1) { C = shape[0]; }
        else if (shape.size() == 2) { H = shape[0]; W = shape[1]; }
        else if (shape.size() == 3) { H = shape[0]; W = shape[1]; C = shape[2]; }
        else if (shape.size() == 4) { N = shape[0]; H = shape[1]; W = shape[2]; C = shape[3]; }
    }
    Tensor4D t(N, H, W, C);
    int total = t.size();
    // Safe copy in case buffer size mismatch
    for (int i = 0; i < total && i < buf.size(); i++) t.raw()[i] = buf[i];
    return t;
}

Tensor4D create_empty_tensor(const std::vector<int>& shape) {
    int N = 1, H = 1, W = 1, C = 1;
    if (!shape.empty()) {
        if (shape.size() == 1) { C = shape[0]; }
        else if (shape.size() == 2) { H = shape[0]; W = shape[1]; }
        else if (shape.size() == 3) { H = shape[0]; W = shape[1]; C = shape[2]; }
        else if (shape.size() == 4) { N = shape[0]; H = shape[1]; W = shape[2]; C = shape[3]; }
    }
    return Tensor4D(N, H, W, C);
}

void write_txt(const std::string& path, const std::vector<float>& arr) {
    std::ofstream f(path);
    f.precision(6);
    f << std::fixed;
    for (float v : arr) f << v << "\n";
}

// ============================================================
// MAIN
// ============================================================
int main() {
    std::string base = fs::current_path().string();
    std::string input_dir  = base + "/Test/data_test/input/";
    std::string output_dir = base + "/Test/data_test/output/";

    fs::create_directories(output_dir);

    std::cout << "=== RUNNING C++ SIMULATION ===\n";

    for (auto& entry : fs::directory_iterator(input_dir)) {
        std::string filename  = entry.path().filename().string();
        
        // Skip shape files and non-txt files
        if (filename.find("_shape") != std::string::npos || filename.find(".txt") == std::string::npos)
            continue;

        std::cout << "Processing: " << filename << "\n";

        // Extract op name
        std::string base_name = filename.substr(0, filename.find(".txt"));
        std::string op = base_name.substr(0, base_name.find("_"));

        // Read Inputs
        std::string shape_file = input_dir + base_name + "_shape.txt";
        std::vector<int> ifm_shape = read_shape(shape_file);
        std::vector<float> flat_input = read_txt(entry.path().string());
        Tensor4D tin = make_tensor4d(flat_input, ifm_shape);

        // =============================
        // Determine OFM SHAPE
        // =============================
        std::vector<int> ofm_shape;

        if (op == "Pad") {
            // Logic: Pad H(dim1) and W(dim2) by 1 on each side -> +2
            ofm_shape = {tin.N, tin.H + 2, tin.W + 2, tin.C};
        }
        else if (op == "Concat") {
            // Logic: Concat input with itself on Axis 0 -> Dimension 0 doubles
            ofm_shape = ifm_shape;
            if (!ofm_shape.empty()) ofm_shape[0] *= 2; 
        }
        else if (op == "Reshape") { ofm_shape = {tin.size()}; }
        else if (op == "Transpose") { ofm_shape = {tin.W, tin.H}; }
        else if (op == "Slice") { ofm_shape = ifm_shape; }
        else if (op == "Shape") { ofm_shape = {4}; }
        else if (op == "Gather") { ofm_shape = {1}; }
        else {
            // Identity, Constant, ConstantOfShape
            ofm_shape = ifm_shape.empty() ? std::vector<int>{1} : ifm_shape;
        }

        Tensor4D tout = create_empty_tensor(ofm_shape);

        // =====================================================
        // EXECUTE OP
        // =====================================================
        if (op == "Pad") {
            OpsGroup3::pad(tout, tin, 1,1,1,1);
        }
        else if (op == "Reshape") {
            OpsGroup3::reshape(tout, tin);
        }
        else if (op == "Transpose") {
            OpsGroup3::transpose(tout, tin);
        }
        else if (op == "Slice") {
            OpsGroup3::slice(tout, tin, 0,tin.N, 0,tin.H, 0,tin.W, 0,tin.C);
        }
        else if (op == "Shape") {
            tout.raw()[0] = (float)tin.N; tout.raw()[1] = (float)tin.H;
            tout.raw()[2] = (float)tin.W; tout.raw()[3] = (float)tin.C;
        }
        else if (op == "Identity") {
            OpsGroup3::identity(tout, tin);
        }
        else if (op == "Constant") {
            // FIX: Copy data from input buffer to output buffer
            OpsGroup3::constant(tout.raw(), flat_input.data(), tout.size());
        }
        else if (op == "ConstantOfShape") {
            OpsGroup3::constant_of_shape(tout.raw(), tout.size(), 1.0f);
        }
        else if (op == "Concat") {
            // FIX: Pass 2 tensors (tin, tin) to match Python's concat([t,t])
            Tensor4D* arr[2] = { &tin, &tin };
            OpsGroup3::concat(tout, arr, 2, 0);
        }
        else if (op == "Gather") {
            int idx_list[1] = {0};
            OpsGroup3::gather(tout, tin, idx_list, 1, 0);
        }

        // Copy tensor to vector for writing
        std::vector<float> out_buf(tout.size());
        for (int i = 0; i < tout.size(); i++) out_buf[i] = tout.raw()[i];

        // Save output
        write_txt(output_dir + filename, out_buf);
    }

    std::cout << "=== DONE ===\n";
    return 0;
}