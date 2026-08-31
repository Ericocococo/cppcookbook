// pybind11 绑定 — 把 CFHSGMgr 暴露给 Python。
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fhsg_mgr.h"

namespace py = pybind11;

PYBIND11_MODULE(fhsg_demo, m) {
    m.doc() = "第七步：单个数据管理器（Load + Query）";

    // py::init<>()         → 无参构造，编译器自动生成的 FHSGRecord()
    // py::init<T1, T2>()  → 有参构造，如 Stock(std::string, double)
    py::class_<FHSGRecord>(m, "FHSGRecord")
        .def(py::init<>())
        // .def_readwrite       → 可读写属性，直接映射成员变量
        // .def_readonly         → 只读属性，成员变量只能读
        // .def_property_readonly → 只读属性，通过函数返回值
        .def_readwrite("symbol", &FHSGRecord::symbol)
        .def_readwrite("ex_divi_date_ns", &FHSGRecord::ex_divi_date_ns)
        .def_readwrite("bonus", &FHSGRecord::bonus)
        .def_readwrite("cash_divi", &FHSGRecord::cash_divi)
        // lambda 语法: [捕获列表](参数) { 函数体 }
        //   []      不捕获外部变量
        //   [&]     按引用捕获所有外部变量
        //   [=]     按值拷贝捕获所有外部变量
        //   [x]     按值捕获 x
        //   [&x]    按引用捕获 x
        // 这里 [] 空捕获，因为只用到了参数 r，不需要外部变量 TODO:lambda
        .def(
            "__repr__",
            [](const FHSGRecord& r) {
                return "<FHSG " + r.symbol + " cash_divi=" + std::to_string(r.cash_divi) + ">";
            });

    py::class_<CFHSGMgr>(m, "CFHSGMgr")
        .def(py::init<>())
        .def("load", &CFHSGMgr::Load, py::arg("records"))
        .def("query", &CFHSGMgr::Query, py::arg("symbol"))
        .def("get_symbols", &CFHSGMgr::GetSymbols)
        .def("size", &CFHSGMgr::Size);
}
