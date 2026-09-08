# -*- coding: utf-8 -*-
"""第四步：types_demo 模块验证。"""
import os
import sys

cur = os.path.dirname(os.path.abspath(__file__))
# sub_list = ["build_py"]
sub_list = ["build_py_vs/Release"]
for sub in sub_list:
    p = os.path.join(cur, sub)
    if os.path.isdir(p) and p not in sys.path:
        sys.path.insert(0, p)

import types_demo as t

# avg：C++ double → Python float
print()
r1 = t.avg([1, 2, 3, 4])
print(f"avg([1,2,3,4]) = {r1}")
print(type(r1))

r2 = t.avg([])
print(f"avg([]) = {r2}")
print(type(r2))

# greet：C++ string → Python str
print()
r3 = t.greet()
print(f"greet() = \"{r3}\"")
print(type(r3))

r4 = t.greet(name="Python")
print(f"greet(name='Python') = \"{r4}\"")
print(type(r4))

# count_letters：C++ map → Python dict
print()
r5 = t.count_letters("hello")
print(f"count_letters('hello') = {r5}")
print(type(r5))

# split_symbol：C++ vector<string> → Python list
print()
r6 = t.split_symbol("600519.SH")
print(f"split_symbol('600519.SH') = {r6}")
print(type(r6))

r7 = t.split_symbol("BTCUSDT")
print(f"split_symbol('BTCUSDT') = {r7}")
print(type(r7))

print()
print("=== 第四步全部通过 ===")
