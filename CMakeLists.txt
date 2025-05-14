cmake_minimum_required(VERSION 3.28)
project(DebugLibC C)

set(CMAKE_C_STANDARD 23)


include_directories(.)
include_directories(colors-C-C-plus-plus)
include_directories(src)

# Permitir establecer modo Debug o Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Configuraciones específicas por tipo de compilación
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wpedantic")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_STANDARD 23)

add_executable(hash
        colors-C-C-plus-plus/src/colors.c
        colors-C-C-plus-plus/colors.h
        example/hash.c
        src/debug_c.c
        src/error_c.c
        src/time_code.c
        debug_c.h
        error_c.h
        time_code.h
        )
