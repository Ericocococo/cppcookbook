// 演示：ctest 基础测试，返回非零视为失败
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

    check("add(1,2)",   add(1, 2),   3);
    check("add(-1,1)",  add(-1, 1),  0);
    check("add(0,0)",   add(0, 0),   0);

    return failed;
}
