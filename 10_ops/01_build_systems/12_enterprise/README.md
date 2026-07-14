# 12_enterprise — 企业级多模块项目

`project_options` 接口库统一选项，`add_subdirectory` 管理依赖链，ctest 集成测试

> 工具完整路径见 [父目录 README](../README.md)

---

## 1. 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"
MAKE="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"
```

### 方案 A：Ninja（推荐）

```bash
# ─── Debug ────────────────────────────────────────────────────────
# 配置
"$CMAKE" -B build-mingw-ninja -G Ninja \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA" \
  -DCMAKE_BUILD_TYPE=Debug

# 构建
"$CMAKE" --build build-mingw-ninja

# 运行主程序
./build-mingw-ninja/app/enterprise_app.exe

# 运行 CTest
ctest --test-dir build-mingw-ninja --output-on-failure

# ─── ASan（内存错误检测，基于 Debug）─────────────────────────────
# 配置
"$CMAKE" -B build-mingw-ninja-asan -G Ninja \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DENABLE_ASAN=ON

# 构建
"$CMAKE" --build build-mingw-ninja-asan

# 运行主程序
./build-mingw-ninja-asan/app/enterprise_app.exe
```

> cmd 三步版：

```bat
:: ─── Debug ────────────────────────────────────────────────────────
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe" -DCMAKE_BUILD_TYPE=Debug

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 运行主程序
build-mingw-ninja\app\enterprise_app.exe

:: 运行 CTest
ctest --test-dir build-mingw-ninja --output-on-failure

:: ─── ASan ─────────────────────────────────────────────────────────
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja-asan -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe" -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja-asan

:: 运行主程序
build-mingw-ninja-asan\app\enterprise_app.exe
```

### 方案 B：MinGW Makefiles（无需 ninja.exe）

```bash
# 配置
"$CMAKE" -B build-mingw-make -G "MinGW Makefiles" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$MAKE" \
  -DCMAKE_BUILD_TYPE=Debug

# 构建
"$CMAKE" --build build-mingw-make

# 运行主程序
./build-mingw-make/app/enterprise_app.exe

# 运行 CTest
ctest --test-dir build-mingw-make --output-on-failure
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-make -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe" -DCMAKE_BUILD_TYPE=Debug

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-make

:: 运行主程序
build-mingw-make\app\enterprise_app.exe
```

### 方案 C：Ninja Multi-Config（一目录同时支持 Debug / Release）

```bash
# 配置
"$CMAKE" -B build-mingw-ninja-mc -G "Ninja Multi-Config" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA"

# 构建 Debug
"$CMAKE" --build build-mingw-ninja-mc --config Debug

# 运行 Debug
./build-mingw-ninja-mc/Debug/app/enterprise_app.exe

# 构建 Release
"$CMAKE" --build build-mingw-ninja-mc --config Release

# 运行 Release
./build-mingw-ninja-mc/Release/app/enterprise_app.exe
```

> cmd 三步版（以 Debug 为例）：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja-mc -G "Ninja Multi-Config" -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja-mc --config Debug
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja-mc --config Release

:: 运行
build-mingw-ninja-mc\Debug\app\enterprise_app.exe
build-mingw-ninja-mc\Release\app\enterprise_app.exe
```

---

## 2. 命令行 · MSVC（cmd）

### 方案 A：vcvarsall + Ninja（推荐）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

:: 激活 — 把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "%VCVARSALL%" x64

:: Debug — 配置
"%CMAKE%" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_BUILD_TYPE=Debug

:: Debug — 构建
"%CMAKE%" --build build-msvc-ninja

:: Debug — 运行主程序
build-msvc-ninja\app\enterprise_app.exe

:: Debug — 测试
ctest --test-dir build-msvc-ninja --output-on-failure

:: ASan — 配置
"%CMAKE%" -B build-msvc-ninja-asan -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON

:: ASan — 构建
"%CMAKE%" --build build-msvc-ninja-asan

:: ASan — 运行主程序
build-msvc-ninja-asan\app\enterprise_app.exe
```

> 四步版（激活、配置、构建、运行各一行）：

```bat
:: 激活 — 把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: Debug — 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_BUILD_TYPE=Debug

:: Debug — 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja

:: Debug — 运行主程序
build-msvc-ninja\app\enterprise_app.exe

:: Debug — 测试（激活已完成，直接继续）
ctest --test-dir build-msvc-ninja --output-on-failure

:: ASan — 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja-asan -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON

:: ASan — 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja-asan

:: ASan — 运行主程序
build-msvc-ninja-asan\app\enterprise_app.exe
```

### 方案 B：Visual Studio Generator（无需激活 vcvarsall）

CMake 自动检测 MSVC 工具链；Generator 是多配置，构建时须指定 `--config`，exe 输出到 `build-msvc-vs\Debug\`。`-A x64` 是 VS Generator 专用的目标平台参数（x64 = 64 位），不加默认为 Win32（32 位）；Ninja Generator 不支持此参数，目标架构由 `vcvarsall.bat x64` 决定。

**CMake 怎么找到 MSVC 工具链：**

1. **`vswhere.exe`**（主要手段）：VS 安装时在固定位置放置 `C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe`，CMake 启动时调用它查询所有已安装的 VS 实例路径和版本。
2. **Windows 注册表**：VS 安装时写入注册表，CMake 作为备用查询手段。

**为什么 VS Generator 不需要 vcvarsall，Ninja 却需要：**

| | VS Generator | Ninja Generator |
|---|---|---|
| CMake 生成的产物 | `.sln` / `.vcxproj` | `build.ninja` |
| 实际构建由谁执行 | MSBuild | ninja → 直接调 cl.exe |
| 编译器环境由谁设置 | MSBuild 通过 vswhere 自行配置 | 无人设置，依赖调用方提前准备 |
| 需要 vcvarsall | 不需要 | 需要 |

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe

:: 配置（无需激活，CMake 自动找到 MSVC；-A x64 指定目标 64 位）
"%CMAKE%" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64

:: 构建（多配置，必须指定 --config）
"%CMAKE%" --build build-msvc-vs --config Debug

:: 运行主程序
build-msvc-vs\Debug\app\enterprise_app.exe
```

> 三步版：

```bat
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-vs --config Debug

:: 运行主程序
build-msvc-vs\Debug\app\enterprise_app.exe
```

**`-A` 可选值：**

| 值 | 说明 |
|---|---|
| `x64` | 64 位 x86（Intel/AMD），现代 Windows 项目标准选择 |
| `Win32` | 32 位 x86，不指定 `-A` 时的默认值 |
| `ARM` | ARM 32 位，嵌入式 / 旧版 Windows on ARM |
| `ARM64` | ARM 64 位，Surface Pro X、新款 ARM PC |
| `ARM64EC` | ARM64 兼容模式（可混合加载 x64 DLL），Windows 11 过渡期使用 |

**VS Generator 专用参数：**

| 参数 | 说明 |
|---|---|
| `-G "Visual Studio 18 2026"` | 指定 Generator，版本号须与本机 VS 一致 |
| `-A x64` | 目标平台；不指定默认 Win32（32 位）；可选值：`x64`、`Win32`、`ARM`、`ARM64`、`ARM64EC` |
| `--config Debug/Release` | 构建时指定配置，VS Generator 是多配置，**不用** `-DCMAKE_BUILD_TYPE` |

其他通用配置参数（`-DCMAKE_INSTALL_PREFIX`、`-DCMAKE_TOOLCHAIN_FILE`、`-DCMAKE_EXPORT_COMPILE_COMMANDS` 等）见 [cmake_syntax.md § 2.7.1](../cmake_syntax.md)。

**生成的 build 目录中各 `.vcxproj` 的作用：**

| 文件 | 作用 |
|---|---|
| `enterprise_app.vcxproj` | 主可执行程序，对应 `add_executable(enterprise_app ...)` |
| `utils.vcxproj` | utils 静态库，对应 `add_library(utils STATIC ...)` |
| `core.vcxproj` | core 静态库，对应 `add_library(core STATIC ...)`，依赖 utils |
| `test_utils.vcxproj` | utils 单元测试，对应 `add_executable(test_utils ...)` |
| `test_engine.vcxproj` | core 单元测试，对应 `add_executable(test_engine ...)` |
| `ALL_BUILD.vcxproj` | 构建所有目标，等价于 `cmake --build`，VS 里默认 Build 触发的就是它 |
| `ZERO_CHECK.vcxproj` | 监控 `CMakeLists.txt` 修改时间；检测到变化时自动重新运行 cmake 配置，刷新 `.vcxproj`，无需手动重跑 cmake |

用 VS 打开工程：双击 `enterprise.slnx`（VS 2022 17.x+ 的新格式）或 `enterprise.sln`，解决方案加载后各项目均可见。

### 方案 C：x64 Native Tools Command Prompt + Ninja（无需激活 vcvarsall）

开始菜单搜索 `x64 Native Tools Command Prompt for VS 2026` 打开。窗口启动时自动运行激活脚本（效果等同于 `call vcvarsall.bat x64`），已把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH。进去后三步运行：

```bat
:: Debug — 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_BUILD_TYPE=Debug

:: Debug — 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja

:: Debug — 运行主程序
build-msvc-ninja\app\enterprise_app.exe

:: Debug — 测试
ctest --test-dir build-msvc-ninja --output-on-failure

:: ASan — 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja-asan -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON

:: ASan — 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja-asan

:: ASan — 运行主程序
build-msvc-ninja-asan\app\enterprise_app.exe
```

---

## 3. 对比

| | MinGW 方案 A<br>Ninja | MinGW 方案 B<br>MinGW Makefiles | MinGW 方案 C<br>Ninja Multi-Config |
|---|---|---|---|
| build 目录 | `build-mingw-ninja` | `build-mingw-make` | `build-mingw-ninja-mc` |
| ASan 变体 | `build-mingw-ninja-asan`<br>（`-DENABLE_ASAN=ON`） | — | — |
| 构建工具 | ninja.exe | mingw32-make.exe | ninja.exe |
| 配置数 | 单配置 | 单配置 | 多配置（Debug/Release 共目录） |
| 速度 | 最快 | 慢 | 快 |
| exe 路径 | `build-mingw-ninja\app\` | `build-mingw-make\app\` | `build-mingw-ninja-mc\Debug\app\` |
| 适用场景 | 日常首选 | 没有 ninja.exe 时备选 | 需频繁切换 Debug/Release |

| | MSVC 方案 A<br>vcvarsall + Ninja | MSVC 方案 B<br>VS Generator | MSVC 方案 C<br>Native Tools + Ninja |
|---|---|---|---|
| build 目录 | `build-msvc-ninja` | `build-msvc-vs` | `build-msvc-ninja` |
| ASan 变体 | `build-msvc-ninja-asan`<br>（`-DENABLE_ASAN=ON`） | — | `build-msvc-ninja-asan`<br>（`-DENABLE_ASAN=ON`） |
| 需要激活 | 是（call vcvarsall） | 否 | 否（窗口自动激活） |
| 构建工具 | ninja.exe | msbuild.exe | ninja.exe |
| 配置数 | 单配置 | 多配置（--config） | 单配置 |
| exe 路径 | `build-msvc-ninja\app\` | `build-msvc-vs\Debug\app\` | `build-msvc-ninja\app\` |
| 适用场景 | 脚本/自动化首选 | 不想手动激活 | 交互式操作 |

---

## 4. CLion IDE

1. `File → Open` 选择 `12_enterprise/` 目录
2. CLion 递归加载所有子目录的 `CMakeLists.txt`，右下角点击 **加载**
3. **Run/Debug Configurations** 下拉列表列出：`enterprise_app`、`test_engine`、`test_utils`
4. 执行所有测试：菜单 **Run → Run 'All CTest'**，Test Runner 窗口显示各模块测试结果
5. 开启 ASan：在 Profile 的 **CMake options** 里加 `-DENABLE_ASAN=ON`，重新配置后运行即可

> build 目录各文件的作用见 [cmake_syntax.md § 3. build 目录文件说明](../cmake_syntax.md)。

### 目录结构

```
12_enterprise/
├── CMakeLists.txt          # 顶层：定义 project_options，add_subdirectory
├── utils/
│   ├── CMakeLists.txt
│   ├── include/utils/
│   └── src/
├── core/
│   ├── CMakeLists.txt      # 依赖 utils
│   ├── include/core/
│   └── src/
├── app/
│   ├── CMakeLists.txt      # 依赖 core（PUBLIC 传播自动拉入 utils）
│   └── main.cpp
└── tests/
    ├── CMakeLists.txt
    ├── test_utils.cpp
    └── test_engine.cpp
```

### 架构要点

| 要点 | 说明 |
|------|------|
| `project_options` 接口库 | 以 `INTERFACE` 属性集中声明警告、标准、ASan 等选项，所有子目标 `target_link_libraries(... project_options)` 继承，避免 `target_compile_options` 全局污染 |
| `PUBLIC` 传播依赖链 | `core` 以 `PUBLIC` 链接 `utils`，`app` 链接 `core` 后自动获得 `utils` 的头文件和链接，无需手动重复声明 |
| 按模块拆分测试 | `tests/` 下各测试目标只链接被测模块，隔离性好，CTest 可按 `-L` 标签并行运行 |
