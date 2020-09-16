cmake_minimum_required(VERSION 3.14)

project(lxd-benchmark)

find_package(benchmark CONFIG REQUIRED)

add_executable(${PROJECT_NAME} fileio.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17 )

target_link_libraries(${PROJECT_NAME} PRIVATE lxd benchmark::benchmark benchmark::benchmark_main)