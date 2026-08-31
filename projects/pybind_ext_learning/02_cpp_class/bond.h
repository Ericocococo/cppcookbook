// C++20 | 依赖：无
// 写法 ②：声明 + 实现全写在 .h 里（类内定义，自动 inline）
// 适用场景：模板类必须这样写；小类也常用，省掉 .cpp 文件
#pragma once

#include <string>

class Bond
{
    std::string m_name; // 债券名称
    double m_rate; // 票面利率（如 0.035 = 3.5%）

public:
    Bond(std::string name, double rate)
        : m_name(std::move(name)), m_rate(rate)
    {
    }

    std::string name() const
    {
        return m_name;
    }

    double rate() const
    {
        return m_rate;
    }

    // 年利息 = 面值 × 利率
    double annual_interest(double face_value) const
    {
        return face_value * m_rate;
    }
};
