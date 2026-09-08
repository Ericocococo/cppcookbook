cmake_minimum_required(VERSION 3.28)
project(stock_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /utf-8)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

# add_executable(目标名 源文件...)：生成 .exe 可执行文件
# 多文件项目：.h（声明）+ .cpp（实现）分开编译
add_executable(stock_demo
        main.cpp
        stock.cpp
)
