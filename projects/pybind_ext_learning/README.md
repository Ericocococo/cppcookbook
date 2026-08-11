# pybind_ext_learning — C++ 扩展学习路径

以量化框架（quant.core.frame）的 pybind11 扩展为背景，**由浅入深八步走**，从纯 C++ 开始，逐步引入 pybind11、自动化脚本、架构模式、第三方依赖。每一步独立可编译、可运行、可验证。

## 学习路径总览

| 步骤 | 目录 | 学什么 | 新引入 |
|:---|:---|:---|:---|
| 1 | `01_hello_cpp/` | 纯 C++ 函数，确认编译环境 | CMake + add_executable |
| 2 | `02_one_function/` | 第一个 pybind11 模块：一个 add 函数 | pybind11、PYBIND11_MODULE |
| 3 | `03_types_and_stl/` | 多种类型 + STL 容器自动转换 | pybind11/stl.h、py::arg |
| 4 | `04_class_binding/` | 类绑定 + .h/.cpp/bindings 三文件分离 | py::class_、头文件分离 |
| 5 | `05_build_and_test/` | build.py 自动化 + test 脚本 | 自动化编译部署脚本 |
| 6 | `06_simple_manager/` | 单个数据管理器（Load + Query） | unordered_map 管理 + def_readwrite |
| 7 | `07_data_provider/` | 多 Mgr + 二分查找 + cur_ns + 聚合类 | SymbolKey、upper_bound、双构建 |
| 8 | `08_parquet_and_dll/` | arrow/parquet 读文件 + 拷贝依赖 dll | vcpkg、dll 部署机制 |

## 八步递进关系

```
第1步  纯 C++                  确认编译环境
  ↓
第2步  一个函数 → Python        pybind11 登场
  ↓
第3步  多种类型 + STL            pybind11/stl.h
  ↓
第4步  类绑定 + .h 分离          py::class_ + 三文件分离
  ↓
第5步  build.py + test          自动化（告别手动 cmake）
  ↓
第6步  单个 Mgr                 Load + Query 基本模式
  ↓
第7步  多 Mgr + 聚合             SymbolKey + 二分查找 + cur_ns 防未来函数
  ↓
第8步  第三方依赖 + dll 拷贝     生产级部署（对应框架 build_engine.py）
```

## 快速开始

```bash
set PY=D:\ProgramData\anaconda3\envs\quant311\python

# 第1步（纯 C++，MinGW 即可）
cd 01_hello_cpp
mkdir -p build && cd build && cmake -G "MinGW Makefiles" .. && cmake --build . && ./hello_cpp.exe
cd ../..

# 第2-4步（手动 cmake，需 MSVC，详见各步 README 四步版）

# 第5-8步有 build.py
cd 05_build_and_test && %PY% build.py --dest lib && %PY% test_basic.py && cd ..
cd 06_simple_manager && %PY% build.py --dest lib && %PY% test_fhsg.py && cd ..
cd 07_data_provider  && %PY% build.py --dest lib && %PY% test_bindings.py && cd ..
cd 08_parquet_and_dll && %PY% gen_test_data.py && %PY% build.py --dest lib && %PY% test_parquet.py && cd ..
```

## 每步完成标准

| 步骤 | 通过标志 |
|:---|:---|
| 1 | 输出 "编译环境正常" |
| 2 | `hello.add(3, 5)` 返回 `8` |
| 3 | `types_demo.avg([1,2,3,4])` 返回 `2.5` |
| 4 | `stock_demo.Stock("600519.SH", 1500).close` 返回 `1500.0` |
| 5 | `test_basic.py` 输出 "=== 第一步全部通过 ===" |
| 6 | `test_fhsg.py` 输出 "=== 第六步通过 ===" |
| 7 | `test_bindings.py` 输出 "=== 全部通过 ===" |
| 8 | `test_parquet.py` 输出 "=== 第三步全部通过 ==="，`lib/` 下有 pyd + 多个 dll |

## 技术栈

| 组件 | 说明 |
|:---|:---|
| C++ | C++17/20，步骤1 MinGW 即可，步骤2 起需 MSVC |
| pybind11 | C++ → Python 绑定库（步骤2 起使用，conda quant311 环境） |
| arrow/parquet | 列式存储 + parquet 读写（步骤8 使用，vcpkg 安装） |
| CMake | 独立工程，`pybind11_add_module` 生成 Python 模块 |
