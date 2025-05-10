cmake_minimum_required(VERSION 3.31)
project(DebugLibC C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(colors-C-C-plus-plus)
include_directories(src)

add_executable(DebugLibC
        colors-C-C-plus-plus/src/colors.c
        colors-C-C-plus-plus/colors.h
        example/hash.c
        src/debug_c.c
        src/error_c.c
        src/time_code.c
        debug_c.h
        error_c.h
        example/hash.c
        time_code.h
        )
