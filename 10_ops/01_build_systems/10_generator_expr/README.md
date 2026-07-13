# 10_generator_expr — 生成器表达式

`$<...>` 在 cmake 生成阶段求值，处理多配置和跨编译器差异

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"

# Debug 构建
"$CMAKE" -B build-mingw-debug -G Ninja -DCMAKE_C_COMPILER="$GCC" -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_BUILD_TYPE=Debug && "$CMAKE" --build build-mingw-debug && ./build-mingw-debug/genexpr_demo.exe

# Release 构建（对比宏差异）
"$CMAKE" -B build-mingw-release -G Ninja -DCMAKE_C_COMPILER="$GCC" -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_BUILD_TYPE=Release && "$CMAKE" --build build-mingw-release && ./build-mingw-release/genexpr_demo.exe
```

---

## 命令行 · MSVC（cmd）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

call "%VCVARSALL%" x64

rem Debug 构建
"%CMAKE%" -B build-msvc-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug && "%CMAKE%" --build build-msvc-debug && build-msvc-debug\genexpr_demo.exe

rem Release 构建（对比宏差异）
"%CMAKE%" -B build-msvc-release -G Ninja -DCMAKE_BUILD_TYPE=Release && "%CMAKE%" --build build-msvc-release && build-msvc-release\genexpr_demo.exe
```

---

## CLion IDE

在工具栏 **CMake Profile** 下拉中分别选择 `Debug` 和 `Release` 后运行，对比程序输出中宏（如 `BUILD_TYPE`、`MY_DEFINE`）的实际值。

### 常用生成器表达式速查表

| 表达式 | 说明 | 示例 |
|--------|------|------|
| `$<CONFIG:Debug>` | 当前配置为 Debug 时为 1 | `$<$<CONFIG:Debug>:-g>` |
| `$<CONFIG:Release>` | 当前配置为 Release 时为 1 | `$<$<CONFIG:Release>:-O3>` |
| `$<CXX_COMPILER_ID:GNU>` | 编译器为 GCC 时为 1 | 针对 GCC 加特定选项 |
| `$<CXX_COMPILER_ID:MSVC>` | 编译器为 MSVC 时为 1 | 针对 MSVC 加 `/W4` 等 |
| `$<COMPILE_LANGUAGE:CXX>` | 编译语言为 C++ 时为 1 | 针对 C++ 文件加特定选项 |
| `$<TARGET_FILE:tgt>` | 目标文件完整路径 | 用于 `add_custom_command` |
| `$<BOOL:var>` | 变量为真时为 1 | `$<$<BOOL:${ENABLE_ASAN}>:-fsanitize=address>` |
| `$<AND:expr1,expr2>` | 逻辑与 | 组合多个条件 |
| `$<OR:expr1,expr2>` | 逻辑或 | 组合多个条件 |
| `$<NOT:expr>` | 逻辑非 | `$<NOT:$<CONFIG:Debug>>` |
| `$<BUILD_INTERFACE:path>` | 仅构建时有效（install 后不包含） | 头文件路径区分构建与安装 |
| `$<INSTALL_INTERFACE:path>` | 仅安装后有效 | 配合 `install(EXPORT)` 使用 |

> 生成器表达式在 `cmake --build` 阶段才求值，**不能**在 `message()` 中直接打印（需用 `file(GENERATE)` 输出到文件调试）。
