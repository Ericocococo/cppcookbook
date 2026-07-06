# 05_compile_options — 编译选项与宏定义

`target_compile_options` + 生成器表达式，跨编译器统一管理

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe"
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"

# Debug 构建
"$CMAKE" -B build-mingw-debug -G Ninja -DCMAKE_C_COMPILER="$GCC" -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_BUILD_TYPE=Debug && "$CMAKE" --build build-mingw-debug && ./build-mingw-debug/compile_options_demo.exe

# Release 构建（对比宏差异）
"$CMAKE" -B build-mingw-release -G Ninja -DCMAKE_C_COMPILER="$GCC" -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_BUILD_TYPE=Release && "$CMAKE" --build build-mingw-release && ./build-mingw-release/compile_options_demo.exe
```

---

## 命令行 · MSVC（cmd）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

call "%VCVARSALL%" x64

rem Debug 构建
"%CMAKE%" -B build-msvc-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug && "%CMAKE%" --build build-msvc-debug && build-msvc-debug\compile_options_demo.exe

rem Release 构建（对比宏差异）
"%CMAKE%" -B build-msvc-release -G Ninja -DCMAKE_BUILD_TYPE=Release && "%CMAKE%" --build build-msvc-release && build-msvc-release\compile_options_demo.exe
```

---

## CLion IDE

1. **Settings → Build, Execution, Deployment → CMake**，点击 `+` 新增 `Release` Profile（默认已有 `Debug`）。
2. 工具栏切换 Profile 后点击 Run，对比两种构建的程序输出（宏 `NDEBUG` / `_DEBUG` 的差异）。
3. 查看实际传入编译器的选项：

```bash
# 在 CLion Terminal 中执行（build-mingw-debug 需先构建）
cat build-mingw-debug/compile_commands.json
```

输出的 JSON 数组中，每个源文件对应一条条目，`command` 字段包含完整编译命令，可直接验证 `-O0`、`-g`、`-DDEBUG` 等宏是否注入。
