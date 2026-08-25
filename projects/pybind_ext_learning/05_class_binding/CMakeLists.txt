cmake_minimum_required(VERSION 3.28)
project(stock_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find_package：让 CMake 去电脑上找 pybind11，找到了才能继续；REQUIRED：没找到就报错退出
find_package(pybind11 REQUIRED)

# pybind11_add_module：源文件可以有多个，全列在后面即可
#   stock.cpp   = 纯 C++ 类实现（不依赖 pybind11）
#   bindings.cpp = pybind11 绑定（唯一引用 pybind11 的地方）
#   stock.h 被两者 include，不需要写进 cmake
pybind11_add_module(stock_demo
    stock.cpp
    bindings.cpp
)
