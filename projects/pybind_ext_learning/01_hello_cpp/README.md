# 01_hello_cpp — 第一步：纯 C++

和 Python 没有任何关系。确认 C++ 编译环境可用。

## 1. 文件

| 文件 | 说明 |
|:---|:---|
| `hello.h` | 函数声明（头文件 = 菜单，列出有什么） |
| `main.cpp` | 函数实现 + main 调用（厨房 = 实际做菜） |
| `CMakeLists.txt` | 最简 C++ 工程（`add_executable`） |

---

## 2. 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"
MAKE="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"
```

### 方案 A：Ninja（推荐）

```bash
# 配置
"$CMAKE" -B build-mingw-ninja -G Ninja \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA"

# 构建
"$CMAKE" --build build-mingw-ninja

# 运行
./build-mingw-ninja/hello_cpp.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 运行
build-mingw-ninja\hello_cpp.exe
```

### 方案 B：MinGW Makefiles（无需 ninja.exe）

```bash
# 配置
"$CMAKE" -B build-mingw-make -G "MinGW Makefiles" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$MAKE"

# 构建
"$CMAKE" --build build-mingw-make

# 运行
./build-mingw-make/hello_cpp.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-make -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-make

:: 运行
build-mingw-make\hello_cpp.exe
```

### 方案 C：Ninja Multi-Config（多配置）

```bash
# 配置
"$CMAKE" -B build-mingw-mc -G "Ninja Multi-Config" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA"

# 构建（多配置须指定 --config）
"$CMAKE" --build build-mingw-mc --config Debug

# 运行
./build-mingw-mc/Debug/hello_cpp.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-mc -G "Ninja Multi-Config" -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-mc --config Debug

:: 运行
build-mingw-mc\Debug\hello_cpp.exe
```

---

## 3. 命令行 · MSVC（cmd）

### vcvarsall 注入的 4 个环境变量

| 变量 | 给谁用 | 找什么 | 示例路径（MSVC 14.51 / Win10 SDK 26100） |
|:---|:---|:---|:---|
| PATH | cmd 命令 | cl.exe / link.exe / ninja / cmake | `...\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\` |
| INCLUDE | cl.exe（编译器） | 头文件 | `...\VC\Tools\MSVC\14.51.36231\include\`<br>`...\Windows Kits\10\Include\10.0.26100.0\ucrt\` 等 |
| LIB | link.exe（链接器） | .lib 库文件 | `...\VC\Tools\MSVC\14.51.36231\lib\x64\`<br>`...\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64\` 等 |
| LIBPATH | .NET 工具 | 程序集 | 本项目用不到 |

- `call` 必须写在当前 cmd 会话里——`call` 让变量修改留在当前窗口；直接运行则只存在临时进程，退出就没了
- 不激活直接调 cl.exe 会报"找不到头文件"：cl.exe 找到 cl 自身但 INCLUDE 没注入，`#include <iostream>` 无从解析
- link.exe 依赖 LIB 找 `libcmt.lib` 等库文件，LIB 没注入则链接失败

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

:: 激活 — 把 cl.exe / link.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "%VCVARSALL%" x64
```

### 方案 A：vcvarsall + Ninja（推荐，单配置）

```bat
:: 配置
"%CMAKE%" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建（Ninja 是单配置生成器，配置时已锁定模式，--build 不需要 --config）
"%CMAKE%" --build build-msvc-ninja

:: 运行
build-msvc-ninja\hello_cpp.exe
```

> 四步版：

```bat
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja

:: 运行
build-msvc-ninja\hello_cpp.exe
```

### 方案 B：Visual Studio 生成器（多配置）

CMake 自动通过 `vswhere.exe` 检测 MSVC 工具链，生成 `.sln` 工程。VS Generator 是多配置，构建须指定 `--config`。

```bat
:: 配置（-G "Visual Studio 18 2026" 生成 .sln 工程，-A x64 指定 64 位）
"%CMAKE%" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64

:: 构建（VS 生成器一个工程含 Debug + Release，必须用 --config 指定；不写默认 Debug）
"%CMAKE%" --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\hello_cpp.exe
```

> 四步版：

```bat
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\hello_cpp.exe
```

---

## 4. 命令行 · Linux / WSL

```bash
cd 01_hello_cpp
mkdir build && cd build
cmake ..
cmake --build .
./hello_cpp
```

---

## 5. CLion IDE

1. `File → Open` 选择 `01_hello_cpp/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. 工具栏选择工具链（MinGW 或 Visual Studio）
4. **构建** `Ctrl+F9`　**运行** `Shift+F10`

---

## 6. 预期输出

```
========== 第一步：纯 C++ ==========

① add(3, 5) = 8
② avg({15.50, 15.80, 16.10, 16.40}) = 15.95
③ split_symbol("600519.SH") = {"600519", "SH"}

④ 指针 vs 引用:
   指针: *p = 1600 后 close = 1600
   引用: r = 1700 后 close = 1700
   指针参数: update_close(&close, 1800) 后 close = 1800
   引用参数: update_close_ref(close, 1900) 后 close = 1900
   值传递参数: update_close_copy(close, 2000) 返回 2000，close 仍 = 1900

========== 编译环境正常 ==========
```

---

## 7. 本步要点

本步的目的不是学 C++ 语法，而是**确认编译工具链正常**：

| 检查项 | 正常标志 |
|:---|:---|
| CMake 可用 | `cmake --version` 有输出 |
| 编译器可用 | MinGW 的 `g++` 或 MSVC 的 `cl.exe` |
| C++20 支持 | 结构化绑定 `auto [code, market] = ...` 编译通过 |

> 如果这一步编译不过，先解决编译环境问题。

---

## 8. 英文及缩写说明

| 词汇 | 说明 |
|:---|:---|
| `const` | 常量修饰符，承诺不修改数据 |
| `&`（参数中）| 引用传递，不拷贝，直接用原数据 |
| `const XXX&` | 只读引用，不拷贝也不能改，函数参数最常用写法 |
| `auto` | 让编译器自动推断类型（C++11） |
| `auto [a, b] = ...` | 结构化绑定，拆 pair/tuple 到多个变量（C++17） |
| `std::pair<A, B>` | 打包两个值的容器，用 `.first` `.second` 访问 |
| `std::vector<T>` | 动态数组，可变长度，用 `push_back()` 追加 |
| `std::string::npos` | `find()` 没找到时的返回值，表示"未找到" |
| `substr(pos, len)` | 从 pos 开始取 len 个字符的子串 |
| `v.empty()` | 判断容器是否为空 |
| `v.size()` | 返回容器中元素个数 |
