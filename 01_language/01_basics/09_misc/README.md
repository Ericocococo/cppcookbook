# 09_misc — 枚举、命名空间、类型转换

演示 `enum`/`enum class`、`namespace`、四种 C++ 类型转换运算符。

## 1. 知识点

| 文件 | 说明 |
|---|---|
| `main.cpp` | 3 个 demo 函数：枚举 → 命名空间 → 类型转换 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `misc` |

### 1.1 枚举：enum（传统）vs enum class（推荐）

#### 1.1.1 传统 enum

传统 `enum` 的枚举名直接暴露到外部作用域，不同枚举之间可能冲突。而且枚举值可以隐式转为 `int`，容易造成意外的类型错误。

```cpp
#include <iostream>

enum Direction { UP, DOWN, LEFT, RIGHT };
Direction d = UP;       // 直接使用枚举名，不需要前缀
int n = UP;             // 可以隐式转为 int（可能意外）
std::cout << n;         // 输出 0
```

#### 1.1.2 enum class（C++11，推荐）

`enum class` 把枚举名限定在枚举类内部，必须用 `Color::RED` 访问，不能隐式转 `int`，类型更安全。还可以指定底层存储类型（如 `uint8_t`），控制内存占用。

```cpp
#include <cstdint>   // uint8_t

enum class Color { RED, GREEN, BLUE };
enum class Status : uint8_t { OK = 0, ERR = 1, TIMEOUT = 2 };

Color c = Color::RED;              // 必须写全名
// int bad = c;                    // 编译错误：不能隐式转 int
int ci = static_cast<int>(c);      // 需要显式转换，ci = 0

Status s = Status::OK;
std::cout << sizeof(s);            // 输出 1（uint8_t 只占 1 字节）
```

#### 1.1.3 switch 配合 enum class

`switch` 搭配 `enum class` 时，每个 `case` 都要写全名。如果漏写某个枚举值，开启 `-Wswitch` 编译器会给出警告，帮助发现遗漏。

```cpp
enum class Color { RED, GREEN, BLUE };
Color c = Color::GREEN;

switch (c) {
case Color::RED:   std::cout << "红\n"; break;
case Color::GREEN: std::cout << "绿\n"; break;
case Color::BLUE:  std::cout << "蓝\n"; break;
}
```

### 1.2 命名空间：避免名字冲突

#### 1.2.1 嵌套命名空间

命名空间可以多层嵌套，用 `::` 逐层访问。不同命名空间里的同名函数互不冲突。

```cpp
namespace math {
    int add(int a, int b) { return a + b; }
    namespace trig {
        double sinApprox(double x) { return x - x*x*x/6.0; }
    }
}
namespace io {
    int add(int a, int b) { return a + b + 100; }  // 同名但不冲突
}

std::cout << math::add(3, 4);             // 7
std::cout << io::add(3, 4);               // 107
std::cout << math::trig::sinApprox(0.1);  // 嵌套访问
```

#### 1.2.2 C++17 嵌套命名空间简写

C++17 支持 `namespace A::B::C {}` 简写，不需要一层层花括号，代码更简洁。

```cpp
// C++17 简写（推荐）
namespace project::utils {
    void hello() { std::cout << "project::utils::hello()\n"; }
}

// 等价的传统写法
namespace project {
    namespace utils {
        void hello() { /* ... */ }
    }
}
```

#### 1.2.3 using 声明

`using` 声明引入单个名字到当前作用域，不需要每次都写完整前缀，同时不会引入不需要的名字。

```cpp
namespace math {
    const double PI = 3.14159265358979;
    int add(int a, int b) { return a + b; }
}

using math::PI;               // 只引入 PI
std::cout << PI << "\n";      // 直接使用，不需要 math:: 前缀
// add(1, 2);                 // 错误：add 没有被 using 引入
```

#### 1.2.4 using namespace

`using namespace` 引入整个命名空间的所有名字。在头文件中使用会污染包含它的所有文件，因此只建议在 `.cpp` 文件的局部作用域中使用。

```cpp
namespace math {
    double circleArea(double r) { return 3.14159 * r * r; }
}

{
    using namespace math;          // 花括号限制作用域
    std::cout << circleArea(2);    // 不需要 math:: 前缀
}  // 出块后 using 失效，不会污染外部
```

#### 1.2.5 访问 C++17 嵌套命名空间

调用 C++17 简写定义的嵌套命名空间，和普通命名空间没有区别，用 `::` 逐层访问。

```cpp
namespace project::utils {
    void hello() { std::cout << "hello\n"; }
}

project::utils::hello();  // 调用嵌套命名空间中的函数
```

### 1.3 四种 C++ 类型转换

#### 1.3.1 static_cast

最常用的 C++ 类型转换，在编译期完成。用于数值类型之间的转换（`int` <-> `double`）。`double` 转 `int` 截断小数部分，不是四舍五入。

```cpp
int i = 42;
double d = static_cast<double>(i);   // int → double，d = 42.0
int j = static_cast<int>(3.99);      // double → int，j = 3（截断）
```

#### 1.3.2 enum class 转 int

`enum class` 不能隐式转 `int`，必须用 `static_cast` 显式转换。

```cpp
enum class Dir { UP = 0, DOWN = 1 };
int di = static_cast<int>(Dir::DOWN);  // di = 1
```

#### 1.3.3 dynamic_cast

运行时类型转换，用于多态类型的向下转型（基类指针转派生类指针）。失败时返回 `nullptr`（指针版本）或抛出 `std::bad_cast`（引用版本）。要求基类有虚函数（`virtual`）。

```cpp
class Base {
public:
    virtual ~Base() {}
};
class Derived : public Base {};

Base* bp = new Derived();
Derived* dp = dynamic_cast<Derived*>(bp);   // 成功：bp 实际指向 Derived
if (dp) {
    std::cout << "转型成功\n";
}

Base* bp2 = new Base();
Derived* dp2 = dynamic_cast<Derived*>(bp2); // 失败：bp2 是真正的 Base
// dp2 == nullptr

delete bp;
delete bp2;
```

#### 1.3.4 const_cast

去除或添加 `const` 限定符。主要用于和不接受 `const` 的旧 API 交互。修改真正定义为 `const` 的变量是未定义行为（UB），后果不可预测。

```cpp
const int ci = 100;
const int* cp = &ci;
int* p = const_cast<int*>(cp);  // 去掉 const
std::cout << *p;                // 100
// *p = 200;  // 危险！ci 本身是 const，修改是未定义行为
```

#### 1.3.5 reinterpret_cast

把内存的二进制内容重新解释为另一种类型，不做任何数值转换。非常危险，几乎只在底层系统编程中使用。

```cpp
int n = 65;
char* ch = reinterpret_cast<char*>(&n);
std::cout << *ch;  // 'A'（ASCII 65），把 int 内存解释为 char
```

#### 1.3.6 C 风格转换（不推荐）

`(double)i` 这种写法看不出意图，编译器会按 `static_cast` -> `const_cast` -> `reinterpret_cast` 顺序尝试，可能做出意料之外的转换。应改用对应的 C++ 转换运算符。

```cpp
double d1 = (double)42;                  // C 风格（不推荐）
double d2 = static_cast<double>(42);     // C++ 风格（推荐，意图清晰）
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/misc.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-mingw-ninja\misc.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/misc.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-mingw-make\misc.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/misc.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-mingw-mc\Release\misc.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\misc.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-msvc-ninja\misc.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\misc.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\09_misc\build-msvc-vs\Release\misc.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/misc
```

## 3. 英文及缩写说明

### 3.1 枚举

| 词汇              | 说明                                                           |
|-----------------|--------------------------------------------------------------|
| `enum`          | Enumeration：枚举，定义一组具名整数常量；名字暴露在外部命名空间，可能冲突                   |
| `enum class`    | 枚举类（C++11，推荐）：名字限定在枚举类内（需写 `Color::RED`），类型安全                |
| enumerator      | 枚举值：枚举中的每个具名常量，如 `RED`、`GREEN`                               |
| underlying type | 底层类型：枚举实际存储的整数类型，默认 `int`，可指定 `enum class E : uint8_t {}`    |
| `uint8_t`       | unsigned integer 8-bit type：无符号 8 位整数（0~255），定义在 `<cstdint>` |

### 3.2 命名空间

| 词汇               | 说明                                                           |
|------------------|--------------------------------------------------------------|
| `namespace`      | 命名空间：将名字划分到独立作用域，避免不同库之间的名字冲突                                |
| `using`          | 引入声明：`using math::add` 引入单个名字；`using namespace std` 引入整个命名空间 |
| `std`            | Standard：C++ 标准库的命名空间                                        |
| `::`             | scope resolution operator：作用域解析运算符，访问命名空间或类的成员               |
| nested namespace | 嵌套命名空间：C++17 可写 `namespace A::B::C {}`                       |

### 3.3 类型转换

| 词汇                   | 说明                                                         |
|----------------------|------------------------------------------------------------|
| `static_cast`        | static cast：编译期类型转换，最安全最常用；整数/浮点/枚举互转                      |
| `dynamic_cast`       | dynamic cast：运行时类型检查转换；多态向下转型，失败返回 `nullptr`               |
| `const_cast`         | const cast：去除或添加 `const`/`volatile` 限定；修改真正 const 变量是未定义行为 |
| `reinterpret_cast`   | reinterpret cast：将内存二进制直接重新解释为另一种类型；非常危险，几乎不用              |
| implicit conversion  | 隐式转换：编译器自动进行的类型转换，可能导致精度丢失                                 |
| narrowing conversion | 窄化转换：从大范围转到小范围（`double` → `int`），列表初始化 `{}` 会拒绝            |
