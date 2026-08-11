# -*- coding: utf-8 -*-
"""第四步：stock_demo 模块验证。"""
import os
import sys

cur = os.path.dirname(os.path.abspath(__file__))
for sub in ["lib", "build_py_vs/Release", "build_py/Release", "build_py"]:
    p = os.path.join(cur, sub)
    if os.path.isdir(p) and p not in sys.path:
        sys.path.insert(0, p)

import stock_demo as sd

s = sd.Stock("600519.SH", 1500.0)

c = s.code()
print(f"code() = \"{c}\"")

cl = s.close
print(f"close = {cl}")

s.set_close(1600.0)
cl2 = s.close
print(f"set_close(1600) 后 close = {cl2}")

up, down = s.limit_prices()
print(f"涨停={up:.1f}, 跌停={down:.1f}")

print(f"repr = {s}")

print("=== 第四步全部通过 ===")
