# 09_parquet_and_dll — 第九步：第三方依赖 + 拷贝 dll

用 arrow/parquet（vcpkg 安装）读真实 parquet 文件。本步对应框架 `build_engine.py` 的核心机制——**编译产物依赖第三方 dll，拷贝
.pyd 时必须连同 dll 一起拷贝**。

## 1. 文件

| 文件                   | 说明                                                                |
|:---------------------|:------------------------------------------------------------------|
| `parquet_reader.cpp` | arrow/parquet 读 parquet → dict of lists（参照框架 bar_data_reader.cpp） |
| `CMakeLists.txt`     | vcpkg toolchain + `find_package(Arrow/Parquet)`                   |
| `build.py`           | 完整版编译脚本：vcpkg toolchain + 编译 + 拷贝 pyd + 依赖 dll                    |
| `gen_test_data.py`   | 用 pandas 生成测试 parquet（fhsg 分红送股风格）                                |
| `test_parquet.py`    | 验证脚本                                                              |

---

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

> pybind11 模块（.pyd）且依赖 vcpkg 安装的 arrow/parquet（MSVC 编译），MinGW 不适用。Linux 构建见 § 2.3。

### 2.2 命令行 · MSVC（cmd）

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\09_parquet_and_dll

# 生成测试数据
D:\ProgramData\anaconda3\envs\quant311\python gen_test_data.py

# 编译 + 拷贝 pyd 和依赖 dll 到 lib/
D:\ProgramData\anaconda3\envs\quant311\python build.py --dest lib

# 验证
D:\ProgramData\anaconda3\envs\quant311\python test_parquet.py
```

</details>

> `build.py` 内部传入 vcpkg toolchain 路径，自动定位 MSVC + pybind11 + Python。
> 内部通过 `vswhere` 自动检测 VS 版本，使用 **VS Generator（方案 B，多配置）**，构建时传 `--config Release`。

### 2.3 命令行 · Linux / WSL

```bash
# cd 到项目目录
cd 09_parquet_and_dll

# 生成测试数据
python gen_test_data.py

# 编译 + 拷贝
python build.py --dest lib

# 验证
python test_parquet.py
```

> pybind11 模块（.so）同样必须使用与 Python 相同的编译器。Linux 上的 Python 由 GCC 编译，因此 .so 用 GCC 构建即可。arrow/parquet
> 依赖通过系统包管理器或 vcpkg 安装，不需要手动拷贝 dll。

---

## 3. CLion IDE

1. `File → Open` 选择 `09_parquet_and_dll/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. CMake Options 中加
   `-DCMAKE_TOOLCHAIN_FILE=D:/software/vcpkg/scripts/buildsystems/vcpkg.cmake -Dpybind11_DIR=... -DPython_EXECUTABLE=...`
4. **构建** `Ctrl+F9`

---

## 4. 本步要点

### 4.1 与第八步的区别

|      | 第八步 data_provider | 第九步（本步）                   |
|:-----|:------------------|:--------------------------|
| 数据来源 | Python 构造后传入 C++  | C++ 自己读 parquet 文件        |
| 依赖   | 零依赖               | arrow/parquet（vcpkg）      |
| 拷贝   | 只拷贝 .pyd          | .pyd + 依赖 dll 一起拷贝        |
| 对应框架 | data_mgrs（内存查询层）  | build_engine.py（dll 部署机制） |

### 4.2 为什么必须拷贝 dll？

```python
import parquet_reader   # ← 失败: DLL load failed
```

.pyd 是 Python 的动态库，但它内部链接了 `arrow.dll`、`parquet.dll` 等。Windows 加载 .pyd 时会去**同目录**找这些 dll，找不到就
import 失败。MSBuild 编译时把依赖 dll 部署到了 build_py/Release/ 下，所以拷贝时必须一起拷走。

本步实测：拷贝了 **11 个依赖 dll**（arrow / parquet / boost / brotli / zstd 等）。

### 4.3 vcpkg toolchain

```cmake
# CMakeLists.txt 中不写 vcpkg 路径，由 build.py 传参
-DCMAKE_TOOLCHAIN_FILE=D:/software/vcpkg/scripts/buildsystems/vcpkg.cmake
```

框架的 build_engine.py 用 `VCPKG_ROOT` 环境变量定位，本步用 `--vcpkg` 参数（默认 `D:/software/vcpkg`）。

### 4.4 arrow API 注意（从编译踩坑总结）

| 坑                                   | 解决                                                               |
|:------------------------------------|:-----------------------------------------------------------------|
| `parquet::arrow::OpenFile` 3 参数旧版签名 | 新版是 2 参数，返回 `Result<unique_ptr<FileReader>>`，用 `.ValueOrDie()` 取 |
| `arrow::compute::Cast` 找不到          | 需 `#include <arrow/compute/api.h>`                               |
| 列类型混合（timestamp/float/int/string）   | 用 `type_id()` 分派，分别转 int64 纳秒 / double / string                  |

---

## 5. 与本机框架的对应

| 本步                              | 框架                                                                  |
|:--------------------------------|:--------------------------------------------------------------------|
| `build.py` 的 toolchain + dll 拷贝 | `pycpp_quant/engine_cpp/build_engine.py` 的 `copy_dynamic_library()` |
| `parquet_reader.cpp` 的读表逻辑      | `quant_engine/src/common/bar_data_reader.cpp`                       |
| vcpkg 的 arrow/parquet           | 同一份 `D:/software/vcpkg` 安装                                          |
