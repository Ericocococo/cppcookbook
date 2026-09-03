# 05_functions — 函数

演示函数定义、默认参数、重载、内联函数、函数指针、lambda 表达式。

## 1. 知识点

| 文件 | 说明 |
|---|---|
| `main.cpp` | 8 个 demo 函数，覆盖函数的各种用法 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `functions` |

### 1.1 基本函数

#### 1.1.1 基本函数：定义、调用、返回值

函数是一段有名字的可复用代码，通过 `返回类型 函数名(参数列表)` 定义。调用时传入实参，函数执行后通过 `return` 返回结果。没有返回值的函数用 `void`。

```cpp
#include <iostream>

int add(int a, int b) {
    return a + b;
}

void printHello() {
    std::cout << "Hello!\n";
}

int main() {
    int sum = add(3, 5);       // 调用 add，返回 8
    std::cout << sum << "\n";  // 输出 8
    printHello();              // 调用 void 函数
    return 0;
}
```

#### 1.1.2 函数声明（前向声明）

函数可以先声明（只写签名，不写函数体），再在后面定义。这样调用方不需要看到完整定义，常用于头文件 `.h` 中声明、源文件 `.cpp` 中定义。

```cpp
#include <iostream>

int multiply(int a, int b);  // 声明（前向声明）

int main() {
    std::cout << multiply(3, 4) << "\n";  // OK：编译器已经知道签名
    return 0;
}

int multiply(int a, int b) {  // 定义（可以在声明之后）
    return a * b;
}
```

### 1.2 参数传递

#### 1.2.1 参数传递方式：值、引用、const 引用、指针

C++ 有四种主要传参方式。值传递拷贝一份副本，函数内修改不影响外部；引用传递直接操作外部变量；const 引用只读且不复制，适合传大对象；指针传递可以传 `nullptr` 表示"没有值"。

```cpp
#include <iostream>

void byValue(int x)            { x = 999; }       // 副本，不影响外部
void byRef(int& x)             { x *= 2; }        // 引用，修改外部变量
void byConstRef(const int& x)  { /* 只读 */ }     // 不复制，不能修改
void byPointer(int* p)         { if (p) *p = 42; } // 指针，可传 nullptr

int main() {
    int n = 10;

    byValue(n);
    std::cout << "值传参后 n=" << n << "\n";     // 10（未变）

    byRef(n);
    std::cout << "引用传参后 n=" << n << "\n";   // 20（变了）

    byPointer(&n);
    std::cout << "指针传参后 n=" << n << "\n";   // 42

    // 选择原则：
    // 小对象(int/double)       → 值传参
    // 需要修改外部变量           → 非 const 引用
    // 大对象只读(string/vector) → const 引用
    // 可能为 nullptr           → 指针
    return 0;
}
```

#### 1.2.2 const 引用可以绑定临时值

普通引用 `int&` 不能绑定临时值（右值），但 `const int&` 可以。编译器会延长临时值的生命周期到引用的作用域结束。

```cpp
// int& ref = 42;          // 错误：非 const 引用不能绑定临时值
const int& ref = 42;       // OK：const 引用绑定临时值，生命周期延长
std::cout << ref << "\n";  // 42
```

### 1.3 默认参数

#### 1.3.1 默认参数

函数参数可以指定默认值，调用时如果省略该参数就使用默认值。默认值在声明中从右往左设置，这样编译器才能确定哪些参数被省略。

```cpp
#include <iostream>
#include <string>

void greet(std::string name,
           std::string prefix = "Hello",
           char ending = '!') {
    std::cout << prefix << ", " << name << ending << "\n";
}

int main() {
    greet("Alice");                 // Hello, Alice!
    greet("Bob", "Hi");            // Hi, Bob!
    greet("Charlie", "Hey", '?');  // Hey, Charlie?
    return 0;
}
```

#### 1.3.2 规则：默认参数只能从右往左设置

右边的参数必须先有默认值，左边的才能有。否则编译器无法判断调用时省略了哪些参数。

```cpp
// void f(int a = 1, int b) {}   // 错误：b 在 a 右边但没有默认值
void f(int a, int b = 2) {}      // OK：从右往左设置
```

### 1.4 函数重载

#### 1.4.1 函数重载：同名函数，参数不同

同名函数如果参数类型或数量不同，就构成重载。编译器根据调用时传入的实参类型自动选择匹配的版本。

```cpp
#include <iostream>

int max(int a, int b)          { return a > b ? a : b; }
double max(double a, double b) { return a > b ? a : b; }
int max(int a, int b, int c)   { return max(max(a, b), c); }

int main() {
    std::cout << max(3, 5) << "\n";       // 调用 int 版 → 5
    std::cout << max(3.1, 2.7) << "\n";   // 调用 double 版 → 3.1
    std::cout << max(1, 5, 3) << "\n";    // 调用三参数版 → 5
    return 0;
}
```

#### 1.4.2 返回类型不同不构成重载

重载只看参数列表（类型和数量），不看返回类型。两个函数只有返回类型不同、参数相同时，编译会报错。

```cpp
// int    foo(int x) { return x; }
// double foo(int x) { return x; }  // 错误：参数相同，仅返回类型不同，不构成重载
```

### 1.5 inline / constexpr 函数

#### 1.5.1 inline 内联函数

`inline` 建议编译器将函数调用展开为函数体，避免函数调用的开销。适合短小、频繁调用的函数。现代编译器通常自动决定是否内联，`inline` 更多用于允许在头文件中定义函数而不产生重复定义错误。

```cpp
inline int square(int x) {
    return x * x;
}

int main() {
    int r = square(5);  // 编译器可能展开为: int r = 5 * 5;
    return 0;
}
```

#### 1.5.2 constexpr 函数：编译期可求值

`constexpr` 函数如果参数是编译期常量，结果在编译期就算出来。如果参数是运行期变量，退化为普通函数在运行期求值。编译期结果可以用作数组大小、模板参数等需要编译期常量的地方。

```cpp
constexpr int cube(int x) {
    return x * x * x;
}

int main() {
    constexpr int c = cube(3);  // 编译期求值 → 27
    int arr[c];                 // OK：c 是编译期常量，可作数组大小

    int n = 4;
    int d = cube(n);            // 运行期求值（n 不是编译期常量）
    return 0;
}
```

### 1.6 函数指针

#### 1.6.1 函数指针基础

函数名本身就是指向函数的地址。可以把函数赋值给一个函数指针变量，通过指针调用函数。

```cpp
#include <iostream>

int add(int a, int b) { return a + b; }

int main() {
    int (*fp)(int, int) = add;         // fp 是函数指针，指向 add
    std::cout << fp(3, 4) << "\n";     // 7：通过指针调用
    return 0;
}
```

#### 1.6.2 函数指针类型声明

函数指针的类型写法是 `返回类型 (*指针名)(参数类型列表)`。括号不能省，否则含义完全不同。

```cpp
int (*fp)(int, int);    // fp 是指向 int(int,int) 函数的指针
// int *fp(int, int);   // 注意：这是声明一个返回 int* 的函数，不是指针！
```

#### 1.6.3 函数指针作为参数（回调）

把函数指针作为参数传给另一个函数，调用方可以传入不同的函数实现不同行为。这就是"回调"模式。

```cpp
#include <iostream>

int add(int a, int b)      { return a + b; }
int multiply(int a, int b) { return a * b; }

int applyOp(int a, int b, int (*op)(int, int)) {
    return op(a, b);
}

int main() {
    std::cout << applyOp(10, 3, add) << "\n";       // 13
    std::cout << applyOp(10, 3, multiply) << "\n";   // 30
    return 0;
}
```

#### 1.6.4 using 简化函数指针类型

函数指针类型写法冗长，可以用 `using` 定义别名，让代码更清晰易读。

```cpp
using BinOp = int(*)(int, int);

int add(int a, int b)      { return a + b; }
int multiply(int a, int b) { return a * b; }

int main() {
    BinOp ops[] = {add, multiply};
    std::cout << ops[0](2, 3) << "\n";  // 5（调用 add）
    std::cout << ops[1](2, 3) << "\n";  // 6（调用 multiply）
    return 0;
}
```

#### 1.6.5 std::function：更灵活的可调用对象包装

`std::function<返回类型(参数类型)>` 可以存储函数指针、lambda、函数对象等任何可调用的东西。比裸函数指针更灵活，但有少量性能开销。需要 `#include <functional>`。

```cpp
#include <functional>
#include <iostream>

int add(int a, int b) { return a + b; }

int main() {
    std::function<int(int, int)> f = add;
    std::cout << f(2, 3) << "\n";  // 5

    f = [](int a, int b) { return a - b; };  // 也可以存 lambda
    std::cout << f(10, 3) << "\n";  // 7
    return 0;
}
```

### 1.7 Lambda 表达式

#### 1.7.1 Lambda 表达式（C++11）

Lambda 是匿名函数，可以在需要的地方直接定义，不用单独写具名函数。特别适合作为回调、排序比较器等短小的一次性函数。

#### 1.7.2 Lambda 基本语法

Lambda 的完整语法：`[捕获列表](参数列表) -> 返回类型 { 函数体 }`。返回类型通常可以省略，编译器自动推导。

```cpp
#include <iostream>

int main() {
    auto mul = [](int a, int b) { return a * b; };
    std::cout << mul(3, 4) << "\n";  // 12

    // 显式指定返回类型
    auto div = [](double a, double b) -> double { return a / b; };
    std::cout << div(10.0, 3.0) << "\n";  // 3.33333
    return 0;
}
```

#### 1.7.3 捕获外部变量

Lambda 通过捕获列表 `[]` 访问外部变量。`[x]` 值捕获（拷贝一份副本，后续外部修改不影响 lambda 内的值）；`[&x]` 引用捕获（直接引用外部变量，外部修改立即反映到 lambda 内）。

```cpp
#include <iostream>

int main() {
    int factor = 3;
    auto scaleVal = [factor](int x)  { return x * factor; };  // 值捕获
    auto scaleRef = [&factor](int x) { return x * factor; };  // 引用捕获

    factor = 10;
    std::cout << scaleVal(5) << "\n";  // 15（用捕获时的副本 3）
    std::cout << scaleRef(5) << "\n";  // 50（用引用的最新值 10）
    return 0;
}
```

#### 1.7.4 mutable lambda：修改值捕获的副本

默认情况下，值捕获的副本在 lambda 内是只读的。加 `mutable` 后可以修改副本，但不影响外部原变量。每次调用时副本的修改会保留到下次调用。

```cpp
#include <iostream>

int main() {
    int count = 0;
    auto inc = [count]() mutable { return ++count; };

    std::cout << inc() << "\n";  // 1
    std::cout << inc() << "\n";  // 2
    std::cout << inc() << "\n";  // 3
    std::cout << "外部 count=" << count << "\n";  // 0（外部未变）
    return 0;
}
```

#### 1.7.5 泛型 lambda（C++14）：参数用 auto

C++14 起 lambda 参数可以用 `auto`，编译器根据调用时的实参类型自动推导，相当于函数模板。

```cpp
#include <iostream>

int main() {
    auto print = [](auto a, auto b) {
        std::cout << "(" << a << ", " << b << ")\n";
    };
    print(1, 3.14);     // (1, 3.14)
    print("hi", 42);    // (hi, 42)
    return 0;
}
```

#### 1.7.6 立即调用 lambda（IIFE）

Lambda 定义后加 `()` 立即调用，常用于初始化需要复杂计算的 `const` 变量。IIFE = Immediately Invoked Function Expression。

```cpp
int result = [](int n) { return n * n; }(7);
// result = 49
```

### 1.8 返回类型

#### 1.8.1 尾置返回类型（trailing return type，C++11）

用 `auto 函数名(参数) -> 返回类型` 把返回类型写在参数列表后面。在模板等场景下返回类型依赖参数时特别有用。

```cpp
auto divide(double a, double b) -> double {
    return a / b;
}
// 等价于：double divide(double a, double b) { return a / b; }
```

#### 1.8.2 返回引用的危险：不要返回局部变量的引用

函数内的局部变量在函数返回后就销毁了。如果返回局部变量的引用或指针，调用方拿到的是悬空引用，使用它是未定义行为。

```cpp
// 危险示例（不要这样写！）
int& badReturn() {
    int local = 42;
    return local;  // 错误：local 在函数结束后销毁，引用悬空
}

// 安全做法：返回值的拷贝
int safeReturn() {
    int local = 42;
    return local;  // OK：返回值的拷贝
}
// 也可以安全地返回：引用参数、成员变量、静态变量的引用
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/functions.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-ninja\functions.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/functions.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-make\functions.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/functions.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-mc\Release\functions.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\functions.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-msvc-ninja\functions.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\functions.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-msvc-vs\Release\functions.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/functions
```

## 3. 英文及缩写说明

### 3.1 关键字

| 词汇       | 说明                                        |
|----------|-------------------------------------------|
| `inline` | 内联：建议编译器把函数调用展开为函数体，避免调用开销；适合短小、频繁调用的函数   |
| `return` | 返回：退出函数并返回一个值；`void` 函数也可写 `return;` 提前退出 |
| `void`   | 空：作为返回类型时表示函数没有返回值                        |

### 3.2 概念

| 概念                      | 说明                                                      |
|-------------------------|---------------------------------------------------------|
| pass by value           | 值传递：函数拿到参数的副本，函数内修改不影响外部变量                              |
| pass by reference       | 引用传递：参数写 `int& x`，函数内修改直接影响外部变量                         |
| pass by const reference | 常量引用传递：`const int& x`，不拷贝也不修改，适合传大对象（`string`、`vector`） |
| default argument        | 默认参数：调用时可省略；只能从右往左设置                                    |
| function overload       | 函数重载：同名函数有不同参数类型或数量，编译器自动选择                             |
| function pointer        | 函数指针：把函数当变量传递；`int (*fp)(int,int)` 声明函数指针               |
| lambda expression       | lambda 表达式（C++11）：匿名函数，在用到的地方直接定义                       |
| capture list            | 捕获列表：lambda 的 `[]` 部分，指定从外部捕获哪些变量                       |
| closure                 | 闭包：lambda 加上它捕获的外部变量，整体打包成可调用对象                         |
| value capture           | 值捕获 `[x]`：拿到副本，外部修改不影响 lambda 内的值                       |
| reference capture       | 引用捕获 `[&x]`：直接引用外部变量，外部修改立即反映                           |
