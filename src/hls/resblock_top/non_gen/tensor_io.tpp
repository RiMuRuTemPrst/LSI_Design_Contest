#include "../gen/Core.h"
#include <iomanip>
#include <fstream>

/**
 * Reads tensor data from a text file into a TensorMem object.
 * Expects space-separated float values in the file.
 */
template<typename T>
void read_tensor(const char* file_path, TensorMem<T> &tens) {
    std::ifstream file(file_path);
    int size = tens.shape.N * tens.shape.H * tens.shape.W * tens.shape.C;
    float val;
    int i = 0;
    
    // Read values one by one and cast to the target type T
    while (file >> val && i < size) {
        tens.raw()[i++] = static_cast<T>(val);
    }
}

/**
 * Writes tensor data from a TensorMem object to a text file.
 * Each value is written on a new line with specified floating-point precision.
 */
template<typename T>
void write_tensor(const char* file_path, TensorMem<T> &X, unsigned int precision) {
    std::ofstream file(file_path);
    int size = X.shape.N * X.shape.H * X.shape.W * X.shape.C;
    float val;
    
    for (int i = 0; i < size; i++) {
        val = (float)X.raw()[i];
        file << std::fixed << std::setprecision(precision) << val << std::endl;
    }
    file.close();
}
