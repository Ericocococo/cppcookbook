cmake_minimum_required(VERSION 3.28)
project(data_provider_ext LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ---- 纯 C++ 可执行文件（不依赖 pybind11）----
add_executable(data_provider_ext
    main.cpp
    data_mgrs.cpp
)

# ---- pybind11 Python 模块（可选，需 -DBUILD_PYBIND=ON）----
option(BUILD_PYBIND "构建 pybind11 Python 模块" OFF)
if(BUILD_PYBIND)
    # find_package：让 CMake 去电脑上找 pybind11，找到了才能继续
    find_package(pybind11 REQUIRED)
    pybind11_add_module(data_provider_ext_py
        bindings.cpp
        data_mgrs.cpp
    )
    # 输出文件名和 Python import 名一致: data_provider_ext
    set_target_properties(data_provider_ext_py PROPERTIES OUTPUT_NAME "data_provider_ext")
endif()
