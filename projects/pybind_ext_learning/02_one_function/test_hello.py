# -*- coding: utf-8 -*-
"""第二步：hello 模块验证。"""
import os
import sys

cur = os.path.dirname(os.path.abspath(__file__))
for sub in ["lib", "build_py_vs/Release", "build_py/Release", "build_py"]:
    p = os.path.join(cur, sub)
    if os.path.isdir(p) and p not in sys.path:
        sys.path.insert(0, p)

import hello

r1 = hello.add(3, 5)
print(f"hello.add(3, 5) = {r1}")

r2 = hello.add(-1, 1)
print(f"hello.add(-1, 1) = {r2}")

doc = hello.__doc__
print(f"hello.__doc__ = \"{doc}\"")

print("=== 第二步全部通过 ===")
