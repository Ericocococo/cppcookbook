cmake_minimum_required(VERSION 3.28)
project(find_package_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ① 查找系统自带模块（CMake 内置 FindXxx.cmake）
find_package(Threads REQUIRED)

# ② 查找支持 Config 模式的包（包自带 XxxConfig.cmake）
# find_package(nlohmann_json CONFIG REQUIRED)   # 需 vcpkg install nlohmann-json

# ③ 可选依赖：未找到时降级处理
find_package(ZLIB)

add_executable(app main.cpp)

# 链接：通过 target 名传递所有编译/链接属性
target_link_libraries(app PRIVATE Threads::Threads)

if(ZLIB_FOUND)
    target_link_libraries(app PRIVATE ZLIB::ZLIB)
    target_compile_definitions(app PRIVATE HAS_ZLIB)
    message(STATUS "ZLIB 已找到，版本：${ZLIB_VERSION_STRING}")
else()
    message(STATUS "ZLIB 未找到，相关功能禁用")
endif()
