// C++20 | 依赖：无
// 第二步：C++ 类的基础 — 学会用 class 封装数据和函数。
//
// 上一步（01_hello_cpp）写的是独立函数，本步把数据 + 操作打包成 class。
// 下一步（03_one_function）才开始把函数暴露给 Python。
#include <iostream>
#include <string>
#include <vector>

#include "stock.h"  // 写法 ①：.h 声明 + .cpp 实现（Stock）
#include "bond.h"   // 写法 ②：全写在 .h 里（Bond）

// 写法 ③：直接定义在当前文件里，不需要 .h（只有本文件用）
class Fund {
    // class 成员默认 private，外部不能直接访问（struct 默认 public）
    std::string m_name;   // m_ 前缀是命名习惯，表示 member（成员变量）
    double      m_nav;    // 净值（NAV = Net Asset Value）

public:  // 以下成员外部可以访问

    /* 构造函数：函数名必须和类名一致，没有返回类型
       Fund(std::string name, double nav)   ← name、nav 是形参，调用时值拷贝传入
           : m_name(...), m_nav(...)        ← 成员初始化列表（: 是固定语法，构造函数专属）
           {}                               ← 函数体为空，初始化列表已完成所有工作

       m_name(std::move(name))  → 用形参 name 初始化成员变量 m_name
         std::move 把字符串"搬"过来（转移内部指针），避免拷贝堆上的字符数组
         string/vector/map 这些管理堆内存的类型用 move 更快
       m_nav(nav)               → 用形参 nav 初始化成员变量 m_nav，等价于 m_nav = nav
         int/double/bool 这些基本类型只有 8 字节，拷贝已经最快，不需要 move */
    Fund(std::string name, double nav)
        : m_name(std::move(name)), m_nav(nav)
    {
    }

    // const 成员函数：承诺不修改任何成员变量，只读操作
    std::string name() const
    {
        return m_name;
    }

    double nav() const
    {
        return m_nav;
    }

    // 非 const 参数 amount 是值传递（拷贝），改了也不影响调用方
    double buy_shares(double amount) const
    {
        return amount / m_nav;
    }
};

int main()
{
    std::cout << "========== 第二步：C++ 类的基础 ==========\n\n";

    // ───── 写法 ① Stock：.h 声明 + .cpp 实现 ─────

    Stock s("600519.SH", 1500.0);
    std::cout << "① 写法 ①  Stock（.h + .cpp 分离）\n";
    std::cout << "   代码: " << s.code()
              << ", 收盘价: " << s.close_price() << "\n";

    s.set_close(1600.0);
    std::cout << "   set_close(1600) 后: " << s.close_price() << "\n";

    auto [up, down] = s.limit_prices();
    std::cout << "   涨停价: " << up << ", 跌停价: " << down << "\n";

    // ───── 写法 ② Bond：全写在 .h 里 ─────

    Bond b("24国债01", 0.025);
    double interest = b.annual_interest(10000.0);
    std::cout << "\n② 写法 ②  Bond（全写在 .h 里）\n";
    std::cout << "   名称: " << b.name()
              << ", 利率: " << b.rate() << "\n";
    std::cout << "   面值 10000 的年利息: " << interest << "\n";

    // ───── 写法 ③ Fund：直接定义在 main.cpp 里 ─────

    Fund f("沪深300ETF", 3.85);
    double shares = f.buy_shares(10000.0);
    std::cout << "\n③ 写法 ③  Fund（直接写在 main.cpp 里）\n";
    std::cout << "   名称: " << f.name()
              << ", 净值: " << f.nav() << "\n";
    std::cout << "   10000 元买入份额: " << shares << "\n";

    // ───── 多个对象 ─────

    std::cout << "\n④ 多个对象:\n";
    std::vector<Stock> stocks;
    stocks.push_back(s);
    stocks.push_back(Stock("000001.SZ", 12.5));
    stocks.push_back(Stock("300750.SZ", 200.0));
    std::cout << "   共 " << stocks.size() << " 只股票\n";

    std::cout << "\n========== 类基础完成 ==========\n";
    return 0;
}
