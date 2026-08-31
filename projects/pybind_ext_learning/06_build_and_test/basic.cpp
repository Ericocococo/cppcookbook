// C++20 | 依赖：pybind11
// 第六步：pybind11 入门 — 最简单的类 + 函数绑定。
//
// 演示:
//   - 普通函数绑定（int / double / string 参数）
//   - 类绑定（成员方法 / 属性）
//   - STL 容器绑定（vector / map 自动转换）
//
// 编译后的 Python 模块名: basic_binding
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>   // std::vector / std::map ↔ Python list / dict 自动转换

namespace py = pybind11;

// ---- 1. 普通函数 ----

int add(int a, int b)
{
    return a + b;
}

double avg(const std::vector<double>& v)
{
    if (v.empty()) return 0.0;

    double sum = 0.0;
    for (double x : v)
    {
        sum += x;
    }
    return sum / v.size();
}

std::map<std::string, int> count_letters(const std::string& s)
{
    std::map<std::string, int> out;
    for (char c : s)
    {
        out[std::string(1, c)]++;
    }
    return out;
}

// ---- 2. 类绑定 ----

class Stock
{
    std::string m_code;
    double m_close;

public:
    Stock(std::string code, double close)
        : m_code(std::move(code)), m_close(close)
    {
    }

    std::string code() const
    {
        return m_code;
    }

    double close() const
    {
        return m_close;
    }

    void set_close(double v)
    {
        m_close = v;
    }

    // 模拟涨跌停价
    std::pair<double, double> limit_prices() const
    {
        return {m_close * 1.1, m_close * 0.9}; // 涨停 / 跌停（简化 ±10%）
    }
};

// ---- 绑定入口 ----

PYBIND11_MODULE(basic_binding, m)
{
    m.doc() = "第六步：pybind11 入门 — 最简单的类 + 函数绑定";

    // 普通函数
    m.def("add", &add, py::arg("a"), py::arg("b"), "加法");
    m.def("avg", &avg, py::arg("v"), "求均值");
    m.def("count_letters", &count_letters, py::arg("s"), "统计字母出现次数");

    // 类（构造 + 成员方法 + 读写属性）
    py::class_<Stock>(m, "Stock")
        .def(py::init<std::string, double>(), py::arg("code"), py::arg("close"))
        .def("code", &Stock::code)
        .def("set_close", &Stock::set_close, py::arg("v"))
        .def_property_readonly("close", &Stock::close, "收盘价（只读）")
        .def("limit_prices", &Stock::limit_prices, "涨停/跌停价（返回 tuple）");
}
