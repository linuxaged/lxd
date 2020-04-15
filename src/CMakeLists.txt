cmake_minimum_required( VERSION 3.14.0 )

add_library( lxd STATIC
	encoding.h
	encoding.cpp
	fileio.h
	fileio.cpp
    smallvector.cpp
    smallvector.h
    )

target_compile_options( lxd PRIVATE -Wall )
target_compile_features( lxd PRIVATE cxx_std_17 )
