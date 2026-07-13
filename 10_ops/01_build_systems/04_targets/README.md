# 04_targets — 现代 target 属性传播

PRIVATE / PUBLIC / INTERFACE 精确控制属性传播范围

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"

"$CMAKE" -B build-mingw -G Ninja -DCMAKE_C_COMPILER="$GCC" -DCMAKE_CXX_COMPILER="$GXX" && "$CMAKE" --build build-mingw && ./build-mingw/app.exe
```

---

## 命令行 · MSVC（cmd）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

call "%VCVARSALL%" x64
"%CMAKE%" -B build-msvc -G Ninja && "%CMAKE%" --build build-msvc && build-msvc\app.exe
```

---

## CLion IDE

打开 `04_targets` 目录，CLion 自动识别 `CMakeLists.txt`（与 01_hello 流程相同）。

在 **Build 输出窗口**可验证 `-I`（include 路径）是否按预期传播：
- `PRIVATE` 的路径只出现在声明该属性的目标编译命令中
- `PUBLIC` 的路径同时出现在该目标及链接它的消费者中
- `INTERFACE` 的路径只出现在消费者中，不出现在库本身

### PRIVATE / PUBLIC / INTERFACE 对比

| 关键字 | 影响本目标 | 传播给消费者 | 典型用途 |
|--------|:---------:|:-----------:|---------|
| `PRIVATE` | ✓ | ✗ | 内部实现细节（不泄漏接口） |
| `PUBLIC` | ✓ | ✓ | 库同时自用且对外暴露的接口 |
| `INTERFACE` | ✗ | ✓ | 纯头文件库 / header-only 接口 |
