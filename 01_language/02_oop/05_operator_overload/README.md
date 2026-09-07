# 05_operator_overload — 运算符重载全形式

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 演示代码 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `operator_overload` |

---

## 知识点详解

### 1.1 算术运算符（+, -, *, /）

#### 1.1.1 operator+= 返回引用

复合赋值运算符（`+=`、`-=`、`*=`）修改自身并返回 `*this` 的引用，允许链式调用 `a += b += c`，语义和内置 `+=` 一致。

#### 1.1.2 标量乘法的两种写法

`vec * scalar` 可以用成员函数实现，但 `scalar * vec` 左操作数不是类类型，必须用非成员函数（通常声明为友元）。

#### 1.1.3 operator+ 用非成员函数的原因

如果写成成员函数 `Vec2 operator+(const Vec2&)`，那么 `3.0 + vec` 无法编译（左操作数不是 `Vec2`）。非成员函数两个参数地位对等，支持任意一侧做隐式转换。

#### 1.1.4 基于 += 实现 + 的惯用法

参数 `lhs` 按值传入（拷贝），然后复用 `+=` 实现。这是经典惯用法：`operator+` 基于 `operator+=` 实现，避免代码重复。

```cpp
#include <iostream>

class Vec2
{
public:
    double x, y;

    Vec2(double x_val, double y_val) : x(x_val), y(y_val) {}

    // 成员函数：+= 修改自身，返回引用
    Vec2& operator+=(const Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vec2& operator*=(double scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // 友元声明：让非成员函数访问私有成员
    friend Vec2 operator+(Vec2 lhs, const Vec2& rhs);
    friend Vec2 operator*(double scalar, Vec2 v);
    friend Vec2 operator*(Vec2 v, double scalar);
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v);
};

// 非成员函数：基于 += 实现 +（lhs 按值传入 = 拷贝）
Vec2 operator+(Vec2 lhs, const Vec2& rhs)
{
    lhs += rhs;
    return lhs;
}

// scalar * vec：左操作数是 double，只能用非成员函数
Vec2 operator*(double scalar, Vec2 v)
{
    v *= scalar;
    return v;
}

// vec * scalar：委托给上面的版本
Vec2 operator*(Vec2 v, double scalar)
{
    return scalar * v;
}

std::ostream& operator<<(std::ostream& os, const Vec2& v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

int main()
{
    Vec2 a(1.0, 2.0), b(3.0, 4.0);
    Vec2 c = a + b;
    std::cout << "a + b = " << c << "\n";       // (4, 6)
    std::cout << "2 * b = " << 2.0 * b << "\n"; // (6, 8)
    std::cout << "b * 3 = " << b * 3.0 << "\n"; // (9, 12)
}
```

### 1.2 比较运算符（==, <=>）

#### 1.2.1 太空船运算符 <=> 的用法（C++20）

`= default` 让编译器按成员声明顺序逐个比较，一行代码自动生成 `==`、`!=`、`<`、`>`、`<=`、`>=` 全部六个运算符。返回 `std::strong_ordering`：`less` 表示小于，`equal` 表示相等，`greater` 表示大于。如果需要自定义比较逻辑，也可以手写 `operator<=>`。

```cpp
#include <iostream>
#include <compare>    // std::strong_ordering
#include <vector>
#include <algorithm>

class Version
{
public:
    int major, minor, patch;

    Version(int maj, int min, int pat)
        : major(maj), minor(min), patch(pat) {}

    // 一行代码自动生成全部六个比较运算符
    auto operator<=>(const Version&) const = default;

    friend std::ostream& operator<<(std::ostream& os, const Version& v)
    {
        os << v.major << "." << v.minor << "." << v.patch;
        return os;
    }
};

int main()
{
    Version v1(1, 0, 0), v2(1, 2, 3), v3(2, 0, 0);

    std::cout << (v1 < v2 ? "true" : "false") << "\n";   // true
    std::cout << (v2 == v2 ? "true" : "false") << "\n";   // true

    // 有了 < 运算符，可以直接排序
    std::vector<Version> versions = {v3, v1, v2};
    std::sort(versions.begin(), versions.end());
    for (const auto& v : versions)
    {
        std::cout << v << "  ";  // 1.0.0  1.2.3  2.0.0
    }
    std::cout << "\n";
}
```

### 1.3 流运算符（<<, >>）

#### 1.3.1 流运算符必须用友元函数

`operator<<` 和 `operator>>` 必须用友元函数（非成员函数），因为左操作数是 `std::ostream` / `std::istream`，不是我们的类。如果写成成员函数，调用方式变成 `v << cout`，语义完全反了。返回 `ostream&` / `istream&` 支持链式操作 `cout << a << b << c`。

```cpp
#include <iostream>
#include <sstream>

class Vec2
{
public:
    double x, y;
    Vec2(double x_val, double y_val) : x(x_val), y(y_val) {}

    // 友元：让非成员函数访问成员
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v);
    friend std::istream& operator>>(std::istream& is, Vec2& v);
};

// 输出运算符
std::ostream& operator<<(std::ostream& os, const Vec2& v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;  // 返回引用支持链式输出
}

// 输入运算符：注意参数 v 不是 const，因为要修改它
std::istream& operator>>(std::istream& is, Vec2& v)
{
    is >> v.x >> v.y;
    return is;
}

int main()
{
    Vec2 v(3.14, 2.72);
    std::cout << "v = " << v << "\n";  // v = (3.14, 2.72)

    // 从字符串流读取
    std::istringstream input("10.5 20.5");
    Vec2 v2(0, 0);
    input >> v2;
    std::cout << "读取: " << v2 << "\n";  // 读取: (10.5, 20.5)
}
```

### 1.4 下标运算符（[]）

#### 1.4.1 下标运算符的 const 版本

下标运算符需要提供两个版本：非 const 版本返回引用允许读写（`arr[i] = 42`），const 版本返回 const 引用只允许读取。当对象本身是 const 时调用 const 版本。不提供 const 版本的话，const 对象无法使用 `[]` 运算符。

```cpp
#include <iostream>

class IntArray
{
    int* data_;
    size_t size_;

public:
    explicit IntArray(size_t n) : data_(new int[n]{}), size_(n) {}
    ~IntArray() { delete[] data_; }

    // 禁止拷贝
    IntArray(const IntArray&) = delete;
    IntArray& operator=(const IntArray&) = delete;

    size_t size() const { return size_; }

    // 非 const 版本：返回引用，允许读写
    int& operator[](size_t index)
    {
        return data_[index];
    }

    // const 版本：返回 const 引用，只允许读取
    const int& operator[](size_t index) const
    {
        return data_[index];
    }
};

int main()
{
    IntArray arr(5);
    for (size_t i = 0; i < arr.size(); ++i)
    {
        arr[i] = static_cast<int>(i * 10);  // 非 const 版本
    }

    // const 对象使用 const 版本
    const IntArray& carr = arr;
    std::cout << carr[2] << "\n";  // 输出: 20
    // carr[2] = 99;  // 编译错误：const 版本返回 const 引用
}
```

### 1.5 函数调用运算符（()）— 仿函数

#### 1.5.1 仿函数（Functor）

重载 `operator()` 让对象像函数一样调用。lambda 的本质就是编译器生成的匿名仿函数类。仿函数的优势是可以携带状态（通过成员变量），并且能传给 STL 算法作为比较器或变换器。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

// 乘法器：构造时存倍数，调用时返回 n * 倍数
class Multiplier
{
    int factor_;

public:
    explicit Multiplier(int factor) : factor_(factor) {}

    // operator() 让对象像函数一样被调用
    int operator()(int n) const
    {
        return n * factor_;
    }
};

// 自定义比较器：按绝对值升序排序
struct AbsLess
{
    bool operator()(int a, int b) const
    {
        return std::abs(a) < std::abs(b);
    }
};

int main()
{
    Multiplier triple(3);
    std::cout << triple(7) << "\n";   // 输出: 21
    std::cout << triple(10) << "\n";  // 输出: 30

    // 在 STL 算法中使用仿函数
    std::vector<int> nums = {-3, 1, -4, 1, 5, -9};
    std::sort(nums.begin(), nums.end(), AbsLess());

    // 等价的 lambda 写法
    std::sort(nums.begin(), nums.end(), [](int a, int b) {
        return std::abs(a) < std::abs(b);
    });

    // std::transform 配合仿函数
    std::vector<int> src = {1, 2, 3, 4, 5};
    std::vector<int> dst(src.size());
    std::transform(src.begin(), src.end(), dst.begin(), Multiplier(10));
    for (int n : dst)
    {
        std::cout << n << " ";  // 10 20 30 40 50
    }
    std::cout << "\n";
}
```

### 1.6 自增 / 自减运算符（++, --）

#### 1.6.1 前缀自增

前缀 `++x` 无参数，修改自身后返回引用（`*this`），调用方拿到的是加完之后的值。

#### 1.6.2 后缀自增 vs 前缀自增

两者用哑参数区分，性能不同：

| 对比项 | 前缀 `++x` | 后缀 `x++` |
|--------|-----------|-----------|
| 签名 | `operator++()` 无参数 | `operator++(int)`，int 仅作哑参数区分 |
| 返回值 | 引用（`*this`，加完后的值） | 值（加之前的旧值，临时对象） |
| 代价 | 无拷贝 | 多一次拷贝（保存旧值） |
| 建议 | 优先用 | 需要旧值时才用 |

```cpp
#include <iostream>

class Counter
{
    int value_;

public:
    explicit Counter(int v = 0) : value_(v) {}
    int value() const { return value_; }

    // 前缀 ++x：无参数，返回引用
    Counter& operator++()
    {
        ++value_;
        return *this;
    }

    // 后缀 x++：int 哑参数区分，返回旧值（值类型）
    Counter operator++(int)
    {
        Counter old = *this;  // 保存旧值（多一次拷贝）
        ++value_;
        return old;
    }

    // 前缀 --x
    Counter& operator--()
    {
        --value_;
        return *this;
    }

    // 后缀 x--
    Counter operator--(int)
    {
        Counter old = *this;
        --value_;
        return old;
    }
};

int main()
{
    Counter c(10);

    Counter& ref = ++c;           // 前缀：返回加完后的值
    std::cout << ref.value() << "\n";  // 11

    Counter old = c++;            // 后缀：返回加之前的旧值
    std::cout << old.value() << "\n";  // 11（旧值）
    std::cout << c.value() << "\n";    // 12（c 已变）
}
```

### 1.7 类型转换运算符

#### 1.7.1 explicit operator bool()

让对象在 `if` / `while` / 逻辑运算中当布尔值使用。加 `explicit` 后只有在条件表达式（`if`/`while`/`&&`/`||`/`!`/`?:`）中才会自动转换，其他地方必须显式 `static_cast<bool>(obj)`。不加 `explicit` 时对象可以隐式转成 bool 再转成 int，导致 `opt + 1` 这种无意义的代码能编译通过。

```cpp
#include <iostream>

class OptionalInt
{
    int value_;
    bool has_value_;

public:
    OptionalInt() : value_(0), has_value_(false) {}
    explicit OptionalInt(int v) : value_(v), has_value_(true) {}

    int value() const { return value_; }

    // explicit operator bool()：
    // 只有在 if/while/&&/||/!/?: 中才自动转换
    explicit operator bool() const
    {
        return has_value_;
    }
};

int main()
{
    OptionalInt a(42);
    OptionalInt b;  // 无值

    // 在 if 条件中自动调用 operator bool()
    if (a)
    {
        std::cout << "a 有值: " << a.value() << "\n";
    }

    if (!b)
    {
        std::cout << "b 无值\n";
    }

    // 下面这些会编译错误（explicit 阻止隐式转换）：
    // int x = a;       // 错误
    // int y = a + 1;   // 错误

    // 必须显式转换
    bool flag = static_cast<bool>(a);
    std::cout << "flag = " << (flag ? "true" : "false") << "\n";
}
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/operator_overload.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-ninja\operator_overload.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/operator_overload.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-make\operator_overload.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/operator_overload.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-mc\Release\operator_overload.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\operator_overload.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-msvc-ninja\operator_overload.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\operator_overload.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-msvc-vs\Release\operator_overload.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/operator_overload
```
