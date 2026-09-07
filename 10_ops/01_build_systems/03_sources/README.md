# 03_sources — 多源文件

多个 `.cpp` 显式列举，`target_include_directories` 配置头文件路径

## 1. 为什么不用 `file(GLOB ...)`

| 方式                      | 问题                                                 |
|-------------------------|----------------------------------------------------|
| `file(GLOB SRCS *.cpp)` | 新增 `.cpp` 文件后，若不重新运行 cmake，构建系统**不会感知到新文件**，导致链接错误 |
| 显式列举源文件                 | 每次 git 拉取新文件后，CMake 重新配置时才正确纳入，不存在静默遗漏             |

> CMake 官方文档和社区均建议**显式列举**源文件，而非使用 GLOB。

## 2. 构建与运行

完整六套方案（MinGW 三方案 / MSVC 三方案，含对比与 CLion）见 [章 README § 5 通用构建流程](../README.md)，本目录只需把 `<exe_name>` 换成 `multi_sources`。日常最常用——Ninja 方案 A（Git Bash 三步）：

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

"$CMAKE" -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_MAKE_PROGRAM="$NINJA"
"$CMAKE" --build build-mingw-ninja
./build-mingw-ninja/multi_sources.exe
```

> cmd 版与 MSVC/Linux 版：同样把通用流程里的 exe 名换成 `multi_sources` 即可。
