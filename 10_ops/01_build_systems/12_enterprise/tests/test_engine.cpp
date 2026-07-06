#include <iostream>
#include "core/engine.h"

int main() {
    int failed = 0;
    auto check = [&](const char* name, const std::string& got, const std::string& expected) {
        if (got != expected) {
            std::cerr << "FAIL " << name << ": \"" << got << "\" != \"" << expected << "\"\n";
            ++failed;
        } else {
            std::cout << "PASS " << name << "\n";
        }
    };
    check("trim+upper", core::process("  hello  "), "HELLO");
    check("already clean", core::process("world"),   "WORLD");
    return failed;
}
