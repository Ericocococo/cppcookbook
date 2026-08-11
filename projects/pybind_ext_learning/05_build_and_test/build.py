# -*- coding: utf-8 -*-
"""第一步：basic_binding 编译脚本。

用法:
  python build.py                    # 仅编译
  python build.py --dest lib         # 编译 + 拷贝 .pyd 到指定目录
  python build.py --python 路径      # 指定 Python 解释器
"""

import argparse
import os
import re
import shutil
import subprocess
import sys

cur_dir = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.join(cur_dir, "build_py")


def parse_args():
    parser = argparse.ArgumentParser(description="basic_binding 编译脚本")
    parser.add_argument("--dest", default=None,
                        help="拷贝 .pyd 到指定目录（不存在则自动创建）")
    parser.add_argument("--python", default=sys.executable,
                        help="指定 Python 解释器路径（默认当前 python）")
    return parser.parse_args()


def get_pybind11_dir(python_exe: str) -> str:
    code = "import pybind11; print(pybind11.get_cmake_dir())"
    try:
        return subprocess.check_output([python_exe, "-c", code], text=True).strip()
    except (subprocess.CalledProcessError, FileNotFoundError):
        print(f"错误: 在 {python_exe} 中未找到 pybind11，请先 pip install pybind11")
        sys.exit(1)


def find_pyd() -> str:
    pyd_re = re.compile(r"basic_binding\.cp\d+-win_amd64\.pyd$")
    for root, _, files in os.walk(build_dir):
        for fn in files:
            if pyd_re.match(fn):
                return os.path.join(root, fn)
    return ""


def main():
    args = parse_args()
    pybind11_dir = get_pybind11_dir(args.python)

    os.makedirs(build_dir, exist_ok=True)
    cmake_cache = os.path.join(build_dir, "CMakeCache.txt")
    if not os.path.exists(cmake_cache):
        print("正在配置 CMake...")
        subprocess.run(
            ["cmake", f"-Dpybind11_DIR={pybind11_dir}",
             f"-DPython_EXECUTABLE={args.python}", cur_dir],
            cwd=build_dir, check=True)

    print("正在编译...")
    subprocess.run(["cmake", "--build", ".", "--config", "Release"],
                   cwd=build_dir, check=True)

    found = find_pyd()
    if not found:
        print("编译完成，但未找到 .pyd")
        return False

    print(f"编译成功: {found}")
    if args.dest:
        os.makedirs(args.dest, exist_ok=True)
        target = os.path.join(args.dest, os.path.basename(found))
        shutil.copy2(found, target)
        print(f".pyd 已拷贝到: {target}")
    return True


if __name__ == "__main__":
    sys.exit(0 if main() else 1)
