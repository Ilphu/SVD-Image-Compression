cmake_minimum_required(VERSION 3.28)
project(SVDImageCompression LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_CXX_COMPILER "/usr/local/bin/g++-11" CACHE STRING "C++ compiler" FORCE)

add_executable(
    compress
    src/main.cc
    src/plotter.cc
)
target_compile_features(compress PRIVATE cxx_std_17)