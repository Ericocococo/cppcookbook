// C++20 | 依赖：无
// Stock 类实现 — 纯 C++，不涉及 pybind11。
#include "stock.h"

Stock::Stock(std::string code, double close) : m_code(std::move(code)), m_close(close) {}

std::string Stock::code() const { return m_code; }
double      Stock::close_price() const { return m_close; }
void        Stock::set_close(double v) { m_close = v; }

std::pair<double, double> Stock::limit_prices() const {
    return {m_close * 1.1, m_close * 0.9};
}

std::string Stock::repr() const {
    return "<Stock " + m_code + " close=" + std::to_string(m_close) + ">";
}
