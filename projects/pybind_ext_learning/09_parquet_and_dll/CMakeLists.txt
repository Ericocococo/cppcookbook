cmake_minimum_required(VERSION 3.28)
project(parquet_reader LANGUAGES CXX)

# 与框架 quant_engine 一致: Python 3.11 + pybind11 + vcpkg 的 Arrow/Parquet
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(Python 3.11 EXACT REQUIRED COMPONENTS Interpreter Development Development.Module)
# find_package：让 CMake 去电脑上找 pybind11，找到了才能继续；REQUIRED：没找到就报错退出
find_package(pybind11 REQUIRED)
find_package(Arrow CONFIG REQUIRED)
find_package(Parquet CONFIG REQUIRED)

# 动态/静态库自动选择（vcpkg 默认动态: ARROW_BUILD_STATIC=OFF → arrow_shared）
set(TP_LINK
    "$<IF:$<BOOL:${ARROW_BUILD_STATIC}>,Arrow::arrow_static,Arrow::arrow_shared>"
    "$<IF:$<BOOL:${ARROW_BUILD_STATIC}>,Parquet::parquet_static,Parquet::parquet_shared>"
)

pybind11_add_module(parquet_reader parquet_reader.cpp)
target_link_libraries(parquet_reader PRIVATE ${TP_LINK})
