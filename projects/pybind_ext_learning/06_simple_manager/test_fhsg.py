# -*- coding: utf-8 -*-
"""第六步：fhsg_demo 验证。"""
import os
import sys

cur = os.path.dirname(os.path.abspath(__file__))
for sub in ["lib", "build_py_vs/Release", "build_py/Release", "build_py"]:
    p = os.path.join(cur, sub)
    if os.path.isdir(p) and p not in sys.path:
        sys.path.insert(0, p)

import fhsg_demo as fd

mgr = fd.CFHSGMgr()

records = []
for ns, divi in [(1686873600_000000000, 25.979), (1718841600_000000000, 30.876)]:
    r = fd.FHSGRecord()
    r.symbol = "600519.SH"
    r.ex_divi_date_ns = ns
    r.bonus = 0.0
    r.cash_divi = divi
    records.append(r)

mgr.load(records)

sz = mgr.size()
print(f"size = {sz}")

syms = mgr.get_symbols()
print(f"symbols = {syms}")

result = mgr.query("600519.SH")
n = len(result)
print(f"600519.SH 分红: {n} 条")
for r in result:
    print(f"  symbol={r.symbol} bonus={r.bonus} cash_divi={r.cash_divi}")

empty = mgr.query("999999.SH")
n2 = len(empty)
print(f"999999.SH: {n2} 条")

print("=== 第六步通过 ===")
