CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(lxd-test
        LANGUAGES CXX)

find_package(Catch2 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} test_fileio.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17 )

target_include_directories(${PROJECT_NAME} PRIVATE ../src)

target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2 lxd)

