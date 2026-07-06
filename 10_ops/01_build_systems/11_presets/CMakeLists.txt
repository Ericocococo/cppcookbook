cmake_minimum_required(VERSION 3.28)
project(presets_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_ASAN "启用 AddressSanitizer" OFF)

add_executable(app main.cpp)

if(ENABLE_ASAN)
    if(MSVC)
        target_compile_options(app PRIVATE /fsanitize=address)
    else()
        target_compile_options(app PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(app PRIVATE -fsanitize=address)
    endif()
    message(STATUS "ASan 已启用")
endif()
