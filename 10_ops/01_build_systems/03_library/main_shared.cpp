// C++20 | 无依赖
// 演示：链接动态库（运行时需要 .dll 在同目录或 PATH 中）
#include <iostream>
#include "math_utils.h"

int main() {
    std::cout << "② 动态库链接\n";
    std::cout << "  add(7, 2)      = " << add(7, 2) << "\n";
    std::cout << "  multiply(7, 2) = " << multiply(7, 2) << "\n";
    return 0;
}
