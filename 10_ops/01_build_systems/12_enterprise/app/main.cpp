// C++20 | 无依赖（三方库）
// 演示：企业级多模块项目，app → core → utils 依赖链
// 构建：cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build

#include <iostream>
#include "core/engine.h"

int main() {
    std::cout << "① 多模块依赖链：app → core → utils\n";

    std::string inputs[] = {"  hello, cmake!  ", "  enterprise build  ", "  c++ cookbook  "};
    for (const auto& s : inputs) {
        std::cout << "  \"" << s << "\"  →  \"" << core::process(s) << "\"\n";
    }

    return 0;
}
