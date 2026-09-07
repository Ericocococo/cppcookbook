# 02_hello — 最简 CMake 工程

`cmake_minimum_required` + `project` + `add_executable` 三行完成一个可运行工程。

## 1. CMakeLists.txt（本目录核心）

```cmake
cmake_minimum_required(VERSION 3.20)  # 声明最低 CMake 版本（低于此版本直接报错）
project(hello_cmake CXX)              # 工程名 + 语言（CXX = C++）
add_executable(hello_cmake main.cpp)  # 从 main.cpp 生成可执行文件 hello_cmake
```

- 每行一个作用：`cmake_minimum_required` 卡版本 → `project` 起工程 → `add_executable` 定义第一个目标。
- 语法细节（`add_executable` 的参数形式、target 概念、`CXX_STANDARD` 设置）见 [cmake_syntax.md § 1.1](../cmake_syntax.md)。

## 2. 构建与运行

完整六套方案（MinGW 三方案 / MSVC 三方案，含对比与 CLion）见 [章 README § 5 通用构建流程](../README.md)，本目录只需把 `<exe_name>` 换成 `hello_cmake`。日常最常用——Ninja 方案 A（Git Bash 三步）：

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

"$CMAKE" -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_MAKE_PROGRAM="$NINJA"
"$CMAKE" --build build-mingw-ninja
./build-mingw-ninja/hello_cmake.exe
```

> cmd 版与 MSVC/Linux 版：同样把通用流程里的 exe 名换成 `hello_cmake` 即可。
