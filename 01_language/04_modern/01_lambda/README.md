# 01_lambda — lambda 表达式

> C++11 引入，C++14/20 持续增强。把函数写成"就地定义的匿名函数"，最常配合 STL 算法使用。
>
> **本章是补充章**：lambda 的入门讲解（基本语法、捕获、mutable、泛型 lambda、IIFE）已在 01_basics 05_functions 1.7 逐一详讲并有完整示例，本章不再重复展开，只补 4 个实战向内容：捕获七种形式一览、默认捕获 `[=]`/`[&]`、lambda 与 STL 算法、`std::function` 批量回调。可运行演示全集在 main.cpp（demo01–demo07）。

## 1. 知识点

### 1.1 基本语法

#### 1.1.1 lambda 基本语法

```cpp
// [捕获列表](参数列表) -> 返回类型 { 函数体 }
// 返回类型可省略，编译器自动推导
```

#### 1.1.2 最简 lambda、带参数、显式返回类型（回顾）

`[](){}` 三部分写法、省略参数、自动推导返回类型、多条 return 时才显式写 `-> 类型`——这些基本用法在 01_basics 05_functions 1.7.1/1.7.2 已逐个讲透，运行演示见 main.cpp demo01，本章不再重复。

### 1.2 捕获列表

#### 1.2.1 捕获列表语法一览

| 写法 | 含义 | 能否修改外部变量 |
|------|------|------|
| `[]` | 不捕获任何外部变量 | — |
| `[x]` | 按值捕获 x（拷贝一份） | 不能（默认 const；加 mutable 可改拷贝） |
| `[&x]` | 按引用捕获 x | 能 |
| `[=]` | 按值捕获所有用到的外部变量 | 不能 |
| `[&]` | 按引用捕获所有用到的外部变量 | 能 |
| `[=, &x]` | 默认按值，x 例外按引用 | 能改 x，改不了其他 |
| `[&, x]` | 默认按引用，x 例外按值 | 能改 x 之外的 |

#### 1.2.2 按值 / 按引用捕获 x（回顾）

`[x]` 拿到拷贝（lambda 内修改不影响外部）、`[&x]` 直接用外部变量（修改影响外部）——这两个基础捕获在 01_basics 05_functions 1.7.3 已详解，运行演示见 main.cpp demo02，这里不再重复。

#### 1.2.3 按值捕获所有

```cpp
int x = 10, y = 20;
auto f = [=]() {
    std::cout << x << " " << y << "\n";
};
```

#### 1.2.4 按引用捕获所有

```cpp
int x = 10, y = 20;
auto f = [&]() {
    x = 100;
    y = 200;
};
f();
// 此时 x == 100, y == 200
```

### 1.3 mutable 与泛型 lambda（回顾）

#### 1.3.1 mutable 与泛型 lambda（回顾）

`mutable` 允许修改按值捕获的拷贝（改的是副本不影响外部）、泛型 lambda 用 `auto` 参数让编译器为每种类型生成版本——两者在 01_basics 05_functions 1.7.4/1.7.5 已详讲，运行演示见 main.cpp demo03/demo04，本章不再重复。

### 1.4 lambda 与 STL 算法

#### 1.4.1 lambda 作为函数参数（总起）

STL 算法的常见形态：把 lambda 当**谓词**（返回 `bool`）或**操作**传给算法，让算法替你写循环。下面按用途演示四个最常用的：`sort`（排序规则）、`count_if`（统计）、`for_each`（逐个操作）、`find_if`（查找）。示例容器：

```cpp
std::vector<int> nums = {5, 2, 8, 1, 9, 3, 7};
```

#### 1.4.2 sort — 自定义比较规则

```cpp
std::sort(nums.begin(), nums.end(), [](int a, int b) {
    return a > b;  // 降序
});
```

#### 1.4.3 count_if — 统计满足条件的元素个数

```cpp
int even_count = std::count_if(nums.begin(), nums.end(), [](int n) {
    return n % 2 == 0;
});
```

#### 1.4.4 for_each — 对每个元素执行一次操作

`for_each` 替代手写 `for` 循环：对容器每个元素调用一次 lambda（按引用收参可以就地修改）。

```cpp
std::for_each(nums.begin(), nums.end(), [](int& n) {
    n += 10;
});
```

#### 1.4.5 find_if — 查找第一个满足条件的元素

`find_if` 线性查找，返回第一个满足条件的元素的迭代器；找不到就返回 `end()`，所以要先比较再使用。

```cpp
auto it = std::find_if(nums.begin(), nums.end(), [](int n) {
    return n > 15;
});
if (it != nums.end()) {
    std::cout << *it << "\n";
}
```

### 1.5 std::function 批量回调

#### 1.5.1 std::function 是什么（回顾）

`std::function` 可以存任意可调用对象（lambda、函数指针、仿函数），在 01_basics 05_functions 1.6.5 已讲过基本用法（存一个、换一个再调用）。本章补它的实战场景——1.6.2 存到容器里批量执行。

#### 1.5.2 存到容器批量执行

```cpp
std::vector<std::function<int(int, int)>> ops = {
    [](int a, int b) { return a + b; },
    [](int a, int b) { return a - b; },
    [](int a, int b) { return a * b; },
};
for (const auto& f : ops) {
    std::cout << f(10, 3) << " ";  // 13 7 30
}
```

### 1.6 IIFE（回顾）

#### 1.6.1 立即调用与复杂 const 初始化（回顾）

定义 lambda 后紧跟 `()` 立即调用（IIFE，Immediately Invoked Function Expression），常用于一段逻辑复杂的 `const` 变量初始化——已在 01_basics 05_functions 1.7.6 详讲，运行演示见 main.cpp demo07，本章不再重复。

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/lambda.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-ninja\lambda.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/lambda.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-make\lambda.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/lambda.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-mc\Release\lambda.exe
```

</details>

### 2.2 命令行 · MSVC（cmd）

> MSVC 路径常量与 vcvarsall 注入原理（4 个环境变量、为什么必须 `call`）见首次详解 [00_hello_world §2.2](../../01_basics/00_hello_world/README.md)，以下命令直接用。

```bat
:: 激活 — 把 cl.exe / link.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```

#### 方案 A — vcvarsall + Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\lambda.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-msvc-ninja\lambda.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\lambda.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-msvc-vs\Release\lambda.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/lambda
```
