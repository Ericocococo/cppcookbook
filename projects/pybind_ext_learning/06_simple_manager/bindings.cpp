// pybind11 绑定 — 把 CFHSGMgr 暴露给 Python。
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fhsg_mgr.h"

namespace py = pybind11;

PYBIND11_MODULE(fhsg_demo, m) {
    m.doc() = "第六步：单个数据管理器（Load + Query）";

    py::class_<FHSGRecord>(m, "FHSGRecord")
        .def(py::init<>())
        .def_readwrite("symbol", &FHSGRecord::symbol)
        .def_readwrite("ex_divi_date_ns", &FHSGRecord::ex_divi_date_ns)
        .def_readwrite("bonus", &FHSGRecord::bonus)
        .def_readwrite("cash_divi", &FHSGRecord::cash_divi)
        .def("__repr__", [](const FHSGRecord& r) {
            return "<FHSG " + r.symbol + " cash_divi=" + std::to_string(r.cash_divi) + ">";
        });

    py::class_<CFHSGMgr>(m, "CFHSGMgr")
        .def(py::init<>())
        .def("load", &CFHSGMgr::Load, py::arg("records"))
        .def("query", &CFHSGMgr::Query, py::arg("symbol"))
        .def("get_symbols", &CFHSGMgr::GetSymbols)
        .def("size", &CFHSGMgr::Size);
}
