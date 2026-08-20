# -*- coding: utf-8 -*-
"""第五步：stock_demo 模块验证。"""
import os
import sys

cur = os.path.dirname(os.path.abspath(__file__))
for sub in ["lib", "build_py_vs/Release", "build_py/Release", "build_py"]:
    p = os.path.join(cur, sub)
    if os.path.isdir(p) and p not in sys.path:
        sys.path.insert(0, p)

import stock_demo as sd

s = sd.Stock("600519.SH", 1500.0)

# code()：方法，返回 string → Python str
print()
c = s.code()
print(f"code() = \"{c}\"")
print(type(c))

# close：只读属性，返回 double → Python float
print()
cl = s.close
print(f"close = {cl}")
print(type(cl))

# set_close()
print()
s.set_close(1600.0)
cl2 = s.close
print(f"set_close(1600) 后 close = {cl2}")
print(type(cl2))

# limit_prices()：返回 pair → Python tuple
print()
up, down = s.limit_prices()
print(f"涨停={up:.1f}, 跌停={down:.1f}")
print(type(up))

# __repr__
print()
print(f"repr = {s}")
print(type(s))

print()
print("=== 第五步全部通过 ===")
