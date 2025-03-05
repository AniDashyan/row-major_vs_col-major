#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <string>
#include "kaizen.h"

using namespace std::chrono;

// Default matrix size
size_t rows = 1000;
size_t cols = 1000;

void initializeMatrix(size_t rows, size_t cols, std::vector<std::vector<int>>& matrix) {
    if (rows == 0 || cols == 0) return;
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            matrix[i][j] = i + j;
}

void row_major(size_t rows, size_t cols, std::vector<std::vector<int>>& matrix) {
    if (rows == 0 || cols == 0) return;

    int product = 1;
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            product *= matrix[i][j];
}

void col_major(size_t rows, size_t cols, std::vector<std::vector<int>>& matrix) {
    if (rows == 0 || cols == 0) return;

    int product = 1;
    for (size_t j = 0; j < cols; j++)
        for (size_t i = 0; i < rows; i++)
            product *= matrix[i][j];
}

int main(int argc, char *argv[]) {
    zen::cmd_args args(argv, argc);
    
    auto row_opts = args.get_options("--row");
    auto col_opts = args.get_options("--col");

    if (row_opts.empty() && col_opts.empty()) {
        zen::log("None of the options are provided. Using default values", rows, "x", cols);
    } 
    else if (row_opts.empty()) {
        cols = static_cast<size_t>(std::atoi(col_opts[0].c_str()));
    } 
    else if (col_opts.empty()) 
        rows = static_cast<size_t>(std::atoi(row_opts[0].c_str()));
    else {
        rows = static_cast<size_t>(std::atoi(row_opts[0].c_str()));
        cols = static_cast<size_t>(std::atoi(col_opts[0].c_str()));
    }

    std::vector<std::vector<int>> A(rows, std::vector<int>(cols));
    initializeMatrix(rows, cols, A);

    auto start = steady_clock::now();
    row_major(rows, cols, A);
    auto end = steady_clock::now();

    auto row_time = duration_cast<milliseconds>(end - start).count();

    start = steady_clock::now();
    col_major(rows, cols, A);
    end = steady_clock::now();

    auto col_time = duration_cast<milliseconds>(end - start).count();

    // Formatted output
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "+" << std::string(30, '-') << "+" << std::string(30, '-') << "+\n";
    std::cout << "|" << std::left << std::setw(30) << "Metric" 
              << "|" << std::setw(30) << "Value" << "|\n";
    std::cout << "+" << std::string(30, '-') << "+" << std::string(30, '-') << "+\n";
    std::cout << "|" << std::left << std::setw(30) << "Matrix size" 
              << "|" << std::setw(29) << (std::to_string(rows) + " x " + std::to_string(cols)) << " |\n";
    std::cout << "|" << std::left << std::setw(30) << "Row major time" 
              << "|" << std::setw(29) << (std::to_string(row_time) + " ms") << " |\n";
    std::cout << "|" << std::left << std::setw(30) << "Column major time" 
              << "|" << std::setw(29) << (std::to_string(col_time) + " ms") << " |\n";
    std::cout << "|" << std::left << std::setw(30) << "Ratio (col/row)" 
              << "|" << std::setw(30) << static_cast<double>(col_time) / row_time << "|\n";
    std::cout << "+" << std::string(30, '-') << "+" << std::string(30, '-') << "+\n";

    return (0);
}