#include <iostream>
#include "utils/string_utils.h"

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
    check("to_upper",     utils::to_upper("hello"), "HELLO");
    check("trim spaces",  utils::trim("  hi  "),    "hi");
    check("trim empty",   utils::trim("   "),        "");
    return failed;
}
