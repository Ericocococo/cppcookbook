cmake_minimum_required(VERSION 3.28)
project(vcpkg_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# vcpkg 集成：通过 CMAKE_TOOLCHAIN_FILE 自动注入
# 方式一：cmake -B build -DCMAKE_TOOLCHAIN_FILE=D:/software/vcpkg/scripts/buildsystems/vcpkg.cmake
# 方式二：在 CMakePresets.json 的 base preset 中配置（推荐，见根目录 CMakePresets.json）
# 方式三：CLion 工具链设置里填写 vcpkg 工具链文件

# 依赖在 vcpkg.json 中声明，cmake 配置阶段自动安装
find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE
    nlohmann_json::nlohmann_json
    fmt::fmt
)
