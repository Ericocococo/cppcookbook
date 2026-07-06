// C++20 | 依赖：Threads（系统自带）；ZLIB 可选
// 演示：find_package 两种模式（Module / Config）及可选依赖处理
// 构建：cmake -B build && cmake --build build

#include <iostream>
#include <thread>

int main() {
    std::cout << "① std::thread 依赖 Threads::Threads\n";
    std::thread t([] {
        std::cout << "  子线程 id: " << std::this_thread::get_id() << "\n";
    });
    t.join();

#ifdef HAS_ZLIB
    std::cout << "② ZLIB 已链接（版本宏由 CMake 注入）\n";
#else
    std::cout << "② ZLIB 未找到，跳过\n";
#endif

    return 0;
}
