# 12_typeid_pack — typeid 与变参包

`typeid`（运行时类型）、`sizeof...`（变参包大小）、变参模板、折叠表达式、`type_traits`。

## 1. 知识点

| 文件 | 说明 |
|---|---|
| `main.cpp` | 4 个 demo：typeid 多态查询 / sizeof... 统计 / 变参模板展开与折叠 / type_traits 类型特性 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `typeid_pack` |

### 1.1 typeid：查运行时类型

#### 1.1.1 typeid().name()

`typeid(表达式)` 返回 `std::type_info` 对象，`.name()` 返回类型名字符串。不同编译器输出格式不统一：GCC 输出缩写（如 `i` 表示 int），MSVC 输出完整名称（如 `int`）。需要 `#include <typeinfo>`。

```cpp
#include <typeinfo>
#include <string>

int n = 42;
double d = 3.14;
std::string s = "hello";

std::cout << typeid(int).name()    << "\n";  // GCC: "i"，MSVC: "int"
std::cout << typeid(double).name() << "\n";  // GCC: "d"，MSVC: "double"
std::cout << typeid(n).name()      << "\n";  // 和 typeid(int) 相同
std::cout << typeid(s).name()      << "\n";  // GCC: 含长度前缀，MSVC: "class std::basic_string<...>"
```

#### 1.1.2 比较类型是否相同

`typeid` 返回的 `type_info` 对象可以用 `==` 比较两个类型是否相同。适合调试时检查变量的实际类型。

```cpp
int n = 42;
std::cout << (typeid(n) == typeid(int));       // 1（true）
std::cout << (typeid(n) == typeid(double));    // 0（false）
```

#### 1.1.3 typeid 在多态中的真正用途

对于有虚函数的多态类型，`typeid(*指针)` 返回指针指向的实际类型（运行期解析），而不是声明的基类类型。不过生产代码更推荐用虚函数多态，`typeid` 主要用于调试。

```cpp
#include <typeinfo>

struct Base {
    virtual ~Base() {}
};
struct Derived : Base {};

Base* bp = new Derived();
std::cout << typeid(*bp).name() << "\n";            // 输出 Derived（不是 Base）
std::cout << (typeid(*bp) == typeid(Derived)) << "\n"; // 1（true）
delete bp;
```

### 1.2 sizeof...：统计变参模板参数数量

#### 1.2.1 sizeof... 统计参数包大小

`sizeof...(Args)` 和 `sizeof...(args)` 在编译期返回变参模板参数包中的参数数量，两者结果相同。`sizeof...(Args)` 统计类型参数个数，`sizeof...(args)` 统计值参数个数。

```cpp
template <typename... Args>
void showCount(Args... args) {
    std::cout << "参数数量=" << sizeof...(args)
              << "  类型数量=" << sizeof...(Args) << "\n";
}

showCount();               // 参数数量=0  类型数量=0
showCount(1);              // 参数数量=1  类型数量=1
showCount(1, 2.0, "hi");   // 参数数量=3  类型数量=3
showCount(1, 2, 3, 4, 5);  // 参数数量=5  类型数量=5
```

### 1.3 变参模板基础（C++11）

#### 1.3.1 递归展开

C++11 处理参数包的经典方式：把参数包拆成"第一个参数 + 剩余参数"，递归调用自身。需要一个无参版本作为递归终止条件。

```cpp
// 递归终止条件
void printAll() { std::cout << "\n"; }

// 递归展开：每次取出第一个参数，剩余继续递归
template <typename T, typename... Rest>
void printAll(T first, Rest... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << ", ";
    }
    printAll(rest...);  // 递归处理剩余参数
}

printAll(1, 2.5, std::string("hi"));  // 输出：1, 2.5, hi
```

#### 1.3.2 C++17 折叠表达式

C++17 的折叠表达式可以更简洁地展开参数包，不需要递归。`(args + ...)` 展开为 `a1 + a2 + a3 + ...`，支持 `+` `-` `*` `/` `&&` `||` `,` 等运算符。

```cpp
// 求和：折叠表达式
template <typename... Args>
auto sumAll(Args... args) {
    return (args + ...);  // a1 + a2 + a3 + ...
}

std::cout << sumAll(1, 2, 3, 4, 5);    // 15
std::cout << sumAll(0.5, 1.5, 2.0);    // 4.0

// 逐个打印：逗号折叠
template <typename... Args>
void printAllFold(Args... args) {
    ((std::cout << args << " "), ...);  // 展开每个 args
    std::cout << "\n";
}

printAllFold(10, 20, 30);  // 输出：10 20 30
```

### 1.4 type_traits（编译期类型查询）

`<type_traits>` 头文件提供编译期类型查询和变换工具，全部以 `_v`（值）或 `_t`（类型）结尾，配合模板和 `if constexpr` 使用。

#### 1.4.1 is_same_v

判断两个类型是否完全相同，返回编译期 `bool`。

```cpp
#include <type_traits>

std::cout << std::is_same_v<int, int>;       // 1（true）
std::cout << std::is_same_v<int, double>;    // 0（false）
std::cout << std::is_same_v<int, int32_t>;   // 通常为 1（取决于平台）
```

#### 1.4.2 is_integral_v / is_floating_point_v

`is_integral_v<T>` 判断 T 是否为整数类型（`bool`/`char`/`int`/`long` 等），`is_floating_point_v<T>` 判断是否为浮点类型（`float`/`double`/`long double`）。

```cpp
std::cout << std::is_integral_v<int>;          // 1
std::cout << std::is_integral_v<double>;       // 0
std::cout << std::is_floating_point_v<double>; // 1
std::cout << std::is_floating_point_v<int>;    // 0
```

#### 1.4.3 is_pointer_v / is_reference_v

判断类型是否为指针或引用。

```cpp
std::cout << std::is_pointer_v<int*>;     // 1
std::cout << std::is_pointer_v<int>;      // 0
std::cout << std::is_reference_v<int&>;   // 1
std::cout << std::is_reference_v<int>;    // 0
```

#### 1.4.4 add/remove 系列：类型变换

`remove_const_t` 去掉 `const`，`add_pointer_t` 变为指针类型。这些工具在模板元编程中用于编译期变换类型。

```cpp
#include <type_traits>

using NoConst = std::remove_const_t<const int>;   // const int → int
using AddPtr  = std::add_pointer_t<int>;           // int → int*

std::cout << std::is_same_v<NoConst, int>;   // 1（成功去掉 const）
std::cout << std::is_same_v<AddPtr, int*>;   // 1（成功加上指针）
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/typeid_pack.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-mingw-ninja\typeid_pack.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/typeid_pack.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-mingw-make\typeid_pack.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/typeid_pack.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-mingw-mc\Release\typeid_pack.exe
```

</details>

### 2.2 命令行 · MSVC（cmd）

> MSVC 路径常量与 vcvarsall 注入原理（4 个环境变量、为什么必须 `call`）见首次详解 [00_hello_world §2.2](../00_hello_world/README.md)，以下命令直接用。

```bat
:: 激活 — 把 cl.exe / link.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```

#### 方案 A — vcvarsall + Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\typeid_pack.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-msvc-ninja\typeid_pack.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\typeid_pack.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\12_typeid_pack\build-msvc-vs\Release\typeid_pack.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/typeid_pack
```

## 3. 英文及缩写说明

| 词汇                            | 说明                                                                         |
|-------------------------------|----------------------------------------------------------------------------|
| `typeid`                      | type id：返回 `std::type_info` 对象；用于多态时查实际类型（运行期）；需要 `#include <typeinfo>`    |
| `type_info`                   | C++ 标准类，`typeid` 的返回类型；`.name()` 返回类型名（GCC 返回缩写，MSVC 返回全名）                 |
| `sizeof...`                   | sizeof 的变参版本（C++11）：统计变参模板参数包的数量，编译期求值                                     |
| parameter pack                | 参数包（C++11）：`typename... Args` 或 `Args... args`，可接收任意数量任意类型的参数              |
| fold expression               | 折叠表达式（C++17）：`(args + ...)` = a1+a2+...，简洁地展开参数包；支持 + - * / &              | && \|\| , 等运算符 |
| `<type_traits>`               | 类型特性头文件：提供编译期类型查询和变换工具，全部以 `_v`（变量模板）或 `_t`（类型别名）结尾                        |
| `std::is_same_v<T1,T2>`       | 两个类型是否完全相同，编译期 `bool`                                                      |
| `std::is_integral_v<T>`       | T 是整数类型（bool/char/int/long 等）时为 true                                       |
| `std::is_floating_point_v<T>` | T 是浮点类型（float/double/long double）时为 true                                   |
| `std::is_pointer_v<T>`        | T 是指针类型时为 true                                                             |
| `std::is_reference_v<T>`      | T 是引用类型时为 true                                                             |
| `std::remove_const_t<T>`      | 移除 T 的 const 限定，如 `remove_const_t<const int>` = `int`                      |
| `std::add_pointer_t<T>`       | 把 T 变为指针，如 `add_pointer_t<int>` = `int*`                                   |
| IIFE                          | Immediately Invoked Function Expression：定义 lambda 后立刻调用，常用于初始化复杂的 const 变量 |
| `<typeinfo>`                  | 包含 `std::type_info` 类；不包含此头文件时，`typeid` 可能无法编译                             |
