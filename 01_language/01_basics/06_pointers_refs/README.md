# 06_pointers_refs — 指针与引用

演示引用、指针、`nullptr`、const 指针四种形式、指针算术、`new`/`delete`、智能指针。

## 1. 知识点

| 文件 | 说明 |
|---|---|
| `main.cpp` | 7 个 demo 函数，从引用到智能指针逐步讲解 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `pointers_refs` |

### 1.1 引用

#### 1.1.1 引用：变量的别名

引用是变量的另一个名字，声明时必须初始化，之后不能改变绑定对象。修改引用等同于修改原变量，引用和原变量共享同一块内存地址。

```cpp
#include <iostream>

int main() {
    int a = 10;
    int& ref = a;  // ref 是 a 的别名，必须初始化

    std::cout << "a=" << a << "\n";      // 10
    std::cout << "ref=" << ref << "\n";  // 10（同一个值）
    // &a == &ref  → 地址相同

    ref = 999;
    std::cout << "ref=999 后 a=" << a << "\n";  // 999（修改引用 = 修改原变量）
    return 0;
}
```

#### 1.1.2 引用 vs 指针

引用必须初始化、不能为空、不能改变绑定；指针可以不初始化、可以为 nullptr、可以改变指向。引用语法更简洁安全，能用引用就不用指针。

```cpp
int a = 10, b = 20;

int& ref = a;     // 引用：必须初始化，不能改为绑定 b
// int& bad;       // 错误：引用必须初始化

int* ptr = &a;    // 指针：可以改变指向
ptr = &b;         // OK
ptr = nullptr;    // OK：指针可以为空
```

#### 1.1.3 const 引用：能绑定临时值

`const int&` 能绑定临时值（右值），普通 `int&` 不行。绑定时编译器会把临时值的生命周期延长到引用作用域结束，引用期间值一直有效。这个特性最大的用途是函数参数——`const T&` 参数能直接接受字面量和临时对象，原理与展开示例已在 05_functions 1.2.2 详讲。

```cpp
const int& cref = 42;   // OK：const 引用绑定临时值，生命周期延长到 cref 结束
// int& ref = 42;        // 错误：非 const 引用不能绑定临时值
```

#### 1.1.4 右值引用（C++11）：绑定临时值，用于移动语义

`int&&` 是右值引用，只能绑定临时值（右值）。右值引用是移动语义和完美转发的基础，允许"窃取"临时对象的资源而不是拷贝。

```cpp
int&& rref = 10 + 20;   // rref 绑定临时值 30
// int a = 5;
// int&& bad = a;        // 错误：a 是左值，不能绑定到右值引用
```

### 1.2 指针

#### 1.2.1 指针：存储地址的变量

指针变量存储的是另一个变量的内存地址。`&` 取地址，`*` 声明指针或解引用（通过地址访问值）。通过指针可以间接修改目标变量。

```cpp
#include <iostream>

int main() {
    int a = 10;
    int* p = &a;     // p 存的是 a 的地址

    std::cout << "p=" << p << "\n";    // 地址值
    std::cout << "*p=" << *p << "\n";  // 10（解引用得到 a 的值）

    *p = 999;         // 通过指针修改值
    std::cout << "a=" << a << "\n";    // 999
    return 0;
}
```

#### 1.2.2 未初始化指针：危险

未初始化的指针指向随机地址，解引用它是未定义行为，可能导致崩溃或数据损坏。声明指针时应该立即初始化（赋地址或 `nullptr`）。

```cpp
// int* bad;      // 危险：bad 指向随机地址
// *bad = 1;      // 未定义行为：可能崩溃

int* safe = nullptr;  // 安全：先置空，用前检查
```

#### 1.2.3 nullptr（C++11）：类型安全的空指针

`nullptr` 是 C++11 引入的空指针常量，替代 C 语言的 `NULL` 和字面量 `0`。`nullptr` 是 `std::nullptr_t` 类型，不会和整数混淆，函数重载时更安全。

```cpp
int* p = nullptr;
if (p == nullptr) {
    // 指针为空，不能解引用
}
// 推荐用 nullptr，不要用 NULL 或 0
```

#### 1.2.4 指针的指针

指针本身也是变量，可以用另一个指针存储它的地址。`int**` 是指向 `int*` 的指针，解引用一次得到 `int*`，解引用两次得到 `int`。

```cpp
int a = 42;
int* p = &a;
int** pp = &p;     // pp 指向 p

// *pp == p        → p 的值（a 的地址）
// **pp == 42      → a 的值
```

### 1.3 const 指针

#### 1.3.1 const 指针四种形式

const 和指针组合有四种写法，理解方法是"从右往左读"。区分的关键：const 修饰的是"指向的值"还是"指针本身"。

#### 1.3.2 const int* p —— 指向 const 的指针

不能通过指针修改值，但可以改变指针的指向。从右往左读：p 是指针（`*`），指向 `const int`。

```cpp
int a = 10, b = 20;
const int* p = &a;
// *p = 99;     // 错误：不能改值
p = &b;         // OK：可以改指向
```

#### 1.3.3 int* const p —— const 指针

可以通过指针修改值，但不能改变指针的指向。从右往左读：p 是 `const` 的指针（`* const`），指向 `int`。

```cpp
int a = 10, b = 20;
int* const p = &a;
*p = 99;        // OK：可以改值
// p = &b;      // 错误：不能改指向
```

#### 1.3.4 const int* const p —— 指向 const 的 const 指针

既不能改值，也不能改指向。最严格的形式。

```cpp
int a = 10;
const int* const p = &a;
// *p = 99;     // 错误：不能改值
// p = &b;      // 错误：不能改指向
```

#### 1.3.5 助记：从右往左读

看 `const` 靠近谁来判断它修饰什么：`const int*` → `*p` 是 `const int` → 值不可改；`int* const` → `p` 是 `const` → 指向不可改。

### 1.4 指针算术

#### 1.4.1 指针加减：+1 移动一个元素大小

对指针做 `+1`，地址不是加 1 字节，而是加一个元素的大小（`sizeof(T)` 字节）。这使得指针天然适合遍历数组。

```cpp
int arr[] = {10, 20, 30, 40, 50};
int* p = arr;        // 数组名 = 首元素地址

// *p == 10（arr[0]）
++p;
// *p == 20（arr[1]，移动了 sizeof(int)=4 字节）
```

#### 1.4.2 用指针遍历数组

`arr` 是首元素地址，`arr + 5` 是尾后地址。用指针从 `arr` 遍历到 `arr + n` 就能访问所有元素。

```cpp
int arr[] = {10, 20, 30, 40, 50};
for (int* q = arr; q < arr + 5; ++q) {
    std::cout << *q << " ";
}
// 输出：10 20 30 40 50
```

#### 1.4.3 指针差：两指针相减得到元素个数

两个指向同一数组的指针相减，得到的是元素个数（不是字节数）。结果类型是 `ptrdiff_t`。

```cpp
int arr[] = {10, 20, 30, 40, 50};
int* first = arr;
int* last = arr + 4;
// last - first == 4（元素个数，不是字节数）
```

#### 1.4.4 注意：指针算术只在同一数组内有效

两个不属于同一数组的指针做加减或比较，是未定义行为。越界访问同样是未定义行为。

### 1.5 void* 指针

#### 1.5.1 void* 万能指针

`void*` 可以指向任何类型的对象，但不能直接解引用（编译器不知道指向什么类型）。必须用 `static_cast` 转换为正确类型后才能解引用。C 语言的 `malloc`/`memcpy` 大量使用 `void*`，C++ 推荐用模板替代。

```cpp
int n = 42;
void* vp = &n;         // void* 可以指向任何类型
// *vp = 1;             // 错误：不能解引用 void*

int* ip = static_cast<int*>(vp);   // 转换为正确类型
// *ip == 42

double d = 3.14;
vp = &d;                // 改为指向 double，也 OK
```

### 1.6 new / delete

#### 1.6.1 动态内存分配基础

`new` 在堆上分配内存并返回指针，`delete` 释放。堆上的内存不会自动释放，必须手动管理。分配数组用 `new[]`，释放用 `delete[]`，两者必须配对。

#### 1.6.2 分配单个对象

```cpp
int* p = new int(42);    // 堆上分配一个 int，初始化为 42
// *p == 42
delete p;                // 必须释放
p = nullptr;             // 释放后置空，防止悬空指针
```

#### 1.6.3 分配数组

```cpp
int* arr = new int[5]{1, 2, 3, 4, 5};
// arr[0]=1, arr[1]=2, ...
delete[] arr;             // 数组必须用 delete[]，不能用 delete
arr = nullptr;
```

#### 1.6.4 常见错误

手动管理内存容易出四类错误：忘记 `delete` 导致内存泄漏；`delete` 后继续使用导致悬空指针（未定义行为）；重复 `delete` 导致崩溃；`new[]` 配 `delete`（不是 `delete[]`）导致未定义行为。推荐用智能指针代替裸 `new`/`delete`。

### 1.7 智能指针

#### 1.7.1 智能指针概述（C++11）

智能指针利用 RAII 原则，在离开作用域时自动释放内存，解决手动 `delete` 的痛点。三种智能指针定义在 `<memory>` 头文件中。

#### 1.7.2 unique_ptr：独占所有权

`unique_ptr` 独占所指对象的所有权，不能复制只能移动。离开作用域时自动 `delete`。用 `std::make_unique<T>(args)` 创建（C++14）。大多数场景首选 `unique_ptr`。

```cpp
#include <memory>

{
    auto up = std::make_unique<int>(10);
    // *up == 10

    // auto up2 = up;               // 错误：不能复制
    auto up2 = std::move(up);       // OK：移动所有权
    // up 变为 nullptr，up2 拥有资源
}  // up2 离开作用域，自动释放
```

#### 1.7.3 shared_ptr：共享所有权，引用计数

多个 `shared_ptr` 可以指向同一个对象，内部维护引用计数。最后一个 `shared_ptr` 销毁时才释放对象。用 `std::make_shared<T>(args)` 创建。

```cpp
#include <memory>

auto sp1 = std::make_shared<int>(99);
// sp1.use_count() == 1

{
    auto sp2 = sp1;                // 共享，引用计数 +1
    // sp1.use_count() == 2
}  // sp2 销毁，引用计数 -1

// sp1.use_count() == 1
```

#### 1.7.4 weak_ptr：弱引用，解决循环引用

`weak_ptr` 观察 `shared_ptr` 管理的对象，但不增加引用计数。用 `lock()` 尝试提升为 `shared_ptr`，如果对象已释放则返回空。主要用于解决 `shared_ptr` 之间的循环引用问题。

```cpp
#include <memory>

auto sp = std::make_shared<int>(42);
std::weak_ptr<int> wp = sp;
// wp.expired() == false（对象还活着）

if (auto locked = wp.lock()) {
    // locked 是 shared_ptr，*locked == 42
}

// 选择原则：
// 独占所有权   → unique_ptr（首选）
// 共享所有权   → shared_ptr
// 观察但不拥有 → weak_ptr（解决循环引用）
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/pointers_refs.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-ninja\pointers_refs.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/pointers_refs.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-make\pointers_refs.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/pointers_refs.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-mc\Release\pointers_refs.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\pointers_refs.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-msvc-ninja\pointers_refs.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\pointers_refs.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-msvc-vs\Release\pointers_refs.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/pointers_refs
```

## 3. 英文及缩写说明

### 3.1 关键字与运算符

| 词汇         | 说明                                       |
|------------|------------------------------------------|
| `&`（取地址）   | address-of operator：取变量的内存地址，结果是指针       |
| `*`（声明指针）  | pointer declarator：声明指针变量，如 `int* p`     |
| `*`（解引用）   | dereference operator：通过指针访问所指地址的值，如 `*p` |
| `nullptr`  | null pointer：空指针（C++11），比 `NULL` 类型安全    |
| `new`      | 在堆（heap）上动态分配内存，返回指针                     |
| `delete`   | 释放 `new` 分配的单个对象内存，不释放 = 内存泄漏            |
| `delete[]` | 释放 `new[]` 分配的数组内存，必须与 `new[]` 配对        |

### 3.2 概念

| 概念               | 说明                                                                |
|------------------|-------------------------------------------------------------------|
| reference        | 引用：变量的别名，声明时必须初始化，不能为空                                            |
| pointer          | 指针：存储内存地址的变量，可为 `nullptr`，可改变指向                                   |
| stack            | 栈：函数内声明的变量，函数返回时自动释放                                              |
| heap             | 堆：`new` 分配的内存，程序员负责手动释放                                           |
| memory leak      | 内存泄漏：`new` 后忘记 `delete`，内存无法回收                                    |
| dangling pointer | 悬空指针：指向已释放内存的指针，访问它是未定义行为                                         |
| RAII             | Resource Acquisition Is Initialization：资源获取即初始化，析构时自动释放；智能指针的设计原则 |
| `unique_ptr`     | 独占所有权的智能指针，不能复制，离开作用域自动 `delete`                                  |
| `shared_ptr`     | 共享所有权的智能指针，引用计数归零时释放                                              |
| `make_unique`    | 创建 `unique_ptr` 的工厂函数（C++14），比直接 `new` 更安全                        |
| `make_shared`    | 创建 `shared_ptr` 的工厂函数，一次内存分配效率更高                                  |
| `use_count`      | `shared_ptr` 的引用计数：当前有几个 `shared_ptr` 共享此内存                       |
| `<memory>`       | 包含 `unique_ptr`、`shared_ptr`、`make_unique`、`make_shared`          |
