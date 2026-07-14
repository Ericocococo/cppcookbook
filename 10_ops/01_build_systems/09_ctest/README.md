# 09_ctest — CTest 单元测试集成

`enable_testing()` + `add_test()` 接入 CTest

> 工具完整路径见 [父目录 README](../README.md)

**为什么需要 ctest？**

构建完会生成主程序和测试程序两类 exe，ctest 只管测试程序：

```
build/
├── app.exe           ← 主程序，直接运行
├── test_basic.exe    ← ctest 管理的测试程序
└── test_edge.exe     ← ctest 管理的测试程序
```

`ctest` 的价值在于：统一运行所有测试、统计通过/失败数、CI 里自动拦截失败。**随手验证直接运行 `test_basic.exe` 即可，ctest 是工程化/CI 阶段才用的。**

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
# 配置
"$CMAKE" -B build-mingw-ninja -G Ninja \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA"

# 构建
"$CMAKE" --build build-mingw-ninja

# 测试
ctest --test-dir build-mingw-ninja --output-on-failure

# 只运行 edge 标签
ctest --test-dir build-mingw-ninja --output-on-failure -L edge
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 测试
ctest --test-dir build-mingw-ninja --output-on-failure
ctest --test-dir build-mingw-ninja --output-on-failure -L edge
```

### 方案 B：MinGW Makefiles（无需 ninja.exe）

```bash
# 配置
"$CMAKE" -B build-mingw-make -G "MinGW Makefiles" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$MAKE"

# 构建
"$CMAKE" --build build-mingw-make

# 测试
ctest --test-dir build-mingw-make --output-on-failure
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-make -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-make

:: 测试
ctest --test-dir build-mingw-make --output-on-failure
```

### 方案 C：Ninja Multi-Config（一目录同时支持 Debug / Release）

```bash
# 配置
"$CMAKE" -B build-mingw-ninja-mc -G "Ninja Multi-Config" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA"

# 构建 Debug
"$CMAKE" --build build-mingw-ninja-mc --config Debug

# 测试 Debug
ctest --test-dir build-mingw-ninja-mc -C Debug --output-on-failure

# 构建 Release
"$CMAKE" --build build-mingw-ninja-mc --config Release

# 测试 Release
ctest --test-dir build-mingw-ninja-mc -C Release --output-on-failure
```

> cmd 三步版（以 Debug 为例）：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja-mc -G "Ninja Multi-Config" -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja-mc --config Debug
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja-mc --config Release

:: 测试
ctest --test-dir build-mingw-ninja-mc -C Debug --output-on-failure
ctest --test-dir build-mingw-ninja-mc -C Release --output-on-failure
```

---

## 2. 命令行 · MSVC（cmd）

### 方案 A：vcvarsall + Ninja（推荐）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

:: 激活 — 把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "%VCVARSALL%" x64

:: 配置
"%CMAKE%" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"%CMAKE%" --build build-msvc-ninja

:: 测试
ctest --test-dir build-msvc-ninja --output-on-failure
```

> 四步版（激活、配置、构建、测试各一行）：

```bat
:: 激活 — 把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja

:: 测试
ctest --test-dir build-msvc-ninja --output-on-failure
ctest --test-dir build-msvc-ninja --output-on-failure -L edge
```

### 方案 B：Visual Studio Generator（无需激活 vcvarsall）

CMake 自动检测 MSVC 工具链；Generator 是多配置，构建时须指定 `--config`，输出到 `build-msvc-vs\Debug\`。`-A x64` 是 VS Generator 专用的目标平台参数（x64 = 64 位），不加默认为 Win32（32 位）；Ninja Generator 不支持此参数，目标架构由 `vcvarsall.bat x64` 决定。

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

:: 测试（多配置，必须指定 -C）
ctest --test-dir build-msvc-vs -C Debug --output-on-failure
```

> 三步版：

```bat
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-vs --config Debug

:: 测试
ctest --test-dir build-msvc-vs -C Debug --output-on-failure
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
| `calc.vcxproj` | 静态库，对应 `add_library(calc STATIC ...)` |
| `test_basic.vcxproj` | 基础测试目标，对应 `add_executable(test_basic ...)` |
| `test_edge.vcxproj` | 边界测试目标，对应 `add_executable(test_edge ...)` |
| `ALL_BUILD.vcxproj` | 构建所有目标，等价于 `cmake --build`，VS 里默认 Build 触发的就是它 |
| `ZERO_CHECK.vcxproj` | 监控 `CMakeLists.txt` 修改时间；检测到变化时自动重新运行 cmake 配置，刷新 `.vcxproj`，无需手动重跑 cmake |

用 VS 打开工程：双击 `ctest_demo.slnx`（VS 2022 17.x+ 的新格式）或 `ctest_demo.sln`，解决方案加载后各项目均可见。

### 方案 C：x64 Native Tools Command Prompt + Ninja（无需激活 vcvarsall）

开始菜单搜索 `x64 Native Tools Command Prompt for VS 2026` 打开。窗口启动时自动运行激活脚本（效果等同于 `call vcvarsall.bat x64`），已把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH。进去后三步运行：

```bat
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja

:: 测试
ctest --test-dir build-msvc-ninja --output-on-failure
```

---

## 3. 对比

| | MinGW 方案 A<br>Ninja | MinGW 方案 B<br>MinGW Makefiles | MinGW 方案 C<br>Ninja Multi-Config |
|---|---|---|---|
| build 目录 | `build-mingw-ninja` | `build-mingw-make` | `build-mingw-ninja-mc` |
| 构建工具 | ninja.exe | mingw32-make.exe | ninja.exe |
| 配置数 | 单配置 | 单配置 | 多配置（Debug/Release 共目录） |
| 速度 | 最快 | 慢 | 快 |
| 测试命令 | `ctest --test-dir build-mingw-ninja` | `ctest --test-dir build-mingw-make` | `ctest --test-dir build-mingw-ninja-mc -C Debug` |
| 适用场景 | 日常首选 | 没有 ninja.exe 时备选 | 需频繁切换 Debug/Release |

| | MSVC 方案 A<br>vcvarsall + Ninja | MSVC 方案 B<br>VS Generator | MSVC 方案 C<br>Native Tools + Ninja |
|---|---|---|---|
| build 目录 | `build-msvc-ninja` | `build-msvc-vs` | `build-msvc-ninja` |
| 需要激活 | 是（call vcvarsall） | 否 | 否（窗口自动激活） |
| 构建工具 | ninja.exe | msbuild.exe | ninja.exe |
| 配置数 | 单配置 | 多配置（--config） | 单配置 |
| 测试命令 | `ctest --test-dir build-msvc-ninja` | `ctest --test-dir build-msvc-vs -C Debug` | `ctest --test-dir build-msvc-ninja` |
| 适用场景 | 脚本/自动化首选 | 不想手动激活 | 交互式操作 |

---

## 4. CLion IDE

1. `File → Open` 选择 `09_ctest/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击 **加载**
3. 菜单 **Run → Run 'All CTest'**（或工具栏 Run Configurations 下拉选择 `All CTest`）
4. 测试执行后在 **Test Runner 窗口**显示每条测试的通过 / 失败状态
5. **Run → Rerun Failed Tests** 只重跑失败用例

> 提示：在 `add_test()` 后通过 `set_tests_properties` 设置 `LABELS` 属性，可在 CLion 的测试筛选器中按标签过滤。

> build 目录各文件的作用见 [cmake_syntax.md § 3. build 目录文件说明](../cmake_syntax.md)。
