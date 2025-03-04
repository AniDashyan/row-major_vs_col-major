#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include "kaizen.h"

using namespace std::chrono;

void initializeMatrix(size_t rows, size_t cols, std::vector<std::vector<int>>& matrix) {
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            matrix[i][j] = i + j;
}

auto row_major(size_t rows, size_t cols, std::vector<std::vector<int>>& matrix) {
    volatile int product = 1;
    auto start = steady_clock::now();
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            product *= matrix[i][j];
    auto end = steady_clock::now();
    return duration_cast<milliseconds>(end - start).count();
}

auto col_major(size_t rows, size_t cols, std::vector<std::vector<int>>& matrix) {
    volatile int product = 1;
    auto start = steady_clock::now();
    for (size_t j = 0; j < cols; j++)
        for (size_t i = 0; i < rows; i++)
            product *= matrix[i][j];
    auto end = steady_clock::now();
    return duration_cast<milliseconds>(end - start).count();
}

int main(int argc, char *argv[]) {
    zen::cmd_args args(argv, argc);

    // Get command-line options
    auto size_opts = args.get_options("--size");
    auto row_opts = args.get_options("--row");
    auto col_opts = args.get_options("--col");

    size_t rows, cols;

    // Determine dimensions based on arguments
    if (!size_opts.empty()) {
        rows = cols = static_cast<size_t>(std::atoi(size_opts[0].c_str()));
        if (rows == 0) {
            std::cerr << "Error: --size must be positive\n";
            return 1;
        }
        if (!row_opts.empty() || !col_opts.empty()) {
            std::cerr << "Error: Use either --size or --row/--col, not both\n";
            return 1;
        }
    } else if (!row_opts.empty() && !col_opts.empty()) {
        rows = static_cast<size_t>(std::atoi(row_opts[0].c_str()));
        cols = static_cast<size_t>(std::atoi(col_opts[0].c_str()));
        if (rows == 0 || cols == 0) {
            std::cerr << "Error: --row and --col must be positive\n";
            return 1;
        }
    } else {
        std::cerr << "Error: Provide --size or both --row and --col\n";
        return 1;
    }

    std::vector<std::vector<int>> A(rows, std::vector<int>(cols));
    initializeMatrix(rows, cols, A);
    auto row_time = row_major(rows, cols, A);
    auto col_time = col_major(rows, cols, A);


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