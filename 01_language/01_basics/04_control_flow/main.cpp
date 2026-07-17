// C++20
// 演示：if/else、switch（fall-through）、while、do-while、for、范围for、break/continue、if constexpr
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <string>
#include <vector>
#include <type_traits>   // is_integral_v 等类型特性

// ① if / else：条件分支
void demo01_if_else() {
    std::cout << "\n① if / else\n";

    int score = 75;
    if      (score >= 90) std::cout << "  " << score << " -> 优秀\n";
    else if (score >= 75) std::cout << "  " << score << " -> 良好\n";
    else if (score >= 60) std::cout << "  " << score << " -> 及格\n";
    else                  std::cout << "  " << score << " -> 不及格\n";

    // 悬空 else 陷阱：else 和最近的 if 配对
    int a = 1, b = -1;
    if (a > 0)
        if (b > 0)
            std::cout << "  a>0 且 b>0\n";
    else   // 属于内层 if(b>0)，不是外层！
        std::cout << "  b<=0（else 配对内层 if，不是外层）\n";
    // 加括号消除歧义是最佳实践

    // C++17 初始化 if：变量作用域限于 if 块
    if (int x = score - 60; x >= 0)
        std::cout << "  初始化if：高于及格线 " << x << " 分\n";
    // x 在这里不可见
}

// ② switch：多值精确匹配
void demo02_switch() {
    std::cout << "\n② switch\n";

    int day = 3;
    switch (day) {
        case 1: std::cout << "  周一\n"; break;
        case 2: std::cout << "  周二\n"; break;
        case 3:
        case 4:
        case 5: std::cout << "  工作日(周三到周五)\n"; break;
        case 6:
        case 7: std::cout << "  周末\n"; break;
        default: std::cout << "  非法\n";
    }

    // fall-through（穿透）+ [[fallthrough]] 属性（C++17）
    int x = 1;
    switch (x) {
        case 1:
            std::cout << "  case 1 执行\n";
            [[fallthrough]];       // 告知编译器：故意穿透，不是忘了 break
        case 2:
            std::cout << "  case 2 也执行（fall-through）\n";
            break;
    }

    // C++17 初始化 switch
    switch (int v = day * 2; v) {
        case 6:  std::cout << "  初始化switch：v=" << v << "\n"; break;
        default: break;
    }
}

// ③ while / do-while
void demo03_while() {
    std::cout << "\n③ while / do-while\n";

    // while：先判断后执行
    int n = 5;
    std::cout << "  while 倒计时: ";
    while (n > 0) std::cout << n-- << " ";
    std::cout << "发射！\n";

    // do-while：先执行后判断，至少执行一次
    n = 0;
    std::cout << "  do-while(n=0,条件n<0): ";
    do { std::cout << "执行！"; ++n; } while (n < 0);
    std::cout << "\n";

    // while(true) + break：用于不确定次数的循环
    std::cout << "  while(true)+break: ";
    int i = 0;
    while (true) {
        if (i >= 3) break;
        std::cout << i++ << " ";
    }
    std::cout << "\n";
}

// ④ for 循环
void demo04_for() {
    std::cout << "\n④ for 循环\n";

    std::cout << "  正序0~4: ";
    for (int i = 0; i < 5; ++i) std::cout << i << " ";
    std::cout << "\n";

    std::cout << "  倒序4~0: ";
    for (int i = 4; i >= 0; --i) std::cout << i << " ";
    std::cout << "\n";

    std::cout << "  偶数0~10: ";
    for (int i = 0; i <= 10; i += 2) std::cout << i << " ";
    std::cout << "\n";

    // for 可以省略任何部分（分号不能省）
    int i = 0;
    std::cout << "  省略初始化(i已有): ";
    for (; i < 3; ++i) std::cout << i << " ";
    std::cout << "\n";

    // 多变量
    std::cout << "  多变量: ";
    for (int a = 0, b = 10; a < b; ++a, --b)
        std::cout << "(" << a << "," << b << ") ";
    std::cout << "\n";
}

// ⑤ 范围 for（C++11）
void demo05_range_for() {
    std::cout << "\n⑤ 范围 for\n";

    int arr[] = {1, 2, 3, 4, 5};

    // 值：副本，修改不影响原数组
    std::cout << "  auto  遍历: ";
    for (auto x : arr) std::cout << x << " ";
    std::cout << "\n";

    // 引用：修改原数组
    for (auto& x : arr) x *= 2;
    std::cout << "  auto& 修改(*2): ";
    for (auto x : arr) std::cout << x << " ";
    std::cout << "\n";

    // const 引用：只读不复制（大对象推荐）
    std::vector<std::string> words = {"hello", "world", "cpp"};
    std::cout << "  const auto& 只读: ";
    for (const auto& w : words) std::cout << w << " ";
    std::cout << "\n";

    // C++17 结构化绑定遍历 pair
    std::vector<std::pair<std::string, int>> data = {
        {"Alice", 95}, {"Bob", 82}
    };
    std::cout << "  结构化绑定:\n";
    for (const auto& [name, score] : data)
        std::cout << "    " << name << ": " << score << "\n";
}

// ⑥ break / continue / goto
void demo06_break_continue() {
    std::cout << "\n⑥ break / continue\n";

    std::cout << "  break(找到5停): ";
    for (int i = 0; i < 10; ++i) {
        if (i == 5) { std::cout << "停！"; break; }
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::cout << "  continue(跳偶数): ";
    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) continue;
        std::cout << i << " ";
    }
    std::cout << "\n";

    // break 只退出最近一层循环
    std::cout << "  嵌套break只退内层:\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (j == 1) break;
            std::cout << "    (" << i << "," << j << ")\n";
        }
    }

    // goto 退出多层：少数合理场景
    std::cout << "  goto 退出多层:\n";
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            if (i == 1 && j == 1) goto done;
            std::cout << "    (" << i << "," << j << ")\n";
        }
    done:
    std::cout << "    (跳出两层)\n";
}

// ⑦ if constexpr（C++17）：编译期条件，条件为 false 的分支完全不编译
template<typename T>
void printType(T val) {
    if constexpr (std::is_integral_v<T>)
        std::cout << "  整数: " << val << "\n";
    else if constexpr (std::is_floating_point_v<T>)
        std::cout << "  浮点: " << val << "\n";
    else
        std::cout << "  其他: " << val << "\n";
}

void demo07_if_constexpr() {
    std::cout << "\n⑦ if constexpr（C++17）\n";
    printType(42);
    printType(3.14);
    printType(std::string("hello"));
    std::cout << "  （false 的分支在编译期丢弃，不参与编译）\n";
}

int main() {
    std::cout << "=== 04_control_flow: 控制流 ===";
    demo01_if_else();
    demo02_switch();
    demo03_while();
    demo04_for();
    demo05_range_for();
    demo06_break_continue();
    demo07_if_constexpr();
    std::cout << "\n完成。\n";
    return 0;
}
