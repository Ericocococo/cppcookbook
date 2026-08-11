cmake_minimum_required(VERSION 3.28)
project(hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find_package：让 CMake 去电脑上找 pybind11，找到了才能继续；REQUIRED：没找到就报错退出
# 找到后创建 pybind11::headers 目标，内部已配好 include 路径：
#   D:/ProgramData/anaconda3/envs/quant311/Lib/site-packages/pybind11/include/

find_package(pybind11 REQUIRED)

# pybind11_add_module：pybind11 提供的 cmake 函数，本质是 add_library(MODULE) 的封装
# 一行搞定下面手写 5 行的事，自动设好头文件路径、链接、.pyd 后缀等：
#
#   add_library(hello MODULE hello.cpp)                      # ① 声明动态库
#   target_include_directories(hello PRIVATE ${PYBIND11_INCLUDE_DIR})  # ② 头文件路径
#   target_link_libraries(hello PRIVATE pybind11::headers)   # ③ 链接 pybind11
#   set_target_properties(hello PROPERTIES
#       PREFIX ""                                            # ④ 去掉 lib 前缀（Python 不认识 libhello.pyd）
#       SUFFIX ".cp311-win_amd64.pyd"                       # ⑤ 设正确的 .pyd 后缀
#   )
#
# 完整链路：
#   find_package(pybind11)  →  创建 pybind11::headers（含 include 路径）
#                                       ↓
#   pybind11_add_module()   →  内部自动链接 pybind11::headers
#                                       ↓
#   编译器加 -I.../pybind11/include/  →  #include <pybind11/pybind11.h> 就能找到了
#
# 第一个参数 = Python import 时的模块名，后面 = 源文件列表
pybind11_add_module(hello hello.cpp)
