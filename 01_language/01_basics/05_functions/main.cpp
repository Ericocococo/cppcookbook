// C++20
// 演示：函数定义、值/引用传参、默认参数、重载、内联、constexpr函数、函数指针、lambda
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <string>
#include <functional>   // std::function

// 知识点 1.1：基本函数：定义、调用、返回值

int add(int a, int b) { return a + b; }
void printSep() { std::cout << "  -----\n"; }

// 知识点 1.2：函数声明（前向声明）：先声明再定义，调用方不需要看到完整定义

int multiply(int a, int b); // 声明

void demo01_basic()
{
    std::cout << "\n① 基本函数\n";
    std::cout << "  add(3,5)=" << add(3, 5) << "\n";
    std::cout << "  multiply(3,4)=" << multiply(3, 4) << "\n";
}

int multiply(int a, int b) { return a * b; } // 定义（可以在声明之后）

// 知识点 2.1：参数传递：值、引用、const 引用、指针

void byValue(int x) { x = 999; (void)x; } // 副本，不影响外部（(void)x：演示后参数没再读，消除警告）
void byRef(int& x) { x *= 2; } // 引用，影响外部
void byConstRef(const int& x) { (void)x; } // 只读，不复制
void byPointer(int* p) { if (p) *p = 42; } // 指针，可以传 nullptr
void byConstPointer(const int* p) { (void)p; } // 只读指针

void demo02_params()
{
    std::cout << "\n② 参数传递方式\n";

    int n = 10;
    byValue(n);
    std::cout << "  值传参后  n=" << n << "（未变）\n";
    byRef(n);
    std::cout << "  引用传参后 n=" << n << "（变了）\n";
    byPointer(&n);
    std::cout << "  指针传参后 n=" << n << "\n";

    // 何时用哪种？
    std::cout << "  规则：\n";
    std::cout << "    小对象(int/double)  → 值传参\n";
    std::cout << "    需要修改           → 非 const 引用\n";
    std::cout << "    大对象只读         → const 引用（避免复制）\n";
    std::cout << "    可能为 nullptr     → 指针\n";

    // 知识点 2.2：const 引用可以绑定临时值（值传参不行）

    const int& ref = 42; // 临时值绑定到 const 引用，延长生命周期
    std::cout << "  const int& ref=42: " << ref << "（临时值可绑 const 引用）\n";
}

// 知识点 3.1：默认参数

void greet(std::string name,
           std::string prefix = "Hello",
           char ending = '!')
{
    std::cout << "  " << prefix << ", " << name << ending << "\n";
}

void demo03_default_params()
{
    std::cout << "\n③ 默认参数（从右往左设置）\n";
    greet("Alice");
    greet("Bob", "Hi");
    greet("Charlie", "Hey", '?');
    // 知识点 3.2：规则：默认参数只能从右往左设置
    // void f(int a=1, int b) {}  // 错误：b 在 a 右边但没有默认值
}

// 知识点 4.1：函数重载：同名，参数不同（类型或数量）

int max(int a, int b) { return a > b ? a : b; }
double max(double a, double b) { return a > b ? a : b; }
int max(int a, int b, int c) { return max(max(a, b), c); }

void demo04_overload()
{
    std::cout << "\n④ 函数重载\n";
    std::cout << "  max(3,5)      = " << max(3, 5) << "（int版）\n";
    std::cout << "  max(3.1,2.7)  = " << max(3.1, 2.7) << "（double版）\n";
    std::cout << "  max(1,5,3)    = " << max(1, 5, 3) << "（三参版）\n";
    // 知识点 4.2：返回类型不同不构成重载！只有参数不同才算重载
}

// 知识点 5.1：inline 内联函数

inline int square(int x) { return x * x; }

// 知识点 5.2：constexpr 函数：编译期可求值（如果参数是编译期常量）

constexpr int cube(int x) { return x * x * x; }

void demo05_inline_constexpr()
{
    std::cout << "\n⑤ inline / constexpr 函数\n";
    std::cout << "  square(5)=" << square(5) << "（inline：建议编译器内联展开）\n";

    constexpr int c = cube(3); // 编译期求值，c 是编译期常量
    int arr[c]; // 可以用作数组大小
    std::cout << "  constexpr cube(3)=" << c
        << "（编译期计算，可作数组大小=" << sizeof(arr) / 4 << "）\n";

    int n = 4;
    int d = cube(n); // n 不是编译期常量，运行期求值
    std::cout << "  cube(" << n << ")=" << d << "（n 非编译期常量，运行时求值）\n";
}

// 知识点 6.1：函数指针

int applyOp(int a, int b, int (*op)(int, int))
{
    return op(a, b);
}

void demo06_function_pointer()
{
    std::cout << "\n⑥ 函数指针\n";

    // 知识点 6.2：函数指针类型：int (*fp)(int, int)

    int (*fp)(int, int) = add;
    std::cout << "  fp=add; fp(3,4)=" << fp(3, 4) << "\n";

    // 知识点 6.3：作为参数（回调）

    std::cout << "  applyOp(10,3,add)=" << applyOp(10, 3, add) << "\n";
    std::cout << "  applyOp(10,3,multiply)=" << applyOp(10, 3, multiply) << "\n";

    // 知识点 6.4：using 简化函数指针类型

    using BinOp = int(*)(int, int);
    BinOp ops[] = {add, multiply};
    std::cout << "  ops[0](2,3)=" << ops[0](2, 3)
        << "  ops[1](2,3)=" << ops[1](2, 3) << "\n";

    // 知识点 6.5：std::function：更灵活，可以存 lambda/函数对象/函数指针

    std::function < int(int, int) > f = add;
    std::cout << "  std::function f=add; f(2,3)=" << f(2, 3) << "\n";
}

// 知识点 7.1：Lambda 表达式（C++11）

void demo07_lambda()
{
    std::cout << "\n⑦ Lambda\n";

    // 知识点 7.2：[捕获列表](参数) -> 返回类型 { 函数体 }

    auto mul = [](int a, int b) { return a * b; };
    std::cout << "  mul(3,4)=" << mul(3, 4) << "\n";

    // 知识点 7.3：捕获外部变量

    int factor = 3;
    auto scaleVal = [factor](int x) { return x * factor; }; // 值捕获（副本）
    auto scaleRef = [&factor](int x) { return x * factor; }; // 引用捕获

    factor = 10;
    std::cout << "  factor改为10后:\n";
    std::cout << "    值捕获  scaleVal(5)=" << scaleVal(5)
        << "（仍用3，副本不变）\n";
    std::cout << "    引用捕获 scaleRef(5)=" << scaleRef(5)
        << "（用10，引用最新值）\n";

    // 知识点 7.4：mutable lambda：值捕获的副本可修改（但不影响外部）

    int count = 0;
    auto inc = [count]() mutable { return ++count; };
    std::cout << "  mutable lambda: " << inc() << " " << inc() << " " << inc()
        << "  count=" << count << "（外部未变）\n";

    // 知识点 7.5：泛型 lambda（C++14）：参数用 auto

    auto print = [](auto a, auto b)
    {
        std::cout << "  (" << a << ", " << b << ")\n";
    };
    std::cout << "  泛型lambda:";
    print(1, 3.14);
    std::cout << "  泛型lambda:";
    print("hi", 42);

    // 知识点 7.6：立即调用 lambda（IIFE）

    int result = [](int n) { return n * n; }(7);
    std::cout << "  立即调用lambda: 7*7=" << result << "\n";
}

// 知识点 8.1：尾置返回类型（C++11）

auto divide(double a, double b) -> double
{
    return a / b;
}

// 知识点 8.2：返回引用的危险
// 禁止写法示例（写成注释：一旦定义，编译器会对 return local 发出
// -Wreturn-local-addr 警告；调用它更是未定义行为）：
/*
int& badReturn()
{
    int local = 42;
    return local; // 危险！返回局部变量的引用，函数结束后 local 已销毁
}
*/
// 规则：绝不返回局部变量的引用/指针——局部变量在函数返回时就销毁了，
// 返回的引用指向已不存在的对象（UB），表现不可预测（垃圾值/旧值/崩溃）

void demo08_return()
{
    std::cout << "\n⑧ 返回类型\n";
    std::cout << "  尾置返回: divide(10,3)=" << divide(10, 3) << "\n";
    std::cout << "  危险：不要返回局部变量的引用！（示例见上方注释，不运行演示）\n";
}

int main()
{
    std::cout << "=== 05_functions: 函数 ===";
    demo01_basic();
    demo02_params();
    demo03_default_params();
    demo04_overload();
    demo05_inline_constexpr();
    demo06_function_pointer();
    demo07_lambda();
    demo08_return();
    std::cout << "\n完成。\n";
    return 0;
}
