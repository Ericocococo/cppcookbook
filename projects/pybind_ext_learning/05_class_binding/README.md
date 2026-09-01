# 05_class_binding — 第五步：类绑定 + 头文件分离

把 C++ 类暴露给 Python，同时学会**声明/实现/绑定三文件分离**——这是 C++ 多文件项目的基本实践。

## 1. 文件

| 文件               | 说明                                                       | 依赖 pybind11？ |
|:-----------------|:---------------------------------------------------------|:------------:|
| `stock.h`        | Stock 类声明（纯 C++ 头文件）                                     |      ✗       |
| `stock.cpp`      | Stock 类实现（纯 C++）                                         |      ✗       |
| `bindings.cpp`   | pybind11 绑定（唯一引用 pybind11 的地方）                           |      ✓       |
| `CMakeLists.txt` | `pybind11_add_module(stock_demo stock.cpp bindings.cpp)` |      —       |
| `test_stock.py`  | 验证脚本                                                     |      —       |

> **分离的好处**：`stock.h/cpp` 是纯 C++，可以被其他 C++ 项目复用，不强制依赖 pybind11。绑定只是"给 Python 开一扇窗"。

---

## 2. 命令行 · MinGW（Git Bash）

> pybind11 模块（.pyd）必须使用与 Python 相同的编译器。Windows 上的 Python（Anaconda）由 MSVC 编译，因此 MinGW 不适用。Linux
> 构建见 § 4。

---

## 3. 命令行 · MSVC（cmd）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

:: 激活 — 把 cl.exe / link.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "%VCVARSALL%" x64
```

### 方案 A：vcvarsall + Ninja（推荐，单配置）

```bat
:: 配置
"%CMAKE%" -B build_py -G Ninja ^
  -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" ^
  -Dpybind11_DIR=D:/ProgramData/anaconda3/envs/quant311/Lib/site-packages/pybind11/share/cmake/pybind11 ^
  -DPython_EXECUTABLE=D:/ProgramData/anaconda3/envs/quant311/python.exe

:: 构建（Ninja 是单配置，不需要 --config）
"%CMAKE%" --build build_py
```

> 四步版：

```bat
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build_py -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -Dpybind11_DIR=D:/ProgramData/anaconda3/envs/quant311/Lib/site-packages/pybind11/share/cmake/pybind11 -DPython_EXECUTABLE=D:/ProgramData/anaconda3/envs/quant311/python.exe

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build_py

:: 验证
D:\ProgramData\anaconda3\envs\quant311\python test_stock.py
```

### 方案 B：Visual Studio 生成器（多配置）

CMake 自动通过 `vswhere.exe` 检测 MSVC 工具链，生成 `.sln` 工程。VS Generator 是多配置，构建须指定 `--config`。

```bat
:: 配置（-G "Visual Studio 18 2026" 生成 .sln 工程，-A x64 指定 64 位）
"%CMAKE%" -B build_py_vs -G "Visual Studio 18 2026" -A x64 ^
  -Dpybind11_DIR=D:/ProgramData/anaconda3/envs/quant311/Lib/site-packages/pybind11/share/cmake/pybind11 ^
  -DPython_EXECUTABLE=D:/ProgramData/anaconda3/envs/quant311/python.exe

:: 构建
"%CMAKE%" --build build_py_vs --config Release
```

> 四步版：

```bat
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build_py_vs -G "Visual Studio 18 2026" -A x64 -Dpybind11_DIR=D:/ProgramData/anaconda3/envs/quant311/Lib/site-packages/pybind11/share/cmake/pybind11 -DPython_EXECUTABLE=D:/ProgramData/anaconda3/envs/quant311/python.exe

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build_py_vs --config Release

:: 验证
D:\ProgramData\anaconda3\envs\quant311\python test_stock.py
```

---

## 4. 命令行 · Linux / WSL

```bash
cd 05_class_binding
mkdir build && cd build
cmake -Dpybind11_DIR=$(python -c "import pybind11;print(pybind11.get_cmake_dir())") \
      -DPython_EXECUTABLE=$(which python) ..
cmake --build .

cd ..
python test_stock.py
```

> pybind11 模块（.so）同样必须使用与 Python 相同的编译器。Linux 上的 Python 由 GCC 编译，因此 .so 用 GCC 构建即可。

---

## 5. CLion IDE

1. `File → Open` 选择 `05_class_binding/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. CMake Options 中加 `-Dpybind11_DIR=... -DPython_EXECUTABLE=...`
4. **构建** `Ctrl+F9`

---

## 6. 验证

```python
import sys; sys.path.insert(0, "build_py/Release")
import stock_demo as sd

s = sd.Stock("600519.SH", 1500.0)
print(s)               # → <Stock 600519.SH close=1500.000000>
print(s.code())        # → "600519.SH"（方法，加括号）
print(s.close)         # → 1500.0（属性，不加括号）
# print(s.close())     # ← TypeError! 属性不能加括号

s.set_close(1600.0)
print(s.close)         # → 1600.0

up, down = s.limit_prices()
print(f"涨停={up:.1f}, 跌停={down:.1f}")   # → 涨停=1760.0, 跌停=1440.0
```

---

## 7. 本步新学了什么

| 前四步      | 本步新增                                          |
|:---------|:----------------------------------------------|
| 只绑定全局函数  | `py::class_<Stock>(m, "Stock")` 绑定整个类         |
| 单文件 .cpp | `.h`（声明）+ `.cpp`（实现）+ `bindings.cpp`（绑定）三文件分离 |
| 无        | `py::init<string, double>()` 绑定构造函数           |
| 无        | `.def_property_readonly("close", ...)` 绑定只读属性 |
| 无        | `.def("__repr__", ...)` 绑定 Python 特殊方法        |

---

## 8. 三文件分离模式

```
stock.h          stock.cpp          bindings.cpp
┌──────────┐    ┌──────────────┐    ┌──────────────────┐
│ class Stock│    │ #include     │    │ #include         │
│ {         │←──│ "stock.h"    │    │ "stock.h"        │
│   声明... │    │              │    │ #include         │
│ };        │    │ Stock::code()│    │ <pybind11/...>   │
└──────────┘    │ { ... }      │    │                  │
                └──────────────┘    │ PYBIND11_MODULE  │
                纯 C++              │ { py::class_     │
                不依赖 pybind11      │   .def(...)  }   │
                                    └──────────────────┘
                                    唯一依赖 pybind11
```

---

## 9. 方法 vs 属性（最常见的坑）

| 绑定方式                                       | Python 访问                   | 加括号？                 |
|:-------------------------------------------|:----------------------------|:---------------------|
| `.def("code", &Stock::code)`               | `s.code()`                  | ✓ 必须加                |
| `.def_property_readonly("close", ...)`     | `s.close`                   | ✗ 不能加（加了报 TypeError） |
| `.def_readwrite("price", &Stock::m_price)` | `s.price` / `s.price = 100` | ✗ 不加（可读可写）           |

> 规律：`def` 注册的是方法（callable），`def_property*` / `def_readwrite` 注册的是属性（descriptor）。
