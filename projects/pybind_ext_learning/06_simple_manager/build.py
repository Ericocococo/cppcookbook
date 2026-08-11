# -*- coding: utf-8 -*-
"""第六步：fhsg_demo 编译脚本。"""
import argparse, os, re, shutil, subprocess, sys

cur_dir = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.join(cur_dir, "build_py")

def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument("--dest", default=None)
    p.add_argument("--python", default=sys.executable)
    return p.parse_args()

def get_pybind11_dir(exe):
    return subprocess.check_output([exe, "-c", "import pybind11;print(pybind11.get_cmake_dir())"], text=True).strip()

def find_pyd():
    r = re.compile(r"fhsg_demo\.cp\d+-win_amd64\.pyd$")
    for root, _, fs in os.walk(build_dir):
        for f in fs:
            if r.match(f): return os.path.join(root, f)
    return ""

def main():
    args = parse_args()
    pb = get_pybind11_dir(args.python)
    os.makedirs(build_dir, exist_ok=True)
    if not os.path.exists(os.path.join(build_dir, "CMakeCache.txt")):
        subprocess.run(["cmake", f"-Dpybind11_DIR={pb}", f"-DPython_EXECUTABLE={args.python}", cur_dir], cwd=build_dir, check=True)
    subprocess.run(["cmake", "--build", ".", "--config", "Release"], cwd=build_dir, check=True)
    found = find_pyd()
    if not found: print("未找到 .pyd"); return False
    print(f"编译成功: {found}")
    if args.dest:
        os.makedirs(args.dest, exist_ok=True)
        shutil.copy2(found, os.path.join(args.dest, os.path.basename(found)))
        print(f".pyd 已拷贝到: {args.dest}")
    return True

if __name__ == "__main__":
    sys.exit(0 if main() else 1)
