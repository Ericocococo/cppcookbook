// C++20 | 依赖：无
// Stock 类声明 — 纯 C++ 头文件，不涉及 pybind11。
//
// 本步的核心变化：类的声明（.h）和实现（.cpp）分离，绑定（bindings.cpp）单独一个文件。
// 这是 C++ 多文件项目的基本实践，也是后续步骤的基础。
#pragma once

#include <string>
#include <utility>

class Stock {
    std::string m_code;
    double      m_close;

public:
    Stock(std::string code, double close);

    std::string code() const;
    double      close_price() const;
    void        set_close(double v);

    // 涨停/跌停价（简化 ±10%）
    std::pair<double, double> limit_prices() const;

    // Python __repr__
    std::string repr() const;
};
