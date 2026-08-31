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

// PYBIND11_MODULE 是宏，展开后是一个函数定义，所以 } 后面不加分号
// m 是 Python 模块对象，所有绑定都通过 m 注册
PYBIND11_MODULE(stock_demo, m)
{
    m.doc() = "第五步：类绑定（声明/实现/绑定三文件分离）";

    // py::class_<Stock>(m, "Stock") → 注册 C++ 的 Stock 类到 Python 模块
    //   <Stock>  = 绑定哪个 C++ 类
    //   m        = 注册到哪个模块
    //   "Stock"  = Python 侧看到的类名（import 后用 stock_demo.Stock）

    // 下面是链式调用：每个 .def() 返回自身，接着调下一个，整个是一条语句，只在最后加分号
    py::class_<Stock>(m, "Stock")

        // py::init<参数类型列表>() = 函数名（__init__）+ 函数地址（按参数类型自动匹配构造函数）
        // py::arg("code") = 给参数起名，Python 侧可 Stock(code="600519.SH", close=1500.0)
        .def(py::init<std::string, double>(), py::arg("code"), py::arg("close"))

        // .def 完整用法：
        //   .def("Python方法名", &C++函数地址, "描述")                  → 无参方法
        //   .def("Python方法名", &C++函数地址, py::arg("参数名"), "描述") → 带参方法
        .def("code", &Stock::code, "股票代码")
        .def("set_close", &Stock::set_close, py::arg("v"), "设置收盘价")
        .def("limit_prices", &Stock::limit_prices, "涨停/跌停价（返回 tuple）")

        // def_property_readonly → 绑定为只读属性（不是方法）
        //   s.close     ✅ 属性访问，不加括号
        //   s.close()   ❌ 报错，不是方法
        //   s.close=100 ❌ 报错，readonly 不能写
        // 对比 .def → 绑定为方法，必须加括号调用：s.code()
        .def_property_readonly("close", &Stock::close_price, "收盘价")

        // __repr__：Python 的 print(s) 或 repr(s) 自动调用
        .def("__repr__", &Stock::repr);

    // // 上面的链式调用等价于拆开写（每行一条语句，每行加分号）：
    // auto cls = py::class_<Stock>(m, "Stock");
    // cls.def(py::init<std::string, double>(), py::arg("code"), py::arg("close"));
    // cls.def("code", &Stock::code, "股票代码");
    // cls.def("set_close", &Stock::set_close, py::arg("v"), "设置收盘价");
    // cls.def("limit_prices", &Stock::limit_prices, "涨停/跌停价（返回 tuple）");
    // cls.def_property_readonly("close", &Stock::close_price, "收盘价");
    // cls.def("__repr__", &Stock::repr);
}
