# Matrix Multiplication Performance Comparison

A C project that compares different parallel computing approaches for matrix multiplication operations.

## Table of Contents

- [Overview](#overview)
- [Requirements](#requirements)
- [Installation](#how-to-build)
- [Usage](#usage)
- [Implementation Details](#implementation-details)
- [Performance Comparison](#performance-comparison)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)

## Overview

This project explores three different approaches to matrix multiplication:

1. **Sequential Processing**: Standard single-threaded matrix multiplication
2. **Joinable Threads**: Using POSIX threads with join operations
3. **Detached Threads**: Using POSIX threads that run independently
4. **Multiprocessing**: Using multiple processes with IPC via pipes

All implementations multiply 100×100 matrices and measure execution time for performance comparison.

## Requirements

- C compiler with C11 support (GCC recommended)
- POSIX threads library (pthread)
- CMake (version 3.28 or higher)
- Unix-like operating system (Linux, macOS)

## How to build

```bash
# Clone the repository
git clone <repository-url>
cd MatrixMultiplication

# Create build directory and build the project
mkdir -p build && cd build
cmake ..
make
```


## Usage

Run each implementation separately:

```bash
# Run joinable threads implementation
./joinable_threads

# Run detached threads implementation
./detached_threads

# Run multiprocessing implementation
./multiprocessing
```
Each program will output the execution time for both sequential and parallel approaches, allowing for direct comparison.

## Implementation Details

### Matrix Generation
All implementations generate two 100×100 matrices with predefined values:

- Matrix A is filled with values from the array {1, 2, 1, 0, 1, 4, 5}
- Matrix B is filled with values from the array {2, 4, 2, 3, 9, 2, 0, 4, 3, 5}

### Parallelization Strategy

- **Joinable Threads**: Creates 20 threads, each handling 5 rows of the matrix. Waits for all threads to complete using pthread_join().
- **Detached Threads**: Similar to joinable threads but uses pthread_detach() so threads run independently.
- **Multiprocessing**: Creates 20 child processes using fork(), each handling 5 rows. Uses pipes for communication between parent and child processes.

## Performance Comparison

Each implementation compares:

- Sequential (naive) matrix multiplication
- Parallel implementation with 20 threads or processes

Execution times are measured using **clock_gettime()** with **CLOCK_MONOTONIC** for high precision.

## Project Structure

- **joinable_threads.c**: Implementation using joinable POSIX threads
- **detached_threads.c**: Implementation using detached POSIX threads
- **processes.c**: Implementation using multiprocessing with fork and pipes
- **CMakeLists.txt**: Build configuration


## Contributing

Contributions are welcome! If you have suggestions for improvements or new features, please open an issue or submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
