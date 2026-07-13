# 05_compile_options — 编译选项与宏定义

`target_compile_options` + 生成器表达式，跨编译器统一管理

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
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
