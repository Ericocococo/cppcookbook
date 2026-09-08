# -*- coding: utf-8 -*-
"""第九步：parquet_reader 编译脚本 — 完整版（对应框架 build_engine.py）。

比第八步多两件事:
  1. 用 vcpkg toolchain（arrow/parquet 来自 vcpkg）
  2. 拷贝 .pyd 时必须连同依赖的 dll 一起拷贝（arrow.dll / parquet 依赖链），
     否则 import 时报 "DLL load failed"

用法:
  python build.py --dest 目录        # 编译 + 拷贝 pyd 和依赖 dll 到指定目录
  python build.py --vcpkg 路径       # 指定 vcpkg 根目录（默认 D:/software/vcpkg）
  python build.py --python 路径      # 指定 Python 解释器
"""

import argparse
import os
import re
import shutil
import subprocess
import sys

cur_dir = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.join(cur_dir, "build_py_vs")
DEFAULT_VCPKG = "D:/software/vcpkg"

# 目标 pyd 名称
MODULE_NAME = "parquet_reader"


def parse_args():
    parser = argparse.ArgumentParser(description="parquet_reader 编译脚本（含 dll 拷贝）")
    parser.add_argument("--dest", default=None,
                        help="拷贝 .pyd 和依赖 dll 到指定目录（不存在则创建）")
    parser.add_argument("--vcpkg", default=DEFAULT_VCPKG, help="vcpkg 根目录")
    parser.add_argument("--python", default=sys.executable,
                        help="指定 Python 解释器路径（默认当前 python）")
    return parser.parse_args([
        "--dest", "lib",
    ])


def get_pybind11_dir(python_exe: str) -> str:
    code = "import pybind11; print(pybind11.get_cmake_dir())"
    try:
        return subprocess.check_output([python_exe, "-c", code], text=True).strip()
    except (subprocess.CalledProcessError, FileNotFoundError):
        print(f"错误: 在 {python_exe} 中未找到 pybind11")
        sys.exit(1)


_VS_MAJOR_TO_YEAR = {
    "14": "2015", "15": "2017", "16": "2019",
    "17": "2022", "18": "2026",
}


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
        year = vs.get("catalog", {}).get("productLineVersion", "")
        if not year.startswith("20"):
            year = _VS_MAJOR_TO_YEAR.get(major, year)
        return ["-G", f"Visual Studio {major} {year}", "-A", "x64"]
    except Exception:
        return []


def find_pyd() -> str:
    """递归查找编译出的 .pyd（锚定文件名结尾，避免 .pyd.recipe 误匹配）。"""
    pyd_re = re.compile(rf"{MODULE_NAME}\.cp\d+-win_amd64\.pyd$")
    for root, _, files in os.walk(build_dir):
        for fn in files:
            if pyd_re.match(fn):
                return os.path.join(root, fn)
    return ""


def copy_output(dest_dir: str) -> bool:
    """拷贝 .pyd + 依赖 dll 到指定目录（对应框架 build_engine.py 的 copy_dynamic_library）。"""
    pyd_path = find_pyd()
    if not pyd_path:
        print("未找到编译产物 .pyd")
        return False

    os.makedirs(dest_dir, exist_ok=True)

    # 1. 拷贝 .pyd
    shutil.copy2(pyd_path, os.path.join(dest_dir, os.path.basename(pyd_path)))
    print(f".pyd 已拷贝: {pyd_path}")

    # 2. 拷贝依赖 dll（MSBuild 会把 vcpkg 依赖 dll 部署到 pyd 同目录，全量拷过去）
    # 不拷贝这些 dll，import 时会报 DLL load failed
    # 06/07/08 不需要拷贝 dll，因为它们只依赖 pybind11（纯头文件库，不产生额外 dll）
    #
    # parquet.dll / arrow.dll 自己也依赖压缩库（zstd/snappy/lz4 等），
    # 运行时 Windows 加载 parquet.dll 时会自动找这些依赖 dll，找不到就报 DLL load failed
    # vcpkg 动态版把这些依赖编译成独立 dll，所以必须一起拷贝
    # 如果 vcpkg 装的是静态版，压缩库会编译进 parquet.lib 里，不需要拷贝额外 dll
    #
    # 编译后产生的 11 个 dll：
    #   arrow.dll              — Arrow 核心库，内存中的列式数据格式
    #   parquet.dll            — Parquet 文件读写，依赖 Arrow
    #   zstd.dll               — Zstandard 压缩算法（Parquet 默认压缩格式）
    #   snappy.dll             — Snappy 压缩算法（Google 出的，速度优先）
    #   lz4.dll                — LZ4 压缩算法（极快压缩/解压）
    #   bz2.dll                — BZip2 压缩算法
    #   z.dll                  — zlib 压缩算法（最经典的通用压缩）
    #   brotlicommon.dll       — Brotli 压缩 — 公共部分
    #   brotlidec.dll          — Brotli 压缩 — 解压
    #   brotlienc.dll          — Brotli 压缩 — 压缩
    #   libcrypto-3-x64.dll    — OpenSSL 加密库（数据校验/完整性验证）
    pyd_dir = os.path.dirname(pyd_path)
    copied = 0
    for fn in os.listdir(pyd_dir):
        if not fn.lower().endswith(".dll"):
            continue
        src = os.path.join(pyd_dir, fn)
        if os.path.isfile(src):
            shutil.copy2(src, os.path.join(dest_dir, fn))
            copied += 1
    print(f"依赖 dll 已拷贝: {copied} 个 → {dest_dir}")
    return True


def main():
    args = parse_args()
    pybind11_dir = get_pybind11_dir(args.python)
    toolchain = f"-DCMAKE_TOOLCHAIN_FILE={args.vcpkg}/scripts/buildsystems/vcpkg.cmake"

    print(f"Python: {args.python}")
    print(f"vcpkg: {args.vcpkg}")

    os.makedirs(build_dir, exist_ok=True)
    cmake_cache = os.path.join(build_dir, "CMakeCache.txt")
    if not os.path.exists(cmake_cache):
        print("[1/2] 配置 CMake...")
        cmd = ["cmake"] + _vs_generator() + [
            "-B", build_dir, "-S", cur_dir,
            toolchain,
            f"-Dpybind11_DIR={pybind11_dir}",
            f"-DPython_EXECUTABLE={args.python}",
        ]
        print("CMD:", subprocess.list2cmdline(cmd))
        subprocess.run(cmd, check=True)

    print("[2/2] 编译...")
    cmd = ["cmake", "--build", build_dir, "--config", "Release"]
    print("CMD:", subprocess.list2cmdline(cmd))
    subprocess.run(cmd, check=True)

    pyd_path = find_pyd()
    if not pyd_path:
        print("编译完成，但未找到 .pyd")
        return False
    print(f"编译成功: {pyd_path}")

    if args.dest:
        copy_output(args.dest)
    return True


if __name__ == "__main__":
    sys.exit(0 if main() else 1)
