// C++20 | 依赖：无
// 第一步：纯 C++ — 确认编译环境可用，和 Python 没有任何关系。
//
// 函数声明放在 hello.h（头文件 = 菜单），实现放在 main.cpp（厨房 = 实际做菜）。
//
// #include：把指定头文件的内容复制进来，相当于"引入工具箱"
// 用到了什么就显式 include 什么，不依赖间接传递
#include <iostream>  // std::cout（控制台输出）
#include <string>    // std::string（字符串）
#include <utility>   // std::pair（打包两个值）、结构化绑定 auto [a, b]
#include <vector>    // std::vector（动态数组）

/* #include "hello.h"：把 hello.h 的内容复制到这里，告诉编译器这三个函数的存在。
   C++ 编译器一个文件一个文件单独编译，main() 里调用 add(3, 5) 之前必须先知道 add 长什么样
   （参数类型、返回类型），否则编译器报错"不认识 add"。

   本文件不写也能编译（函数定义在 main() 前面，编译器已经见过了），但如果把函数实现
   移到另一个 .cpp 文件，不写 #include "hello.h" 编译器就看不到声明，直接报错。
   养成习惯：函数定义在别的文件时，include 声明所在的 .h。

   "" 先在当前目录找，<> 只在系统/库路径找 */
#include "hello.h"

// 一个最简单的 C++ 函数
int add(int a, int b)
{
    return a + b;
}

/* const 在不同位置含义不同（本项目出现的全部 6 种）：

   ┌────────────────────────────┬────────────────────────────┬──────────────────────────────────────────┐
   │ 位置                       │ 含义                       │ 示例                                     │
   ├────────────────────────────┼────────────────────────────┼──────────────────────────────────────────┤
   │ ① 参数前 const 引用        │ 不拷贝、不修改传入的数据   │ avg(const vector<double>& v)              │
   │ ② 成员函数末尾 const       │ 不修改对象的成员变量       │ code() const                             │
   │ ③ for 循环 const auto&     │ 遍历时不修改元素、不拷贝   │ for (const auto& r : v)                  │
   │ ④ const 局部变量           │ 变量值初始化后不可改       │ const int64_t rows = ...                  │
   │ ⑤ lambda 参数              │ lambda 内部不修改参数      │ [](const X& a) { ... }                   │
   │ ⑥ const + noexcept         │ 参数不改 + 对象不改        │ operator()(const K& k) const noexcept    │
   └────────────────────────────┴────────────────────────────┴──────────────────────────────────────────┘

   ② 成员函数末尾 const 只有成员函数能用，普通函数没有。
   因为它的含义是"不修改 this 指向的对象"——普通函数没有 this。
   普通函数想表达"不修改参数"，用 ① 参数前 const。

   ① 和 ② 的区别：
     avg(const vector<double>& v)            ← ① 参数前：不修改传进来的 v
     std::string code() const                ← ② 末尾：不修改对象自身的成员变量
     两个可以同时出现：
     operator()(const Key& k) const          ← 参数 k 不改（①），对象也不改（②）

   noexcept = 承诺绝不抛异常，编译器可放心优化；违背承诺程序直接终止 */
// vector<double>：动态数组里存 double 类型
//   vector           = 可变长度的数组
//   <double>         = 模板参数，告诉 vector 里面装什么类型
//                       vector<int> 就是整数数组，vector<string> 就是字符串数组
//
//   用法示例：
//     vector<double> prices;           // 声明一个空数组
//     prices.push_back(10.5);          // 追加元素
//     prices.push_back(20.3);
//     prices.size();                   // 元素个数 → 2
//     prices[0];                       // 按索引访问 → 10.5
//     prices.empty();                  // 是否为空 → false
//
// const & = 只读引用，避免拷贝整个数组

// 引用（T&）vs 指针（T*）：
//   T& → 引用，给原变量起别名，直接用（函数参数最常用，调用时不加 & *）
//   T* → 指针，存原变量的地址，用 -> 或 * 访问
// ┌──────────┬──────────────────────────┬──────────────────────────┐
// │          │ 引用 &                    │ 指针 *                   │
// ├──────────┼──────────────────────────┼──────────────────────────┤
// │ 是什么    │ 别名，就是原变量            │ 存地址，不是变量本身          │
// │ 怎么访问  │ 直接写名字                 │ this->m_code（-> 通过指针访问）│
// │ 能否为空  │ 不能，必须绑定一个变量        │ 可以是 nullptr             │
// │ 能否换绑  │ 不能                      │ 可以，重新指向别的变量        │
// └──────────┴──────────────────────────┴──────────────────────────┘
// & 和 * 各自都有两个作用，看位置区分：
//   类型后面 &（double& r）→ 引用；变量前面 &（&close）→ 取地址
//   类型后面 *（double* p）→ 指针；变量前面 *（*p）→ 解引用
// 取地址 & 产生指针；引用 & 内部就是个指针，只是编译器隐藏了 * 和 & 细节
// 为什么函数参数用 & 不用 *：调用直接传变量，函数内直接用，省事
// 为什么 this 是指针：C++ 语法规定 this 就是指针，编译器内部机制，改不了
double avg(const std::vector<double>& v)
{
    if (v.empty()) return 0.0;

    double sum = 0.0;

    /* for (double x : v)：范围 for 循环，逐个取出 v 里的元素赋给 x
       相当于 Python 的 `for x in v:`
       等价于
       for (int i = 0; i < v.size(); i++) {
           double x = v[i];
           sum += x;
       } */

    for (double x : v) {
        sum += x;
    }
    return sum / v.size();
}

// 指针参数：函数内通过 *p 修改调用方的变量
//   调用时必须传地址：update_close(&close, 1600.0)，& 是取地址符
void update_close(double* p, double v)
{
    *p = v;
}

// 引用参数：r 是调用方变量的别名，直接修改
//   调用直接传变量：update_close_ref(close, 1600.0)，不用 & 也不用 *
void update_close_ref(double& r, double v)
{
    r = v;
}

// 值传递参数：r 是副本，函数内修改不影响调用方
//   想拿回修改结果必须靠返回值：double after = update_close_copy(close, 2000.0)
//   对比：引用/指针能改到调用方，值传递改不到
double update_close_copy(double r, double v)
{
    r = v;
    return r;
}

// pair<A, B>：打包两个值的容器
//   .first  → 第一个值
//   .second → 第二个值
//
//   用法示例：
//     pair<string, string> p = {"BTC", "USDT"};
//     p.first;   // "BTC"
//     p.second;  // "USDT"
//
// 返回 pair：函数只能返回一个东西，想返回两个就把它们包成 pair
//
// std::string：可变长度的字符串（需要 #include <string>）
//   s.find('.')         → 找 '.' 的位置，返回索引（从 0 开始）
//   s.find("abc")       → 找子串，找不到返回 std::string::npos
//   std::string::npos   → 特殊值，表示"没找到"
//   s.substr(0, 3)      → 从位置 0 取 3 个字符
//   s.substr(dot + 1)   → 从 dot+1 取到末尾
//
//   示例："BTC.USDT".find('.') → 返回 3（点的位置）
//         "BTC.USDT".substr(0, 3) → "BTC"
//         "BTC.USDT".substr(4)    → "USDT"
std::pair<std::string, std::string> split_symbol(const std::string& symbol)
{
    /* auto：让编译器自动推断类型，不用手写
       size_t dot = symbol.find('.')  ← 简单类型显式写，不用 auto */
    size_t dot = symbol.find('.');
    if (dot == std::string::npos) {
        return {symbol, ""};
    }
    return {symbol.substr(0, dot), symbol.substr(dot + 1)};
}

// std::cout：控制台输出（cout = character output），向终端打印内容（需要 #include <iostream>）
//   std::cout << "文字"   → 打印字符串
//   std::cout << 变量       → 打印变量的值
//   << 可以连续拼接多个内容
//   "\n"                   → 换行符
//   std::endl              → 换行 + 强制刷新，比 "\n" 慢，一般不需要
int main()
{
    std::cout << "========== 第一步：纯 C++ ==========\n\n";

    // ① 整数加法
    std::cout << "① add(3, 5) = " << add(3, 5) << "\n";

    // ② 求均值
    std::vector<double> prices = {15.50, 15.80, 16.10, 16.40};
    std::cout << "② avg({15.50, 15.80, 16.10, 16.40}) = " << avg(prices) << "\n";

    // ③ 拆分股票代码
    /* auto [code, market]：结构化绑定（C++17），把 pair 的两个值拆到两个变量里
       等价于
       auto result = split_symbol(...);
       code = result.first;
       market = result.second; */
    auto [code, market] = split_symbol("600519.SH");
    std::cout << "③ split_symbol(\"600519.SH\") = {\"" << code << "\", \"" << market << "\"}\n";

    // ④ 指针 vs 引用：
    //   double* p = &close   → & 是取地址符，把 close 的地址存进 p
    //   *p = 1600.0          → * 是解引用，通过 p 找到 close 并修改
    //   引用（&）是别名直接用；指针（*）存地址，要 * 或 -> 才能访问
    std::cout << "\n④ 指针 vs 引用:\n";
    double close = 1500.0;
    double* p = &close;
    *p = 1600.0;
    std::cout << "   指针: *p = 1600 后 close = " << close << "\n";
    double& r = close;
    r = 1700.0;
    std::cout << "   引用: r = 1700 后 close = " << close << "\n";
    update_close(&close, 1800.0);
    std::cout << "   指针参数: update_close(&close, 1800) 后 close = " << close << "\n";
    update_close_ref(close, 1900.0);
    std::cout << "   引用参数: update_close_ref(close, 1900) 后 close = " << close << "\n";
    double after = update_close_copy(close, 2000.0);
    std::cout << "   值传递参数: update_close_copy(close, 2000) 返回 " << after
              << "，close 仍 = " << close << "\n";

    std::cout << "\n========== 编译环境正常 ==========\n";
    return 0;
}
