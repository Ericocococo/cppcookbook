#include "core.h"

std::string greet(const std::string& name) {
    return "Hello, " + name + "!";
}

int version() {
    return UTILS_VERSION;  // 由 INTERFACE 库注入的宏
}
