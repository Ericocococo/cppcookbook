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

/* const 的几种用法（本项目出现的全部 6 种）：

   ① 参数前 const 引用 — 不拷贝、不修改传入的数据（最常用）
      double avg(const std::vector<double>& v)

   ② 函数末尾 const — 函数不修改对象的成员变量
      std::string code() const

   ③ for 循环 const auto& 遍历 — 不修改元素、不拷贝（遍历惯用法）
      for (const auto& r : records)

   ④ const 局部变量 — 变量值不可改
      const int64_t rows = table->num_rows();

   ⑤ lambda 参数 — lambda 内部不修改捕获的参数
      [](const X& a, const X& b) { return a.x < b.x; }

   ⑥ const + noexcept — 哈希函数：参数不改 + 对象不改
      size_t operator()(const Key& k) const noexcept
        noexcept = 承诺绝不抛异常，编译器可放心优化；违背承诺程序直接终止
*/
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

    std::cout << "\n========== 编译环境正常 ==========\n";
    return 0;
}
