# -*- coding: utf-8 -*-
"""第七步：fhsg_demo 编译脚本。"""
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

def _vs_generator():
    """用 vswhere 检测已安装的 VS 版本，返回 CMake 生成器参数列表。"""
    vswhere = os.path.join(
        os.environ.get("ProgramFiles(x86)", r"C:\Program Files (x86)"),
        "Microsoft Visual Studio", "Installer", "vswhere.exe",
    )
    try:
        import json
        out = subprocess.check_output([vswhere, "-latest", "-format", "json"], text=True)
        vs = json.loads(out)[0]
        major = vs["installationVersion"].split(".")[0]
        year = vs["catalog"]["productLineVersion"]
        return ["-G", f"Visual Studio {major} {year}", "-A", "x64"]
    except Exception:
        return []

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
        subprocess.run(
            ["cmake"] + _vs_generator() + [
            f"-Dpybind11_DIR={pb}",
            f"-DPython_EXECUTABLE={args.python}",
            cur_dir,
        ], cwd=build_dir, check=True)
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
