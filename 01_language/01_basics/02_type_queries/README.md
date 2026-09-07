# 02_type_queries — 类型查询运算符

`sizeof` 进阶（不执行表达式 / 指针退化 / `std::size`）、`alignof`（对齐与结构体填充）、`auto`（深入）、`decltype`（保留 const/引用）。`sizeof` 的基础用法已在 01_types 1.1 详讲。

## 1. 知识点

### 1.1 sizeof（进阶）

`sizeof` 的基础用法——查类型/变量字节数、查数组总字节与元素个数——已在 01_types 1.1 详讲，这里只补三个进阶行为：不执行表达式、对指针与退化的数组参数无效、C++17 推荐的 `std::size`。

#### 1.1.1 sizeof 不执行表达式

`sizeof` 只在编译期分析类型，不会执行括号里的表达式。即使写了 `sizeof(++n)`，`n` 也不会自增。

```cpp
int n = 0;
sizeof(++n);               // ++n 不会执行！
std::cout << n << "\n";    // 0（n 没有改变）

// sizeof 对表达式只推断其结果类型的大小：
// sizeof(1 + 1.0) 等价于 sizeof(double)，结果是 8
```

#### 1.1.2 指针和数组的区别

对指针使用 `sizeof` 只返回指针本身的大小（8 字节），不是它指向的数组的大小。数组传给函数后退化为指针，在函数内 `sizeof` 就得不到数组大小了。

```cpp
int arr[7];
int* p = arr;
std::cout << sizeof(arr) << "\n";   // 28（数组总大小）
std::cout << sizeof(p) << "\n";     // 8（指针大小，不是数组大小！）

// 函数参数里写 int arr[] 等价于 int* arr，sizeof 只有指针大小
// void foo(int arr[]) { sizeof(arr); }  // 结果是 8，不是数组大小
```

#### 1.1.3 数组长度：优先用 std::size（C++17）

用 `sizeof(arr) / sizeof(arr[0])` 算元素个数要写两遍还容易漏括号。C++17 起标准库直接提供 `std::size()`，更清晰也不会写错，还能用于 `std::array`、`std::vector` 等所有有 `size()` 的容器。

```cpp
#include <iterator>     // std::size

int arr[] = {10, 20, 30, 40, 50};
std::cout << std::size(arr) << "\n";   // 5（元素个数）

// 注意：数组退化成指针后 std::size 同样用不了（长度已丢失），见 1.1.2
```

### 1.2 alignof 与结构体填充

`alignof` 返回类型的对齐要求（字节数），即该类型的变量必须存放在几的倍数内存地址上。CPU 按对齐地址访问内存效率最高，不对齐可能导致性能下降甚至崩溃。

```cpp
std::cout << alignof(char)   << "\n";   // 1（任意地址）
std::cout << alignof(short)  << "\n";   // 2（偶数地址）
std::cout << alignof(int)    << "\n";   // 4（4 的倍数地址）
std::cout << alignof(double) << "\n";   // 8（8 的倍数地址）
```

#### 1.2.1 结构体填充

为满足对齐要求，编译器会在结构体字段之间和末尾自动插入填充字节。字段顺序不同，填充量不同，结构体大小也不同。按对齐从大到小排列字段，可以减少填充浪费。

```cpp
struct Bad {                  // 顺序不好，浪费空间
    char  a;                  // 1 字节，偏移 0
    // ← 3 字节填充（让 b 对齐到 4 的倍数）
    int   b;                  // 4 字节，偏移 4
    char  c;                  // 1 字节，偏移 8
    // ← 3 字节填充（结构体总大小必须是最大对齐数 4 的倍数）
};  // sizeof = 12

struct Good {                 // 大字段放前面
    int   b;                  // 4 字节，偏移 0
    char  a;                  // 1 字节，偏移 4
    char  c;                  // 1 字节，偏移 5
    // ← 2 字节填充
};  // sizeof = 8（省了 4 字节）
```

#### 1.2.2 alignas

`alignas`（C++11）可以手动指定更大的对齐要求。常用于 SIMD 向量化指令（要求 16 或 32 字节对齐）和缓存行对齐（避免伪共享）。

```cpp
struct alignas(16) Vec4 {     // 强制 16 字节对齐
    float x, y, z, w;
};

std::cout << sizeof(Vec4)  << "\n";   // 16
std::cout << alignof(Vec4) << "\n";   // 16（对齐要求被提升到 16）

// alignas 的值必须是 2 的幂，且不能小于类型自身的对齐要求
```

### 1.3 auto（类型推断）

`auto` 让编译器根据初始值自动推断变量类型（C++11），减少冗长的类型名。注意 `auto` 会剥掉 `const` 和引用，需要显式加回来。

#### 1.3.1 基本推断

编译器根据 `=` 右边的值推断类型：整数字面量推断为 `int`，小数推断为 `double`，字符推断为 `char`，布尔推断为 `bool`。

```cpp
auto i = 42;                      // int
auto d = 3.14;                    // double
auto c = 'A';                     // char
auto b = true;                    // bool
auto s = std::string("hello");    // std::string（不是 const char*）

// auto 不能用于无初始值的声明：
// auto x;  // 编译错误！没有初始值，无法推断类型
```

#### 1.3.2 auto 剥掉 const 和引用

`auto` 推断时会剥掉顶层 `const` 和引用，得到一个全新的可修改的独立变量。这是为了防止意外修改原变量。

```cpp
const int ci = 10;
auto v1 = ci;       // v1 是 int（不是 const int）
v1 = 99;            // 可以修改！const 被剥掉了

int x = 5;
int& rx = x;
auto v2 = rx;       // v2 是 int（不是 int&），是 x 的副本
v2 = 100;           // 修改 v2 不影响 x
```

#### 1.3.3 保留 const/引用要显式写

如果需要保留 `const` 或引用，必须显式写出 `const auto` 或 `auto&`。

```cpp
const int ci = 10;

const auto v1 = ci;     // const int：显式加 const
auto& v2 = ci;          // const int&：引用绑定到 const，const 自动保留
// v1 = 99;             // 编译错误！
// v2 = 99;             // 编译错误！

int arr[] = {1, 2, 3};
for (const auto& x : arr) {   // 只读引用遍历，不复制
    std::cout << x << " ";
}
```

#### 1.3.4 auto 在范围 for 中的用法

范围 for 中 `auto` 的三种写法：`auto` 拷贝副本、`auto&` 可修改原值、`const auto&` 只读不复制（大对象推荐）。

```cpp
int arr[] = {1, 2, 3, 4, 5};

// auto：拷贝，修改不影响原数组
for (auto x : arr) { /* x 是副本 */ }

// auto&：引用，可以修改原数组
for (auto& x : arr) { x *= 2; }  // arr 变为 {2, 4, 6, 8, 10}

// const auto&：只读引用，不复制也不修改（大对象推荐）
for (const auto& x : arr) { std::cout << x << " "; }
```

### 1.4 decltype（推断类型，保留 const/引用）

`decltype` 推断表达式的类型但不执行它（C++11），与 `auto` 的关键区别是 `decltype` 保留 `const` 和引用。常用于泛型编程中需要精确类型信息的场景。

```cpp
int a = 5;
double b = 3.14;

decltype(a) x = 10;          // int
decltype(b) y = 1.5;         // double
decltype(a + b) z = 8.14;    // double（int+double 推断为 double）
```

#### 1.4.1 decltype vs auto 的关键区别

`auto` 剥掉 const 和引用（得到独立副本），`decltype` 保留 const 和引用（得到完全相同的类型）。选择哪个取决于是否需要保留原始类型信息。

```cpp
const int ci = 42;
int a = 5;
int& ri = a;

// auto 剥掉 const 和引用：
auto av = ci;             // int（剥掉 const，可修改）
auto ar = ri;             // int（剥掉引用，是 a 的副本）

// decltype 保留 const 和引用：
decltype(ci) dv = ci;     // const int（保留 const，不可修改）
decltype(ri) dr = a;      // int&（保留引用，修改 dr 影响 a）
dr = 999;
std::cout << a << "\n";   // 999（dr 是 a 的引用）
```

#### 1.4.2 decltype(auto)（C++14）

`decltype(auto)` 兼具 `auto` 的简洁和 `decltype` 的保留能力。它用 `decltype` 的规则来推断类型，但不需要写出表达式。常用于完美转发返回类型。

```cpp
const int ci = 42;
decltype(auto) da = ci;      // const int（完整保留，C++14）

// 在函数返回中特别有用：
// auto foo() { ... }              // 剥掉 const/引用
// decltype(auto) foo() { ... }    // 保留 const/引用（完美转发返回类型）
```

### 1.5 实际使用场景

#### 1.5.1 decltype 推断函数返回类型

在泛型编程中，`decltype` 可以根据参数类型推断返回类型。`auto` + 尾置返回类型（trailing return type）是 C++11 的写法，C++14 起可以直接用 `auto` 让编译器推断。

```cpp
// C++14 泛型 lambda + decltype 推断返回类型：
auto add = [](auto a, auto b) -> decltype(a + b) {
    return a + b;
};

std::cout << add(1, 2.5) << "\n";    // 3.5（int + double → double）
std::cout << add(1, 2) << "\n";      // 3（int + int → int）
```

#### 1.5.2 验证结构体对齐

用 `sizeof` 检查结构体是否因填充字节而变大，对网络传输和文件读写至关重要：序列化前需要知道实际大小，否则发送/写入的数据可能包含填充字节垃圾值。

```cpp
#include <cstdint>

struct Msg {
    uint8_t  type;       // 1 字节
    uint32_t length;     // 4 字节
    uint8_t  data[3];    // 3 字节
};
// 1+4+3=8 字节？不，sizeof(Msg) 可能是 12（含填充字节）
// 网络序列化时需要手动打包或使用 #pragma pack
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/type_queries.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-ninja\type_queries.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/type_queries.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-make\type_queries.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/type_queries.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-mc\Release\type_queries.exe
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

| 变量 | 给谁用 | 找什么 | 示例路径（MSVC 14.51 / Win10 SDK 26100） |
|------|--------|--------|------|
| PATH | cmd 命令 | cl.exe / link.exe / ninja / cmake | `...\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\` |
| INCLUDE | cl.exe（编译器） | 头文件 | `...\VC\Tools\MSVC\14.51.36231\include\`<br>`...\Windows Kits\10\Include\10.0.26100.0\ucrt\` 等 |
| LIB | link.exe（链接器） | .lib 库文件 | `...\VC\Tools\MSVC\14.51.36231\lib\x64\`<br>`...\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64\` 等 |
| LIBPATH | .NET 工具 | 程序集 | 本项目用不到 |

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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\type_queries.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-msvc-ninja\type_queries.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\type_queries.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-msvc-vs\Release\type_queries.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/type_queries
```

## 3. 英文及缩写说明

| 词汇               | 说明                                                           |
|------------------|--------------------------------------------------------------|
| `sizeof`         | size of：运算符，返回类型/变量占用字节数；**不执行表达式**；对指针只返回指针大小（不是数组大小）       |
| `alignof`        | alignment of：返回类型的对齐要求（字节数），决定变量必须存放在几的倍数地址                  |
| alignment        | 对齐：CPU 读写内存的要求，`int` 要放在 4 的倍数地址，`double` 要放在 8 的倍数地址，否则性能下降 |
| padding          | 填充字节：结构体为满足对齐要求，由编译器自动插入的空白字节；字段顺序影响填充量                      |
| `alignas`        | alignment as（C++11）：手动指定对齐要求，如 `alignas(16)` 用于 SIMD 向量化指令   |
| `auto`           | 自动类型推断（C++11）：由初始值推断变量类型；**会剥掉 const 和引用**                   |
| `decltype`       | declared type：推断表达式类型但不执行；**保留 const 和引用**，与 auto 的关键区别      |
| `decltype(auto)` | C++14：兼具 auto 的简洁和 decltype 的保留能力，常用于完美转发返回类型                |
| `size_t`         | size type：无符号整数，sizeof/alignof 的返回类型，专门表示大小和索引               |
| `std::copy`      | 标准算法，将一个范围的元素复制到另一个位置，来自 `<algorithm>`                       |
