#pragma once
// ============================================================================
// 扩展数据管理器 — 行情辅助数据的内存结构 + 查询接口。
//
// 核心模式: Load(数据) → 内存存储(unordered_map + vector) → Query(cur_ns 过滤)
// 所有 Query 接收 cur_ns 参数, 只返回 <= cur_ns 的数据, 防止未来函数。
// ============================================================================
#include <algorithm>   // std::sort, std::upper_bound
#include <array>       // std::array（用作 map key）
#include <cstdint>     // int64_t
#include <functional>  // std::hash
#include <string>
#include <unordered_map>
#include <vector>

// ---- 定长 symbol key（模拟框架中的 CSymbolType）----

using SymbolKey = std::array<char, 16>;

inline SymbolKey ToSymbolKey(const std::string& s)
{
    SymbolKey k{};
    auto len = std::min(s.size(), k.size() - 1);
    std::copy_n(s.data(), len, k.begin());
    return k;
}

struct SymbolKeyHash {
    size_t operator()(const SymbolKey& k) const noexcept
    {
        // 简单 FNV-1a 哈希
        size_t h = 14695981039346656037ULL;
        for (char c : k) {
            if (c == 0) break;
            h ^= static_cast<size_t>(static_cast<unsigned char>(c));
            h *= 1099511628211ULL;
        }
        return h;
    }
};

// ---- 分红送股 ----

struct FHSGRecord {
    int64_t ex_divi_date_ns;  // 除权除息日（纳秒时间戳）
    double  bonus;            // 送股
    double  trans_add;        // 转增
    double  cash_divi;        // 派息
};

class CFHSGMgr {
public:
    // 批量加载（symbol → 多条记录，自动按日期升序排序）
    void Load(const std::string& symbol, std::vector<FHSGRecord> records);
    // 查询 [start_ns, min(end_ns, cur_ns)] 区间内的分红记录
    std::vector<FHSGRecord> Query(const std::string& symbol,
                                   int64_t start_ns, int64_t end_ns,
                                   int64_t cur_ns) const;
private:
    std::unordered_map<SymbolKey, std::vector<FHSGRecord>, SymbolKeyHash> m_data;
};

// ---- 涨跌停价 ----

struct UpDownLimitRecord {
    int64_t datetime_ns;
    double  up_price;
    double  down_price;
};

class CUpDownLimitMgr {
public:
    void Load(const std::string& symbol, std::vector<UpDownLimitRecord> records);
    // 二分查找: 取 datetime_ns <= cur_ns 的最后一条
    UpDownLimitRecord QueryLatest(const std::string& symbol, int64_t cur_ns) const;
private:
    std::unordered_map<SymbolKey, std::vector<UpDownLimitRecord>, SymbolKeyHash> m_data;
};

// ---- 板块 ----

class CPlateMgr {
public:
    void LoadPlateComponent(const std::string& plate_name, std::vector<std::string> symbols);
    void LoadStockConcept(const std::string& concept_name, std::vector<std::string> symbols);

    std::vector<std::string> GetSectorList() const;
    std::vector<std::string> GetStockListInSector(const std::string& sector) const;
private:
    std::unordered_map<std::string, std::vector<std::string>> m_plate;
    std::unordered_map<std::string, std::vector<std::string>> m_concept;
};

// ---- 证券类型（静态分类，纯函数）----

std::unordered_map<std::string, bool> ClassifyInstrumentType(const std::string& symbol);
