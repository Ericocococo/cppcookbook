// C++20 | 依赖：nlohmann-json、fmt（均通过 vcpkg 安装）
// 演示：vcpkg 包管理 + CMake 集成
// 构建前提：cmake -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake

#include <iostream>
#include <nlohmann/json.hpp>
#include <fmt/core.h>

int main() {
    std::cout << "① nlohmann_json\n";
    nlohmann::json j = {
        {"name", "cppcookbook"},
        {"version", 1},
        {"tags", {"cmake", "vcpkg", "c++"}}
    };
    std::cout << "  " << j.dump(2) << "\n";

    std::cout << "② fmt\n";
    fmt::println("  Hello from {}, version {}", "fmt", FMT_VERSION);

    return 0;
}
