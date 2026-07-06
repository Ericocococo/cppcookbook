// C++20 | 无依赖
// 演示：多源文件项目，头文件 include 路径配置
// 构建：cmake -B build && cmake --build build

#include <iostream>
#include "math_utils.h"

int main() {
    std::cout << "① 多源文件\n";
    std::cout << "  add(3, 4)      = " << add(3, 4) << "\n";
    std::cout << "  multiply(3, 4) = " << multiply(3, 4) << "\n";
    return 0;
}
