cmake_minimum_required(VERSION 3.14)
project(PriorityQueueAnalysis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main benchmark executable
add_executable(benchmark main/main.cpp)
target_include_directories(benchmark PRIVATE main)

# Optimization for accurate benchmarking
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        target_compile_options(benchmark PRIVATE /O2)
    else()
        target_compile_options(benchmark PRIVATE -O2)
    endif()
endif()
