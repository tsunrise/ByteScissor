cmake_minimum_required(VERSION 3.9)
project(poly_split)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenMP REQUIRED)

add_executable(poly_split main.cpp randomMaker.h randomMaker.cpp
        polyfier.h polyfier.cpp fileSplitter.h fileSplitter.cpp unpolyfier.cpp unpolyfier.h fileMerger.cpp fileMerger.h)

target_link_libraries(poly_split PRIVATE OpenMP::OpenMP_CXX)

add_executable(utest test.cpp randomMaker.h randomMaker.cpp
        polyfier.h polyfier.cpp fileSplitter.h fileSplitter.cpp unpolyfier.cpp unpolyfier.h)

target_link_libraries(utest PRIVATE OpenMP::OpenMP_CXX)