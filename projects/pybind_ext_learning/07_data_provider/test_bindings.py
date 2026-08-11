# -*- coding: utf-8 -*-
"""第七步：data_provider_ext 验证。"""
import os
import sys

cur = os.path.dirname(os.path.abspath(__file__))
for sub in ["lib", "build_py_vs/Release", "build_py/Release", "build_py"]:
    p = os.path.join(cur, sub)
    if os.path.isdir(p) and p not in sys.path:
        sys.path.insert(0, p)

import data_provider_ext as dp

def date_to_ns(yyyymmdd):
    y, m, d = yyyymmdd // 10000, (yyyymmdd // 100) % 100, yyyymmdd % 100
    days = (y - 1970) * 365 + (y - 1969) // 4 + (m - 1) * 30 + d
    return days * 86400 * 1_000_000_000

mgr = dp.DataProvider()

# 分红送股：get_divid_factors → py::dict → Python dict
print()
mgr.load_fhsg("600519.SH", [
    dp.FHSGRecord(date_to_ns(20230616), 0.0, 0.0, 25.979),
    dp.FHSGRecord(date_to_ns(20240621), 0.0, 0.0, 30.876),
    dp.FHSGRecord(date_to_ns(20250620), 0.0, 0.0, 32.500),
])
result = mgr.get_divid_factors("600519.SH", date_to_ns(20230101), date_to_ns(20251231), date_to_ns(20241231))
n = len(result["cash_divi"])
print(f"分红: {n} 条, 派息={result['cash_divi']}")
print(type(result))

# 涨跌停价：get_up_down_limit → py::dict → Python dict
print()
mgr.load_up_down_limit("000001.SZ", [
    dp.UpDownLimitRecord(date_to_ns(20250101), 15.50, 12.68),
    dp.UpDownLimitRecord(date_to_ns(20250102), 15.80, 12.92),
    dp.UpDownLimitRecord(date_to_ns(20250103), 16.10, 13.18),
])
r = mgr.get_up_down_limit("000001.SZ", date_to_ns(20250103))
print(f"涨跌停: 涨停={r['up_price']}, 跌停={r['down_price']}")
print(type(r))

# 板块：get_sector_list → vector<string> → Python list
print()
mgr.load_plate_component("银行", ["601398.SH", "601939.SH", "600036.SH"])
sectors = mgr.get_sector_list()
print(f"板块列表: {sectors}")
print(type(sectors))

stks = mgr.get_stock_list_in_sector("银行")
print(f"银行成份股: {stks}")
print(type(stks))

# 证券类型：get_instrument_type → unordered_map → Python dict
print()
t1 = mgr.get_instrument_type("600519.SH")
print(f"600519.SH → {t1}")
print(type(t1))

t2 = mgr.get_instrument_type("510300.SH")
print(f"510300.SH → {t2}")
print(type(t2))

print()
print("=== 第七步全部通过 ===")
