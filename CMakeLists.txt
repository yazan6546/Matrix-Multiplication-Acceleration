cmake_minimum_required(VERSION 3.28)
project(MatrixMultiplication VERSION 1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 11)

set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(detached_threads src/detached_threads.c)
add_executable(joinable_threads src/joinable_threads.c)
add_executable(multiprocessing src/processes.c)
