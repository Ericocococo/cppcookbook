# 01_build_systems — C++ 构建系统演进

从最原始的手动编译，到现代主流的 CMake + vcpkg 全流程。

每个子目录是独立工程，在 CLion 中直接打开对应目录即可构建（CMake 阶段起）。

---

## 1. 上游主流版本参考（截至 2025–2026）

| 工具 | 上游最新稳定 | 备注 |
|------|-------------|------|
| **C++ 标准** | C++23 | C++17 最广泛；C++20 快速普及；GCC 15 默认仍是 `-std=gnu++17` |
| **g++ / GCC** | 15.1（2025-04-25） | 14.3 长期维护；15.x 大幅推进 C++23 支持 |
| **CMake** | 4.4.0（2026-07-10） | 4.0 起不再兼容 `cmake_minimum_required(VERSION < 3.5)` |
| **Ninja** | 1.13.2（2025-11-20） | |
| **cl.exe / MSVC** | VS 2026 v18 / toolset v145 | VS 2022 v17 (v143) 仍是工程主流；VS 2026 起 MSVC 版本号独立 |

> 本地实际版本见下方"工具路径"章节。对照差距可评估是否需要升级。

### 1.1 C++ 标准 × 编译器版本兼容表

#### 1.1.1 GCC

| C++ 标准 | 完整支持最低 GCC | 编译标志 | 说明 |
|---------|----------------|---------|------|
| C++11   | GCC 5          | `-std=c++11` | |
| C++14   | GCC 6          | `-std=c++14` | |
| C++17   | GCC 8          | `-std=c++17` | |
| C++20   | GCC 16（稳定 ABI）| `-std=c++20` | GCC 8 起实验性支持；GCC 16 起为默认标准 |
| C++23   | 实验性（GCC 13+ 逐步改善）| `-std=c++23` | |
| C++26   | GCC 14 起实验性 | `-std=c++26` | |

#### 1.1.2 MSVC

| C++ 标准 | 最低 VS 版本 | `_MSC_VER` | 编译标志 |
|---------|------------|-----------|---------|
| C++14   | VS 2015 Update 3（默认）| ≥ 1900 | `/std:c++14` |
| C++17   | VS 2017    | ≥ 1910  | `/std:c++17` |
| C++20   | VS 2019 16.11 | ≥ 1929 | `/std:c++20` |
| C++23   | VS 2022 17.13+（preview）| ≥ 1943 | `/std:c++23preview` |

#### 1.1.3 MSVC `_MSC_VER` 速查

| Visual Studio | 版本号 | Toolset | `_MSC_VER` |
|--------------|-------|---------|-----------|
| VS 2015      | 14    | v140    | 1900       |
| VS 2017      | 15    | v141    | 1910–1916  |
| VS 2019      | 16    | v142    | 1920–1929  |
| VS 2022      | 17    | v143    | 1930–194x  |
| VS 2026      | 18    | v145    | 1950+      |

### 1.2 CMake × 编译器兼容

| CMake 版本 | 要求 | 说明 |
|-----------|------|------|
| CMake ≥ 4.0 | 项目须 `cmake_minimum_required(VERSION ≥ 3.5)` | 首次移除旧 policy |
| 任意 CMake | MSVC 最低 VS 2010（`_MSC_VER` 1600） | CMake compile-features 支持起点 |
| 任意 CMake | GCC 最低 4.4 | CMake compile-features 支持起点 |

---

## 2. 工具路径（完整路径，不依赖 PATH）

所有命令行示例均使用以下完整路径，无需修改系统 PATH。

### 2.1 工具链 A · MinGW（CLion 内置）

| 工具 | 完整路径 |
|------|----------|
| g++.exe | `D:\ProgramData\JetBrains\CLion20260101\bin\mingw\bin\g++.exe` |
| cmake.exe | `D:\ProgramData\JetBrains\CLion20260101\bin\cmake\win\x64\bin\cmake.exe` |
| ninja.exe | `D:\ProgramData\JetBrains\CLion20260101\bin\ninja\win\x64\ninja.exe` |

**Git Bash 里定义变量（可粘贴到 ~/.bashrc 长期生效）：**

```bash
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

# 验证
"$GXX"   --version   # → GCC 13.1.0
"$CMAKE" --version   # → cmake version 4.2.2
"$NINJA" --version   # → 1.13.2
```

---

### 2.2 工具链 B · MSVC（VS 自带）

| 工具 | 完整路径 |
|------|----------|
| cl.exe | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\cl.exe` |
| link.exe | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\link.exe` |
| cmake.exe | `D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe` |
| ninja.exe | `D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe` |
| vcvarsall.bat | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat` |

**cl.exe 必须通过 vcvarsall.bat 激活环境后才能使用**（头文件、库路径、link.exe 路径均由它注入）。不激活直接用完整路径调 cl.exe 会报"找不到头文件"——vcvarsall 不止把工具加入 PATH，还注入了 `INCLUDE`、`LIB`、`LIBPATH` 等环境变量。

**cmd 里定义变量：**

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

:: 激活 MSVC 环境（当前 cmd 会话有效，cl/link/ninja 均自动加入 PATH）
call "%VCVARSALL%" x64

:: 验证
cl              & :: → Microsoft (R) C/C++ Optimizing Compiler Version 19.51...
"%CMAKE%" --version & :: → cmake version 4.3.1-msvc1
```

**不想手动 call vcvarsall 的两种替代方案：**

**替代方案 A：用 VS 自带的 x64 Native Tools Command Prompt（推荐）**

开始菜单搜索 `x64 Native Tools Command Prompt for VS 2026` 打开，该窗口启动时已自动激活 MSVC 环境，进去后直接三步运行（无需激活步骤）：

```bat
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc -G Ninja

"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc

build-msvc\hello_cmake.exe
```

**替代方案 B：改用 Visual Studio Generator（CMake 自动检测工具链）**

不用 `-G Ninja`，改用 VS Generator，CMake 自行找到 MSVC 工具链，无需任何激活步骤：

```bat
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc -G "Visual Studio 18 2026" -A x64

:: 构建（VS Generator 是多配置，须指定 --config）
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc --config Debug

:: 运行（exe 输出到 build-msvc\Debug\ 下）
build-msvc\Debug\hello_cmake.exe
```

> VS Generator 与 Ninja 的区别：VS Generator 生成 `.sln` / `.vcxproj`，支持多配置（Debug/Release 共用同一个 build 目录）；Ninja 是单配置，速度更快，是 CLion 默认选择。

---

## 3. 版本号查看

### 3.1 工具链 A · MinGW（Git Bash）

```bash
"$GXX"    --version   # g++ / GCC 版本
"$CMAKE"  --version   # CMake 版本
"$NINJA"  --version   # Ninja 版本（仅输出版本号，如 1.13.2）
```

### 3.2 工具链 B · MSVC（cmd，须先激活环境）

```bat
:: 先激活
call "%VCVARSALL%" x64

cl            :: 首行：Microsoft (R) C/C++ Optimizing Compiler Version 19.51.xxxxx
link          :: 首行：Microsoft (R) Incremental Linker Version 14.51.xxxxx
"%CMAKE%" --version
ninja --version
```

> `cl` / `link` 不带参数会打印版本后以错误码退出，**版本信息在第一行，属正常现象**，忽略后续错误即可。

**从 `cl.exe` 版本提取 `_MSC_VER`**：输出格式为 `19.51.xxxxx`，去掉小数点取前四位 = `1951`。

---

## 4. 演进路线

> CMakeLists.txt 语法详解见 [cmake_syntax.md](cmake_syntax.md)

```
手动 g++/cl.exe  →  手写 Makefile  →  CMake 基础  →  CMake 现代化  →  CMake + vcpkg  →  企业级多模块
```

| 阶段 | 目录 | 核心知识点 |
|------|------|------------|
| **原始** | [00_compiler_cli](00_compiler_cli/README.md) | 直接调用 g++/cl.exe，理解编译器在做什么 |
| **Makefile** | [00_makefile](00_makefile/README.md) | 手写规则、模式匹配、增量编译、依赖追踪 |
| **CMake 入门** | [01_hello](01_hello/README.md) | 最简工程：`cmake_minimum_required` + `project` + `add_executable` |
| | [02_sources](02_sources/README.md) | 多源文件：显式列举 + `target_include_directories` |
| | [03_library](03_library/README.md) | 静态库与动态库：`add_library(STATIC/SHARED)` |
| **CMake 现代化** | [04_targets](04_targets/README.md) | target 属性传播：`PRIVATE / PUBLIC / INTERFACE`，接口库 |
| | [05_compile_options](05_compile_options/README.md) | 编译选项与宏：跨编译器生成器表达式 |
| | [06_find_package](06_find_package/README.md) | 查找依赖：Module 模式 vs Config 模式，可选依赖 |
| **包管理** | [07_vcpkg](07_vcpkg/README.md) | vcpkg 集成：`vcpkg.json` 声明，toolchain 注入 |
| **工程质量** | [08_install](08_install/README.md) | 安装规则：`GNUInstallDirs` + 导出 CMake 包配置 |
| | [09_ctest](09_ctest/README.md) | 测试集成：`enable_testing` + `add_test` + 标签/超时 |
| **高阶** | [10_generator_expr](10_generator_expr/README.md) | 生成器表达式：`$<CONFIG>` / `$<CXX_COMPILER_ID>` |
| | [11_presets](11_presets/README.md) | CMakePresets.json：debug / release / asan 多配置预设 |
| **企业级** | [12_enterprise](12_enterprise/README.md) | 多模块：`project_options` + `add_subdirectory` 依赖链 + ctest |
