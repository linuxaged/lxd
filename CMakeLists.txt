cmake_minimum_required(VERSION 3.12)

project(lxd)

add_executable(lxd "main.cpp" "smallvector.cpp")

target_compile_options( lxd PRIVATE -Wall )
target_compile_features( lxd PRIVATE cxx_std_17 )
