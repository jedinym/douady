cmake_minimum_required(VERSION 3.19)
project(douady C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(douady
        src/douady.c
        src/douady.h)
