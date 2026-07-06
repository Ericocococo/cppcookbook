// C++20 | 无依赖
// 演示：现代 target 属性传播（PRIVATE / PUBLIC / INTERFACE）
// 构建：cmake -B build && cmake --build build

#include <iostream>
#include "core.h"

int main() {
    std::cout << "① target_include_directories PUBLIC 传播\n";
    std::cout << "  " << greet("CMake") << "\n";

    std::cout << "② INTERFACE 库注入宏\n";
    std::cout << "  UTILS_VERSION = " << version() << "\n";

    return 0;
}
