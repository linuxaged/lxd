cmake_minimum_required(VERSION 3.14)

project(lxd-benchmark)

find_package(benchmark CONFIG REQUIRED)

add_executable(${PROJECT_NAME} fileio.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE benchmark::benchmark benchmark::benchmark_main)