// 演示：边界用例测试
#include <iostream>
#include "calc.h"

int main() {
    int failed = 0;

    auto check = [&](const char* name, int got, int expected) {
        if (got != expected) {
            std::cerr << "FAIL " << name << ": got " << got << ", expected " << expected << "\n";
            ++failed;
        } else {
            std::cout << "PASS " << name << "\n";
        }
    };

    check("divide(10,2)",  divide(10, 2), 5);
    check("divide(7,0)",   divide(7, 0),  0);   // 除零保护
    check("divide(-6,3)",  divide(-6, 3), -2);

    return failed;
}
