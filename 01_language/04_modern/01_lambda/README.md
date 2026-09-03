# 01_lambda — lambda 表达式

> C++11 引入，C++14/20 持续增强。把函数写成"就地定义的匿名函数"，最常配合 STL 算法使用。

## 1. 知识点

### 1.1 基本语法

#### 1.1.1 lambda 基本语法

```cpp
// [捕获列表](参数列表) -> 返回类型 { 函数体 }
// 返回类型可省略，编译器自动推导
```

#### 1.1.2 最简 lambda — 无参数、无捕获

```cpp
auto hello = []() {
    std::cout << "hello lambda\n";
};
hello();
```

#### 1.1.3 带参数

```cpp
auto add = [](int a, int b) {
    return a + b;
};
int sum = add(3, 5);  // 8
```

#### 1.1.4 显式指定返回类型

```cpp
// 通常不需要，编译器能推导；当函数体有多条 return 且类型不同时才需要显式写
auto divide = [](double a, double b) -> double {
    if (b == 0) {
        return 0.0;
    }
    return a / b;
};
```

### 1.2 捕获列表

#### 1.2.1 捕获列表语法一览

```cpp
// []      不捕获任何外部变量
// [x]     按值捕获 x（拷贝一份，lambda 内修改不影响外部）
// [&x]    按引用捕获 x（lambda 内修改会影响外部）
// [=]     按值捕获所有外部变量
// [&]     按引用捕获所有外部变量
// [=, &x] 默认按值，x 按引用
// [&, x]  默认按引用，x 按值
```

#### 1.2.2 按值捕获 x — lambda 内拿到的是拷贝

```cpp
int x = 10;
auto by_value = [x]() {
    std::cout << x << "\n";  // 拿到的是拷贝
    // x = 99;  // 编译错误：按值捕获默认是 const
};
```

#### 1.2.3 按引用捕获 x — lambda 内修改会影响外部

```cpp
int x = 10;
auto by_ref = [&x]() {
    x = 99;
};
by_ref();
// 此时外部 x == 99
```

#### 1.2.4 按值捕获所有

```cpp
int x = 10, y = 20;
auto f = [=]() {
    std::cout << x << " " << y << "\n";
};
```

#### 1.2.5 按引用捕获所有

```cpp
int x = 10, y = 20;
auto f = [&]() {
    x = 100;
    y = 200;
};
f();
// 此时 x == 100, y == 200
```

### 1.3 mutable

#### 1.3.1 mutable — 允许修改按值捕获的变量

```cpp
// 修改的是拷贝，不影响外部
int count = 0;
auto counter = [count]() mutable {
    count++;
    std::cout << count << "\n";
};
```

#### 1.3.2 mutable 使用效果

```cpp
counter();  // 1（lambda 内部的拷贝 +1）
counter();  // 2（lambda 内部的拷贝再 +1）
counter();  // 3
std::cout << count << "\n";  // 0（外部 count 没变）
```

### 1.4 泛型 lambda

#### 1.4.1 泛型 lambda（C++14）— 参数用 auto

```cpp
// 类似函数模板，编译器对每种类型生成一个版本
auto print = [](const auto& value) {
    std::cout << value << "\n";
};
```

#### 1.4.2 auto 参数 — 编译器对每种类型生成一个版本

```cpp
print(42);                     // int
print(3.14);                   // double
print(std::string("hello"));   // std::string
```

### 1.5 lambda + STL

#### 1.5.1 lambda 作为函数参数

```cpp
// STL 算法最常见的用法：把 lambda 当谓词或操作传给算法
std::vector<int> nums = {5, 2, 8, 1, 9, 3, 7};
```

#### 1.5.2 sort — 自定义比较规则

```cpp
std::sort(nums.begin(), nums.end(), [](int a, int b) {
    return a > b;  // 降序
});
```

#### 1.5.3 count_if — 统计满足条件的元素个数

```cpp
int even_count = std::count_if(nums.begin(), nums.end(), [](int n) {
    return n % 2 == 0;
});
```

#### 1.5.4 for_each — 对每个元素执行操作

```cpp
std::for_each(nums.begin(), nums.end(), [](int& n) {
    n += 10;
});
```

#### 1.5.5 find_if — 查找第一个满足条件的元素

```cpp
auto it = std::find_if(nums.begin(), nums.end(), [](int n) {
    return n > 15;
});
if (it != nums.end()) {
    std::cout << *it << "\n";
}
```

### 1.6 std::function

#### 1.6.1 std::function 概览

```cpp
// 可以存任意可调用对象（lambda、函数指针、仿函数）
// 常用于做回调、存到容器里
```

#### 1.6.2 std::function 存任意可调用对象

```cpp
std::function<int(int, int)> op;
op = [](int a, int b) { return a + b; };
std::cout << op(3, 4) << "\n";  // 7

op = [](int a, int b) { return a * b; };
std::cout << op(3, 4) << "\n";  // 12
```

#### 1.6.3 存到容器批量执行

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

### 1.7 IIFE

#### 1.7.1 立即调用的 lambda（IIFE）

```cpp
// 定义 lambda 后紧跟 () 立即调用
```

#### 1.7.2 IIFE 用于复杂 const 初始化

```cpp
const int value = [] {
    int result = 0;
    for (int i = 1; i <= 10; i++) {
        result += i;
    }
    return result;
}();
// value == 55
```

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
