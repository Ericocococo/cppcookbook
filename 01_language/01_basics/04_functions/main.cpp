// C++20
// 演示：函数定义、默认参数、重载、内联、函数指针、lambda 表达式
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <string>

// ① 基本函数：定义与调用
// 格式：返回类型 函数名(参数列表) { 函数体 }
int add(int a, int b) {
    return a + b;
}

// 无返回值用 void
void printLine(std::string msg) {
    std::cout << "  " << msg << "\n";
}

// 值传递：函数内修改不影响外部变量
void tryModify(int x) {
    x = 999;  // 只是改了副本
}

// 引用传递：函数内修改会影响外部变量
void doubleIt(int& x) {
    x *= 2;   // 直接修改原变量
}

void demo01_basic_functions() {
    std::cout << "\n① 基本函数\n";

    int result = add(3, 5);
    std::cout << "  add(3, 5) = " << result << "\n";
    printLine("printLine 打印这段文字");

    // 值传递 vs 引用传递
    int n = 10;
    tryModify(n);
    std::cout << "  值传递后  n = " << n << "（没变，传的是副本）\n";
    doubleIt(n);
    std::cout << "  引用传递后 n = " << n << "（变了，传的是引用）\n";
}

// ② 默认参数：调用时可以省略
// 注意：默认参数必须从右往左声明
void greet(std::string name, std::string prefix = "Hello", char end = '!') {
    std::cout << "  " << prefix << ", " << name << end << "\n";
}

void demo02_default_params() {
    std::cout << "\n② 默认参数\n";
    greet("Alice");                  // 用默认的 prefix 和 end
    greet("Bob", "Hi");             // 指定 prefix，end 用默认
    greet("Charlie", "Hey", '?');   // 全部指定
}

// ③ 函数重载：同名函数，参数不同（类型或数量）
// 编译器根据调用时的参数自动选择
int multiply(int a, int b) {
    std::cout << "  调用 int 版本: ";
    return a * b;
}
double multiply(double a, double b) {
    std::cout << "  调用 double 版本: ";
    return a * b;
}
int multiply(int a, int b, int c) {
    std::cout << "  调用 三参数版本: ";
    return a * b * c;
}

void demo03_overload() {
    std::cout << "\n③ 函数重载\n";
    std::cout << multiply(2, 3) << "\n";
    std::cout << multiply(2.5, 3.0) << "\n";
    std::cout << multiply(2, 3, 4) << "\n";
}

// ④ 内联函数：建议编译器把函数调用展开为函数体（避免函数调用开销）
// 适合短小、频繁调用的函数
inline int square(int x) {
    return x * x;
}

void demo04_inline() {
    std::cout << "\n④ 内联函数\n";
    std::cout << "  square(5) = " << square(5) << "\n";
    std::cout << "  square(9) = " << square(9) << "\n";
}

// ⑤ 函数指针：把函数当成变量来存储和传递
int applyOp(int a, int b, int (*op)(int, int)) {
    return op(a, b);   // 通过函数指针调用
}

void demo05_function_pointer() {
    std::cout << "\n⑤ 函数指针\n";

    // int (*fp)(int, int) 声明一个指向 "返回int，接受两个int" 函数的指针
    int (*fp)(int, int) = add;    // 指向前面的 add 函数
    std::cout << "  fp(3, 4) = " << fp(3, 4) << "\n";

    // 把函数指针作为参数传给另一个函数（策略模式的基础）
    std::cout << "  applyOp(10, 3, add)      = " << applyOp(10, 3, add)      << "\n";
    std::cout << "  applyOp(10, 3, multiply) = " << applyOp(10, 3, multiply) << "\n";
}

// ⑥ Lambda 表达式（C++11）：在用到的地方直接定义匿名函数
void demo06_lambda() {
    std::cout << "\n⑥ Lambda 表达式\n";

    // 格式：[捕获列表](参数列表) -> 返回类型 { 函数体 }
    // 返回类型可以省略，编译器自动推断

    // 最简单的 lambda
    auto sayHi = []() { std::cout << "  Hi from lambda!\n"; };
    sayHi();

    // 有参数和返回值
    auto mul = [](int a, int b) -> int { return a * b; };
    std::cout << "  mul(3, 4) = " << mul(3, 4) << "\n";

    // 捕获外部变量
    int factor = 3;

    // [factor]：值捕获（lambda 拿到的是 factor 的副本）
    auto scaleVal = [factor](int x) { return x * factor; };

    // [&factor]：引用捕获（lambda 直接用外部的 factor）
    auto scaleRef = [&factor](int x) { return x * factor; };

    std::cout << "  factor=" << factor << "\n";
    std::cout << "  值捕获  scaleVal(5) = " << scaleVal(5) << "\n";
    factor = 10;   // 修改 factor
    std::cout << "  factor 改为 " << factor << " 后:\n";
    std::cout << "  值捕获  scaleVal(5) = " << scaleVal(5) << "（仍然用3，副本不变）\n";
    std::cout << "  引用捕获 scaleRef(5) = " << scaleRef(5) << "（用新的10）\n";

    // [=]：捕获所有外部变量的值；[&]：捕获所有外部变量的引用
    auto all = [=](int x) { return x * factor; };   // 捕获所有（值）
    std::cout << "  [=] 捕获全部值 all(5) = " << all(5) << "\n";
}

int main() {
    std::cout << "=== 04_functions: 函数 ===";
    demo01_basic_functions();
    demo02_default_params();
    demo03_overload();
    demo04_inline();
    demo05_function_pointer();
    demo06_lambda();
    std::cout << "\n完成。\n";
    return 0;
}
