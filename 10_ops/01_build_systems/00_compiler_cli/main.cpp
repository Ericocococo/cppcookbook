// C++20 | 无依赖
// 演示：不依赖任何构建系统，直接调用编译器命令行
// 见 README.md 查看完整编译命令

#include <iostream>
#include <vector>
#include <algorithm>

#include "math_utils.h"

int main() {
    std::cout << "① 单文件编译结果\n";
    std::cout << "  add(3, 4)      = " << add(3, 4) << "\n";
    std::cout << "  multiply(3, 4) = " << multiply(3, 4) << "\n";

    std::cout << "② C++20 特性正常工作\n";
    std::vector v = {5, 3, 1, 4, 2};  // CTAD：C++17
    std::ranges::sort(v);              // ranges：C++20
    std::cout << "  sorted: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
