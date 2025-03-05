cmake_minimum_required(VERSION 3.28)
project(pastel_log C)

set(CMAKE_C_STANDARD 23)

add_executable(pastel_log main.c
        pl_log.h)
