cmake_minimum_required(VERSION 3.28)
project(modern_targets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 现代 CMake 核心：一切皆 target，属性通过 target_* 传播
# PRIVATE：仅本 target 使用
# INTERFACE：仅消费者使用（本 target 不用）
# PUBLIC：本 target 和消费者都使用

# ① 接口库（头文件库 / header-only）：不编译，只传播属性
add_library(utils_iface INTERFACE)
target_include_directories(utils_iface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(utils_iface INTERFACE UTILS_VERSION=2)

# ② 静态库，PUBLIC 传播 include 路径给所有链接它的 target
add_library(core STATIC core.cpp)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(core PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)

# ③ 可执行文件：只需链接 core，include 路径自动继承（PUBLIC）
add_executable(app main.cpp)
target_link_libraries(app PRIVATE core utils_iface)
