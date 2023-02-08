cmake_minimum_required(VERSION 3.24)
project(lsolve_submit)

set(CMAKE_CXX_STANDARD 17)

add_executable(lsolve_submit main.cpp main.h config.h utils/helpers.cpp utils/lsolver.cpp utils/lsolver.h utils/storage.h utils/io_func.cpp utils/io_func.h utils/verif.cpp)
