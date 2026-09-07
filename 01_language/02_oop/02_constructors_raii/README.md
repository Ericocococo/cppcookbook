# 02_constructors_raii — 构造函数全形式与 RAII

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 演示代码 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `constructors_raii` |

---

## 知识点详解

### 1.1 构造函数全家族

#### 1.1.1 参数构造函数

接收所有参数来初始化对象，是最常见的构造函数形式。

```cpp
#include <iostream>
#include <string>

class Widget
{
    std::string name_;
    int value_;

public:
    Widget(std::string name, int value)
        : name_(std::move(name)), value_(value)
    {
    }

    void print() const
    {
        std::cout << "Widget{" << name_ << ", " << value_ << "}\n";
    }
};

int main()
{
    Widget w("alpha", 10);
    w.print();  // 输出: Widget{alpha, 10}
}
```

#### 1.1.2 委托构造函数（C++11）

委托构造（一个构造函数在初始化列表中调用同类的另一个构造函数，避免重复初始化代码）的基本用法在 01_basics 08_oop 1.2.3 已演示，这里补两条入门没讲的规则：委托目标写在初始化列表里时，**不能再同时初始化其他成员**；委托构造的函数体先执行完，才执行当前构造函数的函数体。

```cpp
class Widget
{
    std::string name_;
    int value_;

public:
    Widget(std::string name, int value)
        : name_(std::move(name)), value_(value)
    {
    }

    // 委托给上面的参数构造函数
    Widget()
        : Widget("default", 0)   // 委托目标；此处不能再写 name_(...) 等其他成员
    {
        // 委托构造执行完后才执行这里
    }
};
```

#### 1.1.3 拷贝构造函数

参数必须是 `const T&`（const 引用），不能按值传递。如果按值传递，调用拷贝构造时又需要拷贝构造来创建参数，导致无限递归。

```cpp
class Widget
{
    std::string name_;
    int value_;

public:
    Widget(std::string name, int value)
        : name_(std::move(name)), value_(value)
    {
    }

    // 参数必须是 const 引用
    Widget(const Widget& other)
        : name_(other.name_), value_(other.value_)
    {
    }
};

int main()
{
    Widget w1("alpha", 10);
    Widget w2 = w1;  // 调用拷贝构造（等号初始化，不是赋值）
}
```

#### 1.1.4 移动构造函数（C++11）

参数是右值引用 `T&&`，"窃取"源对象的资源而不是复制。移动构造务必标 `noexcept`（vector 扩容时只有 noexcept 的移动构造才会被使用，原理见 01_basics 11_exceptions 1.4.2）。

```cpp
class Widget
{
    std::string name_;
    int value_;

public:
    Widget(std::string name, int value)
        : name_(std::move(name)), value_(value)
    {
    }

    // noexcept 很重要：vector 扩容时只有 noexcept 移动构造才会被使用
    Widget(Widget&& other) noexcept
        : name_(std::move(other.name_)), value_(other.value_)
    {
        other.value_ = 0;  // 把源对象置为安全状态
    }
};

int main()
{
    Widget w1("alpha", 10);
    Widget w2 = std::move(w1);  // 移动构造：w1 的资源被转移给 w2
    // w1 现在处于"有效但未指定"的状态
}
```

#### 1.1.5 转换构造 + explicit

只有一个参数的构造函数可以做隐式类型转换。`explicit` 阻止这种隐式转换，要求调用方显式构造对象。不加 `explicit` 时，参数类型可以悄悄变成目标类型，容易引发难以排查的 bug。

```cpp
#include <iostream>

class Meter
{
    double value_;

public:
    // explicit 阻止 double -> Meter 的隐式转换
    explicit Meter(double val) : value_(val) {}
    double value() const { return value_; }
};

class Inch
{
    double value_;

public:
    // 没有 explicit：允许 double -> Inch 的隐式转换
    Inch(double val) : value_(val) {}
    double value() const { return value_; }
};

void printMeter(Meter m) { std::cout << m.value() << " m\n"; }
void printInch(Inch i)   { std::cout << i.value() << " in\n"; }

int main()
{
    // printMeter(3.14);       // 编译错误！explicit 阻止隐式转换
    printMeter(Meter(3.14));   // 正确：显式构造
    printInch(2.54);           // 正确：Inch 没有 explicit，允许隐式转换
}
```

### 1.2 初始化列表 vs 函数体赋值

#### 1.2.1 回顾与对比表

两者的本质差别（初始化列表让成员构造一步到位、函数体赋值先默认构造再赋值）在 01_basics 08_oop 1.2.2 已讲，这里用一张表收尾：

| 对比项 | 初始化列表 `: x_(v)` | 函数体赋值 `x_ = v;` |
|--------|------|------|
| 时机 | 成员构造时一步到位 | 成员先默认构造、再赋值（两步） |
| `const` 成员 | 可以（唯一方式） | 不行（只能初始化一次） |
| 引用成员 | 可以（唯一方式） | 不行 |
| 推荐度 | 推荐 | 不推荐 |

#### 1.2.2 初始化顺序

成员变量的初始化顺序由声明顺序决定，不是初始化列表中的书写顺序。如果初始化列表的书写顺序和声明顺序不同，编译器会警告（`-Wreorder`），但按声明顺序执行。依赖未初始化的成员会导致未定义行为。

```cpp
class Trap
{
    int first_;    // 声明在前，先初始化
    int second_;   // 声明在后，后初始化

public:
    // 危险写法：看起来 second_ 先初始化，但实际 first_ 先初始化
    // Trap(int b) : second_(b), first_(second_ + 1) {}
    // first_ 先初始化时 second_ 还未初始化 → 未定义行为！

    // 正确写法：书写顺序与声明顺序一致
    Trap(int a, int b) : first_(a), second_(b) {}
};
```

### 1.3 析构顺序

#### 1.3.1 析构顺序

栈对象后构造的先析构（LIFO，像弹栈）。继承体系中先析构子类再析构父类。成员变量按声明的逆序析构。

```cpp
#include <iostream>
#include <string>

class Component
{
    std::string name_;

public:
    explicit Component(std::string name) : name_(std::move(name))
    {
        std::cout << "[ctor] " << name_ << "\n";
    }

    ~Component()
    {
        std::cout << "[dtor] " << name_ << "\n";
    }
};

int main()
{
    Component a("A");  // 先构造
    Component b("B");
    Component c("C");  // 后构造
    // 离开作用域时析构顺序：C -> B -> A（LIFO）
}
```

#### 1.3.2 继承体系的析构顺序

构造顺序：先父类 -> 按声明顺序构造成员 -> 子类函数体。析构顺序完全相反：子类函数体 -> 逆序析构成员 -> 父类。

```cpp
class Base
{
public:
    Base()           { std::cout << "[Base ctor]\n"; }
    virtual ~Base()  { std::cout << "[Base dtor]\n"; }
};

class Derived : public Base
{
    Component first_{"member-1st"};
    Component second_{"member-2nd"};

public:
    Derived() : Base()
    {
        std::cout << "[Derived ctor]\n";
    }

    ~Derived() override
    {
        std::cout << "[Derived dtor]\n";
        // 之后自动：second_ 析构 -> first_ 析构 -> Base 析构
    }
};
```

### 1.4 RAII（Resource Acquisition Is Initialization）

#### 1.4.1 RAII 核心思想（回顾）

RAII 思想——资源在构造时获取、析构时释放，栈展开保证无论正常退出还是抛异常都会释放——以及"构造开、析构关"的 FileGuard 入门例子，都在 01_basics 11_exceptions 1.5.1 讲过了，这里不再重复演示。直接总结 RAII 资源类（`unique_ptr`、`fstream`、`lock_guard`）的两个实现共同点：

1. **析构函数负责释放资源**——用户只管用，不需要手动释放；
2. **拷贝被禁用**——两个对象共享同一份资源会双重释放，所以用 `= delete` 禁拷贝（`= delete` 见 06_special_members 1.4）。

#### 1.4.2 LockGuard 模式

RAII 管理锁的生命周期：构造时加锁，析构时解锁，不管怎么退出作用域都会释放。标准库的 `std::lock_guard` 就是这个模式。

```cpp
struct FakeMutex
{
    void lock() {}
    void unlock() {}
};
// 真实项目用 std::mutex（<mutex>）；这里用最小假锁，专注 RAII 模式本身

class LockGuard
{
    FakeMutex& mtx_;  // 引用：不拥有 mutex，只管理锁定状态

public:
    explicit LockGuard(FakeMutex& mtx) : mtx_(mtx)
    {
        mtx_.lock();   // 构造时加锁
    }

    ~LockGuard()
    {
        mtx_.unlock(); // 析构时解锁——保证不管怎么退出作用域都会释放
    }

    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};
```

### 1.5 copy-and-swap 惯用法

#### 1.5.1 copy-and-swap 惯用法

手写拷贝赋值运算符容易出错：自赋值（`a = a`）时先 delete 再 new 会访问已释放的内存；new 失败时对象已经被 delete 处于损坏状态。copy-and-swap 三步法解决这些问题：写好拷贝构造（深拷贝）、写 swap 函数（交换所有成员，noexcept）、赋值运算符按值传参然后 swap。

#### 1.5.2 swap 函数

交换两个对象的所有成员，标记 `noexcept` 保证不抛异常。使用 `friend` 让 `std::swap` 通过 ADL（Argument-Dependent Lookup，参数依赖查找）找到——调用 `swap(a, b)` 时编译器除了普通命名空间，还会在**参数类型所在的命名空间**里找 `swap`，于是能找到我们为这个类定义的 `friend swap`；不加 `friend` 写类外同名函数也能被 ADL 找到，但 friend 能访问 private 成员。

#### 1.5.3 赋值运算符（copy-and-swap）

参数按值传递而非引用：传入左值时触发拷贝构造（other 是副本），传入右值时触发移动构造（other 接管资源）。然后 swap 把副本的资源交给自己，旧资源交给副本。副本在函数结束时析构释放旧资源。好处：自赋值安全、异常安全、代码简洁。

```cpp
#include <iostream>
#include <algorithm>  // std::copy

class IntArray
{
    int* data_;
    size_t size_;

public:
    explicit IntArray(size_t size = 0)
        : data_(size > 0 ? new int[size]{} : nullptr), size_(size)
    {
    }

    ~IntArray()
    {
        delete[] data_;
    }

    // 步骤 1：拷贝构造（深拷贝）
    IntArray(const IntArray& other)
        : data_(other.size_ > 0 ? new int[other.size_] : nullptr),
          size_(other.size_)
    {
        if (size_ > 0)
        {
            std::copy(other.data_, other.data_ + size_, data_);
        }
    }

    // 移动构造
    IntArray(IntArray&& other) noexcept
        : data_(other.data_), size_(other.size_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // 步骤 2：swap 函数
    friend void swap(IntArray& a, IntArray& b) noexcept
    {
        using std::swap;
        swap(a.data_, b.data_);
        swap(a.size_, b.size_);
    }

    // 步骤 3：赋值运算符——参数按值传入（不是引用）
    // 传入左值 -> 拷贝构造 other -> swap -> other 析构旧数据
    // 传入右值 -> 移动构造 other -> swap -> other 析构旧数据
    IntArray& operator=(IntArray other)
    {
        swap(*this, other);
        return *this;
    }

    int& operator[](size_t i) { return data_[i]; }
    size_t size() const { return size_; }
};

int main()
{
    IntArray a(3);
    a[0] = 10; a[1] = 20; a[2] = 30;

    IntArray b(1);
    b = a;            // 拷贝赋值：安全
    a = a;            // 自赋值：安全
    b = std::move(a); // 移动赋值：安全
}
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/constructors_raii.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-mingw-ninja\constructors_raii.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/constructors_raii.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-mingw-make\constructors_raii.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/constructors_raii.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-mingw-mc\Release\constructors_raii.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\constructors_raii.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-msvc-ninja\constructors_raii.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\constructors_raii.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\02_constructors_raii\build-msvc-vs\Release\constructors_raii.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/constructors_raii
```
