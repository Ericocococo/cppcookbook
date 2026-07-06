cmake_minimum_required(VERSION 3.28)
project(multi_sources LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 多源文件：显式列举，不用 GLOB（CMake 不会自动感知新增文件）
add_executable(multi_sources
    main.cpp
    math_utils.cpp
)

# 头文件搜索路径
target_include_directories(multi_sources PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
