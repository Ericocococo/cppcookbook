cmake_minimum_required(VERSION 3.28)
project(cmake_library LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ① 静态库：编译进可执行文件，无运行时依赖
add_library(math_static STATIC math_utils.cpp)
target_include_directories(math_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ② 动态库（共享库）：运行时加载的 .dll / .so
add_library(math_shared SHARED math_utils.cpp)
target_include_directories(math_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Windows 下导出符号需要宏，此处演示用静态库更简单
set_target_properties(math_shared PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

# 可执行文件链接静态库
add_executable(use_static main_static.cpp)
target_link_libraries(use_static PRIVATE math_static)

# 可执行文件链接动态库
add_executable(use_shared main_shared.cpp)
target_link_libraries(use_shared PRIVATE math_shared)
