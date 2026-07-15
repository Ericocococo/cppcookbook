// C++20
// 演示：if/else、switch、while、do-while、for、范围for、break/continue
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <string>

// ① if / else if / else：根据条件走不同分支
void demo01_if_else() {
    std::cout << "\n① if / else\n";

    int score = 75;
    std::cout << "  score = " << score << "\n";

    if (score >= 90) {
        std::cout << "  -> 优秀\n";
    } else if (score >= 60) {
        std::cout << "  -> 及格\n";  // score=75，走这里
    } else {
        std::cout << "  -> 不及格\n";
    }

    // C++17 初始化 if：在条件判断里直接声明变量，变量作用域限定在 if 块内
    if (int x = score - 60; x > 0) {
        std::cout << "  高于及格线 " << x << " 分（C++17 初始化if）\n";
    }
}

// ② switch：多分支选择，比多个 if-else 更清晰
void demo02_switch() {
    std::cout << "\n② switch\n";

    int day = 3;  // 1=周一，2=周二，...
    std::cout << "  day = " << day << "\n";

    switch (day) {
        case 1:
            std::cout << "  -> 周一\n";
            break;       // break 跳出 switch，不加 break 会继续执行下一个 case
        case 2:
            std::cout << "  -> 周二\n";
            break;
        case 3:
        case 4:
        case 5:          // 多个 case 共用同一段代码（周三/四/五都是工作日）
            std::cout << "  -> 工作日（周三到周五）\n";
            break;
        case 6:
        case 7:
            std::cout << "  -> 周末\n";
            break;
        default:         // 所有 case 都不匹配时走 default
            std::cout << "  -> 非法日期\n";
    }
}

// ③ while：先判断条件，条件为 true 才执行
void demo03_while() {
    std::cout << "\n③ while（先判断再执行）\n";

    // 倒计时
    int n = 5;
    std::cout << "  倒计时: ";
    while (n > 0) {
        std::cout << n << " ";
        --n;
    }
    std::cout << "发射！\n";

    // 求 1+2+...+100
    int sum = 0, i = 1;
    while (i <= 100) {
        sum += i;
        ++i;
    }
    std::cout << "  1+2+...+100 = " << sum << "\n";
}

// ④ do-while：先执行一次，再判断条件
void demo04_do_while() {
    std::cout << "\n④ do-while（先执行一次再判断）\n";

    // 即使条件一开始就是 false，也会执行至少一次
    int n = 0;
    std::cout << "  n=0，do-while 仍然执行了一次: ";
    do {
        std::cout << "执行！";
        ++n;
    } while (n < 0);   // n=0 < 0 为 false，循环结束
    std::cout << "\n";

    // 经典应用：菜单循环（模拟，不真正读输入）
    int choice = 0;
    std::cout << "  模拟菜单（自动选2退出）:\n";
    do {
        ++choice;  // 模拟用户选择
        std::cout << "    菜单选项=" << choice << "\n";
    } while (choice != 2);
    std::cout << "  退出菜单\n";
}

// ⑤ for：最常用的循环，适合已知循环次数的场景
void demo05_for() {
    std::cout << "\n⑤ for（适合已知次数）\n";

    // 基本格式：for(初始化; 条件; 步进)
    std::cout << "  0 到 4: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    // 倒序
    std::cout << "  倒序: ";
    for (int i = 4; i >= 0; --i) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    // 步进不一定是 1
    std::cout << "  偶数: ";
    for (int i = 0; i <= 10; i += 2) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    // 打印九九乘法表
    std::cout << "  九九乘法表（部分）:\n";
    for (int i = 1; i <= 3; ++i) {
        for (int j = 1; j <= 3; ++j) {
            std::cout << "    " << i << "×" << j << "=" << (i*j) << "\n";
        }
    }
}

// ⑥ 范围 for：遍历数组/容器，更简洁
void demo06_range_for() {
    std::cout << "\n⑥ 范围 for（遍历集合）\n";

    int arr[] = {10, 20, 30, 40, 50};

    // 值拷贝：x 是元素的副本，修改 x 不影响原数组
    std::cout << "  遍历数组: ";
    for (int x : arr) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // 引用：x 是元素的别名，修改 x 会修改原数组
    for (int& x : arr) {
        x *= 2;   // 把每个元素乘以 2
    }
    std::cout << "  乘2后: ";
    for (int x : arr) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // auto 自动推断类型，更方便
    std::string words[] = {"hello", "world", "cpp"};
    std::cout << "  字符串数组: ";
    for (const auto& w : words) {  // const 引用：不修改，但避免拷贝
        std::cout << w << " ";
    }
    std::cout << "\n";
}

// ⑦ break 和 continue：控制循环走向
void demo07_break_continue() {
    std::cout << "\n⑦ break 和 continue\n";

    // break：立即退出整个循环
    std::cout << "  break（找到5就停）: ";
    for (int i = 0; i < 10; ++i) {
        if (i == 5) {
            std::cout << "找到5，停！";
            break;
        }
        std::cout << i << " ";
    }
    std::cout << "\n";

    // continue：跳过本次迭代，继续下一次
    std::cout << "  continue（跳过偶数）: ";
    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) continue;  // 偶数跳过
        std::cout << i << " ";     // 只打印奇数
    }
    std::cout << "\n";
}

int main() {
    std::cout << "=== 03_control_flow: 控制流 ===";
    demo01_if_else();
    demo02_switch();
    demo03_while();
    demo04_do_while();
    demo05_for();
    demo06_range_for();
    demo07_break_continue();
    std::cout << "\n完成。\n";
    return 0;
}
