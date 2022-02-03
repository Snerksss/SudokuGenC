cmake_minimum_required(VERSION 3.21)
project(SodokuGenC C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(SodokuGenC
        main.c
        stack.c
        stack.h)

