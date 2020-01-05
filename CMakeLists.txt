cmake_minimum_required(VERSION 3.9)
project(poly_split)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenMP REQUIRED)

add_executable(poly_split main.cpp randomMaker.h randomMaker.cpp
        polyfier.h polyfier.cpp fileSplitter.h fileSplitter.cpp )

target_link_libraries(poly_split PRIVATE OpenMP::OpenMP_CXX)