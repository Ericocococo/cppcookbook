cmake_minimum_required(VERSION 3.28)
project(ctest_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()  # 启用 ctest

add_library(calc STATIC calc.cpp)
target_include_directories(calc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ① 每个测试可执行文件 = 一组用例
add_executable(test_basic test_basic.cpp)
target_link_libraries(test_basic PRIVATE calc)
add_test(NAME basic COMMAND test_basic)

add_executable(test_edge test_edge.cpp)
target_link_libraries(test_edge PRIVATE calc)
add_test(NAME edge_cases COMMAND test_edge)

# ② 为测试设置超时、环境变量、标签
set_tests_properties(edge_cases PROPERTIES
    TIMEOUT 5
    LABELS "edge"
)

# ③ 运行：cmake --build build && ctest --build-dir build --output-on-failure
