// C++20
// 演示：lambda 表达式 — 语法、捕获、泛型 lambda、常见用法
// 构建：cmake -B build-mingw-ninja（详见 README.md）

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>

// 知识点 1.1：lambda 基本语法
// [捕获列表](参数列表) -> 返回类型 { 函数体 }
//
// 返回类型可省略，编译器自动推导
void demo01_basic() {
    std::cout << "\n① 基本语法\n";

    // 知识点 1.2：最简 lambda — 无参数、无捕获

    auto hello = []() {
        std::cout << "  hello lambda\n";
    };
    hello();

    // 知识点 1.3：带参数

    auto add = [](int a, int b) {
        return a + b;
    };
    int sum = add(3, 5);
    std::cout << "  add(3, 5) = " << sum << "\n";

    // 知识点 1.4：显式指定返回类型
    // 通常不需要，编译器能推导；当函数体有多条 return 且类型不同时才需要显式写

    auto divide = [](double a, double b) -> double {
        if (b == 0) {
            return 0.0;
        }
        return a / b;
    };
    double result = divide(10.0, 3.0);
    std::cout << "  divide(10, 3) = " << result << "\n";
}

// 知识点 2.1：捕获列表
// []      不捕获任何外部变量
// [x]     按值捕获 x（拷贝一份，lambda 内修改不影响外部）
// [&x]    按引用捕获 x（lambda 内修改会影响外部）
// [=]     按值捕获所有外部变量
// [&]     按引用捕获所有外部变量
// [=, &x] 默认按值，x 按引用
// [&, x]  默认按引用，x 按值
void demo02_capture() {
    std::cout << "\n② 捕获列表\n";

    int x = 10;
    int y = 20;

    // 知识点 2.2：按值捕获 x — lambda 内拿到的是拷贝

    auto by_value = [x]() {
        std::cout << "  [x] 按值捕获: x=" << x << "\n";
        // x = 99;  // 编译错误：按值捕获默认是 const
    };
    by_value();

    // 知识点 2.3：按引用捕获 x — lambda 内修改会影响外部

    auto by_ref = [&x]() {
        x = 99;
        std::cout << "  [&x] 按引用捕获: x 改为 " << x << "\n";
    };
    by_ref();
    std::cout << "  外部 x = " << x << "（被 lambda 修改了）\n";

    // 知识点 2.4：按值捕获所有

    auto capture_all_value = [=]() {
        std::cout << "  [=] 按值捕获所有: x=" << x << " y=" << y << "\n";
    };
    capture_all_value();

    // 知识点 2.5：按引用捕获所有

    auto capture_all_ref = [&]() {
        x = 100;
        y = 200;
    };
    capture_all_ref();
    std::cout << "  [&] 按引用捕获所有: x=" << x << " y=" << y << "\n";
}

// 知识点 3.1：mutable — 允许修改按值捕获的变量
// 修改的是拷贝，不影响外部
void demo03_mutable() {
    std::cout << "\n③ mutable\n";

    int count = 0;

    // 知识点 3.2：按值捕获 count，加 mutable 后可以在 lambda 内修改拷贝

    auto counter = [count]() mutable {
        count++;
        std::cout << "  lambda 内 count = " << count << "\n";
    };

    counter(); // 1
    counter(); // 2
    counter(); // 3
    std::cout << "  外部 count = " << count << "（没变，lambda 改的是拷贝）\n";
}

// 知识点 4.1：泛型 lambda（C++14）
// 参数用 auto，类似函数模板
void demo04_generic() {
    std::cout << "\n④ 泛型 lambda\n";

    // 知识点 4.2：auto 参数 — 编译器对每种类型生成一个版本

    auto print = [](const auto& value) {
        std::cout << "  " << value << "\n";
    };

    print(42);
    print(3.14);
    print(std::string("hello"));
}

// 知识点 5.1：lambda + STL 算法
// lambda 作为函数参数，STL 算法最常见的用法
void demo05_stl() {
    std::cout << "\n⑤ lambda + STL 算法\n";

    std::vector<int> nums = {5, 2, 8, 1, 9, 3, 7};

    // 知识点 5.2：sort — 自定义比较规则

    std::sort(
        nums.begin(),
        nums.end(),
        [](int a, int b) {
            return a > b;
        });
    std::cout << "  降序排序: ";
    for (int n : nums) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // 知识点 5.3：count_if — 统计满足条件的元素个数

    int even_count = std::count_if(
        nums.begin(),
        nums.end(),
        [](int n) {
            return n % 2 == 0;
        });
    std::cout << "  偶数个数: " << even_count << "\n";

    // 知识点 5.4：for_each — 对每个元素执行操作

    std::cout << "  每个元素 +10: ";
    std::for_each(
        nums.begin(),
        nums.end(),
        [](int& n) {
            n += 10;
        });
    for (int n : nums) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // 知识点 5.5：find_if — 查找第一个满足条件的元素

    auto it = std::find_if(
        nums.begin(),
        nums.end(),
        [](int n) {
            return n > 15;
        });
    if (it != nums.end()) {
        std::cout << "  第一个 >15 的元素: " << *it << "\n";
    }
}

// 知识点 6.1：std::function
// lambda 存到 std::function，可以做回调、存到容器里
void demo06_function() {
    std::cout << "\n⑥ std::function\n";

    // 知识点 6.2：std::function 存任意可调用对象
    // lambda、函数指针、仿函数都可以

    std::function<int(int, int)> op;

    op = [](int a, int b) { return a + b; };
    std::cout << "  加法: " << op(3, 4) << "\n";

    op = [](int a, int b) { return a * b; };
    std::cout << "  乘法: " << op(3, 4) << "\n";

    // 知识点 6.3：存到 vector 里

    std::vector<std::function<int(int, int)>> ops = {
        [](int a, int b) { return a + b; },
        [](int a, int b) { return a - b; },
        [](int a, int b) { return a * b; },
    };

    std::cout << "  ops 批量执行 (10, 3): ";
    for (const auto& f : ops) {
        std::cout << f(10, 3) << " ";
    }
    std::cout << "\n";
}

// 知识点 7.1：立即调用的 lambda（IIFE）
void demo07_iife() {
    std::cout << "\n⑦ 立即调用的 lambda（IIFE）\n";

    // 知识点 7.2：定义后立即调用 — 常用于复杂的 const 初始化

    const int value = [] {
        int result = 0;
        for (int i = 1; i <= 10; i++) {
            result += i;
        }
        return result;
    }();

    std::cout << "  1+2+...+10 = " << value << "\n";
}

int main() {
    std::cout << "=== lambda 表达式 ===\n";

    demo01_basic();
    demo02_capture();
    demo03_mutable();
    demo04_generic();
    demo05_stl();
    demo06_function();
    demo07_iife();

    return 0;
}
