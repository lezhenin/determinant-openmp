cmake_minimum_required(VERSION 3.15)
project(determinant)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

include_directories(dependencies/include)
link_directories(dependencies/lib)

add_executable(determinant_benchmark sources/benchmark.cpp)
target_link_libraries(determinant_benchmark benchmark benchmark_main pthread)

add_executable(determinant_test sources/test.cpp)
target_link_libraries(determinant_test gtest pthread gcov)
target_compile_options(determinant_test PUBLIC -fprofile-arcs -ftest-coverage)
