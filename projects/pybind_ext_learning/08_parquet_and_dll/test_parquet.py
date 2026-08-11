# -*- coding: utf-8 -*-
"""第八步：parquet_reader 验证。"""
import os
import sys

cur = os.path.dirname(os.path.abspath(__file__))
for sub in ["lib", cur, "build_py/Release"]:
    p = os.path.join(cur, sub) if sub != cur else cur
    if os.path.isdir(p) and p not in sys.path:
        sys.path.insert(0, p)

import pandas as pd
import parquet_reader as pr

# read_parquet：py::dict → Python dict
print()
result = pr.read_parquet("test_fhsg.parquet")
print(f"列名: {list(result.keys())}")
print(type(result))

print()
df = pd.DataFrame(result)
df["ex_divi_date"] = pd.to_datetime(df["ex_divi_date"])
df["cash_divi"] = df["cash_divi"].astype(float)
print(df.to_string(index=False))
print(type(df))

print()
moutai = df[df["symbol"] == "600519.SH"]
print(f"茅台 {len(moutai)} 条分红:")
print(moutai.to_string(index=False))

print()
print("=== 第八步全部通过 ===")
