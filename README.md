# Row-Major vs Column-Major Matrix Access Performance

This repository explores the performance differences between **row-major** and **column-major** access of a 2D array (i.e., matrix). The program benchmarks the time taken to traverse a matrix in row-major order and in column-major order, demonstrating the performance differences due to memory access patterns and cache line efficiency.

## Project Overview

The program accepts command-line arguments for matrix dimensions (either through `--size` for square matrices or `--row` and `--col` for specifying non-square matrices). It then initializes the matrix, computes the time required for both row-major and column-major traversals, and prints a comparison of the results.

Key features:
- **Row-major** and **column-major** matrix access benchmarks
- **kaizen.h** library is used for command-line argument handling
- Outputs formatted timing and ratio of the two access methods

## Why Row-Major Is Faster?

Row-major access is generally faster than column-major access due to how data is stored in memory. In row-major order, consecutive elements of a row are stored contiguously in memory, allowing for more efficient cache usage. Column-major access, on the other hand, jumps between memory locations on each access, resulting in cache inefficiency and slower performance.

## Getting Started

### Prerequisites

- C++ Compiler (g++, clang++, etc.)
- CMake (for building the project)
- kaizen.h library: This project uses the [kaizen.h](https://github.com/heinsaar/kaizen) library for handling command-line arguments. Please make sure to include the `kaizen.h` file in your repository.

### Cloning the Repository

```bash
git clone https://github.com/AniDashyan/row-major_vs_col-major
cd row-major_vs_col-major
```

### Build & Run

1. Generate the build files using CMake:

    ```bash
    cmake -S . -B build
    ```

2. Build the project:

    ```bash
    cmake --build build --config Release
    ```

### Running the Program

Once built, run the program with the following command:

```bash
./build/RowMajorvsColumnMajor [--size N] OR [--row R --col C]
```

#### Command-Line Options

- `--size N`: Defines a square matrix of size `N x N`.
- `--row R --col C`: Defines a rectangular matrix with `R` rows and `C` columns.

### Example Output

For a matrix size of 10000 x 10000, you can run the program as:

```bash
./build/RowMajorvsColumnMajor --size 10000
```

This will output something like:

```
+------------------------------+------------------------------+
| Metric                       | Value                         |
+------------------------------+------------------------------+
| Matrix size                  | 10000 x 10000                 |
| Row major time               | 190 ms                        |
| Column major time            | 695 ms                        |
| Ratio (col/row)              | 3.66                          |
+------------------------------+------------------------------+
```

### Explanation of Output

- **Matrix size**: The dimensions of the matrix used for the test.
- **Row major time**: Time taken to traverse the matrix in row-major order.
- **Column major time**: Time taken to traverse the matrix in column-major order.
- **Ratio (col/row)**: Ratio of the column-major time to the row-major time, indicating how much slower column-major access is compared to row-major access.

## Explanation: Row-Major vs Column-Major Access

- **Row-Major Access**: In row-major order, data is stored row by row in contiguous memory. As a result, accessing consecutive elements of a row is efficient because they are already in the CPU's cache, reducing memory access time.
  
- **Column-Major Access**: In column-major order, elements in the same column are not stored contiguously in memory. This leads to frequent cache misses because the CPU needs to access different memory locations for each access, resulting in slower performance.
