# 00_compiler_cli — 直接调用编译器（无构建系统）

最原始的构建方式：手动敲编译器命令，没有任何构建文件。

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

```bash
# 定义路径变量（下同）
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"

# ① 多源文件一步编译链接
"$GXX" -std=c++20 -Wall -Wextra -o app.exe main.cpp math_utils.cpp

# ② 分步：先编译生成 .o，再链接
"$GXX" -std=c++20 -Wall -c main.cpp        # → main.o
"$GXX" -std=c++20 -Wall -c math_utils.cpp  # → math_utils.o
"$GXX" -o app.exe main.o math_utils.o

# ③ 运行
./app.exe
```

常用标志：

| 标志 | 含义 |
|------|------|
| `-std=c++20` | 指定 C++ 标准 |
| `-Wall -Wextra -Wpedantic` | 开启常用警告 |
| `-O0 / -O2 / -O3` | 优化级别（默认 -O0）|
| `-g3` | 生成调试信息（配合 GDB）|
| `-DFOO=1` | 定义宏 |
| `-Ipath` | 添加头文件搜索路径 |
| `-Lpath -lname` | 链接库 |

---

## 命令行 · MSVC（cmd）

```bat
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat
set CL_EXE=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\cl.exe

:: ① 激活 MSVC 环境（cl.exe 依赖此步注入头文件/库路径，必须执行）
call "%VCVARSALL%" x64

:: ② 多源文件一步编译链接
"%CL_EXE%" /std:c++20 /W4 /EHsc /Fe:app.exe main.cpp math_utils.cpp

:: ③ 分步：先编译 .obj，再链接
"%CL_EXE%" /std:c++20 /W4 /EHsc /c main.cpp
"%CL_EXE%" /std:c++20 /W4 /EHsc /c math_utils.cpp
link /OUT:app.exe main.obj math_utils.obj

:: ④ 运行
app.exe
```

常用标志：

| 标志 | 含义 |
|------|------|
| `/std:c++20` | 指定 C++ 标准 |
| `/W4` | 警告级别 4 |
| `/WX` | 警告视为错误 |
| `/EHsc` | 标准 C++ 异常处理（必加）|
| `/O2` | 优化 |
| `/Zi` | 调试信息 |
| `/permissive-` | 关闭宽松模式 |
| `/utf-8` | 源文件 UTF-8 编码 |

---

## CLion IDE

本示例无 `CMakeLists.txt`，在 `View → Tool Windows → Terminal`（`Alt+F12`）里执行上方命令。

---

## 为什么要学这个？

构建系统只是把这些编译器命令自动化。理解原始命令，才能看懂 CI 日志和排查构建问题。
