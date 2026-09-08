# -*- coding: utf-8 -*-
"""生成测试 parquet 数据（fhsg 风格：分红送股）。

运行: conda activate quant311 && python gen_test_data.py
"""
import pandas as pd

df = pd.DataFrame({
    "symbol": ["600519.SH"] * 4 + ["000001.SZ"] * 3,
    "ex_divi_date": pd.to_datetime(["20230616", "20240621", "20250620", "20260619",
                                    "20230714", "20240806", "20250711"]),
    "bonus": [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
    "trans_add": [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
    "cash_divi": [25.979, 30.876, 32.5, 33.0, 0.355, 0.36, 0.365],
})
df.to_parquet("test_fhsg.parquet", index=False)
print("已生成 test_fhsg.parquet")
print(df.to_string(index=False))
