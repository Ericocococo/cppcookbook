# 11_presets — CMakePresets.json

预设文件管理多套构建配置，一个命令切换

> 工具完整路径见 [父目录 README](../README.md)

---

## 1. 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"
```

### 方案 A：Ninja（由预设固定）

CMakePresets.json 中 `base` 预设已固定 `"generator": "Ninja"`，`binaryDir` 为 `${sourceDir}/build/${presetName}`，无需手动传
`-G`、`-B` 参数；直接使用 `--preset` 命令即可。

```bash
# 列出可用预设
"$CMAKE" --list-presets

# Debug 预设 - 配置
"$CMAKE" --preset debug

# Debug 预设 - 构建
"$CMAKE" --build --preset debug

# Debug 预设 - 运行
./build/debug/presets_demo.exe

# Release 预设 - 配置
"$CMAKE" --preset release

# Release 预设 - 构建
"$CMAKE" --build --preset release

# Release 预设 - 运行
./build/release/presets_demo.exe

# ASan 预设（AddressSanitizer，检测内存错误）- 配置
"$CMAKE" --preset asan

# ASan 预设 - 构建
"$CMAKE" --build --preset asan

# ASan 预设 - 运行
./build/asan/presets_demo.exe
```

> cmd 步骤版：

```bat
:: 列出可用预设
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --list-presets

:: Debug - 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --preset debug

:: Debug - 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build --preset debug

:: Debug - 运行
build\debug\presets_demo.exe

:: Release - 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --preset release

:: Release - 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build --preset release

:: Release - 运行
build\release\presets_demo.exe

:: ASan - 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --preset asan

:: ASan - 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build --preset asan

:: ASan - 运行
build\asan\presets_demo.exe
```

---

## 2. 命令行 · MSVC（cmd）

CMakePresets.json 中 generator 固定为 Ninja，VS Generator 方案不适用。激活 MSVC 工具链后直接使用 `--preset` 命令。

### 方案 A：vcvarsall + 预设（推荐）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

:: 激活 — 把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "%VCVARSALL%" x64

:: 列出可用预设
"%CMAKE%" --list-presets

:: Debug 预设 - 配置
"%CMAKE%" --preset debug

:: Debug 预设 - 构建
"%CMAKE%" --build --preset debug

:: Debug 预设 - 运行
build\debug\presets_demo.exe

:: Release 预设 - 配置
"%CMAKE%" --preset release

:: Release 预设 - 构建
"%CMAKE%" --build --preset release

:: Release 预设 - 运行
build\release\presets_demo.exe

:: ASan 预设（需 VS 2019 16.9+，安装 C++ AddressSanitizer 组件）- 配置
"%CMAKE%" --preset asan

:: ASan 预设 - 构建
"%CMAKE%" --build --preset asan

:: ASan 预设 - 运行
build\asan\presets_demo.exe
```

> 各步单行版（激活一次，各预设独立执行）：

```bat
:: 激活 — 把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: Debug - 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --preset debug

:: Debug - 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build --preset debug

:: Debug - 运行
build\debug\presets_demo.exe

:: Release - 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --preset release

:: Release - 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build --preset release

:: Release - 运行
build\release\presets_demo.exe

:: ASan - 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --preset asan

:: ASan - 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build --preset asan

:: ASan - 运行
build\asan\presets_demo.exe
```

### 方案 C：x64 Native Tools Command Prompt + 预设（无需激活 vcvarsall）

开始菜单搜索 `x64 Native Tools Command Prompt for VS 2026` 打开。窗口启动时自动运行激活脚本（效果等同于
`call vcvarsall.bat x64`），已把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB /
LIBPATH。进去后直接使用 `--preset` 命令：

```bat
:: 列出可用预设
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --list-presets

:: Debug - 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --preset debug

:: Debug - 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build --preset debug

:: Debug - 运行
build\debug\presets_demo.exe

:: Release - 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --preset release

:: Release - 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build --preset release

:: Release - 运行
build\release\presets_demo.exe

:: ASan - 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --preset asan

:: ASan - 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build --preset asan

:: ASan - 运行
build\asan\presets_demo.exe
```

---

## 3. 对比

|                                    | debug 预设      | release 预设      | asan 预设      |
|------------------------------------|---------------|-----------------|--------------|
| build 目录                           | `build/debug` | `build/release` | `build/asan` |
| CMAKE_BUILD_TYPE                   | Debug         | Release         | Debug        |
| CMAKE_INTERPROCEDURAL_OPTIMIZATION | 否             | ON（LTO）         | 否            |
| ENABLE_ASAN                        | 否             | 否               | ON           |

---

## 4. CLion IDE

1. **Settings → Build, Execution, Deployment → CMake**，点击 `+` → **From CMakePresets.json**（CLion 检测到
   `CMakePresets.json` 后也会自动提示导入）。
2. 导入后每个 `configurePreset` 对应一个 CMake Profile，可在工具栏直接切换。
3. 选择 `asan` Profile 后运行：若程序存在内存错误，ASan 运行时会在控制台打印红色 `ERROR: AddressSanitizer` 报告，精确定位到出错行。

> **MSVC ASan 注意**：需在 VS Installer 中安装 **C++ AddressSanitizer** 组件（Individual Components → C++
> AddressSanitizer）。

> build 目录各文件的作用见 [cmake_syntax.md § 3. build 目录文件说明](../cmake_syntax.md)。

### CMakePresets.json 与直接写 -D 参数的区别

| 对比项    | CMakePresets.json                | 命令行 -D 参数   |
|--------|----------------------------------|-------------|
| 配置共享   | 提交到 git，团队共享                     | 每人本地命令，不统一  |
| 切换配置   | `cmake --preset debug` 一条命令      | 需记住所有参数     |
| IDE 集成 | CLion / VS Code / VS 自动识别        | 需手动配置每个 IDE |
| 个人本地配置 | CMakeUserPresets.json（gitignore） | 本地脚本        |
