# 03_operators — 运算符

演示算术、比较、逻辑、位运算、赋值、自增自减、三目运算符及 `sizeof`。

## 1. 知识点

### 1.1 算术运算符

C++ 的算术运算符包括 `+`（加）、`-`（减）、`*`（乘）、`/`（除）、`%`（取余）。两个整数相除结果仍是整数，小数部分直接截断。

```cpp
int a = 10, b = 3;
std::cout << a + b << "\n";   // 13
std::cout << a - b << "\n";   // 7
std::cout << a * b << "\n";   // 30
std::cout << a / b << "\n";   // 3（整数除法，截断小数）
std::cout << a % b << "\n";   // 1（取余：10 = 3*3 + 1）
```

#### 1.1.1 负数取余

C++ 中 `%` 运算的结果符号与被除数（左操作数）一致。这是 C++11 起的标准规定，不同语言的规则不同（如 Python 取余结果与除数同号）。

```cpp
std::cout << (-7 % 3)  << "\n";   // -1（符号同被除数 -7）
std::cout << (7 % -3)  << "\n";   //  1（符号同被除数 7）
std::cout << (-7 % -3) << "\n";   // -1（符号同被除数 -7）
```

#### 1.1.2 浮点除法

两个整数相除会截断小数。至少有一个操作数是浮点数时，才会做浮点除法得到小数结果。可以用强制类型转换让整数变量做浮点除法。

```cpp
std::cout << 10 / 3 << "\n";           // 3（整数除法，截断）
std::cout << 10.0 / 3 << "\n";         // 3.33333（浮点除法）
std::cout << (double)10 / 3 << "\n";   // 3.33333（强转为浮点）
```

#### 1.1.3 整数提升

`char` 和 `short` 参与算术运算时，会先自动提升为 `int`，避免小类型溢出。提升是隐式的，结果类型也是 `int`。

```cpp
char c1 = 100, c2 = 100;
// char 范围 -128~127，100+100=200 会溢出 char
// 但运算时先提升为 int，结果是 int 类型的 200，不会溢出
std::cout << (c1 + c2) << "\n";   // 200（结果是 int）
```

### 1.2 比较运算符

比较运算符 `==` `!=` `<` `>` `<=` `>=` 返回 `bool` 类型（`true` 或 `false`）。

```cpp
int a = 5, b = 10;
std::cout << (a == b) << "\n";    // 0（false）
std::cout << (a != b) << "\n";    // 1（true）
std::cout << (a < b)  << "\n";    // 1（true）
```

#### 1.2.1 浮点比较陷阱

浮点数**不能**用 `==` 直接比较：二进制精度误差会让 `0.1 + 0.2 == 0.3` 得到 `false`。原理和正确写法（比较两数之差是否小于阈值）已在 01_types 1.3.1 详讲，这里只记住结论——浮点相等判断必须走误差比较。

```cpp
double x = 0.1 + 0.2;
// 不要：x == 0.3              // 精度误差，可能 false
// 要：std::abs(x - 0.3) < 1e-9  // 误差比较，写法见 01_types 1.3.1
```

#### 1.2.2 链式比较陷阱

C++ 不支持数学写法的链式比较 `1 < n < 10`。这个表达式会被解析为 `(1 < n) < 10`，先算 `1 < n` 得到 `bool`（0 或 1），再和 10 比较，几乎永远为 `true`。必须用 `&&` 拆成两个独立比较。

```cpp
int n = 5;
bool trap    = (1 < n < 10);          // 先 1<5=true(1)，再 1<10=true → 永远 true！
bool correct = (1 < n && n < 10);     // 正确：两个比较用 && 连接
```

### 1.3 逻辑运算符与短路求值

逻辑运算符 `&&`（与）、`||`（或）、`!`（非）用于组合多个条件，结果为 `bool`。

```cpp
std::cout << (true && true)  << "\n";   // 1
std::cout << (true && false) << "\n";   // 0
std::cout << (true || false) << "\n";   // 1
std::cout << (!true)         << "\n";   // 0
```

#### 1.3.1 短路求值

`&&` 左边为 `false` 时，右边不执行（整体已确定为 false）；`||` 左边为 `true` 时，右边不执行（整体已确定为 true）。这不是优化，而是语言标准保证的行为。

```cpp
int counter = 0;
auto inc = [&]() -> bool { ++counter; return true; };

counter = 0;
false && inc();       // inc 未执行，counter 仍为 0
// 因为 && 左边已经是 false，右边不需要求值

counter = 0;
true || inc();        // inc 未执行，counter 仍为 0
// 因为 || 左边已经是 true，右边不需要求值
```

#### 1.3.2 短路求值防崩溃

利用短路求值特性，可以在解引用空指针或除以零之前先检查条件，避免崩溃。把检查条件写在 `&&` 左边，只有检查通过才执行右边的危险操作。

```cpp
int* p = nullptr;
bool r1 = (p != nullptr && *p > 0);   // p 空时不解引用，不崩溃

int n = 0;
bool r2 = (n != 0 && 10 / n > 1);     // n=0 时不除，不崩溃

// 反面：如果写成 (*p > 0 && p != nullptr)，先解引用空指针就崩溃了
```

### 1.4 位运算符

位运算符直接操作二进制位：`&`（与）、`|`（或）、`^`（异或）、`~`（取反）、`<<`（左移）、`>>`（右移）。只用于无符号整数，对有符号整数右移的行为依赖实现。

```cpp
unsigned a = 0b1100, b = 0b1010;   // a=12, b=10
std::cout << (a & b)  << "\n";     // 8  (0b1000，两位都为 1 才为 1)
std::cout << (a | b)  << "\n";     // 14 (0b1110，有一个 1 就为 1)
std::cout << (a ^ b)  << "\n";     // 6  (0b0110，两位不同才为 1)
std::cout << (a << 1) << "\n";     // 24（左移 1 位 = 乘 2）
std::cout << (a >> 1) << "\n";     // 6 （右移 1 位 = 除 2）
```

#### 1.4.1 标志位操作

位运算最常见的应用是标志位（flags）：每个 bit 代表一个独立的开关。用 `|` 设置标志、`&` 检查标志、`& ~` 清除标志。

```cpp
enum Flags : unsigned {
    READ  = 1 << 0,   // 0b001 = 1
    WRITE = 1 << 1,   // 0b010 = 2
    EXEC  = 1 << 2    // 0b100 = 4
};

unsigned perm = READ | WRITE;        // 设置：0b011 = 3
bool can_write = perm & WRITE;       // 检查：0b010 != 0 → true
perm &= ~WRITE;                      // 清除：0b001 = 1（只剩 READ）
perm ^= EXEC;                        // 翻转：0b101 = 5（加了 EXEC）
```

### 1.5 自增自减

`++` 和 `--` 有前缀和后缀两种形式。前缀 `++i` 先加再用（返回加后的值），后缀 `i++` 先用再加（返回加前的值）。对迭代器等非基本类型，前缀版性能更好（后缀版需要创建临时拷贝）。

```cpp
int a = 5;

int b = ++a;    // 前缀：先加 1（a=6），再赋给 b（b=6）
int c = a++;    // 后缀：先赋给 c（c=6），再加 1（a=7）

int d = --a;    // 前缀减：先减 1（a=6），再赋给 d（d=6）
int e = a--;    // 后缀减：先赋给 e（e=6），再减 1（a=5）

// for 循环中推荐前缀 ++i（对迭代器等非基本类型更快）
for (int i = 0; i < 10; ++i) {
    // ...
}
```

### 1.6 三目运算符

`条件 ? 真值 : 假值` 是 if-else 的紧凑写法，整个表达式有返回值。

```cpp
int a = 8, b = 3;
int max_val = (a > b) ? a : b;    // 8
```

#### 1.6.1 用三目初始化 const

三目运算符可以用在初始化表达式中，这是 if-else 做不到的。用于给 `const` 变量根据条件赋不同的初始值。

```cpp
const int score = 85;
const std::string grade = (score >= 90) ? "优"
                        : (score >= 75) ? "良"
                        : (score >= 60) ? "及格"
                        : "不及格";
// 用 if-else 就无法声明为 const（需要先声明再赋值）
```

#### 1.6.2 两分支类型不同时会提升

三目运算符的两个分支类型不同时，编译器会自动提升到更大的类型（和算术运算的隐式转换规则一致）。

```cpp
auto x = true ? 1 : 3.14;      // int 提升为 double → x 是 double
std::cout << sizeof(x) << "\n"; // 8（double）

// 注意：两个分支必须能转换为同一类型，否则编译错误
// auto y = true ? 1 : "hello";  // 错误！int 和 const char* 不兼容
```

### 1.7 运算符优先级

运算符优先级从高到低排列。不确定优先级时加括号，让代码意图清晰。

```
1.  () [] -> .           最高：括号、下标、成员访问
2.  一元前缀: ++ -- ~ ! + -
3.  乘除取余: * / %
4.  加减: + -
5.  位移: << >>
6.  比较: < <= > >=
7.  相等: == !=
8.  位与: &
9.  位异或: ^
10. 位或: |
11. 逻辑与: &&
12. 逻辑或: ||
13. 三目: ?:
14. 赋值: = += -= ...     最低
```

```cpp
std::cout << (2 + 3 * 4)   << "\n";   // 14（* 先于 +）
std::cout << ((2 + 3) * 4) << "\n";   // 20（括号改变优先级）
```

#### 1.7.1 位运算优先级陷阱

位运算符 `&` `|` `^` 的优先级低于比较运算符 `==` `!=`，这是 C++ 最常见的优先级陷阱。写位运算表达式时务必加括号。

```cpp
// 陷阱：1 | 2 == 3 被解析为 1 | (2 == 3) = 1 | 0 = 1
std::cout << (1 | 2 == 3)   << "\n";   // 1（不是预期的结果！）
std::cout << ((1 | 2) == 3) << "\n";   // 1（加括号后正确）

// 陷阱：x & mask != 0 被解析为 x & (mask != 0)
// 正确写法：(x & mask) != 0
```

### 1.8 逗号运算符

逗号运算符从左到右依次求值每个表达式，整体的值是最后一个表达式的值。最常见的用法是 `for` 循环中同时操作多个变量。

```cpp
int a = (1, 2, 3);       // 从左到右求值，a = 3（最后一个）
std::cout << a << "\n";  // 3

// 最常见用途：for 循环中同时初始化和步进多个变量
for (int i = 0, j = 10; i < j; ++i, --j) {
    std::cout << "(" << i << "," << j << ") ";
}
// 输出：(0,10) (1,9) (2,8) (3,7) (4,6)
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/operators.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-ninja\operators.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/operators.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-make\operators.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/operators.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-mc\Release\operators.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\operators.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-msvc-ninja\operators.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\operators.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-msvc-vs\Release\operators.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/operators
```

## 3. 英文及缩写说明

### 3.1 运算符术语

| 词汇                  | 说明                                           |
|---------------------|----------------------------------------------|
| arithmetic operator | 算术运算符：`+` `-` `*` `/` `%`，执行数学计算             |
| comparison operator | 比较运算符：`==` `!=` `<` `>` `<=` `>=`，结果为 `bool` |
| logical operator    | 逻辑运算符：`&&`（与）`\|\|`（或）`!`（非），组合多个条件          |
| bitwise operator    | 位运算符：直接操作二进制位                                |
| assignment operator | 赋值运算符：`=`；复合赋值 `+=` `-=` `*=` `/=` `%=`      |
| ternary operator    | 三目运算符：`条件 ? 真值 : 假值`，是 if-else 的紧凑写法         |
| `sizeof`            | size of：运算符，返回类型或变量占用的字节数，编译期确定              |
| `alignof`           | alignment of：运算符，返回对齐字节数                     |

### 3.2 重要概念

| 概念                       | 说明                                                    |
|--------------------------|-------------------------------------------------------|
| short-circuit evaluation | 短路求值：`&&` 左边为 `false` 时右边不执行；`\|\|` 左边为 `true` 时右边不执行 |
| integer division         | 整数除法：两个整数相除结果仍是整数，小数部分直接截断。`10/3 = 3`                 |
| modulo                   | 取余/取模：`%` 运算符，返回除法的余数。`10 % 3 = 1`                    |
| prefix vs postfix        | 前缀 `++i`：先加再用；后缀 `i++`：先用再加                           |
| bitwise AND              | 按位与 `&`：两位都为 1 才为 1，常用于检查/清除标志位                       |
| bitwise OR               | 按位或 `\|`：有一个 1 就为 1，常用于设置标志位                          |
| bitwise XOR              | 按位异或 `^`：两位不同才为 1，常用于翻转标志位                            |
| left shift               | 左移 `<<`：相当于乘以 2ⁿ。`1 << 3 = 8`                         |
| right shift              | 右移 `>>`：相当于除以 2ⁿ。`8 >> 1 = 4`                         |
