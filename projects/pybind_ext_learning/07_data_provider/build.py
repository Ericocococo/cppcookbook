# -*- coding: utf-8 -*-
"""data_provider_ext 编译脚本 — 参考框架 build_engine.py 简化版。

自动完成:
  1. 定位 pybind11（当前 Python 环境）
  2. 配置 CMake（-DBUILD_PYBIND=ON）
  3. 编译 .pyd
  4. 拷贝 .pyd 到指定目录（便于 import）

用法:
  python build.py                    # 仅编译（Release）
  python build.py --dest 目录         # 编译 + 拷贝 .pyd 到指定目录（不存在则创建）
  python build.py --dest 目录 --python 路径  # 指定 Python 解释器

注意事项:
  - Windows 下必须用 MSVC 编译器（Python 是 MSVC 编译的，MinGW 的 .pyd 无法加载）
  - pybind11 需已安装: pip install pybind11
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
    parser = argparse.ArgumentParser(description="data_provider_ext 编译脚本")
    parser.add_argument("--dest", default=None,
                        help="拷贝 .pyd 到指定目录（不存在则自动创建）")
    parser.add_argument("--python", default=sys.executable,
                        help="指定 Python 解释器路径（默认当前 python）")
    return parser.parse_args()


def get_pybind11_dir(python_exe: str) -> str:
    """从指定 Python 环境取 pybind11 的 cmake 目录。"""
    code = "import pybind11; print(pybind11.get_cmake_dir())"
    try:
        out = subprocess.check_output([python_exe, "-c", code], text=True).strip()
        return out
    except (subprocess.CalledProcessError, FileNotFoundError) as e:
        print(f"错误: 在 {python_exe} 中未找到 pybind11，请先 pip install pybind11")
        print(f"  {e}")
        sys.exit(1)


def find_pyd() -> str:
    """在 build_py 目录下递归查找编译出的 .pyd，返回完整路径。

    注意: 必须锚定文件名结尾($)，否则会误匹配 MSBuild 生成的
    "xxx.pyd.recipe" 中间文件。
    """
    pyd_re = re.compile(r"data_provider_ext\.cp\d+-win_amd64\.pyd$")
    for rootpath, _, filenames in os.walk(build_dir):
        for fn in filenames:
            if pyd_re.match(fn):
                return os.path.join(rootpath, fn)
    return ""


def copy_pyd(dest_dir: str) -> bool:
    """把编译出的 .pyd 拷贝到指定目录（不存在则自动创建）。"""
    found = find_pyd()
    if not found:
        print("未找到编译产物 .pyd")
        return False
    os.makedirs(dest_dir, exist_ok=True)
    target = os.path.join(dest_dir, os.path.basename(found))
    shutil.copy2(found, target)
    print(f".pyd 已拷贝到: {target}")
    return True


def build():
    args = parse_args()
    python_exe = args.python
    pybind11_dir = get_pybind11_dir(python_exe)

    print(f"Python: {python_exe}")
    print(f"pybind11: {pybind11_dir}")

    os.makedirs(build_dir, exist_ok=True)

    # 配置（失败时重新生成）
    cmake_cache = os.path.join(build_dir, "CMakeCache.txt")
    if not os.path.exists(cmake_cache):
        print("正在配置 CMake...")
        cmd = [
            "cmake",
            f"-Dpybind11_DIR={pybind11_dir}",
            f"-DPython_EXECUTABLE={python_exe}",
            "-DBUILD_PYBIND=ON",
            cur_dir,
        ]
        subprocess.run(cmd, cwd=build_dir, check=True)

    # 编译
    print("正在编译...")
    subprocess.run(["cmake", "--build", ".", "--config", "Release"],
                   cwd=build_dir, check=True)

    found = find_pyd()
    if not found:
        print("编译完成，但未找到 .pyd 产物")
        return False

    print(f"编译成功: {found}")

    if args.dest:
        copy_pyd(args.dest)
    return True


if __name__ == "__main__":
    if build():
        print("完成！")
    else:
        print("编译失败")
        sys.exit(1)
