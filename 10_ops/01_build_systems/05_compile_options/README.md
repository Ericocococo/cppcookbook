# 05_compile_options — 编译选项与宏定义

`target_compile_options` + 生成器表达式，跨编译器统一管理

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
# Debug 配置
"$CMAKE" -B build-mingw-ninja-debug -G Ninja \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA" \
  -DCMAKE_BUILD_TYPE=Debug

# Debug 构建
"$CMAKE" --build build-mingw-ninja-debug

# Debug 运行
./build-mingw-ninja-debug/compile_options_demo.exe

# Release 配置
"$CMAKE" -B build-mingw-ninja-release -G Ninja \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA" \
  -DCMAKE_BUILD_TYPE=Release

# Release 构建
"$CMAKE" --build build-mingw-ninja-release

# Release 运行
./build-mingw-ninja-release/compile_options_demo.exe
```

> cmd 各步单行版：

```bat
:: Debug - 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja-debug -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe" -DCMAKE_BUILD_TYPE=Debug

:: Debug - 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja-debug

:: Debug - 运行
build-mingw-ninja-debug\compile_options_demo.exe

:: Release - 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja-release -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe" -DCMAKE_BUILD_TYPE=Release

:: Release - 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja-release

:: Release - 运行
build-mingw-ninja-release\compile_options_demo.exe
```

### 方案 B：MinGW Makefiles（无需 ninja.exe）

```bash
# Debug 配置
"$CMAKE" -B build-mingw-make-debug -G "MinGW Makefiles" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$MAKE" \
  -DCMAKE_BUILD_TYPE=Debug

# Debug 构建
"$CMAKE" --build build-mingw-make-debug

# Debug 运行
./build-mingw-make-debug/compile_options_demo.exe

# Release 配置
"$CMAKE" -B build-mingw-make-release -G "MinGW Makefiles" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$MAKE" \
  -DCMAKE_BUILD_TYPE=Release

# Release 构建
"$CMAKE" --build build-mingw-make-release

# Release 运行
./build-mingw-make-release/compile_options_demo.exe
```

> cmd 各步单行版：

```bat
:: Debug - 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-make-debug -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe" -DCMAKE_BUILD_TYPE=Debug

:: Debug - 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-make-debug

:: Debug - 运行
build-mingw-make-debug\compile_options_demo.exe

:: Release - 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-make-release -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe" -DCMAKE_BUILD_TYPE=Release

:: Release - 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-make-release

:: Release - 运行
build-mingw-make-release\compile_options_demo.exe
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
./build-mingw-ninja-mc/Debug/compile_options_demo.exe

# 构建 Release
"$CMAKE" --build build-mingw-ninja-mc --config Release

# 运行 Release
./build-mingw-ninja-mc/Release/compile_options_demo.exe
```

> cmd 各步单行版（以 Debug 为例）：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja-mc -G "Ninja Multi-Config" -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja-mc --config Debug
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja-mc --config Release

:: 运行
build-mingw-ninja-mc\Debug\compile_options_demo.exe
build-mingw-ninja-mc\Release\compile_options_demo.exe
```

---

## 2. 命令行 · MSVC（cmd）

### 方案 A：vcvarsall + Ninja（推荐）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

:: 激活 — 把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "%VCVARSALL%" x64

:: Debug 配置
"%CMAKE%" -B build-msvc-ninja-debug -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_BUILD_TYPE=Debug

:: Debug 构建
"%CMAKE%" --build build-msvc-ninja-debug

:: Debug 运行
build-msvc-ninja-debug\compile_options_demo.exe

:: Release 配置
"%CMAKE%" -B build-msvc-ninja-release -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_BUILD_TYPE=Release

:: Release 构建
"%CMAKE%" --build build-msvc-ninja-release

:: Release 运行
build-msvc-ninja-release\compile_options_demo.exe
```

> 各步单行版（激活一次，Debug/Release 各三步）：

```bat
:: 激活 — 把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: Debug - 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja-debug -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_BUILD_TYPE=Debug

:: Debug - 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja-debug

:: Debug - 运行
build-msvc-ninja-debug\compile_options_demo.exe

:: Release - 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja-release -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_BUILD_TYPE=Release

:: Release - 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja-release

:: Release - 运行
build-msvc-ninja-release\compile_options_demo.exe
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

:: 运行
build-msvc-vs\Debug\compile_options_demo.exe
```

> 三步版：

```bat
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-vs --config Debug
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Debug\compile_options_demo.exe
build-msvc-vs\Release\compile_options_demo.exe
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

**生成的 build 目录中三个 `.vcxproj` 的作用：**

| 文件 | 作用 |
|---|---|
| `compile_options_demo.vcxproj` | 主项目，对应 `add_executable(compile_options_demo ...)` |
| `ALL_BUILD.vcxproj` | 构建所有目标，等价于 `cmake --build`，VS 里默认 Build 触发的就是它 |
| `ZERO_CHECK.vcxproj` | 监控 `CMakeLists.txt` 修改时间；检测到变化时自动重新运行 cmake 配置，刷新 `.vcxproj`，无需手动重跑 cmake |

用 VS 打开工程：双击 `compile_options_demo.slnx`（VS 2022 17.x+ 的新格式）或 `compile_options_demo.sln`，解决方案加载后三个项目均可见。

### 方案 C：x64 Native Tools Command Prompt + Ninja（无需激活 vcvarsall）

开始菜单搜索 `x64 Native Tools Command Prompt for VS 2026` 打开。窗口启动时自动运行激活脚本（效果等同于 `call vcvarsall.bat x64`），已把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH。进去后各步运行：

```bat
:: Debug - 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja-debug -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_BUILD_TYPE=Debug

:: Debug - 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja-debug

:: Debug - 运行
build-msvc-ninja-debug\compile_options_demo.exe

:: Release - 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja-release -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_BUILD_TYPE=Release

:: Release - 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja-release

:: Release - 运行
build-msvc-ninja-release\compile_options_demo.exe
```

---

## 3. 对比

| | MinGW 方案 A<br>Ninja | MinGW 方案 B<br>MinGW Makefiles | MinGW 方案 C<br>Ninja Multi-Config |
|---|---|---|---|
| build 目录 | `build-mingw-ninja-{debug,release}` | `build-mingw-make-{debug,release}` | `build-mingw-ninja-mc` |
| 构建工具 | ninja.exe | mingw32-make.exe | ninja.exe |
| 配置数 | 单配置（两目录） | 单配置（两目录） | 多配置（Debug/Release 共目录） |
| 速度 | 最快 | 慢 | 快 |
| 适用场景 | 日常首选 | 没有 ninja.exe 时备选 | 需频繁切换 Debug/Release |

| | MSVC 方案 A<br>vcvarsall + Ninja | MSVC 方案 B<br>VS Generator | MSVC 方案 C<br>Native Tools + Ninja |
|---|---|---|---|
| build 目录 | `build-msvc-ninja-{debug,release}` | `build-msvc-vs` | `build-msvc-ninja-{debug,release}` |
| 需要激活 | 是（call vcvarsall） | 否 | 否（窗口自动激活） |
| 构建工具 | ninja.exe | msbuild.exe | ninja.exe |
| 配置数 | 单配置（两目录） | 多配置（--config） | 单配置（两目录） |
| exe 路径 | `build-msvc-ninja-{debug,release}\` | `build-msvc-vs\Debug\` | `build-msvc-ninja-{debug,release}\` |
| 适用场景 | 脚本/自动化首选 | 不想手动激活 | 交互式操作 |

---

## 4. CLion IDE

1. **Settings → Build, Execution, Deployment → CMake**，点击 `+` 新增 `Release` Profile（默认已有 `Debug`）。
2. 工具栏切换 Profile 后点击 Run，对比两种构建的程序输出。
3. 查看 `compile_commands.json` 验证编译选项：

```bash
# 在 CLion Terminal 中执行（build-mingw-ninja-debug 需先构建）
cat build-mingw-ninja-debug/compile_commands.json
```

输出的 JSON 数组中，每个源文件对应一条条目，`command` 字段包含完整编译命令，可直接验证 `-O0`、`-g`、`-DDEBUG` 等选项是否注入。


> build 目录各文件的作用见 [cmake_syntax.md § 3. build 目录文件说明](../cmake_syntax.md)。

---

## 标志速查

### GCC/Clang 警告标志（`-W` 前缀）

| 标志 | 说明 |
|------|------|
| `-Wall` | 高价值警告集合（未使用变量、类型不匹配等），名字骗人，并非全部警告 |
| `-Wextra` | `-Wall` 之外的额外警告，误报稍多，单独一档 |
| `-Wpedantic` | 严格 C++ 标准模式，报所有 GCC 扩展用法 |
| `-Werror` | 将所有警告提升为错误，强制修复 |

### 优化级别（`-O` 大写 O）vs 输出文件名（`-o` 小写 o）

> **易混淆！** 大写 `-O` 控制优化，小写 `-o` 指定输出文件名。

| 标志 | 类别 | 说明 |
|------|------|------|
| `-O0` | 优化（大写 O）| 不优化，默认，调试最准确 |
| `-O1` | 优化 | 基础优化 |
| `-O2` | 优化 | 中等优化，Release 常用 |
| `-O3` | 优化 | 激进优化，可能增大二进制体积 |
| `-Os` | 优化 | 优先减小体积 |
| `-o app.exe` | 输出（小写 o）| 指定生成文件的名称 |

### MSVC 等价标志

| GCC/Clang | MSVC | 说明 |
|-----------|------|------|
| `-Wall -Wextra` | `/W4` | 高警告级别（MSVC 体系：/W0=无，/W1-/W4 递增，/W4 最高常用级）|
| `-Werror` | `/WX` | 警告视为错误 |
| `-O0` | `/Od` | 关闭优化（d = Disable）|
| `-O2` | `/O2` | 中等优化 |
| `-O3` | `/Ox` | 最大优化 |
| `-g` / `-g3` | `/Zi` | 生成调试信息；GCC 写入二进制，MSVC 生成独立 `.pdb` 文件 |
| `-DNDEBUG` | `/DNDEBUG` | 定义 NDEBUG 宏，关闭 `assert()`，Release 构建标准做法 |
| `-o app.exe` | `/Fe:app.exe` | 指定输出文件名（Fe = File Executable）|
| `-EHsc`（无对应）| `/EHsc` | 启用标准 C++ 异常处理（MSVC 独有，GCC 默认已启用）|
| 无对应 | `/permissive-` | 关闭 MSVC 历史兼容宽松模式，强制严格 C++ 标准 |
| 无对应 | `/utf-8` | 源文件和执行字符集均为 UTF-8，避免中文乱码 |
