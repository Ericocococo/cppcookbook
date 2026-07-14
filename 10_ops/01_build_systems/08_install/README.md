# 08_install — 安装规则与包导出

`install()` 定义安装位置，导出包配置让其他项目 find_package

> 工具完整路径见 [父目录 README](../README.md)

**为什么需要 install？**

构建完 exe 在 `build/` 下，目录结构杂乱（混有中间文件）。`cmake --install` 把产物整理成规范结构：

```
build/          →    install-root/
├── CMakeFiles/       ├── bin/app.exe
├── app.exe           ├── lib/libmylib.a
├── mylib.a           └── include/mylib.h
└── ...
```

用途：发布给用户、打包成安装包、或让其他 CMake 项目通过 `find_package` 找到这个库。**日常开发调试不需要此步骤。**

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
  -DCMAKE_MAKE_PROGRAM="$NINJA" \
  -DCMAKE_INSTALL_PREFIX=./install-root

# 构建
"$CMAKE" --build build-mingw-ninja

# 安装
"$CMAKE" --install build-mingw-ninja
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe" -DCMAKE_INSTALL_PREFIX=./install-root

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 安装
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --install build-mingw-ninja
```

### 方案 B：MinGW Makefiles（无需 ninja.exe）

```bash
# 配置
"$CMAKE" -B build-mingw-make -G "MinGW Makefiles" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$MAKE" \
  -DCMAKE_INSTALL_PREFIX=./install-root

# 构建
"$CMAKE" --build build-mingw-make

# 安装
"$CMAKE" --install build-mingw-make
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-make -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe" -DCMAKE_INSTALL_PREFIX=./install-root

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-make

:: 安装
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --install build-mingw-make
```

### 方案 C：Ninja Multi-Config（一目录同时支持 Debug / Release）

```bash
# 配置
"$CMAKE" -B build-mingw-ninja-mc -G "Ninja Multi-Config" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA" \
  -DCMAKE_INSTALL_PREFIX=./install-root

# 构建 Debug
"$CMAKE" --build build-mingw-ninja-mc --config Debug

# 安装 Debug
"$CMAKE" --install build-mingw-ninja-mc --config Debug

# 构建 Release
"$CMAKE" --build build-mingw-ninja-mc --config Release

# 安装 Release
"$CMAKE" --install build-mingw-ninja-mc --config Release
```

> cmd 三步版（以 Debug 为例）：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja-mc -G "Ninja Multi-Config" -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe" -DCMAKE_INSTALL_PREFIX=./install-root

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja-mc --config Debug
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja-mc --config Release

:: 安装
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --install build-mingw-ninja-mc --config Debug
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --install build-mingw-ninja-mc --config Release
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
"%CMAKE%" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_INSTALL_PREFIX=./install-root

:: 构建
"%CMAKE%" --build build-msvc-ninja

:: 安装
"%CMAKE%" --install build-msvc-ninja
```

> 四步版（激活、配置、构建、安装各一行）：

```bat
:: 激活 — 把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_INSTALL_PREFIX=./install-root

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja

:: 安装
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --install build-msvc-ninja
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
"%CMAKE%" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64 -DCMAKE_INSTALL_PREFIX=./install-root

:: 构建（多配置，必须指定 --config）
"%CMAKE%" --build build-msvc-vs --config Debug

:: 安装
"%CMAKE%" --install build-msvc-vs --config Debug
```

> 三步版：

```bat
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64 -DCMAKE_INSTALL_PREFIX=./install-root

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-vs --config Debug
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-vs --config Release

:: 安装
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --install build-msvc-vs --config Debug
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --install build-msvc-vs --config Release
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
| `install_demo.vcxproj` | 主项目，对应 `add_executable(install_demo ...)` |
| `ALL_BUILD.vcxproj` | 构建所有目标，等价于 `cmake --build`，VS 里默认 Build 触发的就是它 |
| `ZERO_CHECK.vcxproj` | 监控 `CMakeLists.txt` 修改时间；检测到变化时自动重新运行 cmake 配置，刷新 `.vcxproj`，无需手动重跑 cmake |

用 VS 打开工程：双击 `install_demo.slnx`（VS 2022 17.x+ 的新格式）或 `install_demo.sln`，解决方案加载后三个项目均可见。

### 方案 C：x64 Native Tools Command Prompt + Ninja（无需激活 vcvarsall）

开始菜单搜索 `x64 Native Tools Command Prompt for VS 2026` 打开。窗口启动时自动运行激活脚本（效果等同于 `call vcvarsall.bat x64`），已把 cl.exe / link.exe / ninja.exe / cmake.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH。进去后三步运行：

```bat
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" -DCMAKE_INSTALL_PREFIX=./install-root

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja

:: 安装
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --install build-msvc-ninja
```

---

## 3. 对比

| | MinGW 方案 A<br>Ninja | MinGW 方案 B<br>MinGW Makefiles | MinGW 方案 C<br>Ninja Multi-Config |
|---|---|---|---|
| build 目录 | `build-mingw-ninja` | `build-mingw-make` | `build-mingw-ninja-mc` |
| 构建工具 | ninja.exe | mingw32-make.exe | ninja.exe |
| 配置数 | 单配置 | 单配置 | 多配置（Debug/Release 共目录） |
| 速度 | 最快 | 慢 | 快 |
| 适用场景 | 日常首选 | 没有 ninja.exe 时备选 | 需频繁切换 Debug/Release |

| | MSVC 方案 A<br>vcvarsall + Ninja | MSVC 方案 B<br>VS Generator | MSVC 方案 C<br>Native Tools + Ninja |
|---|---|---|---|
| build 目录 | `build-msvc-ninja` | `build-msvc-vs` | `build-msvc-ninja` |
| 需要激活 | 是（call vcvarsall） | 否 | 否（窗口自动激活） |
| 构建工具 | ninja.exe | msbuild.exe | ninja.exe |
| 配置数 | 单配置 | 多配置（--config） | 单配置 |
| 安装目录 | `install-root\` | `install-root\` | `install-root\` |
| 适用场景 | 脚本/自动化首选 | 不想手动激活 | 交互式操作 |

---

## 4. CLion IDE

1. `File → Open` 选择 `08_install/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击 **加载**
3. 工具栏选择工具链（MinGW 或 Visual Studio）和构建类型
4. **构建** `Ctrl+F9`
5. 构建完成后执行安装：**Tools → CMake → Install**，或在 CLion 内置终端执行 `cmake --install build-mingw-ninja`（或 `build-msvc-ninja`）

> 安装前缀在 Profile 的 **CMake options** 里设置 `-DCMAKE_INSTALL_PREFIX=路径`。

> build 目录各文件的作用见 [cmake_syntax.md § 3. build 目录文件说明](../cmake_syntax.md)。

### 安装后目录结构

```
install-root/
├── bin/
│   └── install_demo.exe        # 可执行文件
├── lib/
│   ├── libmylib.a              # 静态库（MinGW）或 mylib.lib（MSVC）
│   └── cmake/
│       └── mylib/
│           ├── mylibConfig.cmake
│           └── mylibConfigVersion.cmake
└── include/
    └── mylib/
        └── mylib.h             # 公开头文件
```

其他项目可通过 `-DCMAKE_PREFIX_PATH=<install-root 路径>` 让 `find_package(mylib CONFIG)` 找到此安装包。
