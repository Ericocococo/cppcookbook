// C++20 | 依赖：无
// 第二步：C++ 类的基础 — 学会用 class 封装数据和函数。
//
// 上一步（01_hello_cpp）写的是独立函数，本步把数据 + 操作打包成 class。
// 下一步（03_one_function）才开始把函数暴露给 Python。
#include <iostream>
#include <string>
#include <vector>

#include "stock.h"

int main()
{
    std::cout << "========== 第二步：C++ 类的基础 ==========\n\n";

    // ① 创建对象（实例化）
    Stock s("600519.SH", 1500.0);

    // ② 调用成员函数（获取数据）
    std::cout << "① 股票代码: " << s.code() << "\n";
    std::cout << "   收盘价: " << s.close_price() << "\n";

    // ③ 不能直接访问 private 成员
    // s.m_code = "xxx";   // ✗ 编译错误：m_code 是 private

    // ④ 修改数据
    s.set_close(1600.0);
    std::cout << "\n② set_close(1600) 后: " << s.close_price() << "\n";

    // ⑤ 计算涨跌停价（返回 pair）
    auto [up, down] = s.limit_prices();
    std::cout << "\n③ 涨停价: " << up << ", 跌停价: " << down << "\n";

    // ⑥ 创建多个对象
    std::cout << "\n④ 多个对象:\n";
    Stock t("000001.SZ", 12.5);
    std::cout << "   " << t.code() << " close=" << t.close_price() << "\n";

    std::vector<Stock> stocks;
    stocks.push_back(s);
    stocks.push_back(t);
    stocks.push_back(Stock("300750.SZ", 200.0));
    std::cout << "   共 " << stocks.size() << " 只股票\n";

    std::cout << "\n========== 类基础完成 ==========\n";
    return 0;
}
