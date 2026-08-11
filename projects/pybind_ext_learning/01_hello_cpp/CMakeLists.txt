cmake_minimum_required(VERSION 3.28)
project(hello_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 纯 C++ 可执行文件，不依赖任何第三方库
add_executable(hello_cpp main.cpp)
