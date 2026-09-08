# 03_one_function — 第三步：第一个 pybind11 模块

从零开始，把一个 C++ 函数暴露给 Python。本步只有 3 行有效代码，但跑通了完整流程。

## 1. 文件

| 文件               | 说明                                     |
|:-----------------|:---------------------------------------|
| `hello.cpp`      | 一个 `add(a, b)` 函数 + PYBIND11_MODULE 绑定 |
| `CMakeLists.txt` | 最简 pybind11 工程（`pybind11_add_module`）  |
| `test_hello.py`  | 验证脚本                                   |

---

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

> pybind11 模块（.pyd）必须使用与 Python 相同的编译器。Windows 上的 Python（Anaconda）由 MSVC 编译，因此 MinGW 不适用。Linux
> 构建见 § 2.3。

### 2.2 命令行 · MSVC（cmd）

> MSVC 路径常量表与 vcvarsall 注入说明（4 个环境变量、为什么必须 `call`）见第 1 步 [01_hello_cpp §2.2](../01_hello_cpp/README.md#22-命令行--msvccmd)，以下命令直接用。


```bat
:: 激活 — 把 cl.exe / link.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```

#### 方案 A — vcvarsall + Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bat
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\03_one_function

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build_py -G Ninja ^
  -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" ^
  -Dpybind11_DIR=D:/ProgramData/anaconda3/envs/quant311/Lib/site-packages/pybind11/share/cmake/pybind11 ^
  -DPython_EXECUTABLE=D:/ProgramData/anaconda3/envs/quant311/python.exe

:: 构建（Ninja 是单配置生成器，配置时已锁定模式，--build 不需要 --config）
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build_py
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```bat
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build_py -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -Dpybind11_DIR=D:/ProgramData/anaconda3/envs/quant311/Lib/site-packages/pybind11/share/cmake/pybind11 -DPython_EXECUTABLE=D:/ProgramData/anaconda3/envs/quant311/python.exe

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build_py

:: 验证
D:\ProgramData\anaconda3\envs\quant311\python test_hello.py
```

</details>

#### 方案 B — VS Generator

CMake 自动通过 `vswhere.exe` 检测 MSVC 工具链，生成 `.sln` 工程。VS Generator 是多配置，构建须指定 `--config`。

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bat
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\03_one_function

:: 配置（-G "Visual Studio 18 2026" 生成 .sln 工程，-A x64 指定 64 位）
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build_py_vs -G "Visual Studio 18 2026" -A x64 ^
  -Dpybind11_DIR=D:/ProgramData/anaconda3/envs/quant311/Lib/site-packages/pybind11/share/cmake/pybind11 ^
  -DPython_EXECUTABLE=D:/ProgramData/anaconda3/envs/quant311/python.exe

:: 构建（VS 生成器一个工程含 Debug + Release，必须用 --config 指定；不写默认 Debug）
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build_py_vs --config Release
```

</details>

> `--config` 只在多配置生成器（VS / Ninja Multi-Config）下需要，单配置的 Ninja 不需要。
> Debug（慢，含调试信息，可断点）vs Release（快，优化全开，日常用）。

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```bat
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build_py_vs -G "Visual Studio 18 2026" -A x64 -Dpybind11_DIR=D:/ProgramData/anaconda3/envs/quant311/Lib/site-packages/pybind11/share/cmake/pybind11 -DPython_EXECUTABLE=D:/ProgramData/anaconda3/envs/quant311/python.exe

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build_py_vs --config Release

:: 验证
D:\ProgramData\anaconda3\envs\quant311\python test_hello.py
```

</details>

> 本步故意不用 build.py，让你亲手跑一遍 cmake 命令，理解编译流程。第六步才引入自动化。

### 2.3 命令行 · Linux / WSL

```bash
# cd 到项目目录
cd 03_one_function

# 配置
mkdir build && cd build
cmake -Dpybind11_DIR=$(python -c "import pybind11;print(pybind11.get_cmake_dir())") \
      -DPython_EXECUTABLE=$(which python) ..

# 构建
cmake --build .

# 产物是 .so（不是 .pyd），返回上级目录验证
cd ..
python test_hello.py
```

> pybind11 模块（.so）同样必须使用与 Python 相同的编译器。Linux 上的 Python 由 GCC 编译，因此 .so 用 GCC 构建即可，无需额外激活环境。

---

## 3. CLion IDE

1. `File → Open` 选择 `03_one_function/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. CMake Options 中加 `-Dpybind11_DIR=... -DPython_EXECUTABLE=...`
4. **构建** `Ctrl+F9`

---

## 4. 验证

直接运行仓库自带的 `test_hello.py` 即可（脚本第 7-8 行 `sub_list` 已按当前构建方案配置好产物目录；切换构建方案时改这两行）：

```bash
python test_hello.py
# 预期输出：8
#           第三步：最小 pybind11 模块
```

手动验证等价代码（Ninja 方案；VS Generator 方案把路径换成 `build_py_vs/Release`）：

```python
import sys
sys.path.insert(0, "build_py")

import hello
print(hello.add(3, 5))    # → 8
print(hello.__doc__)       # → "第三步：最小 pybind11 模块"
```

---

## 5. 本步要点

| 概念                                     | 说明                                          |
|:---------------------------------------|:--------------------------------------------|
| `#include <pybind11/pybind11.h>`       | pybind11 唯一必须的头文件                           |
| `PYBIND11_MODULE(hello, m)`            | 入口宏：`hello` = Python 模块名，`m` = 模块对象         |
| `m.def("add", &add)`                   | 把 C++ 函数指针注册为 Python 函数                     |
| `pybind11_add_module(hello hello.cpp)` | CMake 中生成 Python 模块的一行命令                    |
| 产物 `hello.cp311-win_amd64.pyd`         | Windows 上的 Python C 扩展模块（等同于 Linux 的 `.so`） |

---

## 6. 常见问题

- **DLL load failed**：编译器与 Python 的运行时不匹配（见 § 2 说明），确认使用 MSVC 编译
- **cmake 找不到 pybind11**：检查 `-Dpybind11_DIR` 路径是否正确，用
  `python -c "import pybind11; print(pybind11.get_cmake_dir())"` 获取
