// C++20 | 依赖：无
// Stock 类实现 — 纯 C++，不涉及 pybind11。
#include "stock.h"

/* 构造函数：创建对象时自动调用
   Stock::Stock        → 类名::构造函数名，:: 是作用域运算符，表示"Stock 类的 Stock 函数"
   : m_code(...)        → 初始化列表，在构造时直接初始化成员变量（比在 {} 里赋值快）
   std::move(code)      → 转移所有权，把 code 的字符串数据"搬"过来而不是拷贝一份
   {}                   → 函数体为空，所有工作已在初始化列表完成 */
Stock::Stock(std::string code, double close)
    : m_code(std::move(code)), m_close(close)
{
}

// 返回股票代码
std::string Stock::code() const
{
    return m_code;
}

// 返回收盘价
double Stock::close_price() const
{
    return m_close;
}

// 更新收盘价
void Stock::set_close(double v)
{
    m_close = v;
}

// 涨停/跌停价（简化 ±10%），返回 pair<涨停, 跌停>
std::pair<double, double> Stock::limit_prices() const
{
    return {m_close * 1.1, m_close * 0.9};
}

// Python __repr__ 的 C++ 实现
std::string Stock::repr() const
{
    return "<Stock " + m_code + " close=" + std::to_string(m_close) + ">";
}
