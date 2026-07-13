# 02_sources — 多源文件

多个 `.cpp` 显式列举，`target_include_directories` 配置头文件路径

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"

"$CMAKE" -B build-mingw -G Ninja -DCMAKE_C_COMPILER="$GCC" -DCMAKE_CXX_COMPILER="$GXX" && "$CMAKE" --build build-mingw && ./build-mingw/multi_sources.exe
```

---

## 命令行 · MSVC（cmd）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

call "%VCVARSALL%" x64
"%CMAKE%" -B build-msvc -G Ninja && "%CMAKE%" --build build-msvc && build-msvc\multi_sources.exe
```

---

## CLion IDE

打开 `02_sources` 目录，CLion 自动识别 `CMakeLists.txt` 并完成配置（与 01_hello 流程相同）。点击工具栏运行按钮即可。

### 为什么不用 `file(GLOB ...)`

| 方式 | 问题 |
|------|------|
| `file(GLOB SRCS *.cpp)` | 新增 `.cpp` 文件后，若不重新运行 cmake，构建系统**不会感知到新文件**，导致链接错误 |
| 显式列举源文件 | 每次 git 拉取新文件后，CMake 重新配置时才正确纳入，不存在静默遗漏 |

> CMake 官方文档和社区均建议**显式列举**源文件，而非使用 GLOB。
