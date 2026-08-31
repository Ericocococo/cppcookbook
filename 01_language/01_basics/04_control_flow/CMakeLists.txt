cmake_minimum_required(VERSION 3.28)
project(control_flow LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /utf-8)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

add_executable(control_flow main.cpp)
