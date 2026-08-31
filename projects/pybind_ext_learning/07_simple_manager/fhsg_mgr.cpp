// ============================================================================
// CFHSGMgr 实现 — Load 存入内存 + Query 线性扫描。
// ============================================================================
#include "fhsg_mgr.h"

#include <algorithm>
#include <set>

// 加载分红记录：按 symbol 分组存入 map，每组内部按除权日升序排序
void CFHSGMgr::Load(const std::vector<FHSGRecord>& records) {
    for (const auto& r : records) {
        m_data[r.symbol].push_back(r);
    }
    for (auto& [sym, vec] : m_data) {
        std::sort(
            vec.begin(),
            vec.end(),
            [](const FHSGRecord& a, const FHSGRecord& b) {
                return a.ex_divi_date_ns < b.ex_divi_date_ns;
            });
    }
}

// 按 symbol 查询该标的的全部分红记录，找不到返回空 vector
std::vector<FHSGRecord> CFHSGMgr::Query(const std::string& symbol) const {
    auto it = m_data.find(symbol);
    if (it == m_data.end()) {
        return {};
    }
    return it->second;
}

// 返回所有已加载标的列表
std::vector<std::string> CFHSGMgr::GetSymbols() const {
    std::vector<std::string> out;
    out.reserve(m_data.size());
    for (const auto& [sym, _] : m_data) {
        out.push_back(sym);
    }
    return out;
}
