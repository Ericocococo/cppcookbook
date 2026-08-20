// C++20 | 依赖：无
/* .h 头文件的作用：
   声明"有什么"，不写"怎么做"。
   不写 .h、全部塞一个 .cpp 也能编译运行，但有了 .h 后：

   ① 别的 .cpp 也能用 → include 一下就能调用这些函数
   ② 改实现不用重编译全部 → 只改 .cpp 时，其他文件不用重新编译
   ③ 接口和实现分离 → 看 .h 就知道能调什么，不需要翻 .cpp

   类比：头文件 = 菜单（列出菜名），.cpp = 厨房（实际做菜）

   #pragma once：防止头文件被重复 include，每个 .h 文件第一行标配 */
/* #include：把指定头文件的内容复制进来，相当于"引入工具箱"
   <string>  → 提供 std::string 字符串类型
   <utility> → 提供 std::pair 打包两个值
   <vector>  → 提供 std::vector 动态数组 */
#pragma once

#include <string>
#include <utility>  // pair
#include <vector>

// 两个整数相加
int add(int a, int b);

// 求 vector<double> 的均值
double avg(const std::vector<double>& v);

// 按 '.' 拆分股票代码，返回 {代码, 市场}
std::pair<std::string, std::string> split_symbol(const std::string& symbol);
