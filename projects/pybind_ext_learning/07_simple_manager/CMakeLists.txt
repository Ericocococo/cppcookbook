cmake_minimum_required(VERSION 3.28)
project(fhsg_demo LANGUAGES CXX)

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
pybind11_add_module(fhsg_demo
    fhsg_mgr.cpp
    bindings.cpp
)
