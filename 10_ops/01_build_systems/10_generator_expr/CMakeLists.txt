cmake_minimum_required(VERSION 3.28)
project(generator_expr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(app main.cpp)

# 生成器表达式（Generator Expressions）：$<...>
# 在 cmake 配置阶段求值，用于处理多配置生成器（VS/Xcode）的差异

# ① 按编译器类型设置选项
target_compile_options(app PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wno-unused-parameter>
)

# ② 按构建类型设置宏
target_compile_definitions(app PRIVATE
    $<$<CONFIG:Debug>:MY_DEBUG=1>
    $<$<CONFIG:Release>:MY_RELEASE=1>
    BUILD_TYPE="$<CONFIG>"
)

# ③ BUILD_INTERFACE vs INSTALL_INTERFACE
add_library(iface INTERFACE)
target_include_directories(iface INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(app PRIVATE iface)

# ④ 复合条件
target_compile_definitions(app PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:GCC_DEBUG=1>
)
