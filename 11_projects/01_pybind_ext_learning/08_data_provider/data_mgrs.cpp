// ============================================================================
// 扩展数据管理器实现 — Load（内存存储）+ Query（时间过滤 / 二分查找）。
// ============================================================================
#include "data_mgrs.h"

#include <cctype>   // toupper
#include <set>

// ========================== CFHSGMgr ==========================

// 加载分红送股记录：按 symbol 存储，内部按除权日升序排序
void CFHSGMgr::Load(const std::string& symbol, std::vector<FHSGRecord> records) {
    std::sort(
        records.begin(),
        records.end(),
        [](const FHSGRecord& a, const FHSGRecord& b) {
            return a.ex_divi_date_ns < b.ex_divi_date_ns;
        });
    m_data[ToSymbolKey(symbol)] = std::move(records);
}

// 查询 [start_ns, min(end_ns, cur_ns)] 区间内的分红记录
std::vector<FHSGRecord> CFHSGMgr::Query(
    const std::string& symbol,
    int64_t start_ns,
    int64_t end_ns,
    int64_t cur_ns) const {
    auto it = m_data.find(ToSymbolKey(symbol));
    if (it == m_data.end()) {
        return {};
    }
    // cur_ns 截止: 防未来函数，只返回 <= cur_ns 的数据
    int64_t actual_end = std::min(end_ns, cur_ns);
    std::vector<FHSGRecord> out;
    for (const auto& r : it->second) {
        if (r.ex_divi_date_ns >= start_ns && r.ex_divi_date_ns <= actual_end) {
            out.push_back(r);
        }
    }
    return out;
}

// ========================== CUpDownLimitMgr ==========================

// 加载涨跌停价记录：按 symbol 存储，内部按时间升序排序
void CUpDownLimitMgr::Load(const std::string& symbol, std::vector<UpDownLimitRecord> records) {
    std::sort(
        records.begin(),
        records.end(),
        [](const UpDownLimitRecord& a, const UpDownLimitRecord& b) {
            return a.datetime_ns < b.datetime_ns;
        });
    m_data[ToSymbolKey(symbol)] = std::move(records);
}

// 查最新一条记录：二分查找取 datetime_ns <= cur_ns 的最后一条
UpDownLimitRecord CUpDownLimitMgr::QueryLatest(const std::string& symbol, int64_t cur_ns) const {
    auto it = m_data.find(ToSymbolKey(symbol));
    if (it == m_data.end()) {
        return {0, 0.0, 0.0};
    }
    const auto& vec = it->second;
    auto pos = std::upper_bound(
        vec.begin(),
        vec.end(),
        cur_ns,
        [](int64_t ns, const UpDownLimitRecord& r) {
            return ns < r.datetime_ns;
        });
    if (pos == vec.begin()) {
        return {0, 0.0, 0.0}; // 所有记录都在 cur_ns 之后，无有效数据
    }
    --pos;
    return *pos;
}

// ========================== CPlateMgr ==========================

// 加载行业板块成份股
void CPlateMgr::LoadPlateComponent(const std::string& plate_name, std::vector<std::string> symbols) {
    m_plate[plate_name] = std::move(symbols);
}

// 加载概念板块成份股
void CPlateMgr::LoadStockConcept(const std::string& concept_name, std::vector<std::string> symbols) {
    m_concept[concept_name] = std::move(symbols);
}

// 合并行业板块 + 概念板块名称，去重排序后返回
std::vector<std::string> CPlateMgr::GetSectorList() const {
    std::set<std::string> names;
    for (const auto& kv : m_plate) {
        names.insert(kv.first);
    }
    for (const auto& kv : m_concept) {
        names.insert(kv.first);
    }
    return {names.begin(), names.end()};
}

// 查询板块成份股：先查行业板块，未命中再查概念板块
std::vector<std::string> CPlateMgr::GetStockListInSector(const std::string& sector) const {
    auto it = m_plate.find(sector);
    if (it != m_plate.end()) {
        return it->second;
    }
    it = m_concept.find(sector);
    if (it != m_concept.end()) {
        return it->second;
    }
    return {};
}

// ========================== ClassifyInstrumentType ==========================

// 按 symbol 前缀规则判断证券类型（stock / fund / etf / index），返回类型→bool 的 map
std::unordered_map<std::string, bool> ClassifyInstrumentType(const std::string& symbol) {
    size_t dot = symbol.find('.');
    if (dot == std::string::npos) {
        return {};
    }
    std::string code = symbol.substr(0, dot);
    std::string mkt = symbol.substr(dot + 1);
    for (auto& c : mkt) {
        c = static_cast<char>(toupper(c));
    }

    std::unordered_map<std::string, bool> types;
    auto starts = [&](const std::vector<std::string>& prefixes) {
        for (const auto& p : prefixes) {
            if (code.compare(0, p.size(), p) == 0)
                return true;
        }
        return false;
    };

    if (mkt == "SH") {
        if (starts({"600", "601", "603", "605", "688", "689"}))
            types["stock"] = true;
        else if (starts({"510", "511", "512", "513", "515", "516"}))
            types["fund"] = types["etf"] = true;
        else if (starts({"000"}))
            types["index"] = true;
    } else if (mkt == "SZ") {
        if (starts({"000", "001", "002", "003", "300", "301"}))
            types["stock"] = true;
        else if (starts({"159"}))
            types["fund"] = types["etf"] = true;
        else if (starts({"399"}))
            types["index"] = true;
    } else if (mkt == "BJ") {
        if (starts({"43", "83", "87", "88", "92"}))
            types["stock"] = true;
    }
    return types;
}