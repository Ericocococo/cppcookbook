# 08_data_provider — 第八步：多 Mgr + 二分查找 + 聚合 DataProvider

演示量化回测引擎中"行情辅助数据下沉 C++"的核心模式：

- `Load()` 加载数据到内存（`unordered_map<SymbolKey, vector<Record>>`）
- `Query()` 带 `cur_ns` 参数过滤，防止未来函数
- `QueryLatest()` 二分查找取 `<= cur_ns` 的最后一条
- `ClassifyInstrumentType()` 纯函数静态分类

去掉了实际项目中的 arrow/parquet 依赖，用内存构造数据直接演示查询逻辑。

项目提供两种使用方式：纯 C++ 可执行文件（零依赖）和 pybind11 Python 模块（需要 pybind11）。

## 1. 文件

| 文件                 | 说明                                                           |
|:-------------------|:-------------------------------------------------------------|
| `data_mgrs.h`      | Mgr 类声明 + 数据结构（FHSGRecord / UpDownLimitRecord 等）             |
| `data_mgrs.cpp`    | Mgr 类实现：Load（排序存储）+ Query（区间过滤 / 二分查找）                       |
| `main.cpp`         | 4 个 C++ demo：分红送股 / 涨跌停价 / 板块 / 证券类型                         |
| `bindings.cpp`     | pybind11 绑定：DataProvider 聚合类 + 记录类型暴露给 Python                |
| `test_bindings.py` | Python 侧验证脚本：Load → Query → pd.DataFrame                     |
| `build.py`         | 一键编译脚本（参考框架 build_engine.py）：定位 pybind11 → 配置 → 编译 → 拷贝 .pyd |
| `CMakeLists.txt`   | 纯 C++ exe（默认）+ pybind11 模块（`-DBUILD_PYBIND=ON`）              |

---

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

> MinGW 仅支持编译纯 C++ 可执行文件（`main.cpp`），pybind11 模块必须使用 MSVC。

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/data_provider_ext.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-mingw-ninja\data_provider_ext.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/data_provider_ext.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-mingw-make\data_provider_ext.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Debug

# 运行
./build-mingw-mc/Debug/data_provider_ext.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-mingw-mc --config Debug

:: 运行
D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-mingw-mc\Debug\data_provider_ext.exe
```

</details>

### 2.2 命令行 · MSVC（cmd）

> MSVC 路径常量表与 vcvarsall 注入说明（4 个环境变量、为什么必须 `call`）见第 1 步 [01_hello_cpp §2.2](../01_hello_cpp/README.md#22-命令行--msvccmd)，以下命令直接用。


```bat
:: 激活 — 把 cl.exe / link.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```

#### 纯 C++ 可执行文件

##### 方案 A — vcvarsall + Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\data_provider_ext.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-msvc-ninja\data_provider_ext.exe
```

</details>

##### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider

:: 配置（-G "Visual Studio 18 2026" 生成 .sln 工程，-A x64 指定 64 位）
cmake -B build-msvc-vs -G "Visual Studio 18 2026" -A x64 -S .

:: 构建（VS 生成器一个工程含 Debug + Release，必须用 --config 指定；不写默认 Debug）
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\data_provider_ext.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider\build-msvc-vs\Release\data_provider_ext.exe
```

</details>

#### pybind11 Python 模块

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\11_projects\01_pybind_ext_learning\08_data_provider

# 编译 + 拷贝 .pyd 到 lib/
D:\ProgramData\anaconda3\envs\quant311\python build.py --dest lib

# 验证
D:\ProgramData\anaconda3\envs\quant311\python test_bindings.py
```

</details>

> `build.py` 内部通过 `vswhere` 自动检测 VS 版本，使用 **VS Generator（方案 B，多配置）**，传入 `-DBUILD_PYBIND=ON` 配置编译。

### 2.3 命令行 · Linux / WSL

#### 纯 C++ 可执行文件

```bash
# cd 到项目目录
cd 08_data_provider

# 配置
mkdir build && cd build
cmake ..

# 构建
cmake --build .

# 运行
./data_provider_ext
```

#### pybind11 Python 模块

```bash
# cd 到项目目录
cd 08_data_provider

# 编译 + 拷贝
python build.py --dest lib

# 验证
python test_bindings.py
```

> pybind11 模块（.so）同样必须使用与 Python 相同的编译器。Linux 上的 Python 由 GCC 编译，因此 .so 用 GCC 构建即可。

---

## 3. CLion IDE

1. `File → Open` 选择 `08_data_provider/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. 工具栏选择工具链（MinGW 或 Visual Studio）
4. **纯 C++**：直接 Build & Run `data_provider_ext` target
5. **pybind11**：CMake Options 中加 `-DBUILD_PYBIND=ON -Dpybind11_DIR=...`，Build `data_provider_ext_py` target

---

## 4. 预期输出（纯 C++）

```
========== 扩展数据提供者最小实现 ==========

① 分红送股查询（CFHSGMgr）
  cur_ns=2024-12-31, 查到 2 条（预期 2）
    派息=25.979
    派息=30.876
  cur_ns=2025-12-31, 查到 3 条（预期 3）
  不存在的标的: 0 条（预期 0）

② 涨跌停价查询（CUpDownLimitMgr — 二分查找）
  cur_ns=2025-01-03: 涨停=16.1 跌停=13.18
  cur_ns=2025-01-04(非交易日): 涨停=16.1（回退到 1/3 的值）
  cur_ns=2020-01-01(无数据): 涨停=0（预期 0）

③ 板块查询（CPlateMgr）
  板块总数: 3 → ChatGPT 保险 银行
  银行成份股: 4 只 → 601398.SH 601939.SH 601288.SH 600036.SH
  不存在的板块: 0 只（预期 0）

④ 证券类型（ClassifyInstrumentType — 纯函数）
  600519.SH → stock ✓
  ...
  AAPL → 空(0)

========== 全部完成 ==========
```

---

## 5. 架构设计

### 5.1 问题背景

量化回测引擎每个 bar 需要查询多种辅助数据（分红送股、涨跌停价、板块成份股、证券类型等）。原始实现在 Python 侧每次调用都读
parquet 文件 + pandas 过滤，存在两个问题：

1. **性能**：回测 5000 标的 × 2500 bar，每次都读文件+内存过滤，开销大
2. **未来函数**：Python 实现未统一做时间截止过滤，查询可能返回"未来"数据导致回测结果失真

### 5.2 解决方案

```
启动阶段                                 每个 bar（高频调用）
┌──────────────────────┐                ┌──────────────────────────────┐
│  Load: 读文件 → 内存  │                │  Query: 纯内存查询            │
│                      │                │                              │
│  parquet ──→ vector  │     ──→        │  unordered_map.find(symbol)  │
│  排序（升序）          │                │  + 二分查找 / 区间扫描         │
│  存入 unordered_map   │                │  + cur_ns 截止过滤            │
└──────────────────────┘                └──────────────────────────────┘
   一次性开销（启动时）                        O(log N) 或 O(K)（每次查询）
```

**核心思路**：把 I/O 开销集中到启动阶段（Load 一次），运行时全部走内存查询（Query 零 I/O）。

### 5.3 数据结构选型

```
                    DataProvider（聚合类）
                    ┌─────────────────────────────────┐
                    │  CFHSGMgr         m_fhsg        │
                    │  CUpDownLimitMgr  m_up_down_limit│
                    │  CShareStructureMgr ...          │
                    │  CPlateMgr        m_plate        │
                    │  CIPODateMgr      m_ipo_date     │
                    └──────────┬──────────────────────┘
                               │
               每个 Mgr 内部结构相同
               │
               ▼
    unordered_map<SymbolKey, vector<Record>>
    ┌─────────────────────────────────────┐
    │ "600519.SH" → [{ns=..., bonus=...}, │
    │                 {ns=..., bonus=...}] │  ← 按时间升序
    │ "000001.SZ" → [...]                 │
    │ "300750.SZ" → [...]                 │
    └─────────────────────────────────────┘
```

| 选型                                                 | 理由                                                             |
|:---------------------------------------------------|:---------------------------------------------------------------|
| `SymbolKey`（`std::array<char,16>`）而非 `std::string` | 定长 16 字节，避免堆分配，哈希更快；股票代码最长 `688981.SH`（9 字节）足够                 |
| `vector<Record>` 按时间升序                             | 支持 `upper_bound` 二分查找 O(log N)，QueryLatest 取 `<= cur_ns` 的最后一条 |
| `unordered_map` 而非 `map`                           | 按 symbol 查找 O(1) 均摊，回测中每 bar 多次查找性能关键                          |

### 5.4 cur_ns 防未来函数

回测引擎逐 bar 推进时，当前 bar 的时间戳为 `cur_ns`。所有 Query 方法只返回 `<= cur_ns` 的数据：

```
时间轴: ──────┼───────┼───────┼───────┼──────→
              2023    2024    2025    2026
              分红A   分红B   分红C

cur_ns = 2024-12-31 时:
  Query 返回: [分红A, 分红B]      ← 分红C 被 cur_ns 截止，不返回
  
cur_ns = 2025-12-31 时:
  Query 返回: [分红A, 分红B, 分红C]  ← 全部可见
```

这确保回测中策略只能看到"当时已知"的数据，避免用未来信息做决策。

### 5.5 二分查找 QueryLatest

涨跌停价、股本结构等数据只需要"当前最新一条"（不需要历史区间），用 `upper_bound` 二分查找：

```cpp
// vec 已按 datetime_ns 升序排列
auto pos = std::upper_bound(vec.begin(), vec.end(), cur_ns,
    [](int64_t ns, const Record& r) { return ns < r.datetime_ns; });
// pos 指向第一个 > cur_ns 的位置，减一即 <= cur_ns 的最后一条
if (pos == vec.begin()) return {};  // 无数据
--pos;
return *pos;
```

时间复杂度 O(log N)，5000 条涨跌停记录（约 20 年日频）只需 ~13 次比较。

### 5.6 pybind11 绑定层

```
C++ 层                          Python 层
┌──────────────┐               ┌─────────────────────────┐
│ DataProvider  │   pybind11   │ import data_provider_ext │
│   .load_fhsg()│ ──────────→  │ mgr = dp.DataProvider()  │
│   .get_divid_ │              │ mgr.load_fhsg(...)       │
│    factors()  │              │ result = mgr.get_divid_  │
│              │              │   factors(sym, s, e, cur) │
└──────────────┘               └──────────┬──────────────┘
                                          │
                                          ▼
                                   pd.DataFrame(result)
```

绑定要点：

| C++ 返回类型                     | pybind11 转换           | Python 侧接收                        |
|:-----------------------------|:----------------------|:----------------------------------|
| `vector<string>`             | pybind11/stl 自动转      | `list[str]`                       |
| `unordered_map<string,bool>` | pybind11/stl 自动转      | `dict[str, bool]`                 |
| `vector<FHSGRecord>`         | 绑定中手动转为 dict of lists | `dict`，可直接 `pd.DataFrame(result)` |

`vector<Record>` 不直接暴露给 Python（逐条构造 dict 开销大），而是在绑定层转为 dict of lists（列式），Python 侧一行
`pd.DataFrame()` 即可。

---

## 6. 与实际框架的对应关系

| demo 中                           | 框架中                                           | 差异                             |
|:---------------------------------|:----------------------------------------------|:-------------------------------|
| `data_mgrs.h/cpp`                | `pycpp_quant/.../reference_data.h/cpp`（框架侧现名） | 去掉 arrow/parquet 读取，用内存直接 Load |
| `bindings.cpp` DataProvider 类    | `bindings_engine.cpp` CDataProvider 绑定段       | 结构一致，框架版多了文件加载                 |
| `FHSGRecord` 构造后传入               | 框架版从 parquet 读取后填充                            | Load 数据来源不同，Query 逻辑完全一致       |
| `SymbolKey`（std::array<char,16>） | `CSymbolType`（同定义）                            | 同一设计，名字不同                      |

---

## 7. 英文及缩写说明

| 缩写/术语          | 含义                                                           |
|:---------------|:-------------------------------------------------------------|
| `cur_ns`       | current nanoseconds — 当前 bar 的纳秒时间戳，用于截止查询防止未来函数             |
| `FHSG`         | 分红送股（Fen Hong Song Gu）                                       |
| `QueryLatest`  | 查最新一条 — 二分查找取 `<= cur_ns` 的最后一条记录                            |
| `SymbolKey`    | 定长 16 字节 char 数组，用作 unordered_map 的 key（避免 std::string 的堆分配） |
| `upper_bound`  | STL 算法 — 找第一个大于目标值的位置，减一即为 `<=` 目标值的最后一个                     |
| `pybind11`     | C++ → Python 绑定库，将 C++ 类/函数暴露为 Python 可调用的模块                 |
| `.pyd`         | Windows 上 Python C 扩展模块的动态库格式（等同于 Linux 的 `.so`）             |
| `BUILD_PYBIND` | CMake 选项，`ON` 时额外编译 pybind11 Python 模块                       |
