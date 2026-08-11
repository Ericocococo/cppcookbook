# -*- coding: utf-8 -*-
"""第三步：types_demo 模块验证。"""
import os
import sys

cur = os.path.dirname(os.path.abspath(__file__))
for sub in ["lib", "build_py_vs/Release", "build_py/Release", "build_py"]:
    p = os.path.join(cur, sub)
    if os.path.isdir(p) and p not in sys.path:
        sys.path.insert(0, p)

import types_demo as t

r1 = t.avg([1, 2, 3, 4])
print(f"avg([1,2,3,4]) = {r1}")

r2 = t.avg([])
print(f"avg([]) = {r2}")

r3 = t.greet()
print(f"greet() = \"{r3}\"")

r4 = t.greet(name="Python")
print(f"greet(name='Python') = \"{r4}\"")

r5 = t.count_letters("hello")
print(f"count_letters('hello') = {r5}")

r6 = t.split_symbol("600519.SH")
print(f"split_symbol('600519.SH') = {r6}")

r7 = t.split_symbol("BTCUSDT")
print(f"split_symbol('BTCUSDT') = {r7}")

print("=== 第三步全部通过 ===")
