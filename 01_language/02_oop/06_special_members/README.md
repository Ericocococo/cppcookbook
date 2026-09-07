# 06_special_members — 特殊成员函数 — =default/=delete、Rule 0/5

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 演示代码 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `special_members` |

---

## 知识点详解

### 1.1 六个特殊成员函数

#### 1.1.1 六个特殊成员函数总览

C++ 编译器最多可以为一个类自动生成 6 个特殊成员函数：默认构造函数 `T()`、析构函数 `~T()`、拷贝构造函数 `T(const T&)`、拷贝赋值运算符 `T& operator=(const T&)`、移动构造函数 `T(T&&) noexcept`、移动赋值运算符 `T& operator=(T&&) noexcept`。理解它们的自动生成规则是写好 C++ 类的基础。

```cpp
#include <iostream>
#include <string>

class Traced
{
    std::string name_;

public:
    // 1. 默认构造
    Traced() : name_("(unnamed)")
    {
        std::cout << "[默认构造] " << name_ << "\n";
    }

    // 带参构造（不是特殊成员，但很常用）
    explicit Traced(std::string name) : name_(std::move(name))
    {
        std::cout << "[带参构造] " << name_ << "\n";
    }

    // 2. 析构
    ~Traced() { std::cout << "[析构] " << name_ << "\n"; }

    // 3. 拷贝构造
    Traced(const Traced& other) : name_(other.name_ + "_copy")
    {
        std::cout << "[拷贝构造] " << name_ << "\n";
    }

    // 4. 拷贝赋值
    Traced& operator=(const Traced& other)
    {
        name_ = other.name_ + "_assigned";
        std::cout << "[拷贝赋值] " << name_ << "\n";
        return *this;
    }

    // 5. 移动构造
    Traced(Traced&& other) noexcept : name_(std::move(other.name_))
    {
        other.name_ = "(moved-from)";
        std::cout << "[移动构造] " << name_ << "\n";
    }

    // 6. 移动赋值
    Traced& operator=(Traced&& other) noexcept
    {
        name_ = std::move(other.name_);
        other.name_ = "(moved-from)";
        std::cout << "[移动赋值] " << name_ << "\n";
        return *this;
    }
};

int main()
{
    Traced a("alpha");          // 带参构造
    Traced b(a);                // 拷贝构造
    Traced c;                   // 默认构造
    c = a;                      // 拷贝赋值
    Traced d(std::move(a));     // 移动构造
    d = Traced("temp");         // 移动赋值
}
```

### 1.2 编译器何时自动生成

#### 1.2.1 编译器自动生成规则

核心规则：定义了析构函数 -> 移动构造/移动赋值不再自动生成，`std::move` 退化为拷贝。定义了拷贝构造 -> 移动不再自动生成。定义了移动 -> 编译器把拷贝 `delete` 掉。

#### 1.2.2 移动退化为拷贝

当类自定义了析构函数但没有定义移动构造时，`std::move(obj)` 想触发移动构造但移动构造未生成，编译器退回到拷贝构造（`const T&` 可以绑定右值）。结果是源对象没被"偷走"资源，目标对象是源对象的副本。

```cpp
#include <iostream>
#include <string>

class HasDestructor
{
    std::string name_;

public:
    HasDestructor(std::string n) : name_(std::move(n)) {}

    // 自定义析构 -> 编译器不再生成移动构造/移动赋值
    ~HasDestructor()
    {
        std::cout << "[析构] " << name_ << "\n";
    }

    const std::string& name() const { return name_; }
};

int main()
{
    HasDestructor h1("hello");

    // std::move(h1) 想触发移动构造，但移动构造未生成
    // 编译器退回到拷贝构造
    HasDestructor h2(std::move(h1));

    // h1 没被移走，h2 是 h1 的副本
    std::cout << "h1=" << h1.name() << "\n";  // hello（仍在）
    std::cout << "h2=" << h2.name() << "\n";  // hello
}
```

### 1.3 = default：显式让编译器生成

#### 1.3.1 = default 恢复默认构造

定义了任何构造函数后，默认构造函数不再自动生成。`= default` 恢复编译器默认实现。

#### 1.3.2 = default 用于拷贝和移动

当自定义了析构函数但默认实现够用时，显式写 `= default` 清晰表达意图："我知道规则，我要编译器版本"。

```cpp
#include <iostream>
#include <string>

class Widget
{
    int id_ = 0;
    std::string label_;

public:
    // 自定义了带参构造 -> 默认构造不再自动生成
    Widget(int id, std::string label)
        : id_(id), label_(std::move(label)) {}

    // = default 恢复默认构造
    Widget() = default;

    // 显式写 = default，清晰表达意图
    Widget(const Widget&) = default;
    Widget& operator=(const Widget&) = default;
    Widget(Widget&&) noexcept = default;
    Widget& operator=(Widget&&) noexcept = default;
    ~Widget() = default;

    void print() const
    {
        std::cout << "Widget{" << id_ << ", \"" << label_ << "\"}\n";
    }
};

int main()
{
    Widget w1;                   // 默认构造可用（被 = default 恢复）
    Widget w2(42, "hello");
    Widget w3 = w2;              // 拷贝
    Widget w4 = std::move(w2);   // 移动
    w4.print();
}
```

### 1.4 = delete：显式禁用

#### 1.4.1 = delete 禁止拷贝

资源句柄（文件、网络连接、数据库）不能被复制，两个对象持有同一个 handle 会导致双重释放。`= delete` 让编译器在任何尝试拷贝的地方报错。`= delete` 也可以用于普通函数，禁止某些重载（比如只接受 `double` 不接受 `int`，避免隐式转换丢精度）。

下面代码的移动构造里出现了 `std::exchange(x, 新值)`（C++14，`<utility>`）：它一步完成两件事——读出 `x` 的旧值作为返回值，同时把 `x` 写成新值。移动构造用它把源对象的 handle 置空最简洁，完整讲解见 1.6.3。

```cpp
#include <iostream>
#include <string>
#include <utility>  // std::exchange

class UniqueResource
{
    std::string name_;
    int handle_;

public:
    UniqueResource(std::string name, int handle)
        : name_(std::move(name)), handle_(handle)
    {
        std::cout << "[获取资源] " << name_ << "\n";
    }

    ~UniqueResource()
    {
        if (handle_ >= 0)
        {
            std::cout << "[释放资源] " << name_ << "\n";
        }
    }

    // 禁止拷贝：两个对象持有同一个 handle 会双重释放
    UniqueResource(const UniqueResource&) = delete;
    UniqueResource& operator=(const UniqueResource&) = delete;

    // 允许移动：资源所有权可以转移
    UniqueResource(UniqueResource&& other) noexcept
        : name_(std::move(other.name_)),
          handle_(std::exchange(other.handle_, -1))
    {
        std::cout << "[转移资源] " << name_ << "\n";
    }
};

// delete 也可以用于普通函数
void process(double val) { std::cout << val << "\n"; }
void process(int) = delete;  // 传 int 直接编译错误

int main()
{
    UniqueResource r1("database", 100);
    // UniqueResource r2 = r1;         // 编译错误：拷贝被 delete
    UniqueResource r2 = std::move(r1); // 移动是允许的

    process(3.14);
    // process(42);   // 编译错误：use of deleted function
}
```

### 1.5 Rule of Zero

#### 1.5.1 Rule of Zero

最佳实践：尽量不自定义任何特殊成员函数。`std::string`、`std::vector` 等 RAII 容器自己管理资源，编译器默认生成的拷贝/移动/析构会逐成员调用对应操作。每个成员的操作已经正确，组合起来自然也正确。不写特殊成员 = 不出错 = 最安全。

#### 1.5.2 unique_ptr 与 Rule of Zero

用 `std::unique_ptr` 管理裸指针也符合 Rule of Zero。`unique_ptr` 不可拷贝 -> 包含它的类自动不可拷贝。`unique_ptr` 可移动 -> 包含它的类自动可移动。析构时 `unique_ptr` 自动释放 -> 不需要写析构。

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Rule of Zero：没有任何自定义特殊成员函数
class Person
{
    std::string name_;
    int age_;
    std::vector<std::string> hobbies_;

    // 编译器自动生成的全部正确：
    // 拷贝：string 和 vector 做深拷贝
    // 移动：string 和 vector 转移所有权
    // 析构：string 和 vector 释放内存

public:
    Person(std::string name, int age, std::vector<std::string> hobbies)
        : name_(std::move(name)), age_(age), hobbies_(std::move(hobbies))
    {
    }

    void print() const
    {
        std::cout << name_ << " (age=" << age_ << ")\n";
    }
};

// unique_ptr 管理裸指针，也符合 Rule of Zero
class ResourceOwner
{
    std::unique_ptr<int[]> data_;
    size_t size_;

    // unique_ptr 不可拷贝 -> ResourceOwner 也不可拷贝
    // unique_ptr 可移动 -> ResourceOwner 也可移动
    // 不需要写析构

public:
    explicit ResourceOwner(size_t n)
        : data_(std::make_unique<int[]>(n)), size_(n) {}
};

int main()
{
    Person p1("Alice", 30, {"reading", "coding"});
    Person p2 = p1;              // 拷贝：安全（深拷贝）
    Person p3 = std::move(p1);   // 移动：安全（转移所有权）

    ResourceOwner r1(10);
    // ResourceOwner r2 = r1;    // 编译错误：unique_ptr 不可拷贝
    ResourceOwner r2 = std::move(r1);  // 移动：安全
}
```

### 1.6 Rule of Five 完整示例（copy-and-swap 惯用法）

#### 1.6.1 三步法回顾（详讲见 02_constructors_raii）

copy-and-swap 的完整推导——为什么赋值参数按值传、为什么自赋值和异常都安全、swap 为什么用 `friend` 加 `noexcept`——以及 IntArray 完整示例，都在 02_constructors_raii 1.5 详讲过了，这里不重复。需要手写五个特殊成员的场景照 02 的 IntArray 写即可；能用 `unique_ptr`/`shared_ptr` 管理资源就优先用（本目录 1.5 的 Rule of Zero），只有必须裸指针时才手写全套。

#### 1.6.2 noexcept 与 vector 扩容（回顾）

移动构造标 `noexcept` 与否、`std::vector` 扩容时为什么 noexcept 才走移动，已在 01_basics 的 11_exceptions 1.4.2 讲透。这里补一个验证工具：`std::is_nothrow_move_constructible_v<T>` 在编译期检查类型能否不抛异常地移动构造——确认自己写的移动构造真标了 noexcept（忘了标就会得到 `false`）。

#### 1.6.3 std::exchange：一步完成"取走旧值并置空"

02 的 IntArray 移动构造分两步写：先 `data_ = other.data_` 取走指针，再 `other.data_ = nullptr` 把源对象置空。C++14 起 `<utility>` 提供 `std::exchange(x, 新值)`，把这两步合成一步：读出 `x` 的旧值并返回，同时把 `x` 写成新值。移动构造用它写最简洁，也不容易漏掉置空：

```cpp
#include <utility>   // std::exchange

// std::exchange(other.data_, nullptr)：
//   返回 other.data_ 的旧值，同时把 other.data_ 改成 nullptr
//   等价于两行：data_ = other.data_;  other.data_ = nullptr;
DynamicArray(DynamicArray&& other) noexcept
    : data_(std::exchange(other.data_, nullptr)),
      size_(std::exchange(other.size_, 0))
{
}

// 编译期验证移动构造确实 noexcept（忘写 noexcept 这里就报错）
static_assert(std::is_nothrow_move_constructible_v<DynamicArray>);
```

`DynamicArray` 的完整可运行实现（五个特殊成员全写 + 拷贝/移动/自赋值调用演示）在 main.cpp 的 demo06。

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/special_members.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-mingw-ninja\special_members.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/special_members.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-mingw-make\special_members.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/special_members.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-mingw-mc\Release\special_members.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\special_members.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-msvc-ninja\special_members.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\special_members.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\06_special_members\build-msvc-vs\Release\special_members.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/special_members
```
