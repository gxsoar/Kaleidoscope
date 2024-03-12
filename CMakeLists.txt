cmake_minimum_required(VERSION 3.5)

project(Kaleidoscope)

add_compile_options(-Wall -Wextra -Wpedantic)

set (CMAKE_CXX_COMPILER "/usr/bin/clang++")
set (CMAKE_CXX_FLAGS -std=c++20)

include_directories(include/*.h)
