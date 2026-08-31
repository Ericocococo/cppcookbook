# 07_simple_manager — 第七步：单个数据管理器

从"绑定一个类"到"绑定一个完整的数据管理器"。本步只有一个 Mgr（CFHSGMgr），用最简单的 `unordered_map + 线性扫描`。

## 1. 文件

| 文件             | 说明                              |
|:---------------|:--------------------------------|
| `fhsg_mgr.h`   | CFHSGMgr 声明 + FHSGRecord 结构体    |
| `fhsg_mgr.cpp` | Load（存入 map）+ Query（按 symbol 查） |
| `bindings.cpp` | pybind11 绑定                     |
| `build.py`     | 编译脚本                            |
| `test_fhsg.py` | 验证脚本                            |

---

## 2. 命令行 · MinGW（Git Bash）

> pybind11 模块（.pyd）必须使用与 Python 相同的编译器。Windows 上的 Python（Anaconda）由 MSVC 编译，因此 MinGW 不适用。Linux
> 构建见 § 4。

---

## 3. 命令行 · MSVC（cmd）

```bash
D:\ProgramData\anaconda3\envs\quant311\python build.py --dest lib
D:\ProgramData\anaconda3\envs\quant311\python test_fhsg.py
```

> `build.py` 内部自动定位 MSVC + pybind11 + Python，调用 cmake 配置和编译。
> 内部通过 `vswhere` 自动检测 VS 版本，使用 **VS Generator（方案 B，多配置）**，构建时传 `--config Release`。

---

## 4. 命令行 · Linux / WSL

```bash
cd 07_simple_manager
D:\ProgramData\anaconda3\envs\quant311\python build.py --dest lib
D:\ProgramData\anaconda3\envs\quant311\python test_fhsg.py
```

> pybind11 模块（.so）同样必须使用与 Python 相同的编译器。Linux 上的 Python 由 GCC 编译，因此 .so 用 GCC 构建即可。

---

## 5. CLion IDE

1. `File → Open` 选择 `07_simple_manager/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. CMake Options 中加 `-Dpybind11_DIR=... -DPython_EXECUTABLE=...`
4. **构建** `Ctrl+F9`

---

## 6. 核心模式

```
Load(records)                   Query("600519.SH")
┌──────────────────┐           ┌──────────────────┐
│ 遍历 records      │           │ map.find(symbol)  │
│ 按 symbol 分组    │    →      │ 返回 vector       │
│ 存入 map          │           │ (线性扫描)         │
└──────────────────┘           └──────────────────┘
```

---

## 7. 本步新学了什么

| 上一步（06） | 本步新增                                     |
|:--------|:-----------------------------------------|
| 绑定现有的类  | **设计并实现一个数据管理器**（Load + Query + map 存储）  |
| 一个类     | 两个类（FHSGRecord 结构体 + CFHSGMgr 管理器）       |
| 无       | `def_readwrite` 直接暴露成员变量（FHSGRecord 的字段） |
| 无       | `py::init<>()` 无参构造函数                    |

---

## 8. 和下一步的区别

| 本步                      | 下一步（08_data_provider）             |
|:------------------------|:----------------------------------|
| 1 个 Mgr                 | 多个 Mgr（FHSG + 涨跌停 + 板块 + 分类）      |
| `std::string` 做 map key | `SymbolKey`（定长 16 字节 array，避免堆分配） |
| 线性扫描                    | `upper_bound` 二分查找                |
| 无时间过滤                   | `cur_ns` 防未来函数                    |
| 只有 pybind 模块            | 纯 C++ exe + pybind 双构建            |
