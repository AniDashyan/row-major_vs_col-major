#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "kaizen.h"

// Default matrix size
size_t rows = 1000; 
size_t cols = 1000;

void initialize_matrix(size_t rows, size_t cols, std::vector<int>& matrix, size_t offset = 0) {
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            matrix[offset + i * cols + j] = static_cast<int>(i * cols + j);
}

double run_test(size_t rows, size_t cols, std::vector<int>& matrix, bool row_major_order, 
                size_t iterations, size_t offset = 0) {
    zen::timer t;
    volatile int product = 1;
    
    t.start();
    for (size_t iter = 0; iter < iterations; iter++) {
        if (row_major_order) {
            for (size_t i = 0; i < rows; i++)
                for (size_t j = 0; j < cols; j++)
                    product *= matrix[offset + i * cols + j];
        } else {
            for (size_t j = 0; j < cols; j++)
                for (size_t i = 0; i < rows; i++)
                    product *= matrix[offset + i * cols + j];
        }
    }
    t.stop();
    
    return t.duration<zen::timer::msec>().count();
}


void print_results(const std::string& test_name, size_t rows, size_t cols, 
                  double row_time, double col_time) {
    std::cout << "\nTest: " << test_name << "\n";
    std::cout << "+" << std::string(30, '-') << "+" << std::string(30, '-') << "+\n";
    std::cout << "|" << std::left << std::setw(30) << "Metric" 
              << "|" << std::setw(30) << "Value" << "|\n";
    std::cout << "+" << std::string(30, '-') << "+" << std::string(30, '-') << "+\n";
    std::cout << "|" << std::left << std::setw(30) << "Matrix size" 
              << "|" << std::setw(29) << (std::to_string(rows) + " x " + std::to_string(cols)) << " |\n";
    std::cout << "|" << std::left << std::setw(30) << "Row major time" 
              << "|" << std::setw(29) << (std::to_string(static_cast<long long>(row_time)) + " ms") << " |\n";
    std::cout << "|" << std::left << std::setw(30) << "Column major time" 
              << "|" << std::setw(29) << (std::to_string(static_cast<long long>(col_time)) + " ms") << " |\n";
    std::cout << "|" << std::left << std::setw(30) << "Ratio (col/row)" 
              << "|" << std::fixed << std::setprecision(2) << std::setw(30) << (col_time / row_time) << "|\n";
    std::cout << "+" << std::string(30, '-') << "+" << std::string(30, '-') << "+\n";
}

int main(int argc, char *argv[]) {
    zen::cmd_args args(argv, argc);
    size_t iterations = 10000;

    auto row_opts = args.get_options("--row");
    auto col_opts = args.get_options("--col");
    if (!row_opts.empty()) rows = std::stoul(row_opts[0]);
    if (!col_opts.empty()) cols = std::stoul(col_opts[0]);

    // Default matrix
    std::vector<int> matrix(rows * cols);
    initialize_matrix(rows, cols, matrix);
    double large_row_time = run_test(rows, cols, matrix, true, iterations / 10);
    double large_col_time = run_test(rows, cols, matrix, false, iterations / 10);
    print_results("Default Matrix", rows, cols, large_row_time, large_col_time);

    // Aligned 2x3 matrix
    alignas(64) std::vector<int> aligned_2x3(2 * 3);
    initialize_matrix(2, 3, aligned_2x3);
    double aligned_2x3_row_time = run_test(2, 3, aligned_2x3, true, iterations * 10); 
    double aligned_2x3_col_time = run_test(2, 3, aligned_2x3, false, iterations * 10);
    print_results("Aligned 2x3 Matrix", 2, 3, aligned_2x3_row_time, aligned_2x3_col_time);

    // Misaligned 2x3 matrix
    std::vector<int> temp_2x3(2 * 3 + 1);
    initialize_matrix(2, 3, temp_2x3, 1);
    double misaligned_2x3_row_time = run_test(2, 3, temp_2x3, true, iterations * 10, 1);
    double misaligned_2x3_col_time = run_test(2, 3, temp_2x3, false, iterations * 10, 1);
    print_results("Misaligned 2x3 Matrix", 2, 3, misaligned_2x3_row_time, misaligned_2x3_col_time);

    // Aligned 4x5 matrix
    alignas(64) std::vector<int> aligned_matrix(4 * 5);
    initialize_matrix(4, 5, aligned_matrix);
    double aligned_row_time = run_test(4, 5, aligned_matrix, true, iterations);
    double aligned_col_time = run_test(4, 5, aligned_matrix, false, iterations);
    print_results("Aligned 4x5 Matrix", 4, 5, aligned_row_time, aligned_col_time);

    // Misaligned 4x5 matrix
    std::vector<int> temp_matrix(4 * 5 + 1);
    initialize_matrix(4, 5, temp_matrix, 1);
    double misaligned_row_time = run_test(4, 5, temp_matrix, true, iterations, 1);
    double misaligned_col_time = run_test(4, 5, temp_matrix, false, iterations, 1);
    print_results("Misaligned 4x5 Matrix", 4, 5, misaligned_row_time, misaligned_col_time);

    return 0;
}