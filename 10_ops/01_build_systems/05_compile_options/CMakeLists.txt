cmake_minimum_required(VERSION 3.28)
project(compile_options LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(app main.cpp)

# ① 编译器无关写法：用生成器表达式区分 MSVC / GCC-Clang
target_compile_options(app PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4          # 警告级别 4（最高常用级，/W0=无警告，/W1-/W4 递增）
        /WX          # 警告视为错误，强制修复
        /utf-8       # 源文件和执行字符集均为 UTF-8，避免中文乱码
        /permissive- # 关闭历史兼容宽松模式，强制执行严格 C++ 标准
    >
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
        -Wall        # GCC 高价值警告集合（非全部警告，名字有误导）
        -Wextra      # Wall 之外的额外警告
        -Wpedantic   # 严格 C++ 标准，报所有 GCC 扩展
        -Werror      # 警告视为错误，强制修复
    >
)

# ② Debug 专属：关闭优化 + 调试信息
target_compile_options(app PRIVATE
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Debug>>:-O0 -g3>
    # -O0：不优化，调试断点准确；-g3：生成完整调试信息（含宏定义），供 GDB 使用
)

# ③ Release 专属：开优化
target_compile_options(app PRIVATE
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Release>>:-O3 -DNDEBUG>
    # -O3：激进优化；-DNDEBUG：定义 No Debug 宏，关闭所有 assert()，C/C++ 标准约定
)

# ④ 预处理宏定义
target_compile_definitions(app PRIVATE
    APP_VERSION="1.0.0"
    $<$<CONFIG:Debug>:DEBUG_BUILD>
)

# ⑤ 生成 compile_commands.json（供 clangd / clang-tidy 使用）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
