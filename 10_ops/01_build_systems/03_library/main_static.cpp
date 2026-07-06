// C++20 | 无依赖
// 演示：链接静态库
#include <iostream>
#include "math_utils.h"

int main() {
    std::cout << "① 静态库链接\n";
    std::cout << "  add(5, 3)      = " << add(5, 3) << "\n";
    std::cout << "  multiply(5, 3) = " << multiply(5, 3) << "\n";
    return 0;
}
