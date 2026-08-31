cmake_minimum_required(VERSION 3.28)
project(data_provider_ext LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /utf-8)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

# ---- 纯 C++ 可执行文件（不依赖 pybind11）----
add_executable(data_provider_ext
        main.cpp
        data_mgrs.cpp
)

# ---- pybind11 Python 模块（可选，需 -DBUILD_PYBIND=ON）----

# option(变量名 "描述" 默认值)：CMake 布尔开关，值只有 ON / OFF
# 命令行用 -DBUILD_PYBIND=ON 覆盖默认值
option(BUILD_PYBIND "构建 pybind11 Python 模块" OFF)

if (BUILD_PYBIND)
    find_package(pybind11 REQUIRED)

    # 目标名加 _py 后缀，避免和上面的 exe 目标 data_provider_ext 重名
    pybind11_add_module(data_provider_ext_py
            bindings.cpp
            data_mgrs.cpp
    )

    # CMake 目标名和输出文件名是两回事：
    #   目标名 data_provider_ext_py → 默认输出 data_provider_ext_py.cp311-win_amd64.pyd
    #   Python import data_provider_ext 找不到 data_provider_ext_py
    # OUTPUT_NAME 把输出文件名改回 data_provider_ext，.exe 和 .pyd 后缀不同不会冲突
    set_target_properties(data_provider_ext_py PROPERTIES OUTPUT_NAME "data_provider_ext")
endif ()
