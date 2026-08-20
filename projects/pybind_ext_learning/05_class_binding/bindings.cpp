// C++20 | 依赖：pybind11
// pybind11 绑定 — 只负责把 Stock 类暴露给 Python，不包含业务逻辑。
//
// 三文件分工:
//   stock.h       → 类声明（纯 C++，不依赖 pybind11）
//   stock.cpp     → 类实现（纯 C++）
//   bindings.cpp  → pybind11 绑定（本文件，唯一引用 pybind11 的地方）
//
// 好处: stock.h/cpp 可以被其他 C++ 项目复用，不强制依赖 pybind11。
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "stock.h"

namespace py = pybind11;

PYBIND11_MODULE(stock_demo, m) {
    m.doc() = "第五步：类绑定（声明/实现/绑定三文件分离）";

    py::class_<Stock>(m, "Stock")
        .def(py::init<std::string, double>(), py::arg("code"), py::arg("close"))
        .def("code", &Stock::code, "股票代码")
        .def("set_close", &Stock::set_close, py::arg("v"), "设置收盘价")
        .def("limit_prices", &Stock::limit_prices, "涨停/跌停价（返回 tuple）")
        .def_property_readonly("close", &Stock::close_price, "收盘价")
        .def("__repr__", &Stock::repr);
}
