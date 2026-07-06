// C++20 | 无依赖
// 演示：生成器表达式注入的宏在运行时可见
// 构建：cmake -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build

#include <iostream>

int main() {
    std::cout << "BUILD_TYPE = " << BUILD_TYPE << "\n";

#ifdef MY_DEBUG
    std::cout << "MY_DEBUG   = " << MY_DEBUG << "\n";
#endif
#ifdef MY_RELEASE
    std::cout << "MY_RELEASE = " << MY_RELEASE << "\n";
#endif
#ifdef GCC_DEBUG
    std::cout << "GCC_DEBUG  = " << GCC_DEBUG << " (GCC Debug 专属)\n";
#endif

    return 0;
}
