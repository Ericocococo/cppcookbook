# -*- coding: utf-8 -*-
"""第六步：basic_binding 验证。"""
import os
import sys

cur = os.path.dirname(os.path.abspath(__file__))
for sub in ["lib", "build_py_vs/Release", "build_py/Release", "build_py"]:
    p = os.path.join(cur, sub)
    if os.path.isdir(p) and p not in sys.path:
        sys.path.insert(0, p)

import basic_binding as bb

# add：int → Python int
print()
r1 = bb.add(3, 5)
print(f"add(3, 5) = {r1}")
print(type(r1))

# avg：vector<double> → Python float
print()
r2 = bb.avg([1, 2, 3, 4])
print(f"avg([1,2,3,4]) = {r2}")
print(type(r2))

# count_letters：map → Python dict
print()
r3 = bb.count_letters("hello")
print(f"count_letters('hello') = {r3}")
print(type(r3))

# Stock 类
print()
s = bb.Stock("600519.SH", 1500.0)
print(f"code = {s.code()}, close = {s.close}")
print(type(s))

up, down = s.limit_prices()
print(f"涨停/跌停 = {up:.1f} / {down:.1f}")
print(type(up))

s.set_close(1600.0)
c2 = s.close
print(f"set_close(1600) 后 close = {c2}")

print()
print("=== 第六步全部通过 ===")
