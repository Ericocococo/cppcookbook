// C++20 | 无依赖
// 演示：install 规则 + 导出 CMake 包配置
// 构建：cmake -B build -DCMAKE_INSTALL_PREFIX=./install && cmake --build build && cmake --install build

#include <iostream>
#include "mylib.h"

int main() {
    std::cout << "compute(7) = " << compute(7) << "\n";
    return 0;
}
