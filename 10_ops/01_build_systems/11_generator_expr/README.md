# 11_generator_expr — 生成器表达式

`$<...>` 在 cmake 生成阶段求值，处理多配置和跨编译器差异

## 1. 常用生成器表达式速查表

| 表达式                         | 说明                   | 示例                                             |
|-----------------------------|----------------------|------------------------------------------------|
| `$<CONFIG:Debug>`           | 当前配置为 Debug 时为 1     | `$<$<CONFIG:Debug>:-g>`                        |
| `$<CONFIG:Release>`         | 当前配置为 Release 时为 1   | `$<$<CONFIG:Release>:-O3>`                     |
| `$<CXX_COMPILER_ID:GNU>`    | 编译器为 GCC 时为 1        | 针对 GCC 加特定选项                                   |
| `$<CXX_COMPILER_ID:MSVC>`   | 编译器为 MSVC 时为 1       | 针对 MSVC 加 `/W4` 等                              |
| `$<COMPILE_LANGUAGE:CXX>`   | 编译语言为 C++ 时为 1       | 针对 C++ 文件加特定选项                                 |
| `$<TARGET_FILE:tgt>`        | 目标文件完整路径             | 用于 `add_custom_command`                        |
| `$<BOOL:var>`               | 变量为真时为 1             | `$<$<BOOL:${ENABLE_ASAN}>:-fsanitize=address>` |
| `$<AND:expr1,expr2>`        | 逻辑与                  | 组合多个条件                                         |
| `$<OR:expr1,expr2>`         | 逻辑或                  | 组合多个条件                                         |
| `$<NOT:expr>`               | 逻辑非                  | `$<NOT:$<CONFIG:Debug>>`                       |
| `$<BUILD_INTERFACE:path>`   | 仅构建时有效（install 后不包含） | 头文件路径区分构建与安装                                   |
| `$<INSTALL_INTERFACE:path>` | 仅安装后有效               | 配合 `install(EXPORT)` 使用                        |

> 生成器表达式在 `cmake --build` 阶段才求值，**不能**在 `message()` 中直接打印（需用 `file(GENERATE)` 输出到文件调试）。

## 2. 本目录专属命令（Debug / Release 对比验证）

本目录的 CMakeLists 用 `$<CONFIG:Debug>` / `$<CONFIG:Release>` 按构建类型注入不同宏（`MY_DEBUG=1`、`MY_RELEASE=1`），并用 `BUILD_TYPE="$<CONFIG>"` 把当前配置名直接写进代码；main.cpp 把它们打印出来。单配置生成器（Ninja、MinGW Makefiles）一个 build 目录只代表一种配置，所以须分别配置 Debug、Release **两个目录**再运行对比输出。多配置生成器（Ninja Multi-Config / VS Generator）则一个目录用 `--config` 切换即可，见章 README §5。

## 3. 构建与运行

完整六套方案（MinGW 三方案 / MSVC 三方案，含对比与 CLion）见 [章 README § 5 通用构建流程](../README.md)，本目录 exe 名是 `app`，且要按 `-DCMAKE_BUILD_TYPE` 各配一个目录来对比宏值差异。日常最常用——Ninja 方案 A 配 Debug / Release 两目录（Git Bash）：

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

# Debug
"$CMAKE" -B build-mingw-ninja-debug -G Ninja -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_MAKE_PROGRAM="$NINJA" -DCMAKE_BUILD_TYPE=Debug
"$CMAKE" --build build-mingw-ninja-debug
./build-mingw-ninja-debug/app.exe

# Release
"$CMAKE" -B build-mingw-ninja-release -G Ninja -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_MAKE_PROGRAM="$NINJA" -DCMAKE_BUILD_TYPE=Release
"$CMAKE" --build build-mingw-ninja-release
./build-mingw-ninja-release/app.exe
```

对比两次输出：第一行 `BUILD_TYPE` 是 `$<CONFIG>` 求值得到的字面配置名（Debug / Release），随后的宏行随构建类型切换（Debug 目录有 `MY_DEBUG`，Release 目录有 `MY_RELEASE`）——即 `$<CONFIG:Debug>` 是否成立由该目录配置时的 `-DCMAKE_BUILD_TYPE` 决定。

> cmd 版与 MSVC/Linux 版：把通用流程里的 exe 名换成 `app`，并给配置命令加 `-DCMAKE_BUILD_TYPE=Debug` / `Release`、build 目录名带 `-debug` / `-release` 后缀各跑一遍即可。
