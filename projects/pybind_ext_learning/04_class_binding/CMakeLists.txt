cmake_minimum_required(VERSION 3.28)
project(stock_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find_package：让 CMake 去电脑上找 pybind11，找到了才能继续；REQUIRED：没找到就报错退出
find_package(pybind11 REQUIRED)

# 三文件: stock.cpp（实现）+ bindings.cpp（绑定）。stock.h 被两者 include。
pybind11_add_module(stock_demo
    stock.cpp
    bindings.cpp
)
