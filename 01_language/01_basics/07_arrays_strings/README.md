# 07_arrays_strings — 数组与字符串

演示原始数组、`std::array`（固定大小）、`std::vector`（动态数组）、`std::string` 的基本用法。

## 1. 知识点

| 文件 | 说明 |
|---|---|
| `main.cpp` | 5 个 demo 函数，覆盖原始数组到 string_view 的常用操作 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `arrays_strings` |

### 1.1 原始数组

#### 1.1.1 原始数组声明与初始化

原始数组在栈上分配，大小必须是编译期常量。用 `类型 名字[大小] = {初始值列表}` 声明和初始化，下标从 0 开始。

```cpp
int arr[5] = {1, 2, 3, 4, 5};
// arr[0]=1, arr[4]=5
```

#### 1.1.2 自动推断长度

省略数组大小时，编译器根据初始化列表推断。用 `sizeof(arr)/sizeof(arr[0])` 计算元素个数。

```cpp
int arr[] = {10, 20, 30};              // 编译器推断大小为 3
size_t len = sizeof(arr) / sizeof(arr[0]);  // len = 3
```

#### 1.1.3 零初始化

`int zeros[5]{};` 让全部元素为 0（`{}` 的完整规则——全置零、部分指定补 0——在 01_types 1.6.2 已详讲）。这里要重点记住反面：**不写 `{}` 的局部数组，元素值不确定**，不能依赖。

```cpp
int zeros[5]{};     // 全部初始化为 0
int bad[5];         // 不推荐：局部数组值不确定
```

#### 1.1.4 二维数组

二维数组是"数组的数组"，用 `类型 名字[行][列]` 声明。内存是连续的，按行存储。

```cpp
int mat[2][3] = {{1, 2, 3}, {4, 5, 6}};
// mat[1][2] == 6（第 2 行第 3 列）
```

#### 1.1.5 数组的陷阱：传给函数时退化为指针

数组作为函数参数时退化为指针：只把首元素地址传进去，长度信息一并丢失，函数里无法知道数组有几个元素。`sizeof` 和 `std::size` 为什么在函数里拿不到长度，原理见 02_type_queries 1.1.2/1.1.3。

```cpp
void f(int arr[]) {
    // 等价于 void f(int* arr)，长度信息已经丢了
}

// 推荐：用 std::array 替代，或传 size 参数
void f2(int* arr, size_t size) { /* ... */ }
```

### 1.2 std::array（C++11，固定大小）

#### 1.2.1 std::array 基本用法

`std::array<T, N>` 是对原始数组的安全封装，大小固定在编译期。有 `size()` 方法，支持迭代器和范围 for，需要 `#include <array>`。

```cpp
#include <array>

std::array<int, 5> arr = {5, 3, 1, 4, 2};
// arr.size() == 5
// arr[0] == 5
// arr.front() == 5, arr.back() == 2
```

#### 1.2.2 at() 带越界检查

`arr[i]` 不检查越界（和原始数组一样快但危险），`arr.at(i)` 会检查，越界时抛出 `std::out_of_range` 异常。

```cpp
std::array<int, 5> arr = {1, 2, 3, 4, 5};
int x = arr.at(2);    // OK，返回 3
// int y = arr.at(10); // 抛出 std::out_of_range 异常
```

#### 1.2.3 传给函数不退化

`std::array` 作为参数传递时保留大小信息，不会退化为指针。声明参数类型时需要指定元素类型和大小。

```cpp
#include <array>
#include <iostream>

void printArr(const std::array<int, 5>& a) {
    for (auto x : a) {
        std::cout << x << " ";
    }
    std::cout << "\n";
}
```

#### 1.2.4 排序

`std::array` 支持标准库算法，可以直接用 `std::sort` 排序。需要 `#include <algorithm>`。

```cpp
#include <array>
#include <algorithm>

std::array<int, 5> arr = {5, 3, 1, 4, 2};
std::sort(arr.begin(), arr.end());
// arr = {1, 2, 3, 4, 5}
```

### 1.3 std::vector（动态数组）

#### 1.3.1 std::vector 基本用法

`std::vector<T>` 是最常用的容器，大小可以在运行时动态变化。内部用连续内存存储，支持随机访问。需要 `#include <vector>`。

```cpp
#include <vector>

std::vector<int> v;
// v.size() == 0, v.capacity() == 0
```

#### 1.3.2 push_back 与自动扩容

`push_back(x)` 在末尾添加元素。当 `size` 超过 `capacity` 时触发扩容：分配更大的新内存（通常容量翻倍），将旧元素复制过去，释放旧内存。扩容代价较高，元素较多时建议先 `reserve`。

```cpp
std::vector<int> v;
for (int i = 1; i <= 8; ++i) {
    v.push_back(i);   // 可能触发扩容，capacity 翻倍增长
}
// v.size() == 8
```

#### 1.3.3 reserve：预留空间，避免反复扩容

`reserve(n)` 预分配至少能容纳 n 个元素的空间，但不改变 `size`。当已知大致元素数量时，先 `reserve` 可以避免多次扩容拷贝，提高性能。

```cpp
std::vector<int> v;
v.reserve(100);         // 预留 100 个元素的空间
// v.size() == 0        // 元素数量没变
// v.capacity() >= 100  // 容量至少 100
```

#### 1.3.4 常用操作

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
v[0];            // 下标访问（不检查越界）
v.at(0);         // 带越界检查的访问
v.front();       // 第一个元素
v.back();        // 最后一个元素
v.pop_back();    // 移除最后一个元素（不返回值）
v.empty();       // 是否为空
v.size();        // 元素数量
v.clear();       // 清空所有元素
```

#### 1.3.5 emplace_back：直接在末尾构造

`emplace_back(args...)` 直接在 vector 末尾用参数构造对象，比 `push_back` 少一次拷贝或移动。对于简单类型（int/double）差别不大，对于复杂对象（string/自定义类）效率更高。

```cpp
std::vector<int> v = {1, 2, 3};
v.emplace_back(99);     // 直接构造，v.back() == 99
```

#### 1.3.6 insert / erase

`insert` 在指定位置插入元素，`erase` 删除指定位置的元素。两者都需要移动后续元素，时间复杂度 O(n)，频繁在中间插删应考虑 `std::list`。

```cpp
std::vector<int> v = {1, 2, 3};
v.insert(v.begin(), 0);      // 头部插入 0 → {0, 1, 2, 3}
v.erase(v.begin());          // 删除第一个 → {1, 2, 3}
```

#### 1.3.7 多种初始化方式

```cpp
std::vector<int> v1;               // 空
std::vector<int> v2(5, 42);        // 5 个 42 → {42, 42, 42, 42, 42}
std::vector<int> v3 = {1, 2, 3};   // 初始化列表
std::vector<int> v4(v3);           // 拷贝构造
```

### 1.4 std::string

#### 1.4.1 std::string 基本用法

`std::string` 是动态长度的字符串类，内部管理内存，支持拼接、查找、截取等丰富操作。需要 `#include <string>`。

```cpp
#include <string>

std::string s = "Hello";
// s.length() == 5（字节数）
// s.size() == 5（与 length 完全相同）
```

#### 1.4.2 拼接

用 `+=` 追加字符串或字符，用 `append()` 追加字符串。`+` 可以拼接两个 string 或 string 与字面量。

```cpp
std::string s = "Hello";
s += ", World";       // s = "Hello, World"
s.append("!");        // s = "Hello, World!"
```

#### 1.4.3 访问字符

用 `[]` 或 `at()` 按下标访问单个字符。`front()` 返回第一个字符，`back()` 返回最后一个字符。

```cpp
std::string s = "Hello";
char c = s[0];        // 'H'
char d = s.back();    // 'o'
```

#### 1.4.4 子串 substr

`substr(pos, len)` 从 pos 位置截取 len 个字符，返回新 string。不修改原字符串。

```cpp
std::string s = "Hello, World!";
std::string sub = s.substr(0, 5);   // "Hello"
```

#### 1.4.5 查找 find

`find(子串)` 返回子串第一次出现的位置（下标），找不到返回 `std::string::npos`（一个非常大的数）。

```cpp
std::string s = "Hello, World!";
size_t pos = s.find("World");    // pos = 7
size_t bad = s.find("xyz");      // bad = std::string::npos（未找到）
```

#### 1.4.6 替换 replace

`replace(pos, count, str)` 从 pos 位置替换 count 个字符为新字符串 str。会修改原字符串。

```cpp
std::string s = "Hello, World!";
s.replace(7, 5, "C++");   // s = "Hello, C++!"
```

#### 1.4.7 数值与字符串互转

`std::stoi()`/`std::stod()` 将字符串转为数值，`std::to_string()` 将数值转为字符串。

```cpp
int n = std::stoi("123");            // 123
double d = std::stod("3.14");        // 3.14
std::string s = std::to_string(42);  // "42"
```

#### 1.4.8 字符串比较

`std::string` 支持 `<`/`>`/`==` 等比较运算符，按字典序（逐字符 ASCII 值）比较。

```cpp
std::string a = "apple";
std::string b = "banana";
// a < b → true（'a' < 'b'）
```

#### 1.4.9 C 字符串互转

`c_str()` 返回 `const char*`，用于和 C 函数交互。从 `const char*` 可以直接构造 `std::string`。

```cpp
std::string s = "Hello";
const char* cs = s.c_str();      // string → const char*
std::string from_c(cs);          // const char* → string
```

### 1.5 std::string_view（C++17，只读视图）

#### 1.5.1 string_view 基本用法

`std::string_view` 是字符串的只读视图，不复制、不分配内存，只存一个指针和长度。可以从 `std::string` 或 C 字符串字面量构造。需要 `#include <string_view>`。

```cpp
#include <string_view>
#include <iostream>

void printSV(std::string_view sv) {
    std::cout << sv << " len=" << sv.length() << "\n";
}

int main() {
    std::string s = "Hello, World";
    printSV(s);               // 从 string 构造，不复制
    printSV("C++ literal");   // 从字面量构造，不复制
    return 0;
}
```

#### 1.5.2 string_view 比 const string& 更灵活

函数参数用 `std::string_view` 比 `const std::string&` 更灵活：`const string&` 接收字面量时需要构造临时 `string`，而 `string_view` 不需要，零开销。

#### 1.5.3 string_view 的子串也不复制

`string_view::substr()` 返回的也是 `string_view`，不分配新内存，只是调整起始指针和长度。

```cpp
std::string_view sv = "Hello, World";
std::string_view sub = sv.substr(0, 5);   // "Hello"，不分配内存
```

#### 1.5.4 注意：string_view 不拥有数据

`string_view` 不拥有底层数据，原字符串销毁后 `string_view` 变成悬空引用。不要把临时 `string` 的 `string_view` 存储到超出临时值生命周期的地方。

```cpp
// 危险！临时 string 销毁后 sv 悬空
// std::string_view sv = std::string("temp");
// std::cout << sv;   // 未定义行为
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/arrays_strings.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-ninja\arrays_strings.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/arrays_strings.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-make\arrays_strings.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/arrays_strings.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-mc\Release\arrays_strings.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\arrays_strings.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-msvc-ninja\arrays_strings.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\arrays_strings.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-msvc-vs\Release\arrays_strings.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/arrays_strings
```

## 3. 英文及缩写说明

### 3.1 数组与容器

| 词汇                | 说明                                            |
|-------------------|-----------------------------------------------|
| array             | 数组：同类型元素的连续序列，下标从 0 开始                        |
| index / subscript | 下标/索引：访问元素的位置编号，从 0 开始，越界访问是未定义行为             |
| `std::array`      | 标准库固定大小数组（C++11），有 `size()`/`at()` 方法，比原始数组安全 |
| `std::vector`     | 标准库动态数组：大小运行时可变，是最常用的容器                       |

### 3.2 vector 常用方法

| 方法                   | 说明                                              |
|----------------------|-------------------------------------------------|
| `push_back(x)`       | push back：在末尾追加元素 x                             |
| `pop_back()`         | pop back：移除末尾元素（不返回值）                           |
| `emplace_back(...)`  | emplace back：在末尾直接构造元素，比 `push_back` 少一次拷贝，效率更高 |
| `size()`             | 当前元素数量                                          |
| `capacity()`         | 容量：当前预分配内存能容纳的元素数；超出时自动扩容                       |
| `empty()`            | 是否为空，返回 `bool`                                  |
| `front()` / `back()` | 第一个 / 最后一个元素的引用                                 |
| `at(i)`              | 带越界检查的下标访问，越界抛 `std::out_of_range`              |
| `fill(v)`            | `std::array` 专用：把所有元素设置为 v                      |

### 3.3 string 常用方法

| 方法                    | 说明                                                    |
|-----------------------|-------------------------------------------------------|
| `length()` / `size()` | 字符串长度（字节数）                                            |
| `substr(pos, len)`    | substring：从 pos 位置取 len 个字符                           |
| `find(s)`             | 查找子串 s 第一次出现的位置，找不到返回 `string::npos`                  |
| `npos`                | not a position：`string::npos` 表示"没找到"，值为 `size_t` 最大值 |
| `stoi(s)`             | string to integer：字符串转 `int`                          |
| `stod(s)`             | string to double：字符串转 `double`                        |
| `to_string(n)`        | 数值转字符串                                                |
| `c_str()`             | 返回 C 风格字符串（`const char*`），与 C 接口交互时用                  |

### 3.4 类型与头文件

| 词汇         | 说明                                         |
|------------|--------------------------------------------|
| `size_t`   | size type：无符号整数类型，专门表示大小/长度                |
| `<array>`  | 包含 `std::array`                            |
| `<vector>` | 包含 `std::vector`                           |
| `<string>` | 包含 `std::string`、`stoi`、`stod`、`to_string` |
