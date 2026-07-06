cmake_minimum_required(VERSION 3.28)
project(compile_options LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(app main.cpp)

# ① 编译器无关写法：用生成器表达式区分 MSVC / GCC-Clang
target_compile_options(app PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4         # 警告级别 4（最高）
        /WX         # 警告视为错误
        /utf-8      # 源文件 UTF-8 编码
        /permissive- # 关闭 MSVC 宽松模式，更接近标准
    >
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
        -Wall -Wextra -Wpedantic
        -Werror     # 警告视为错误
    >
)

# ② Debug 专属：关闭优化 + 调试信息
target_compile_options(app PRIVATE
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Debug>>:-O0 -g3>
)

# ③ Release 专属：开优化
target_compile_options(app PRIVATE
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Release>>:-O3 -DNDEBUG>
)

# ④ 预处理宏定义
target_compile_definitions(app PRIVATE
    APP_VERSION="1.0.0"
    $<$<CONFIG:Debug>:DEBUG_BUILD>
)

# ⑤ 生成 compile_commands.json（供 clangd / clang-tidy 使用）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
