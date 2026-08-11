// 单个数据管理器实现 — Load 存内存 + Query 线性扫描。
#include "fhsg_mgr.h"

#include <algorithm>
#include <set>

void CFHSGMgr::Load(const std::vector<FHSGRecord>& records)
{
    for (const auto& r : records) {
        m_data[r.symbol].push_back(r);
    }
    // 按除权日升序排列（虽然本步只做线性扫描，排序是好习惯）
    for (auto& [sym, vec] : m_data) {
        std::sort(vec.begin(), vec.end(),
                  [](const FHSGRecord& a, const FHSGRecord& b) {
                      return a.ex_divi_date_ns < b.ex_divi_date_ns;
                  });
    }
}

std::vector<FHSGRecord> CFHSGMgr::Query(const std::string& symbol) const
{
    auto it = m_data.find(symbol);
    if (it == m_data.end()) return {};
    return it->second;
}

std::vector<std::string> CFHSGMgr::GetSymbols() const
{
    std::vector<std::string> out;
    out.reserve(m_data.size());
    for (const auto& [sym, _] : m_data) {
        out.push_back(sym);
    }
    return out;
}
