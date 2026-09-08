cmake_minimum_required(VERSION 3.28)
project(hello_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /utf-8)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

# add_executable(目标名 源文件...)：生成 .exe 可执行文件
#   第一个参数 = 目标名（生成的文件就叫 hello_cpp.exe，可以自己随便取名）
#   后面 = 源文件列表（可以有多个 .cpp）
add_executable(hello_cpp main.cpp)
