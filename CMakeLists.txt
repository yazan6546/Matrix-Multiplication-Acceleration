cmake_minimum_required(3.28)
project(MatrixMultiplication VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(detached_threads detached_threads.c)
add_executable(joined_threads joined_threads.c)
add_executable(multiprocessing multiprocessing.c)