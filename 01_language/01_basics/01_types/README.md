# 01_types — 数据类型

演示 C++ sizeof 运算符、内置数据类型、字面量后缀、`auto`/`decltype` 类型推断、`const`/`constexpr` 常量。

## 1. 知识点

### 1.1 sizeof 运算符

`sizeof` 是编译期运算符（不是函数），返回类型或变量占用的字节数（`size_t` 类型）。编译时就确定结果，运行时零开销。C++ 标准保证 `sizeof(char) == 1`，其他类型的大小由实现决定。

以下是 64 位系统上各基本类型的典型大小：

| 类型 | 字节数 | 说明 |
|------|--------|------|
| `char` | 1 | 标准保证恒为 1 |
| `bool` | 1 | 通常 1 字节 |
| `short` | 2 | 至少 2 字节 |
| `int` | 4 | 至少 2 字节，通常 4 |
| `long` | 4（Win）/ 8（Linux） | 至少 4 字节，平台相关 |
| `long long` | 8 | 至少 8 字节（C++11） |
| `float` | 4 | IEEE 754 单精度 |
| `double` | 8 | IEEE 754 双精度 |

```cpp
#include <iostream>

int main() {
    // sizeof(类型名)：括号必须加
    std::cout << sizeof(int) << "\n";       // 4
    std::cout << sizeof(double) << "\n";    // 8

    // sizeof 变量：括号可省，但建议统一加
    int x = 42;
    std::cout << sizeof(x) << "\n";         // 4（和 sizeof(int) 相同）
    std::cout << sizeof x << "\n";          // 4（变量可省括号，类型名不行）
}
```

#### 1.1.1 sizeof 与数组

`sizeof(数组)` 返回整个数组占用的总字节数，不是元素个数。用 `sizeof(arr) / sizeof(arr[0])` 可以计算元素个数。注意：数组作为函数参数传递时退化为指针，`sizeof` 返回的是指针大小（8 字节），不是数组大小，所以这个技巧只能在数组定义的作用域内使用。

```cpp
int arr[5] = {1, 2, 3, 4, 5};
std::cout << sizeof(arr) << "\n";           // 20（5 个 int = 5×4）
std::cout << sizeof(arr[0]) << "\n";        // 4（单个元素）
std::cout << sizeof(arr) / sizeof(arr[0]);  // 5（元素个数）

// 注意：数组退化为指针后，sizeof 不再返回数组大小
void foo(int a[]) {
    std::cout << sizeof(a);   // 8（指针大小，不是数组大小！）
}
```

### 1.2 整数类型

#### 1.2.1 有符号整数

C++ 提供 5 种有符号整数类型，可以存负数，范围是 -2^(n-1) ~ 2^(n-1)-1，其中 n 是位数。字节数和范围随平台变化，以下为 64 位系统典型值。需要 `<climits>` 头文件来获取各类型的范围常量。

| 类型 | 字节数（64 位） | 范围 | 范围常量 |
|------|---------------|------|---------|
| `char` | 1 | -128 ~ 127 | `CHAR_MIN` / `CHAR_MAX` |
| `short` | 2 | -32,768 ~ 32,767 | `SHRT_MIN` / `SHRT_MAX` |
| `int` | 4 | ≈ ±21.5 亿 | `INT_MIN` / `INT_MAX` |
| `long` | 4 或 8 | 平台相关 | `LONG_MIN` / `LONG_MAX` |
| `long long` | 8 | ≈ ±9.2×10¹⁸ | `LLONG_MIN` / `LLONG_MAX` |

```cpp
#include <iostream>
#include <climits>   // INT_MAX / INT_MIN 等范围常量

int main() {
    std::cout << "char:      " << sizeof(char)      << " 字节  "
              << (int)CHAR_MIN << " ~ " << (int)CHAR_MAX << "\n";
    std::cout << "short:     " << sizeof(short)     << " 字节  "
              << SHRT_MIN << " ~ " << SHRT_MAX << "\n";
    std::cout << "int:       " << sizeof(int)       << " 字节  "
              << INT_MIN  << " ~ " << INT_MAX  << "\n";
    std::cout << "long:      " << sizeof(long)      << " 字节  "
              << LONG_MIN << " ~ " << LONG_MAX << "\n";
    std::cout << "long long: " << sizeof(long long) << " 字节  "
              << LLONG_MIN << " ~ " << LLONG_MAX << "\n";
}
```

#### 1.2.2 无符号整数

在类型前加 `unsigned`，不能存负数，但正数范围翻倍（0 ~ 2^n-1）。`unsigned` 单独写等价于 `unsigned int`。

| 类型 | 字节数（64 位） | 范围 | 范围常量 |
|------|---------------|------|---------|
| `unsigned char` | 1 | 0 ~ 255 | `UCHAR_MAX` |
| `unsigned short` | 2 | 0 ~ 65,535 | `USHRT_MAX` |
| `unsigned int` | 4 | 0 ~ ≈42.9 亿 | `UINT_MAX` |
| `unsigned long long` | 8 | 0 ~ ≈1.8×10¹⁹ | `ULLONG_MAX` |

```cpp
#include <iostream>
#include <climits>

int main() {
    std::cout << "unsigned char:      " << sizeof(unsigned char)
              << " 字节  0 ~ " << (unsigned)UCHAR_MAX << "\n";
    std::cout << "unsigned int:       " << sizeof(unsigned int)
              << " 字节  0 ~ " << UINT_MAX << "\n";
    std::cout << "unsigned long long: " << sizeof(unsigned long long)
              << " 字节  0 ~ " << ULLONG_MAX << "\n";
}
```

#### 1.2.3 整数溢出

无符号整数溢出时按模运算回绕（定义行为，合法）；有符号整数溢出是未定义行为（UB），编译器可以做任何事，不要依赖有符号溢出的结果。

```cpp
unsigned char uc = 255;
++uc;
// uc 变为 0：无符号溢出取模 256，合法的定义行为

// int si = INT_MAX;
// ++si;
// 有符号溢出：未定义行为！编译器可能优化掉后续的溢出检测代码
```

### 1.3 浮点类型

| 类型 | 字节数 | 有效数字 | 用途 |
|------|--------|---------|------|
| `float` | 4 | ≈7 位 | 省内存、GPU 计算 |
| `double` | 8 | ≈15 位 | 推荐默认使用 |
| `long double` | 8 或 16 | ≥15 位 | 极少使用 |

#### 1.3.1 浮点精度问题

计算机用二进制存储小数，`0.1`、`0.2` 在二进制里是无限循环小数（类似 1/3 在十进制是 0.333...），存进有限位数时被截断产生微小误差。不能用 `==` 比较浮点数，正确做法是比较两数之差的绝对值是否足够小。

```cpp
#include <iostream>
#include <cmath>    // std::abs

int main() {
    double a = 0.1 + 0.2;
    std::cout << (a == 0.3) << "\n";                  // 0（false！精度误差）
    std::cout << (std::abs(a - 0.3) < 1e-9) << "\n";  // 1（正确比较方式）
}
```

#### 1.3.2 浮点特殊值

浮点数有两个特殊值：正无穷 `inf`（溢出产生）和 `NaN`（Not a Number，0/0 产生）。NaN 不等于任何值，包括自身，可以用 `std::isnan()` 检测。`float` 占 4 字节、约 7 位有效数字，`double` 占 8 字节、约 15 位有效数字（推荐用 double）。

```cpp
#include <iostream>
#include <cfloat>    // FLT_MAX
#include <cmath>     // std::isnan

int main() {
    float inf = FLT_MAX * 2.0F;   // 溢出 → +∞
    float nan = 0.0F / 0.0F;      // 0/0 → NaN

    std::cout << inf << "\n";              // inf
    std::cout << nan << "\n";              // nan 或 -nan
    std::cout << (nan == nan) << "\n";     // 0（NaN 不等于自身！）
    std::cout << std::isnan(nan) << "\n";  // 1（正确检测方式）
}
```

### 1.4 bool 与 char

#### 1.4.1 bool 类型

`bool` 只有 `true`（1）和 `false`（0）两个值。任何非零整数转 bool 都是 `true`，零转 bool 是 `false`。`cout` 默认把 bool 打印为 1/0，加 `std::boolalpha` 可打印 true/false 字符串。

```cpp
#include <iostream>

int main() {
    std::cout << "true=" << true << "  false=" << false << "\n";  // 1  0
    std::cout << "bool(42)="  << (bool)42 << "\n";   // 1（非零即 true）
    std::cout << "bool(0)="   << (bool)0  << "\n";   // 0
    std::cout << "bool(-1)="  << (bool)-1 << "\n";   // 1（负数也是非零）
    std::cout << std::boolalpha << true << "\n";      // true
}
```

#### 1.4.2 char 类型

`char` 存储单个字符，本质是一个小整数（通常 -128~127），对应 ASCII 码。字符之间可以做算术运算，利用 ASCII 连续编码的特性：大写 A~Z 是 65~90，小写 a~z 是 97~122，差值固定为 32。

```cpp
#include <iostream>

int main() {
    char c = 'A';
    std::cout << c << "\n";                // A
    std::cout << (int)c << "\n";           // 65（ASCII 码）
    std::cout << (char)('A' + 1) << "\n";  // B（ASCII 连续排列）
    std::cout << ('a' - 'A') << "\n";      // 32（大小写差 32）
    // 大写转小写：c + 32；小写转大写：c - 32
}
```

#### 1.4.3 转义字符（char 场景）

反斜杠 `\` 开头的序列是转义字符。常用集合（`\n` 换行、`\t` 制表、`\\` 反斜杠、`\'` 单引号、`\"` 双引号）的完整表在 00_hello_world 1.4.3 已列过，这里补 char 语境独有的两点：转义同样出现在字符字面量里，以及 `\0` 空字符是字符串的结束标志。

```cpp
char q  = '\'';   // 字符字面量里的转义：存一个单引号
char nl = '\n';   // 转义后是一个字符（换行符本身）

// "abc" 的末尾隐含一个 '\0'——C 风格字符串的结束标志
// char 只能存一个字符；存多个字符要用字符串，后面会专门讲
```

### 1.5 字面量写法

#### 1.5.1 不同进制

整数字面量支持四种进制写法。十进制直接写数字；十六进制以 `0x` 开头，常用于颜色值、内存地址；八进制以 `0` 开头（容易误写，如 `010` 是 8 不是 10）；二进制以 `0b` 开头（C++14 起）。

```cpp
int dec = 255;          // 十进制
int hex = 0xFF;         // 十六进制，= 255
int oct = 0377;         // 八进制，= 255（注意：前导 0 不是无意义的！）
int bin = 0b11111111;   // 二进制，= 255（C++14）
```

#### 1.5.2 数字分隔符（C++14）

C++14 起可以用单引号 `'` 分隔数字，增加大数的可读性。分隔符可以放在任意位置，编译器完全忽略它，不影响数值。

```cpp
int million = 1'000'000;             // 等价于 1000000
long long big = 1'234'567'890LL;
double pi = 3.141'592'653'589;
int mask = 0b1111'0000'1111'0000;    // 二进制也能分隔
```

#### 1.5.3 科学计数法

浮点字面量可以用 `e` 或 `E` 表示 10 的幂。`aEn` 表示 a * 10^n。科学计数法的字面量类型是 `double`，加 `F` 后缀变成 `float`。

```cpp
double a = 3.14e2;     // 314.0（3.14 * 10^2）
double b = 1.5e-3;     // 0.0015（1.5 * 10^-3）
double c = 6.022e23;   // 阿伏伽德罗常数
float  d = 1.0e3F;     // 1000.0（float 类型）
```

#### 1.5.4 字面量后缀

不加后缀时，整数默认为 `int`，小数默认为 `double`。后缀可以改变字面量的类型，在需要特定类型的场景（如函数重载、模板推断）很有用。

```cpp
42        // int（默认）
42L       // long
42LL      // long long
42U       // unsigned int
42ULL     // unsigned long long
3.14      // double（默认）
3.14F     // float（比 double 小，精度低但省内存）
3.14L     // long double（精度高于 double）
```

### 1.6 初始化方式

C++ 有四种初始化语法。推荐使用列表初始化 `{}`（C++11），因为它能检测窄化转换，防止意外的精度丢失。

| 语法 | 名称 | 说明 |
|------|------|------|
| `int a = 10;` | 拷贝初始化 | C 风格，最常见 |
| `int b(20);` | 直接初始化 | 构造函数风格 |
| `int c{30};` | 列表初始化 | C++11 推荐，防窄化 |
| `int d{};` | 值初始化 | 默认值（整数为 0） |

```cpp
int a = 10;    // 拷贝初始化（C 风格，最常见）
int b(20);     // 直接初始化（构造函数风格）
int c{30};     // 列表初始化（推荐，C++11，防窄化）
int d{};       // 值初始化（= 0，等价于 int d = 0）
```

#### 1.6.1 列表初始化防窄化

`{}` 初始化会在编译期检查窄化转换（大类型到小类型丢失精度），发现就报错。`=` 初始化不检查，悄悄截断，运行时才发现数值不对。

```cpp
// int bad{3.14};    // 编译错误！double→int 窄化，{} 阻止了精度丢失
int ok = 3.14;       // 编译通过，但 ok = 3（悄悄截断小数部分）
// long long big{42}; // OK：int→long long 不窄化（不丢精度）
```

#### 1.6.2 数组零初始化

用 `{}` 初始化数组时，未指定的元素自动填 0。空 `{}` 让整个数组全部为 0，比写循环赋值更简洁也更快。

```cpp
int arr[5]{};          // 全部为 0：{0, 0, 0, 0, 0}
int arr2[5]{1, 2};     // 部分指定：{1, 2, 0, 0, 0}
double d[3]{};         // 全部为 0.0
```

#### 1.6.3 结构体部分初始化

结构体用 `{}` 初始化时，按字段声明顺序赋值，未列出的字段补 0。建议显式写出所有字段的值，避免编译器 warning，也让代码意图更清晰。

```cpp
struct Point {
    int x, y, z;
};

Point p{1, 2, 0};    // 显式写 0
// Point p2{1, 2};   // z 自动为 0，但某些编译器会给 warning
// Point p3{};       // 全部为 0
```

### 1.7 const / constexpr / volatile

| 限定符 | 值确定时机 | 可否修改 | 典型用途 |
|--------|-----------|---------|---------|
| `const` | 运行时 | 否 | 不可变配置值、函数参数保护 |
| `constexpr` | 编译期 | 否 | 数组大小、模板参数、编译期计算 |
| `volatile` | — | 是（但禁止优化） | 硬件寄存器映射、信号处理 |

#### 1.7.1 const 运行时常量

`const` 声明的变量初始化后不可修改，试图修改会编译报错。值可以在运行时确定（例如从用户输入获取），但一旦确定就不能再改。

```cpp
const int MAX = 100;
// MAX = 200;         // 编译错误！const 不可修改

// 运行时确定的 const 也可以：
// const int n = get_input();  // OK，运行时赋值一次后不可变
```

#### 1.7.2 constexpr 编译期常量

`constexpr` 比 `const` 更严格，要求值在编译期就能算出来。可以用作数组大小、模板参数等需要编译期常量的场景。如果值无法在编译期确定，会编译报错。

```cpp
constexpr int SIZE = 10 * 10;  // 编译期计算，= 100
int arr[SIZE];                  // OK：constexpr 可作数组大小

// const int n = some_runtime_func();
// int arr2[n];  // 错误！n 是运行时确定的，不能作数组大小（VLA 不是标准 C++）
```

#### 1.7.3 volatile

`volatile` 告诉编译器每次都从内存读写该变量，不要缓存到寄存器或优化掉读写操作。主要用于硬件寄存器映射和信号处理。现代 C++ 多线程推荐用 `std::atomic`，不要用 `volatile`。

```cpp
volatile int reg = 0;  // 每次读写都访问内存，不被编译器优化掉
reg = 1;               // 即使下一行又赋值，这一行也不会被优化删除
reg = 2;
// 编译器不会把上面两行合并为 reg = 2
```

### 1.8 定宽整数类型

| 类型 | 位数 | 字节数 | 范围 |
|------|------|--------|------|
| `int8_t` | 8 | 1 | -128 ~ 127 |
| `int16_t` | 16 | 2 | -32,768 ~ 32,767 |
| `int32_t` | 32 | 4 | ≈ ±21.5 亿 |
| `int64_t` | 64 | 8 | ≈ ±9.2×10¹⁸ |
| `uint8_t` | 8 | 1 | 0 ~ 255 |
| `uint16_t` | 16 | 2 | 0 ~ 65,535 |
| `uint32_t` | 32 | 4 | 0 ~ ≈42.9 亿 |
| `uint64_t` | 64 | 8 | 0 ~ ≈1.8×10¹⁹ |

`<cstdint>` 提供精确位宽的整数类型，保证在所有平台上位数一致。普通的 `int`、`long` 在不同平台可能有不同大小（如 `long` 在 Windows 上 4 字节，Linux 上 8 字节），跨平台代码应该用定宽类型。

```cpp
#include <cstdint>
#include <iostream>

int main() {
    int8_t   i8  = 127;                      // 精确 8 位有符号，-128 ~ 127
    int16_t  i16 = 32767;                    // 精确 16 位，-32768 ~ 32767
    int32_t  i32 = 2147483647;              // 精确 32 位
    int64_t  i64 = 9223372036854775807LL;   // 精确 64 位
    uint8_t  u8  = 255;                      // 精确 8 位无符号，0 ~ 255
    uint32_t u32 = 4294967295U;             // 精确 32 位无符号

    // 用于：网络协议解析、文件格式读写、跨平台序列化
    // 注意：uint8_t 用 cout 打印时显示字符而非数字，需要 (unsigned) 转换
    std::cout << (unsigned)u8 << "\n";  // 255（不转换会打印字符）
}
```

### 1.9 类型别名

#### 1.9.1 using 与 typedef

`using` 和 `typedef` 都能给类型起别名，让代码更易读。`using` 是 C++11 新增的写法，语法更直观（左边是别名，右边是原类型），推荐使用。

```cpp
using Score = int;          // using（C++11，推荐）：Score 就是 int
typedef double Price;       // typedef（传统写法）：Price 就是 double

Score s = 95;     // 等价于 int s = 95;
Price p = 9.99;   // 等价于 double p = 9.99;

// using 处理复杂类型时更清晰：
using IntPtr = int*;           // using：一目了然
// typedef int* IntPtr;        // typedef：等价但不如 using 直观
```

#### 1.9.2 size_t

`size_t` 是标准库定义的无符号整数类型别名，专门表示大小和索引。`sizeof` 运算符的返回类型就是 `size_t`。在 64 位系统上通常是 `unsigned long long`（8 字节）。注意它是无符号的，和负数比较或做减法时可能产生意外结果。

```cpp
#include <cstddef>   // size_t 定义在此（<iostream> 等头文件也间接包含）

size_t len = sizeof(int);  // 4（int 占 4 字节）
size_t idx = 0;             // 用于数组索引

// 注意：size_t 是无符号的，和负数混用很危险
// size_t n = 5;
// if (n - 10 > 0) ...   // 永远为 true！5-10 回绕为巨大正数
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/types.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-ninja\types.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/types.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-make\types.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/types.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-mc\Release\types.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\types.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-msvc-ninja\types.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\types.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-msvc-vs\Release\types.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/types
```

## 3. 英文及缩写说明

### 3.1 C++ 关键字

| 词汇          | 全称 / 读法                         | 说明                                              |
|-------------|---------------------------------|-------------------------------------------------|
| `bool`      | Boolean                         | 布尔类型，只有 `true`（真）和 `false`（假）两个值                |
| `char`      | Character                       | 字符类型，存单个字符，本质是 -128~127 的小整数（对应 ASCII 码）        |
| `short`     | Short integer                   | 短整数，2 字节                                        |
| `int`       | Integer                         | 整数，最常用的整数类型，4 字节                                |
| `long`      | Long integer                    | 长整数，大小平台相关（Windows 上 4 字节，Linux 上 8 字节）         |
| `long long` | Long long integer               | 更长的整数，8 字节，用于存非常大的整数                            |
| `float`     | Floating point                  | 单精度浮点数，4 字节，精度约 7 位有效数字                         |
| `double`    | Double precision floating point | 双精度浮点数，8 字节，精度约 15 位有效数字，推荐用                    |
| `unsigned`  | Unsigned integer                | 无符号整数，不能存负数，但正数范围翻倍（如 `unsigned int`：0 ~ 2³²-1） |
| `const`     | Constant                        | 常量，声明后不可修改；运行时确定值                               |
| `constexpr` | Constant Expression             | 常量表达式，编译期就计算好，可用作数组大小、模板参数                      |
| `volatile`  | （拉丁语：易变的）                       | 告知编译器不要优化此变量，每次都从内存读写；常用于硬件寄存器、多线程              |
| `auto`      | Automatic（自动）                   | 让编译器根据右边的值自动推断变量类型（C++11）                       |
| `decltype`  | Declared type（声明类型）             | 推断表达式的类型但不执行它（C++11）                            |

### 3.2 字面量后缀

| 后缀        | 说明                | 示例            |
|-----------|-------------------|---------------|
| `L`       | long              | `42L`         |
| `LL`      | long long         | `42LL`        |
| `U` / `u` | unsigned          | `42U`         |
| `F` / `f` | float             | `3.14F`       |
| `0x` 前缀   | 十六进制（Hexadecimal） | `0xFF` = 255  |
| `0b` 前缀   | 二进制（Binary，C++14） | `0b1010` = 10 |

### 3.3 运算符与头文件

| 词汇            | 说明                                                       |
|---------------|----------------------------------------------------------|
| `sizeof`      | size of：运算符，返回类型或变量占用的字节数（1 字节 = 8 位）                    |
| `alignof`     | alignment of：运算符，返回类型的对齐要求（字节数）                          |
| `static_cast` | static cast：编译期类型转换，最安全最常用的 C++ 风格转换                     |
| `<climits>`   | C limits：包含整数类型的范围常量，如 `INT_MAX`（int 最大值）、`INT_MIN`（最小值） |
