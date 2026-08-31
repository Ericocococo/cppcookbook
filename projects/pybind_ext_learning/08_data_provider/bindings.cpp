// C++20 | 依赖：pybind11
// pybind11 绑定 — 将 C++ 数据管理器暴露给 Python。
//
// 生成的 Python 模块名: data_provider_ext
// Python 使用示例:
//   import data_provider_ext as dp
//   mgr = dp.DataProvider()
//   mgr.load_fhsg("600519.SH", [...])
//   result = mgr.get_divid_factors("600519.SH", start_ns, end_ns, cur_ns)
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>        // std::vector / std::unordered_map ↔ Python list / dict 自动转换

#include "data_mgrs.h"

namespace py = pybind11;

// 聚合类: 模拟框架中的 CDataProvider，持有各 Mgr 并暴露统一接口
class DataProvider {
    CFHSGMgr m_fhsg;
    CUpDownLimitMgr m_up_down_limit;
    CPlateMgr m_plate;

public:
    // ---- Load ----

    void load_fhsg(const std::string& symbol, const std::vector<FHSGRecord>& records) {
        m_fhsg.Load(symbol, records);
    }

    void load_up_down_limit(const std::string& symbol, const std::vector<UpDownLimitRecord>& records) {
        m_up_down_limit.Load(symbol, records);
    }

    void load_plate_component(const std::string& name, const std::vector<std::string>& symbols) {
        m_plate.LoadPlateComponent(name, symbols);
    }

    void load_stock_concept(const std::string& name, const std::vector<std::string>& symbols) {
        m_plate.LoadStockConcept(name, symbols);
    }

    // ---- Query ----

    // 返回 dict of lists，Python 侧直接 pd.DataFrame(result)
    py::dict get_divid_factors(
        const std::string& symbol,
        int64_t start_ns,
        int64_t end_ns,
        int64_t cur_ns) const {
        auto records = m_fhsg.Query(symbol, start_ns, end_ns, cur_ns);
        py::list ex_dates, bonuses, trans_adds, cash_divis;
        for (const auto& r : records) {
            ex_dates.append(r.ex_divi_date_ns);
            bonuses.append(r.bonus);
            trans_adds.append(r.trans_add);
            cash_divis.append(r.cash_divi);
        }
        py::dict result;
        result["ex_divi_date"] = ex_dates;
        result["bonus"] = bonuses;
        result["trans_add"] = trans_adds;
        result["cash_divi"] = cash_divis;
        return result;
    }

    py::dict get_up_down_limit(const std::string& symbol, int64_t cur_ns) const {
        auto r = m_up_down_limit.QueryLatest(symbol, cur_ns);
        py::dict result;
        result["up_price"] = r.up_price;
        result["down_price"] = r.down_price;
        return result;
    }

    std::vector<std::string> get_sector_list() const {
        return m_plate.GetSectorList();
    }

    std::vector<std::string> get_stock_list_in_sector(const std::string& sector) const {
        return m_plate.GetStockListInSector(sector);
    }

    std::unordered_map<std::string, bool> get_instrument_type(const std::string& symbol) const {
        return ClassifyInstrumentType(symbol);
    }
};

PYBIND11_MODULE(data_provider_ext, m) {
    m.doc() = "扩展数据提供者 — C++ 数据管理器的 Python 绑定";

    // FHSGRecord: Python 侧可构造并传入
    py::class_<FHSGRecord>(m, "FHSGRecord")
        .def(
            py::init<int64_t, double, double, double>(),
            py::arg("ex_divi_date_ns"),
            py::arg("bonus"),
            py::arg("trans_add"),
            py::arg("cash_divi"))
        .def_readwrite("ex_divi_date_ns", &FHSGRecord::ex_divi_date_ns)
        .def_readwrite("bonus", &FHSGRecord::bonus)
        .def_readwrite("trans_add", &FHSGRecord::trans_add)
        .def_readwrite("cash_divi", &FHSGRecord::cash_divi);

    // UpDownLimitRecord
    py::class_<UpDownLimitRecord>(m, "UpDownLimitRecord")
        .def(
            py::init<int64_t, double, double>(),
            py::arg("datetime_ns"),
            py::arg("up_price"),
            py::arg("down_price"))
        .def_readwrite("datetime_ns", &UpDownLimitRecord::datetime_ns)
        .def_readwrite("up_price", &UpDownLimitRecord::up_price)
        .def_readwrite("down_price", &UpDownLimitRecord::down_price);

    // DataProvider 聚合类
    py::class_<DataProvider>(m, "DataProvider")
        .def(py::init<>())
        // Load
        .def(
            "load_fhsg",
            &DataProvider::load_fhsg,
            py::arg("symbol"),
            py::arg("records"))
        .def(
            "load_up_down_limit",
            &DataProvider::load_up_down_limit,
            py::arg("symbol"),
            py::arg("records"))
        .def(
            "load_plate_component",
            &DataProvider::load_plate_component,
            py::arg("name"),
            py::arg("symbols"))
        .def(
            "load_stock_concept",
            &DataProvider::load_stock_concept,
            py::arg("name"),
            py::arg("symbols"))
        // Query
        .def(
            "get_divid_factors",
            &DataProvider::get_divid_factors,
            py::arg("symbol"),
            py::arg("start_ns"),
            py::arg("end_ns"),
            py::arg("cur_ns"))
        .def(
            "get_up_down_limit",
            &DataProvider::get_up_down_limit,
            py::arg("symbol"),
            py::arg("cur_ns"))
        .def("get_sector_list", &DataProvider::get_sector_list)
        .def(
            "get_stock_list_in_sector",
            &DataProvider::get_stock_list_in_sector,
            py::arg("sector"))
        .def(
            "get_instrument_type",
            &DataProvider::get_instrument_type,
            py::arg("symbol"));
}
