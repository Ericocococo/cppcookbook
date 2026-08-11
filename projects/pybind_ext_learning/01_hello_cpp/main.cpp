// C++20 | 依赖：无
// 第一步：纯 C++ — 确认编译环境可用，和 Python 没有任何关系。
//
// 写一个最简单的 C++ 函数，编译运行，确认工具链正常。
#include <iostream>
#include <string>
#include <vector>

// 一个最简单的 C++ 函数
int add(int a, int b)
{
    return a + b;
}

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
       auto dot = symbol.find('.')  等价于  size_t dot = symbol.find('.') */
    auto dot = symbol.find('.');
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
