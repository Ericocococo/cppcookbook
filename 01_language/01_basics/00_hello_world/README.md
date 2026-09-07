# 00_hello_world — 程序结构

每个 C++ 程序都有的"必须件"：`#include`、`main()`、变量声明、`cout` 打印、注释、作用域。

## 1. 知识点

### 1.1 程序结构

#### 1.1.1 `#include` 引入工具箱

把头文件内容复制进来，不写就报"未定义"错误：

```cpp
#include <iostream>   // 之后才能用 std::cout
#include <string>     // 之后才能用 std::string
```

#### 1.1.2 `std` 命名空间

标准库的东西都装在 `std` 命名空间里，用 `std::` 前缀访问：

```cpp
std::cout << "hello";    // std:: 表示"去标准库里找"
std::string name = "hi"; // std::string 是标准库的字符串类型
```

#### 1.1.3 标准库常用头文件

| 头文件 | 提供什么 | 用途 |
|------|------|------|
| `<iostream>` | cout、cin、cerr | 打印和读输入 |
| `<string>` | std::string | 字符串 |
| `<vector>` | std::vector | 动态数组（用得最多） |
| `<map>` | std::map | 键值对（类似 Python dict） |
| `<algorithm>` | sort、find、count_if | 算法 |
| `<cmath>` | sqrt、sin、pow | 数学函数 |
| `<memory>` | unique_ptr、shared_ptr | 智能指针 |
| `<functional>` | std::function | 存函数/lambda |
| `<fstream>` | ifstream、ofstream | 读写文件 |
| `<chrono>` | 时间点、时长 | 计时 |

> 这只是最常用的 10 个；完整分类速查（含 IO/容器/算法/工具/并发等 40+ 头文件与详解位置）见 [03_stl/headers.md](../../../03_stl/headers.md)（后续章节学到哪可以回查）。

#### 1.1.4 最小可运行程序（main 函数）

```cpp
#include <iostream>         // 引入工具箱

int main() {                // 程序入口，有且只有一个
    std::cout << "hello\n"; // 打印
    return 0;               // 返回 0 = 正常结束，非零 = 出错
}                           // 每条语句以 ; 结尾，{} 划定作用域
```

### 1.2 注释

#### 1.2.1 单行注释 `//`

```cpp
// 单行注释：从 // 到行尾都被编译器忽略

/* 多行注释：
   从 /* 到 */ 之间全被忽略，
   可以跨多行 */

// 2.2 多行注释：/* 到 */ 之间全被忽略，可以跨多行

// 2.3 注释写什么：
int speed = 60;    // 不好：设置速度为60（代码已经说了）
int speed = 60;    // 好：限速 60 km/h，超过触发警报（解释了原因）
// 好注释写"为什么"，不重复代码已经表达的"做了什么"
```

### 1.3 变量

#### 1.3.1 声明格式

```cpp
// 类型 变量名 = 初始值;
int    age    = 18;
double height = 1.75;
bool   passed = true;      // 只有 true(1) 和 false(0)
char   grade  = 'A';       // 单个字符用单引号
std::string name = "小白"; // 字符串用双引号
```

#### 1.3.2 未初始化变量

```cpp
int bad;
std::cout << bad;  // 危险！值是随机垃圾值，读取是未定义行为（UB）
```

#### 1.3.3 命名规则

```cpp
int count1 = 1;    // 合法：字母/数字/下划线
int Count1 = 2;    // 合法：和 count1 是不同变量（区分大小写）
// int 1count = 3; // 非法：不能以数字开头
```

### 1.4 cout 打印

> `cout` = character output（字符输出），`cin` = character input（字符输入），`cerr` = character error（字符错误输出）。`c` 是 character（字符），不是 console。

#### 1.4.1 基本用法

```cpp
std::cout << "文字\n";                        // 打印文字
std::cout << 42 << "\n";                      // 打印数字
std::cout << "age=" << 18 << "\n";            // 链式：可以连续写任意多个 <<
// 每个 << 返回 std::cout 本身，所以能继续接下一个 <<
```

#### 1.4.2 链式调用 + `"\n"` vs `std::endl`

```cpp
std::cout << "行1\n";            // \n：只换行，内容攒在缓冲区，攒够再写屏幕（快）
std::cout << "行2" << std::endl; // endl：换行 + 强制刷新缓冲区，立即写屏幕（慢）
// 日常用 "\n" 就够，debug 怕崩溃丢输出时才用 std::endl
```

#### 1.4.3 转义字符

| 转义 | 含义 |
|------|------|
| `\n` | 换行 |
| `\t` | 制表符 |
| `\\` | 反斜杠 |
| `\'` | 单引号 |
| `\"` | 双引号 |
| `\0` | 空字符（字符串结束标志） |

#### 1.4.4 `std::cerr` 错误输出

`cerr` = character error，和 `cout` 一样用 `<<`，但不走缓冲区，立即输出。用于确保崩溃前错误信息不丢。

```cpp
std::cerr << "出错了！\n";  // 不走缓冲区，立即输出
```

### 1.5 作用域

#### 1.5.1 作用域 `{}`

```cpp
int outer = 10;
{
    int inner = 20;           // inner 只在这个 {} 内有效
    std::cout << outer;       // 内层可以访问外层变量
}
// 这里 inner 不存在，访问会编译报错
```

- 变量出了 `{}` 就消失（内存被回收）
- 内层可以访问外层，外层不能访问内层

#### 1.5.2 变量屏蔽（shadowing）

```cpp
int outer = 10;
{
    int outer = 999;          // 屏蔽外层的 outer
    std::cout << outer;       // 输出 999
}
std::cout << outer;           // 输出 10，外层变量恢复
```

### 1.6 cin 输入

> `cin` = character input（字符输入），从键盘读数据。

#### 1.6.1 `cin >>` 读单个值

```cpp
int age;
std::cin >> age;          // 从键盘读一个整数，遇空白（空格/换行/Tab）停止

std::string name;
std::cin >> name;         // 读一个单词，遇空格就停
```

#### 1.6.2 `cin.ignore()` 清缓冲区

```cpp
std::cin >> age;          // 读完后缓冲区留有换行符
std::cin.ignore();        // 丢弃那个残留的换行符
// 不清掉会导致后面的 getline 直接读到空行
```

#### 1.6.3 `std::getline` 读整行

```cpp
std::string line;
std::getline(std::cin, line);  // 读一整行（含空格），遇回车结束
// 和 cin >> 的区别：cin >> 遇空格就停，getline 遇回车才停
```

## 2. 构建

### CMakeLists.txt 第一次详解（CMake 是什么）

每个示例目录是一个**独立 CMake 工程**：目录里的 `CMakeLists.txt` 告诉 CMake 怎么编译你的代码，CMake 再调用真正的编译器（g++ / cl.exe）干活。本目录的 `CMakeLists.txt` 逐行解释：

```cmake
cmake_minimum_required(VERSION 3.28)   # ① 声明最低 CMake 版本：低于 3.28 直接报错
project(hello_world LANGUAGES CXX)     # ② 工程名 hello_world，只启用 C++ 语言

set(CMAKE_CXX_STANDARD 20)             # ③ 用 C++20 标准编译
set(CMAKE_CXX_STANDARD_REQUIRED ON)    #    编译器不支持 C++20 就报错（不悄悄降级）
set(CMAKE_CXX_EXTENSIONS OFF)          #    不用编译器私有扩展，严格按标准

if (MSVC)                              # ④ 编译警告开关：不同编译器语法不同
    add_compile_options(/W4 /utf-8)    #    MSVC（cl.exe）用 /W4 开警告
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)  # GCC/Clang 用 -Wall 系列
endif ()

add_executable(hello_world main.cpp)   # ⑤ 把 main.cpp 编译链接成可执行文件 hello_world
```

构建流程三步：**配置** `cmake -B <目录>`（读 CMakeLists.txt，生成构建文件）→ **构建** `cmake --build <目录>`（真正编译链接）→ **运行** 生成的 exe。每个示例目录都用这套流程，命令里的目录名换成对应目录即可。

> CMake 的**专门系统教学**在仓库末尾的 [10_ops/01_build_systems](../../../10_ops/01_build_systems/README.md) 章（从手动编译一步步演进到 CMake、包管理、企业级），学完本仓库 01_language 的 C++ 语法后再去；那里也是 CMakeLists.txt 各命令的完整参考。现在只需要会跑这三步、能看懂本目录 14 行即可。

### 2.1 命令行 · MinGW（Git Bash）

**路径常量（绝对路径版使用）：**

| 工具 | 完整路径 |
|------|------|
| cmake | `D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe` |
| g++ | `D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe` |
| ninja | `D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe` |
| mingw32-make | `D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe` |

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/hello_world.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-ninja\hello_world.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/hello_world.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-make\hello_world.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/hello_world.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-mc\Release\hello_world.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\hello_world.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-msvc-ninja\hello_world.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\hello_world.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-msvc-vs\Release\hello_world.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/hello_world
```

## 3. 英文及缩写说明

### 3.1 程序结构关键字

| 词汇 | 全称 / 读法 | 说明 |
|------|------|------|
| `#include` | include（包含） | 预处理指令：把指定头文件的内容复制进来，相当于"引入工具箱" |
| `#`（井号） | preprocessor directive（预处理指令） | 以 `#` 开头的行由预处理器在编译前处理，不是普通 C++ 语句，结尾不加 `;` |
| `<iostream>` | input/output stream（输入输出流） | 提供 `std::cout`（打印到终端）和 `std::cin`（从终端读取） |
| `<string>` | — | 提供 `std::string` 字符串类型 |
| `main` | — | 程序入口函数，固定名字，操作系统从这里开始执行；每个程序有且只有一个 |
| `return 0` | — | 退出 `main` 并返回 0；0 表示"程序正常结束"，非零表示出错 |
| `{` `}` | braces / curly braces（花括号） | 划定"作用域"，里面声明的变量只在 `{}` 内有效 |
| `;` | semicolon（分号） | 每条语句的结束符，必须有，缺少会编译报错 |

### 3.2 变量与类型

| 词汇 | 说明 |
|------|------|
| `int` | Integer：整数，如 `18`、`-5`、`0` |
| `double` | Double precision float：双精度小数，如 `1.75`、`3.14` |
| `bool` | Boolean：只有 `true`（真/1）和 `false`（假/0） |
| `char` | Character：单个字符，用**单引号**，如 `'A'`、`'1'` |
| `std::string` | 一段文字（字符串），用**双引号**，如 `"Hello"` |

### 3.3 输出相关

| 词汇 | 说明 |
|------|------|
| `std::` | Standard（标准）的命名空间前缀；表示"去 C++ 标准库里找" |
| `cout` | Console output：控制台输出，向终端打印内容 |
| `<<` | 流插入运算符（stream insertion）：把右边的内容"送进"输出流 |
| `"\n"` | 换行符（newline），推荐用这个 |
| `std::endl` | end line：换行并强制刷新输出缓冲区；比 `"\n"` 慢 |
