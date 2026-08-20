// C++20 | 依赖：无
// 扩展数据提供者最小实现 — 演示量化回测引擎中"行情辅助数据下沉 C++"的核心模式。
//
// 核心模式: Load(数据) → 内存存储(unordered_map<SymbolKey, vector>) → Query(cur_ns 过滤)
// 所有 Query 接收 cur_ns（当前 bar 的纳秒时间戳），只返回 <= cur_ns 的数据，防止未来函数。
#include "data_mgrs.h"

#include <cstdint>
#include <iostream>
#include <string>

// 纳秒时间戳辅助：YYYYMMDD → 近似 ns（仅用于 demo，不处理时区）
static int64_t date_to_ns(int yyyymmdd)
{
    int y = yyyymmdd / 10000;
    int m = (yyyymmdd / 100) % 100;
    int d = yyyymmdd % 100;
    // 简化公式：从 1970-01-01 起算天数 * 86400 * 1e9
    // 此处不追求精确，只要保证日期之间的大小关系正确
    int days = (y - 1970) * 365 + (y - 1969) / 4 + (m - 1) * 30 + d;
    int64_t ns = static_cast<int64_t>(days) * 86400LL * 1000000000LL;
    return ns;
}

// ---- demo01: 分红送股查询（区间过滤 + cur_ns 防未来函数）----
void demo01_fhsg_query()
{
    std::cout << "① 分红送股查询（CFHSGMgr）\n";

    CFHSGMgr mgr;
    // 加载茅台 2023~2025 三年的分红记录
    mgr.Load("600519.SH", {
        {date_to_ns(20230616), 0.0, 0.0, 25.979},  // 2023 年中期
        {date_to_ns(20240621), 0.0, 0.0, 30.876},  // 2024 年中期
        {date_to_ns(20250620), 0.0, 0.0, 32.500},  // 2025 年中期
    });

    // 场景 A: 当前 bar 在 2024-12-31，查 2023~2025 全部
    // 预期: 只返回 2023 和 2024 两条（2025 的被 cur_ns 截止）
    auto result = mgr.Query("600519.SH",
                            date_to_ns(20230101),
                            date_to_ns(20251231),
                            date_to_ns(20241231));  // ← cur_ns
    std::cout << "  cur_ns=2024-12-31, 查到 " << result.size() << " 条（预期 2）\n";
    for (const auto& r : result) {
        std::cout << "    派息=" << r.cash_divi << "\n";
    }

    // 场景 B: 当前 bar 推进到 2025-12-31
    // 预期: 返回全部 3 条
    auto result2 = mgr.Query("600519.SH",
                             date_to_ns(20230101),
                             date_to_ns(20251231),
                             date_to_ns(20251231));
    std::cout << "  cur_ns=2025-12-31, 查到 " << result2.size() << " 条（预期 3）\n";

    // 场景 C: 查不存在的标的
    auto result3 = mgr.Query("999999.SH", 0, INT64_MAX, INT64_MAX);
    std::cout << "  不存在的标的: " << result3.size() << " 条（预期 0）\n\n";
}

// ---- demo02: 涨跌停价查询（二分查找 QueryLatest）----
void demo02_up_down_limit()
{
    std::cout << "② 涨跌停价查询（CUpDownLimitMgr — 二分查找）\n";

    CUpDownLimitMgr mgr;
    // 加载平安银行连续 5 天的涨跌停价
    mgr.Load("000001.SZ", {
        {date_to_ns(20250101), 15.50, 12.68},
        {date_to_ns(20250102), 15.80, 12.92},
        {date_to_ns(20250103), 16.10, 13.18},
        {date_to_ns(20250106), 16.40, 13.42},
        {date_to_ns(20250107), 16.70, 13.66},
    });

    // 查 1/3 当天的涨跌停（QueryLatest: 取 <= cur_ns 的最后一条）
    auto r = mgr.QueryLatest("000001.SZ", date_to_ns(20250103));
    std::cout << "  cur_ns=2025-01-03: 涨停=" << r.up_price << " 跌停=" << r.down_price << "\n";

    // 查 1/4（周六，数据中无此日）→ 应回退到 1/3
    auto r2 = mgr.QueryLatest("000001.SZ", date_to_ns(20250104));
    std::cout << "  cur_ns=2025-01-04(非交易日): 涨停=" << r2.up_price << "（回退到 1/3 的值）\n";

    // 查极早日期 → 无数据
    auto r3 = mgr.QueryLatest("000001.SZ", date_to_ns(20200101));
    std::cout << "  cur_ns=2020-01-01(无数据): 涨停=" << r3.up_price << "（预期 0）\n\n";
}

// ---- demo03: 板块查询 ----
void demo03_sector()
{
    std::cout << "③ 板块查询（CPlateMgr）\n";

    CPlateMgr mgr;
    mgr.LoadPlateComponent("银行", {"601398.SH", "601939.SH", "601288.SH", "600036.SH"});
    mgr.LoadPlateComponent("保险", {"601318.SH", "601628.SH", "601601.SH"});
    mgr.LoadStockConcept("ChatGPT", {"300418.SZ", "002230.SZ"});

    // 板块列表（行业 + 概念合并去重）
    auto sectors = mgr.GetSectorList();
    std::cout << "  板块总数: " << sectors.size() << " → ";
    for (const auto& s : sectors) {
        std::cout << s << " ";
    }
    std::cout << "\n";

    // 查成份股（先查行业板块，未命中再查概念）
    auto stks = mgr.GetStockListInSector("银行");
    std::cout << "  银行成份股: " << stks.size() << " 只 →";
    for (const auto& s : stks) {
        std::cout << " " << s;
    }
    std::cout << "\n";

    // 不存在的板块
    auto empty = mgr.GetStockListInSector("不存在的板块");
    std::cout << "  不存在的板块: " << empty.size() << " 只（预期 0）\n\n";
}

// ---- demo04: 证券类型（静态分类）----
void demo04_instrument_type()
{
    std::cout << "④ 证券类型（ClassifyInstrumentType — 纯函数）\n";

    struct TestCase { std::string symbol; std::string expected; };
    std::vector<TestCase> cases = {
        {"600519.SH", "stock"}, {"000001.SZ", "stock"}, {"688981.SH", "stock"},
        {"510300.SH", "etf"},   {"159915.SZ", "etf"},
        {"000300.SH", "index"}, {"399001.SZ", "index"},
        {"430047.BJ", "stock"},
    };
    for (const auto& tc : cases) {
        auto types = ClassifyInstrumentType(tc.symbol);
        bool ok = types.count(tc.expected) > 0;
        std::string mark = ok ? " ✓" : " ✗";
        std::cout << "  " << tc.symbol << " → " << tc.expected << mark << "\n";
    }

    // 无法识别
    auto unknown = ClassifyInstrumentType("AAPL");
    std::cout << "  AAPL → 空(" << unknown.size() << ")\n";
}

int main()
{
    std::cout << "========== 扩展数据提供者最小实现 ==========\n\n";
    demo01_fhsg_query();
    demo02_up_down_limit();
    demo03_sector();
    demo04_instrument_type();
    std::cout << "\n========== 全部完成 ==========\n";
    return 0;
}
