# 01_build_systems — C++ 构建系统演进

从最原始的手动编译，到现代主流的 CMake + vcpkg 全流程。

每个子目录是独立工程，在 CLion 中直接打开对应目录即可构建（CMake 阶段起）。

---

## 1. 上游主流版本参考（截至 2025–2026）

| 工具                | 上游最新稳定                     | 备注                                                |
|-------------------|----------------------------|---------------------------------------------------|
| **C++ 标准**        | C++23                      | C++17 最广泛；C++20 快速普及；GCC 15 默认仍是 `-std=gnu++17`   |
| **g++ / GCC**     | 15.1（2025-04-25）           | 14.3 长期维护；15.x 大幅推进 C++23 支持                      |
| **CMake**         | 4.4.0（2026-07-10）          | 4.0 起不再兼容 `cmake_minimum_required(VERSION < 3.5)` |
| **Ninja**         | 1.13.2（2025-11-20）         |                                                   |
| **cl.exe / MSVC** | VS 2026 v18 / toolset v145 | VS 2022 v17 (v143) 仍是工程主流；VS 2026 起 MSVC 版本号独立    |

> 本地实际版本见下方"工具路径"章节。对照差距可评估是否需要升级。

### 1.1 C++ 标准 × 编译器版本兼容表

#### 1.1.1 GCC

| C++ 标准 | 完整支持最低 GCC        | 编译标志         | 说明                         |
|--------|-------------------|--------------|----------------------------|
| C++11  | GCC 5             | `-std=c++11` |                            |
| C++14  | GCC 6             | `-std=c++14` |                            |
| C++17  | GCC 8             | `-std=c++17` |                            |
| C++20  | GCC 16（稳定 ABI）    | `-std=c++20` | GCC 8 起实验性支持；GCC 16 起为默认标准 |
| C++23  | 实验性（GCC 13+ 逐步改善） | `-std=c++23` |                            |
| C++26  | GCC 14 起实验性       | `-std=c++26` |                            |

#### 1.1.2 MSVC

| C++ 标准 | 最低 VS 版本                | `_MSC_VER` | 编译标志                |
|--------|-------------------------|------------|---------------------|
| C++14  | VS 2015 Update 3（默认）    | ≥ 1900     | `/std:c++14`        |
| C++17  | VS 2017                 | ≥ 1910     | `/std:c++17`        |
| C++20  | VS 2019 16.11           | ≥ 1929     | `/std:c++20`        |
| C++23  | VS 2022 17.13+（preview） | ≥ 1943     | `/std:c++23preview` |

#### 1.1.3 MSVC `_MSC_VER` 速查

| Visual Studio | 版本号 | Toolset | `_MSC_VER` |
|---------------|-----|---------|------------|
| VS 2015       | 14  | v140    | 1900       |
| VS 2017       | 15  | v141    | 1910–1916  |
| VS 2019       | 16  | v142    | 1920–1929  |
| VS 2022       | 17  | v143    | 1930–194x  |
| VS 2026       | 18  | v145    | 1950+      |

### 1.2 CMake × 编译器兼容

| CMake 版本    | 要求                                          | 说明                          |
|-------------|---------------------------------------------|-----------------------------|
| CMake ≥ 4.0 | 项目须 `cmake_minimum_required(VERSION ≥ 3.5)` | 首次移除旧 policy                |
| 任意 CMake    | MSVC 最低 VS 2010（`_MSC_VER` 1600）            | CMake compile-features 支持起点 |
| 任意 CMake    | GCC 最低 4.4                                  | CMake compile-features 支持起点 |

---

## 2. 工具路径（完整路径，不依赖 PATH）

所有命令行示例均使用以下完整路径，无需修改系统 PATH。

### 2.1 工具链 A · MinGW（CLion 内置）

| 工具        | 完整路径                                                                     |
|-----------|--------------------------------------------------------------------------|
| g++.exe   | `D:\ProgramData\JetBrains\CLion20260101\bin\mingw\bin\g++.exe`           |
| cmake.exe | `D:\ProgramData\JetBrains\CLion20260101\bin\cmake\win\x64\bin\cmake.exe` |
| ninja.exe | `D:\ProgramData\JetBrains\CLion20260101\bin\ninja\win\x64\ninja.exe`     |

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

| 工具            | 完整路径                                                                                                                     |
|---------------|--------------------------------------------------------------------------------------------------------------------------|
| cl.exe        | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\cl.exe`                 |
| link.exe      | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\link.exe`               |
| cmake.exe     | `D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe` |
| ninja.exe     | `D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe`     |
| vcvarsall.bat | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat`                                 |

**cl.exe 必须通过 vcvarsall.bat 激活环境后才能使用**（头文件、库路径、link.exe 路径均由它注入）。不激活直接用完整路径调
cl.exe 会报"找不到头文件"——vcvarsall 不止把工具加入 PATH，还注入了 `INCLUDE`、`LIB`、`LIBPATH` 等环境变量。

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
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja -G Ninja

"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja

build-msvc-ninja\hello_cmake.exe
```

**替代方案 B：改用 Visual Studio Generator（CMake 自动检测工具链）**

不用 `-G Ninja`，改用 VS Generator，CMake 自行找到 MSVC 工具链，无需任何激活步骤：

```bat
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64

:: 构建（VS Generator 是多配置，须指定 --config）
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-vs --config Debug

:: 运行（exe 输出到 build-msvc-vs\Debug\ 下）
build-msvc-vs\Debug\hello_cmake.exe
```

> VS Generator 与 Ninja 的区别：VS Generator 生成 `.sln` / `.vcxproj`，支持多配置（Debug/Release 共用同一个 build 目录）；Ninja
> 是单配置，速度更快，是 CLion 默认选择。

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

| 阶段            | 目录                                                 | 核心知识点                                                        |
|---------------|----------------------------------------------------|--------------------------------------------------------------|
| **原始**        | [00_compiler_cli](00_compiler_cli/README.md)       | 直接调用 g++/cl.exe，理解编译器在做什么                                    |
| **Makefile**  | [01_makefile](01_makefile/README.md)               | 手写规则、模式匹配、增量编译、依赖追踪                                          |
| **CMake 入门**  | [02_hello](02_hello/README.md)                     | 最简工程：`cmake_minimum_required` + `project` + `add_executable` |
|               | [03_sources](03_sources/README.md)                 | 多源文件：显式列举 + `target_include_directories`                     |
|               | [04_library](04_library/README.md)                 | 静态库与动态库：`add_library(STATIC/SHARED)`                         |
| **CMake 现代化** | [05_targets](05_targets/README.md)                 | target 属性传播：`PRIVATE / PUBLIC / INTERFACE`，接口库               |
|               | [06_compile_options](06_compile_options/README.md) | 编译选项与宏：跨编译器生成器表达式                                            |
|               | [07_find_package](07_find_package/README.md)       | 查找依赖：Module 模式 vs Config 模式，可选依赖                             |
| **包管理**       | [08_vcpkg](08_vcpkg/README.md)                     | vcpkg 集成：`vcpkg.json` 声明，toolchain 注入                        |
| **工程质量**      | [09_install](09_install/README.md)                 | 安装规则：`GNUInstallDirs` + 导出 CMake 包配置                         |
|               | [10_ctest](10_ctest/README.md)                     | 测试集成：`enable_testing` + `add_test` + 标签/超时                   |
| **高阶**        | [11_generator_expr](11_generator_expr/README.md)   | 生成器表达式：`$<CONFIG>` / `$<CXX_COMPILER_ID>`                    |
|               | [12_presets](12_presets/README.md)                 | CMakePresets.json：debug / release / asan 多配置预设               |
| **企业级**       | [13_enterprise](13_enterprise/README.md)           | 多模块：`project_options` + `add_subdirectory` 依赖链 + ctest       |

---

## 5. 通用构建流程（本章所有 CMake 目录共用）

> 本章每个 CMake 知识目录的 README 不再重复整套构建命令，统一在这里展开一次。使用步骤：① 把 `<exe_name>` 换成该目录的可执行名（各目录 README 的「本目录差异」小节给出）；② 其余命令通用。以下是 02_hello 验证过的六套方案（MinGW 三套 + MSVC 三套）与对比：

### 5.1 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"
MAKE="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"
```

#### 方案 A：Ninja（推荐）

```bash
# 配置
"$CMAKE" -B build-mingw-ninja -G Ninja \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA"

# 构建
"$CMAKE" --build build-mingw-ninja

# 运行
./build-mingw-ninja/<exe_name>.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 运行
build-mingw-ninja\<exe_name>.exe
```

#### 方案 B：MinGW Makefiles（无需 ninja.exe）

```bash
# 配置
"$CMAKE" -B build-mingw-make -G "MinGW Makefiles" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$MAKE"

# 构建
"$CMAKE" --build build-mingw-make

# 运行
./build-mingw-make/<exe_name>.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-make -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-make

:: 运行
build-mingw-make\<exe_name>.exe
```

#### 方案 C：Ninja Multi-Config（一目录同时支持 Debug / Release）

```bash
# 配置
"$CMAKE" -B build-mingw-ninja-mc -G "Ninja Multi-Config" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA"

# 构建 Debug
"$CMAKE" --build build-mingw-ninja-mc --config Debug

# 运行 Debug
./build-mingw-ninja-mc/Debug/<exe_name>.exe

# 构建 Release
"$CMAKE" --build build-mingw-ninja-mc --config Release

# 运行 Release
./build-mingw-ninja-mc/Release/<exe_name>.exe
```

> cmd 三步版（以 Debug 为例）：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja-mc -G "Ninja Multi-Config" -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja-mc --config Debug
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja-mc --config Release

:: 运行
build-mingw-ninja-mc\Debug\<exe_name>.exe
build-mingw-ninja-mc\Release\<exe_name>.exe
```

---

### 5.2 命令行 · MSVC（cmd）

#### 方案 A：vcvarsall + Ninja（推荐）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

:: 激活 — 把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "%VCVARSALL%" x64

:: 配置
"%CMAKE%" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"%CMAKE%" --build build-msvc-ninja

:: 运行
build-msvc-ninja\<exe_name>.exe
```

> 四步版（激活、配置、构建、运行各一行）：

```bat
:: 激活 — 把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja

:: 运行
build-msvc-ninja\<exe_name>.exe
```

#### 方案 B：Visual Studio Generator（无需激活 vcvarsall）

CMake 自动检测 MSVC 工具链；Generator 是多配置，构建时须指定 `--config`，exe 输出到 `build-msvc-vs\Debug\`。`-A x64` 是 VS
Generator 专用的目标平台参数（x64 = 64 位），不加默认为 Win32（32 位）；Ninja Generator 不支持此参数，目标架构由
`vcvarsall.bat x64` 决定。

**CMake 怎么找到 MSVC 工具链：**

1. **`vswhere.exe`**（主要手段）：VS 安装时在固定位置放置
   `C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe`，CMake 启动时调用它查询所有已安装的 VS 实例路径和版本。
2. **Windows 注册表**：VS 安装时写入注册表，CMake 作为备用查询手段。

**为什么 VS Generator 不需要 vcvarsall，Ninja 却需要：**

|              | VS Generator            | Ninja Generator    |
|--------------|-------------------------|--------------------|
| CMake 生成的产物  | `.sln` / `.vcxproj`     | `build.ninja`      |
| 实际构建由谁执行     | MSBuild                 | ninja → 直接调 cl.exe |
| 编译器环境由谁设置    | MSBuild 通过 vswhere 自行配置 | 无人设置，依赖调用方提前准备     |
| 需要 vcvarsall | 不需要                     | 需要                 |

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe

:: 配置（无需激活，CMake 自动找到 MSVC；-A x64 指定目标 64 位）
"%CMAKE%" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64

:: 构建（多配置，必须指定 --config）
"%CMAKE%" --build build-msvc-vs --config Debug

:: 运行
build-msvc-vs\Debug\<exe_name>.exe
```

> 三步版：

```bat
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-vs --config Debug
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Debug\<exe_name>.exe
build-msvc-vs\Release\<exe_name>.exe
```

**`-A` 可选值：**

| 值         | 说明                                         |
|-----------|--------------------------------------------|
| `x64`     | 64 位 x86（Intel/AMD），现代 Windows 项目标准选择      |
| `Win32`   | 32 位 x86，不指定 `-A` 时的默认值                    |
| `ARM`     | ARM 32 位，嵌入式 / 旧版 Windows on ARM           |
| `ARM64`   | ARM 64 位，Surface Pro X、新款 ARM PC           |
| `ARM64EC` | ARM64 兼容模式（可混合加载 x64 DLL），Windows 11 过渡期使用 |

**VS Generator 专用参数：**

| 参数                           | 说明                                                               |
|------------------------------|------------------------------------------------------------------|
| `-G "Visual Studio 18 2026"` | 指定 Generator，版本号须与本机 VS 一致                                       |
| `-A x64`                     | 目标平台；不指定默认 Win32（32 位）；可选值：`x64`、`Win32`、`ARM`、`ARM64`、`ARM64EC` |
| `--config Debug/Release`     | 构建时指定配置，VS Generator 是多配置，**不用** `-DCMAKE_BUILD_TYPE`            |

其他通用配置参数（`-DCMAKE_INSTALL_PREFIX`、`-DCMAKE_TOOLCHAIN_FILE`、`-DCMAKE_EXPORT_COMPILE_COMMANDS`
等）见 [cmake_syntax.md § 0.1 配置阶段](cmake_syntax.md)。

**生成的 build 目录中三个 `.vcxproj` 的作用：**

| 文件                    | 作用                                                                        |
|-----------------------|---------------------------------------------------------------------------|
| `<exe_name>.vcxproj` | 主项目，对应 `add_executable(<exe_name> ...)`                                  |
| `ALL_BUILD.vcxproj`   | 构建所有目标，等价于 `cmake --build`，VS 里默认 Build 触发的就是它                            |
| `ZERO_CHECK.vcxproj`  | 监控 `CMakeLists.txt` 修改时间；检测到变化时自动重新运行 cmake 配置，刷新 `.vcxproj`，无需手动重跑 cmake |

用 VS 打开工程：双击 `<exe_name>.slnx`（VS 2022 17.x+ 的新格式）或 `<exe_name>.sln`，解决方案加载后三个项目均可见。

#### 方案 C：x64 Native Tools Command Prompt + Ninja（无需激活 vcvarsall）

开始菜单搜索 `x64 Native Tools Command Prompt for VS 2026` 打开。窗口启动时自动运行激活脚本（效果等同于
`call vcvarsall.bat x64`），已把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB /
LIBPATH。进去后三步运行：

```bat
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja

:: 运行
build-msvc-ninja\<exe_name>.exe
```

---

### 5.3 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/<exe_name>
```

---

### 5.4 方案对比

|          | MinGW 方案 A<br>Ninja | MinGW 方案 B<br>MinGW Makefiles | MinGW 方案 C<br>Ninja Multi-Config |
|----------|---------------------|-------------------------------|----------------------------------|
| build 目录 | `build-mingw-ninja` | `build-mingw-make`            | `build-mingw-ninja-mc`           |
| 构建工具     | ninja.exe           | mingw32-make.exe              | ninja.exe                        |
| 配置数      | 单配置                 | 单配置                           | 多配置（Debug/Release 共目录）           |
| 速度       | 最快                  | 慢                             | 快                                |
| 适用场景     | 日常首选                | 没有 ninja.exe 时备选              | 需频繁切换 Debug/Release              |

|          | MSVC 方案 A<br>vcvarsall + Ninja | MSVC 方案 B<br>VS Generator | MSVC 方案 C<br>Native Tools + Ninja |
|----------|--------------------------------|---------------------------|-----------------------------------|
| build 目录 | `build-msvc-ninja`             | `build-msvc-vs`           | `build-msvc-ninja`                |
| 需要激活     | 是（call vcvarsall）              | 否                         | 否（窗口自动激活）                         |
| 构建工具     | ninja.exe                      | msbuild.exe               | ninja.exe                         |
| 配置数      | 单配置                            | 多配置（--config）             | 单配置                               |
| exe 路径   | `build-msvc-ninja\`            | `build-msvc-vs\Debug\`    | `build-msvc-ninja\`               |
| 适用场景     | 脚本/自动化首选                       | 不想手动激活                    | 交互式操作                             |

---

### 5.5 CLion IDE

1. `File → Open` 选择 <本目录>/ 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击 **加载**
3. 工具栏选择工具链（MinGW 或 Visual Studio）和构建类型
4. **构建** `Ctrl+F9`**运行** `Shift+F10`

> build 目录各文件的作用见 [cmake_syntax.md § 3. build 目录文件说明](cmake_syntax.md)。

> 各目录 README 只保留：本目录差异（exe 名 / build 目录 / 专属 `-D` 参数 / ctest・install・preset 等专属命令）与独有知识点。
