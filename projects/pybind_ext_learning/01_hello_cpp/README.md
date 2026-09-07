# 01_hello_cpp — 第一步：纯 C++

和 Python 没有任何关系。确认 C++ 编译环境可用。

## 1. 文件

| 文件               | 说明                          |
|:-----------------|:----------------------------|
| `hello.h`        | 函数声明（头文件 = 菜单，列出有什么）        |
| `main.cpp`       | 函数实现 + main 调用（厨房 = 实际做菜）   |
| `CMakeLists.txt` | 最简 C++ 工程（`add_executable`） |

---

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/hello_cpp.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-mingw-ninja\hello_cpp.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/hello_cpp.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-mingw-make\hello_cpp.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Debug

# 运行
./build-mingw-mc/Debug/hello_cpp.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-mingw-mc --config Debug

:: 运行
D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-mingw-mc\Debug\hello_cpp.exe
```

</details>

### 2.2 命令行 · MSVC（cmd）

**路径常量（绝对路径版使用）：**

| 工具 | 完整路径 |
|------|------|
| cmake | `D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe` |
| ninja | `D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe` |
| vcvarsall | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat` |
| cl.exe | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\cl.exe` |

> cl.exe 一般不用在命令里指定：`call vcvarsall.bat` 会把它注入 PATH，CMake 自动找到。
> 列出仅供参考；版本号 14.51.36231 随 VS 更新可能变化。

#### vcvarsall 注入的 4 个环境变量

| 变量      | 给谁用           | 找什么                               | 示例路径（MSVC 14.51 / Win10 SDK 26100）                                                             |
|:--------|:--------------|:----------------------------------|:-----------------------------------------------------------------------------------------------|
| PATH    | cmd 命令        | cl.exe / link.exe / ninja / cmake | `...\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\`                                               |
| INCLUDE | cl.exe（编译器）   | 头文件                               | `...\VC\Tools\MSVC\14.51.36231\include\`<br>`...\Windows Kits\10\Include\10.0.26100.0\ucrt\` 等 |
| LIB     | link.exe（链接器） | .lib 库文件                          | `...\VC\Tools\MSVC\14.51.36231\lib\x64\`<br>`...\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64\` 等 |
| LIBPATH | .NET 工具       | 程序集                               | 本项目用不到                                                                                         |

- `call` 必须写在当前 cmd 会话里——`call` 让变量修改留在当前窗口；直接运行则只存在临时进程，退出就没了
- 不激活直接调 cl.exe 会报"找不到头文件"：cl.exe 找到 cl 自身但 INCLUDE 没注入，`#include <iostream>` 无从解析
- link.exe 依赖 LIB 找 `libcmt.lib` 等库文件，LIB 没注入则链接失败

```bat
:: 激活 — 把 cl.exe / link.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```

#### 方案 A — vcvarsall + Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建（Ninja 是单配置生成器，配置时已锁定模式，--build 不需要 --config）
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\hello_cpp.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-msvc-ninja\hello_cpp.exe
```

</details>

#### 方案 B — VS Generator

CMake 自动通过 `vswhere.exe` 检测 MSVC 工具链，生成 `.sln` 工程。VS Generator 是多配置，构建须指定 `--config`。

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp

:: 配置（-G "Visual Studio 18 2026" 生成 .sln 工程，-A x64 指定 64 位）
cmake -B build-msvc-vs -G "Visual Studio 18 2026" -A x64 -S .

:: 构建（VS 生成器一个工程含 Debug + Release，必须用 --config 指定；不写默认 Debug）
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\hello_cpp.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\01_hello_cpp\build-msvc-vs\Release\hello_cpp.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# cd 到项目目录
cd 01_hello_cpp

# 配置
mkdir build && cd build
cmake ..

# 构建
cmake --build .

# 运行
./hello_cpp
```

---

## 3. CLion IDE

1. `File → Open` 选择 `01_hello_cpp/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. 工具栏选择工具链（MinGW 或 Visual Studio）
4. **构建** `Ctrl+F9`**运行** `Shift+F10`

---

## 4. 预期输出

> ④ 里的指针/引用是 `main.cpp` 顺带的语法演示（本步重点是编译环境，不是语法）。看不懂没关系，只要前 ③ 项输出正确即可；指针与引用的正式学习在 01_language 的 06_pointers_refs 章，这里只保证它们能编译、能跑。

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

## 5. 本步要点

本步的目的不是学 C++ 语法，而是**确认编译工具链正常**：

| 检查项      | 正常标志                                   |
|:---------|:---------------------------------------|
| CMake 可用 | `cmake --version` 有输出                  |
| 编译器可用    | MinGW 的 `g++` 或 MSVC 的 `cl.exe`        |
| C++20 支持 | 结构化绑定 `auto [code, market] = ...` 编译通过 |

> 如果这一步编译不过，先解决编译环境问题。

---

## 6. 英文及缩写说明

| 词汇                  | 说明                               |
|:--------------------|:---------------------------------|
| `const`             | 常量修饰符，承诺不修改数据                    |
| `*`（声明/解引用）        | 指针：存地址的变量；`*p` 读/写 p 指向的值          |
| `&`（取地址/参数中）       | 引用传递，不拷贝，直接用原数据；`&close` 取变量地址      |
| `const XXX&`        | 只读引用，不拷贝也不能改，函数参数最常用写法           |
| `auto`              | 让编译器自动推断类型（C++11）                |
| `auto [a, b] = ...` | 结构化绑定，拆 pair/tuple 到多个变量（C++17）  |
| `std::pair<A, B>`   | 打包两个值的容器，用 `.first` `.second` 访问 |
| `std::vector<T>`    | 动态数组，可变长度，用 `push_back()` 追加     |
| `std::string::npos` | `find()` 没找到时的返回值，表示"未找到"        |
| `substr(pos, len)`  | 从 pos 开始取 len 个字符的子串             |
| `v.empty()`         | 判断容器是否为空                         |
| `v.size()`          | 返回容器中元素个数                        |
