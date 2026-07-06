cmake_minimum_required(VERSION 3.28)
project(enterprise VERSION 2.0.0 LANGUAGES CXX)

# ① 全局设置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ② 全局编译选项（通过 INTERFACE 库传播，比 add_compile_options 更精准）
add_library(project_options INTERFACE)
target_compile_options(project_options INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /utf-8 /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
target_compile_features(project_options INTERFACE cxx_std_20)

# ③ ASan 支持
option(ENABLE_ASAN "启用 AddressSanitizer" OFF)
if(ENABLE_ASAN)
    if(MSVC)
        target_compile_options(project_options INTERFACE /fsanitize=address)
    else()
        target_compile_options(project_options INTERFACE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(project_options INTERFACE -fsanitize=address)
    endif()
endif()

# ④ 子模块（顺序即依赖顺序）
add_subdirectory(utils)
add_subdirectory(core)
add_subdirectory(app)

# ⑤ 测试
enable_testing()
add_subdirectory(tests)
