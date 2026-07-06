// C++20 | 无依赖
// 演示：CMakePresets.json 管理多套构建配置（Debug/Release/ASan）
// 构建：cmake --preset debug && cmake --build --preset debug
//       cmake --preset asan  && cmake --build --preset asan

#include <iostream>
#include <vector>

int main() {
    std::cout << "预设构建演示\n";

    // ASan 会在堆越界时报错
    std::vector<int> v = {1, 2, 3};
    std::cout << "  v[0] = " << v[0] << "\n";
    std::cout << "  size = " << v.size() << "\n";

    return 0;
}
