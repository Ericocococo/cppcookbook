// C++20 | 无依赖
// 演示：手写 Makefile 管理多文件项目
// 构建：make  或  make run

#include <iostream>
#include "math_utils.h"
#include "logger.h"

int main() {
    Logger log("main");

    log.info("程序启动");

    int a = 6, b = 7;
    log.info("计算 add(" + std::to_string(a) + ", " + std::to_string(b) + ")");
    std::cout << "  add      = " << add(a, b) << "\n";
    std::cout << "  multiply = " << multiply(a, b) << "\n";

    log.info("程序结束");
    return 0;
}
