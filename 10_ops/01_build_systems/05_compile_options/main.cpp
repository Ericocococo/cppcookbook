// C++20 | 无依赖
// 演示：编译选项、宏定义、生成器表达式
// 构建：cmake -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build

#include <iostream>

int main() {
    std::cout << "① APP_VERSION = " << APP_VERSION << "\n";

#ifdef DEBUG_BUILD
    std::cout << "② 当前是 Debug 构建\n";
#else
    std::cout << "② 当前是 Release 构建\n";
#endif

#ifdef _MSC_VER
    std::cout << "③ 编译器：MSVC " << _MSC_VER << "\n";
#elif defined(__GNUC__)
    std::cout << "③ 编译器：GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "\n";
#endif

    return 0;
}
