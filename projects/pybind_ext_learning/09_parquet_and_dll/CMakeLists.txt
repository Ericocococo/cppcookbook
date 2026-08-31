cmake_minimum_required(VERSION 3.28)
project(parquet_reader LANGUAGES CXX)

# 与框架 quant_engine 一致: Python 3.11 + pybind11 + vcpkg 的 Arrow/Parquet
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4 /utf-8)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

# Python 3.11 EXACT：必须精确匹配 3.11.x，不接受 3.12 或 3.10
#   .pyd 文件名带版本号（parquet_reader.cp311-win_amd64.pyd），只能被对应版本加载
# COMPONENTS（组件）告诉 find_package 需要 Python 的哪些部分，缺一个就报错：
#   Interpreter（解释器）       → 找 python.exe（运行脚本用）
#   Development（开发库）       → 找 python311.lib + Python.h（链接用）
#   Development.Module（模块开发） → 找构建 .pyd 模块所需的头文件和链接库
find_package(Python 3.11 EXACT REQUIRED COMPONENTS Interpreter Development Development.Module)

# find_package 找包有两种模式：
#   Module 模式 → 找 CMake 自带的 FindXxx.cmake
#   Config 模式 → 找包自己提供的 XxxConfig.cmake
# CMake 默认先试 Module，再试 Config
#
# pybind11 不需要加 CONFIG：pip install 后同时提供了两种模式的配置文件，都能找到
# Arrow/Parquet 必须加 CONFIG：CMake 没有自带 FindArrow.cmake，Module 模式找不到
# ---- ① 找库（知道库在哪） ----
find_package(pybind11 REQUIRED)
# 读 .parquet 文件需要两个库：
#   Arrow   → 内存中的列式数据格式（数据在内存里怎么存）
#   Parquet → 磁盘上的文件读写（把 .parquet 文件解析成 Arrow Table）
# 流程：磁盘 .parquet → Parquet 解析 → Arrow Table（内存） → 代码使用
find_package(Arrow CONFIG REQUIRED)
find_package(Parquet CONFIG REQUIRED)

# ---- ② 准备链接列表 ----
# set(变量名 值)：定义变量，用 ${变量名} 读取，可以存多个值（列表）
# TP_LINK：根据 vcpkg 装的是动态库还是静态库，自动选对的那个
# ARROW_BUILD_STATIC 不是自己定义的，是 find_package(Arrow) 时
# CMake 读取 vcpkg 生成的 ArrowConfig.cmake 后自动带进来的变量
#   ARROW_BUILD_STATIC=ON  → vcpkg 装的静态版，用 arrow_static / parquet_static
#   ARROW_BUILD_STATIC=OFF → vcpkg 装的动态版（默认），用 arrow_shared / parquet_shared
# 不直接写死 arrow_shared，因为换台机器 vcpkg 可能装的是静态版，写死就编译不过
# $<BOOL:值>：把 ON/OFF/YES/NO/TRUE/FALSE 统一转成 1 或 0，不加则 $<IF> 不认识 OFF 这类字符串
# $<IF:条件,真值,假值>：CMake 生成器表达式，类似三元运算符
set(TP_LINK
        "$<IF:$<BOOL:${ARROW_BUILD_STATIC}>,Arrow::arrow_static,Arrow::arrow_shared>"
        "$<IF:$<BOOL:${ARROW_BUILD_STATIC}>,Parquet::parquet_static,Parquet::parquet_shared>"
)

# ---- ③ 创建目标（parquet_reader 这个名字从这里才存在） ----
pybind11_add_module(parquet_reader parquet_reader.cpp)

# ---- ④ 给目标链接库（目标必须已存在，所以放在 ③ 后面，顺序不能变） ----
# 如果把 target_link_libraries 放到 pybind11_add_module 前面，CMake 会报错：parquet_reader 目标不存在
# target_link_libraries：链接 Arrow/Parquet 库，不加则编译时找不到这两个库的函数
# PRIVATE：依赖只在 parquet_reader 内部用，不传递给其他目标
target_link_libraries(parquet_reader PRIVATE ${TP_LINK})
