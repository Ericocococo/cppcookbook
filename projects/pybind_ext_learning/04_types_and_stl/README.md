# 04_types_and_stl — 第四步：多种类型 + STL 容器

传输多种类型参数：double / string / vector / map，以及 `py::arg` 关键字参数。

## 1. 文件

| 文件               | 说明                                               |
|:-----------------|:-------------------------------------------------|
| `types.cpp`      | 4 个函数：avg / greet / count_letters / split_symbol |
| `CMakeLists.txt` | 和第三步一样的 pybind11 工程                              |
| `test_types.py`  | 验证脚本                                             |

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
cd /d D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\04_types_and_stl

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build_py -G Ninja ^
  -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" ^
  -Dpybind11_DIR=D:/ProgramData/anaconda3/envs/quant311/Lib/site-packages/pybind11/share/cmake/pybind11 ^
  -DPython_EXECUTABLE=D:/ProgramData/anaconda3/envs/quant311/python.exe

:: 构建（Ninja 是单配置，不需要 --config）
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
D:\ProgramData\anaconda3\envs\quant311\python test_types.py
```

</details>

#### 方案 B — VS Generator

CMake 自动通过 `vswhere.exe` 检测 MSVC 工具链，生成 `.sln` 工程。VS Generator 是多配置，构建须指定 `--config`。

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bat
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\04_types_and_stl

:: 配置（-G "Visual Studio 18 2026" 生成 .sln 工程，-A x64 指定 64 位）
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build_py_vs -G "Visual Studio 18 2026" -A x64 ^
  -Dpybind11_DIR=D:/ProgramData/anaconda3/envs/quant311/Lib/site-packages/pybind11/share/cmake/pybind11 ^
  -DPython_EXECUTABLE=D:/ProgramData/anaconda3/envs/quant311/python.exe

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build_py_vs --config Release
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```bat
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build_py_vs -G "Visual Studio 18 2026" -A x64 -Dpybind11_DIR=D:/ProgramData/anaconda3/envs/quant311/Lib/site-packages/pybind11/share/cmake/pybind11 -DPython_EXECUTABLE=D:/ProgramData/anaconda3/envs/quant311/python.exe

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build_py_vs --config Release

:: 验证
D:\ProgramData\anaconda3\envs\quant311\python test_types.py
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# cd 到项目目录
cd 04_types_and_stl

# 配置
mkdir build && cd build
cmake -Dpybind11_DIR=$(python -c "import pybind11;print(pybind11.get_cmake_dir())") \
      -DPython_EXECUTABLE=$(which python) ..

# 构建
cmake --build .

# 返回上级目录验证
cd ..
python test_types.py
```

> pybind11 模块（.so）同样必须使用与 Python 相同的编译器。Linux 上的 Python 由 GCC 编译，因此 .so 用 GCC 构建即可。

---

## 3. CLion IDE

1. `File → Open` 选择 `04_types_and_stl/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. CMake Options 中加 `-Dpybind11_DIR=... -DPython_EXECUTABLE=...`
4. **构建** `Ctrl+F9`

---

## 4. 验证

直接运行仓库自带的 `test_types.py` 即可（脚本第 7-8 行 `sub_list` 已按当前构建方案配置好产物目录；切换构建方案时改这两行）：

```bash
python test_types.py
```

手动验证等价代码（Ninja 方案；VS Generator 方案把路径换成 `build_py_vs/Release`）：

```python
import sys; sys.path.insert(0, "build_py")
import types_demo as t

t.avg([1, 2, 3, 4])          # → 2.5（Python list → C++ vector 自动转换）
t.greet()                     # → "你好, 世界!"（默认参数）
t.greet(name="Python")        # → "你好, Python!"（关键字传参）
t.count_letters("hello")      # → {'e': 1, 'h': 1, 'l': 2, 'o': 1}（C++ map → Python dict）
t.split_symbol("600519.SH")   # → ['600519', 'SH']（C++ vector → Python list）
```

---

## 5. 本步新学了什么

| 上一步         | 本步新增                                                             |
|:------------|:-----------------------------------------------------------------|
| 只有 `int` 参数 | `double` / `string` / `vector` / `map` 全部支持                      |
| 无           | `#include <pybind11/stl.h>` — 加这一行，STL 容器自动和 Python list/dict 互转 |
| 无           | `py::arg("name") = "世界"` — 带默认值的关键字参数                            |
| 返回 `int`    | 返回 `map` / `vector`，pybind11 自动转为 `dict` / `list`                |

---

## 6. 核心原理

`pybind11/stl.h` 为常见 STL 容器注册了**类型转换器（type caster）**：

| 方向 | C++ | Python |
|------|-----|--------|
| C++ → Python | `std::vector<T>` | `list` |
| C++ → Python | `std::map<K, V>` | `dict` |
| C++ → Python | `std::pair<A, B>` | `tuple` |
| C++ → Python | `std::string` | `str` |
| Python → C++ | `list` | `std::vector<T>` |
| Python → C++ | `dict` | `std::map<K, V>` |
| Python → C++ | `tuple` | `std::pair<A, B>` |
| Python → C++ | `str` | `std::string` |

转换是**值拷贝**（不是引用），Python 侧修改 list 不影响 C++ 侧的 vector。
