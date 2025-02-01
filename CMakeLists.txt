cmake_minimum_required(VERSION 3.5.0)

project(
    labwork3
    VERSION 1.0
    DESCRIPTION "C++ Year 2024 Labwork3"
    LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS= "-O3")
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)