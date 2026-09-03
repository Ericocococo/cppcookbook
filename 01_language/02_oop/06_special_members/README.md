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

#### 1.6.1 Rule of Five 与 copy-and-swap

管理裸资源时，必须定义全部 5 个特殊成员：析构 + 拷贝构造 + 拷贝赋值 + 移动构造 + 移动赋值。copy-and-swap 惯用法：拷贝赋值的参数按值传入（触发拷贝构造），然后和自己 swap。好处：异常安全（拷贝失败时 `*this` 不变）、自赋值安全（`a = a` 也安全）、代码简洁（复用拷贝构造的逻辑）。

#### 1.6.2 移动构造与 noexcept

`noexcept` 至关重要：`std::vector` 扩容时，如果元素的移动构造是 noexcept，vector 才会用移动而非拷贝。不标 noexcept 会导致性能退化。

#### 1.6.3 copy-and-swap 按值传参实现移动赋值

copy-and-swap 的拷贝赋值参数按值传入。传入左值时调用拷贝构造，传入右值时调用移动构造，不需要单独写移动赋值运算符。

```cpp
#include <iostream>
#include <algorithm>  // std::copy
#include <utility>    // std::exchange

class DynamicArray
{
    int* data_;
    size_t size_;

public:
    // 构造
    explicit DynamicArray(size_t n = 0)
        : data_(n > 0 ? new int[n]{} : nullptr), size_(n) {}

    // 1. 析构
    ~DynamicArray()
    {
        delete[] data_;
    }

    // 2. 拷贝构造（深拷贝）
    DynamicArray(const DynamicArray& other)
        : data_(other.size_ > 0 ? new int[other.size_] : nullptr),
          size_(other.size_)
    {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    // 3. 拷贝赋值（copy-and-swap：参数按值传入）
    // 传入左值 -> 拷贝构造 other
    // 传入右值 -> 移动构造 other（同时作为移动赋值）
    DynamicArray& operator=(DynamicArray other)
    {
        swap(*this, other);
        return *this;
    }

    // 4. 移动构造（noexcept 至关重要）
    DynamicArray(DynamicArray&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)),
          size_(std::exchange(other.size_, 0))
    {
    }

    // swap 函数
    friend void swap(DynamicArray& a, DynamicArray& b) noexcept
    {
        using std::swap;
        swap(a.data_, b.data_);
        swap(a.size_, b.size_);
    }

    void set(size_t i, int val)
    {
        if (i < size_)
        {
            data_[i] = val;
        }
    }

    size_t size() const { return size_; }
};

int main()
{
    DynamicArray a(3);
    a.set(0, 10);
    a.set(1, 20);
    a.set(2, 30);

    DynamicArray b(a);              // 拷贝构造
    DynamicArray c(2);
    c = a;                          // 拷贝赋值（copy-and-swap）
    DynamicArray d(std::move(a));   // 移动构造
    // a.size() == 0（被移走）

    // noexcept 验证
    std::cout << "noexcept = "
              << std::is_nothrow_move_constructible_v<DynamicArray> << "\n";  // 1
}
```

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
