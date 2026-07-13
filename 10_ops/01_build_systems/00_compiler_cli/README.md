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

常用标志（GCC / MinGW）：

| 标志 | 含义 |
|------|------|
| `-std=c++20` | 指定 C++ 标准版本 |
| `-Wall` | 开启 GCC 认为"高价值、误报少"的一批警告（未使用变量、类型不匹配等）；名字骗人，并非"所有"警告 |
| `-Wextra` | `-Wall` 之外的额外警告（函数参数未使用、符号比较等）；误报稍多，单独一档 |
| `-Wpedantic` | 严格遵循 C++ 标准，报所有 GCC 扩展语法的使用；三个一起加才算完整 |
| `-Werror` | 把所有警告升级为编译错误，强制修复，CI 常用 |
| `-c` | **只编译，不链接**：把 `.cpp` 编译成目标文件 `.o`，不生成可执行文件；分步编译时用，最终还需再手动链接 |
| `-O0` | 优化级别 0：不优化（默认），编译最快，调试断点最准确 |
| `-O1` | 优化级别 1：基础优化 |
| `-O2` | 优化级别 2：中等优化，Release 常用 |
| `-O3` | 优化级别 3：激进优化，可能增大二进制体积 |
| `-Os` | 以减小二进制体积为优先目标 |
| `-o app.exe` | **小写 o**：指定输出文件名（output）；与上面大写 O 完全不同，注意区分 |
| `-g` | 生成调试信息，供 GDB 读取 |
| `-g3` | 生成更完整的调试信息，包含宏定义，比 `-g` 更详细 |
| `-DFOO=1` | 向代码注入宏定义，等价于在代码顶部写 `#define FOO 1` |
| `-DNDEBUG` | 定义 `NDEBUG` 宏（No Debug）：C/C++ 标准规定，此宏被定义后所有 `assert()` 自动变为空操作；Release 构建通常加此宏 |
| `-Ipath` | 添加头文件搜索路径，编译器从这里查找 `#include` 的文件 |
| `-Lpath` | 告诉链接器去这个目录下搜索库文件（`.a` / `.so`）|
| `-lname` | 链接名为 `libname.a` 或 `libname.so` 的库；`-lm` = 链接数学库 `libm.so`（自动补全 `lib` 前缀和后缀）|

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

常用标志（MSVC）：

| 标志 | 含义 |
|------|------|
| `/std:c++20` | 指定 C++ 标准版本 |
| `/W0` ~ `/W4` | 警告级别：/W0 = 无警告，/W1-/W4 逐级递增；`/W4` 是最高常用级（比 `/W3` 多报更多潜在问题）；`/Wall` 才是真正全部但噪音极多，实际很少用 |
| `/WX` | 把所有警告升级为编译错误，等价 GCC 的 `-Werror` |
| `/EHsc` | 启用标准 C++ 异常处理；EH = Exception Handling，s = 启用同步异常（C++ throw/catch），c = extern "C" 函数不向外传播异常；MSVC 默认不启用，使用 STL 时**必须加**，否则链接报错 |
| `/c` | 只编译不链接，生成 `.obj` 目标文件，等价 GCC 的 `-c` |
| `/Od` | 关闭优化（d = Disable），调试构建用，等价 GCC 的 `-O0` |
| `/O2` | 中等优化，Release 常用，等价 GCC 的 `-O2` |
| `/Zi` | 生成调试信息到独立的 `.pdb`（Program Database）文件，调试器（WinDbg / LLDB）通过 `.pdb` 读取符号和行号 |
| `/permissive-` | 关闭 MSVC 的历史兼容模式；MSVC 默认接受某些不符合 C++ 标准的写法（为兼容旧代码），加此标志强制执行严格标准，新项目建议加 |
| `/utf-8` | 指定源文件和执行字符集均为 UTF-8，避免中文注释或字符串在 Windows 上出现乱码 |
| `/Fe:app.exe` | 指定输出可执行文件名（Fe = File Executable），等价 GCC 的 `-o app.exe` |
| `/DFOO=1` | 向代码注入宏定义，等价 GCC 的 `-DFOO=1` |
| `/Ipath` | 添加头文件搜索路径，等价 GCC 的 `-Ipath` |

---

## CLion IDE

本示例无 `CMakeLists.txt`，在 `View → Tool Windows → Terminal`（`Alt+F12`）里执行上方命令。

---

## 为什么要学这个？

构建系统只是把这些编译器命令自动化。理解原始命令，才能看懂 CI 日志和排查构建问题。
