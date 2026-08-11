#pragma once
// 单个数据管理器 — 分红送股（CFHSGMgr）。
//
// 本步只有一个 Mgr，只用线性扫描查询（不做二分查找，下一步才加）。
// 目的：先学会"Load 存内存 → Query 从内存查"的基本模式。
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

struct FHSGRecord {
    std::string symbol;
    int64_t     ex_divi_date_ns;  // 除权除息日（纳秒时间戳）
    double      bonus;            // 送股
    double      cash_divi;        // 派息
};

class CFHSGMgr {
public:
    // 加载：把一批记录存入内存
    void Load(const std::vector<FHSGRecord>& records);

    // 查询：按 symbol 过滤，返回该标的的全部分红记录（线性扫描）
    std::vector<FHSGRecord> Query(const std::string& symbol) const;

    // 全部标的列表
    std::vector<std::string> GetSymbols() const;

    size_t Size() const { return m_data.size(); }

private:
    // symbol → 该标的的分红记录列表
    std::unordered_map<std::string, std::vector<FHSGRecord>> m_data;
};
