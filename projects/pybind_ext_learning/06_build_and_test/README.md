# 06_build_and_test — 第六步：build.py 自动化 + test 脚本

前三步都是手动 cmake 编译。本步引入 `build.py` 自动化脚本和 `test_basic.py` 验证脚本——从此编译只需一行命令。

> `basic.cpp` 里的绑定内容（add / avg / count_letters / Stock）是 3~5 步的**复习**，函数、类、属性绑定写法都在前几步学过；本步真正新学的是 `build.py` 与测试脚本组织，代码重复是为了让脚本有东西可测。

## 1. 文件

| 文件               | 说明                                   |
|:-----------------|:-------------------------------------|
| `basic.cpp`      | 绑定源文件：3 个普通函数 + 1 个类（含属性/方法）         |
| `CMakeLists.txt` | pybind11 标准工程（`pybind11_add_module`） |
| `build.py`       | 一键编译脚本（`--dest lib` 拷贝 .pyd 到指定目录）   |
| `test_basic.py`  | 验证脚本：函数 / 类 / 属性 / STL 容器转换          |

---

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

> pybind11 模块（.pyd）必须使用与 Python 相同的编译器。Windows 上的 Python（Anaconda）由 MSVC 编译，因此 MinGW 不适用。Linux
> 构建见 § 2.3。

### 2.2 命令行 · MSVC（cmd）

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\06_build_and_test

# 编译 + 拷贝 .pyd 到 lib/
D:\ProgramData\anaconda3\envs\quant311\python build.py --dest lib

# 验证
D:\ProgramData\anaconda3\envs\quant311\python test_basic.py
```

</details>

> `build.py` 内部自动定位 MSVC + pybind11 + Python，调用 cmake 配置和编译。
> 内部通过 `vswhere` 自动检测 VS 版本，使用 **VS Generator（方案 B，多配置）**，构建时传 `--config Release`。

### 2.3 命令行 · Linux / WSL

```bash
# cd 到项目目录
cd 06_build_and_test

# 编译 + 拷贝
python build.py --dest lib

# 验证
python test_basic.py
```

> pybind11 模块（.so）同样必须使用与 Python 相同的编译器。Linux 上的 Python 由 GCC 编译，`build.py` 自动使用 GCC，无需额外配置。

---

## 3. CLion IDE

1. `File → Open` 选择 `06_build_and_test/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. CMake Options 中加 `-Dpybind11_DIR=... -DPython_EXECUTABLE=...`
4. **构建** `Ctrl+F9`

---

## 4. 本步要点

| 绑定     | C++ 侧                                 | Python 侧                              |
|:-------|:--------------------------------------|:--------------------------------------|
| 普通函数   | `m.def("add", &add)`                  | `bb.add(3, 5)`                        |
| 带默认参数名 | `py::arg("a"), py::arg("b")`          | `bb.add(b=5, a=3)` 可关键字传参             |
| STL 容器 | `#include <pybind11/stl.h>`           | `list` ↔ `vector`，`dict` ↔ `map` 自动转换 |
| 类      | `py::class_<Stock>(m, "Stock")`       | `s = bb.Stock(...)`                   |
| 只读属性   | `def_property_readonly("close", ...)` | `s.close`（不是方法，不能加括号）                 |

---

## 5. 常见坑

- `def_property_readonly` 定义的是**属性**不是方法，Python 侧访问不要加 `()`
- 浮点计算有精度误差（`1500*1.1 = 1650.0000000000002`），断言用 `abs(a-b) < 1e-9` 而不是 `==`
