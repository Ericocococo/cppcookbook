# 08_oop — 面向对象

演示 `struct`、`class` 封装（构造/析构/访问控制）、继承、多态（虚函数）。

## 1. 知识点

| 文件 | 说明 |
|---|---|
| `main.cpp` | 5 个 demo 函数：struct → class 封装 → 继承 → 多态 → Rule of 0/3/5 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `oop` |

### 1.1 struct

#### 1.1.1 struct：数据聚合，默认 public

`struct` 用于组织相关的数据字段，默认访问权限是 `public`。可以用聚合初始化（大括号）直接赋值，也可以定义成员函数。适合纯数据结构或简单的值对象。

```cpp
#include <iostream>
#include <cmath>

struct Point {
    double x, y;

    double distance() const {
        return std::sqrt(x * x + y * y);
    }

    void print() const {
        std::cout << "Point(" << x << ", " << y
                  << ")  距原点=" << distance() << "\n";
    }
};

int main() {
    Point p1{3.0, 4.0};         // 聚合初始化
    Point p2 = {1.0, 2.0};      // 也可以用 = {} 形式
    p1.print();                  // Point(3, 4) 距原点=5
    p1.x = 0;                   // public 成员可以直接访问和修改
    return 0;
}
```

### 1.2 class 封装

#### 1.2.1 class 与 struct 的区别

`class` 和 `struct` 的唯一区别是默认访问权限：`class` 默认 `private`，`struct` 默认 `public`。通常 `class` 用于有封装逻辑的对象（数据 private，通过 public 方法访问），`struct` 用于纯数据聚合。

```cpp
class Rectangle {
private:
    double width_, height_;      // 私有数据，外部不能直接访问
    static int count_;           // 静态成员：属于类，所有对象共享

public:
    double area() const { return width_ * height_; }

    void setWidth(double w) {
        if (w < 0) {
            return;              // 封装：在 setter 中校验数据
        }
        width_ = w;
    }

    static int count() { return count_; }  // 静态成员函数
};

int Rectangle::count_ = 0;  // 静态成员在类外初始化
```

#### 1.2.2 构造函数 + 初始化列表

构造函数在对象创建时自动调用。初始化列表（`:` 后面的部分）直接初始化成员，比在函数体内赋值更高效。对于 `const` 成员和引用成员，只能用初始化列表。

```cpp
class Rectangle {
    double width_, height_;

public:
    // 初始化列表：width_(w) 是直接初始化，不是先默认构造再赋值
    Rectangle(double w, double h)
        : width_(w), height_(h)
    {
        // 函数体：可以做额外逻辑（日志、计数等）
    }
};
```

#### 1.2.3 默认构造与委托构造（C++11）

默认构造函数没有参数（或所有参数都有默认值）。委托构造让一个构造函数调用同类的另一个构造函数，避免重复初始化代码。

```cpp
class Rectangle {
    double width_, height_;

public:
    Rectangle(double w, double h) : width_(w), height_(h) {}
    Rectangle() : Rectangle(0, 0) {}  // 委托给上面的构造函数
};

int main() {
    Rectangle r1(3.0, 4.0);  // 调用双参数构造
    Rectangle r2;            // 调用默认构造 → 委托 → Rectangle(0, 0)
    return 0;
}
```

#### 1.2.4 析构函数：对象销毁时自动调用

析构函数 `~类名()` 在对象离开作用域或被 `delete` 时自动调用，用于释放资源（关闭文件、释放内存等）。不需要手动调用。

```cpp
class Rectangle {
    double width_, height_;
    static int count_;

public:
    Rectangle(double w, double h) : width_(w), height_(h) { ++count_; }

    ~Rectangle() {
        --count_;
        // 析构时自动执行，适合清理资源
    }
};

int main() {
    {
        Rectangle r(3.0, 4.0);  // 构造：count_ = 1
    }                           // r 离开作用域，析构自动调用：count_ = 0
    return 0;
}
```

### 1.3 继承

#### 1.3.1 继承基础：子类继承父类的成员和方法

用 `class Child : public Parent` 声明继承。子类拥有父类的所有成员（受访问权限限制），可以添加新成员和方法，也可以覆盖父类的虚函数。基类有子类时，析构函数必须是 `virtual`。

```cpp
#include <string>
#include <utility>

class Shape {
protected:
    std::string color_;

public:
    explicit Shape(std::string color) : color_(std::move(color)) {}
    virtual ~Shape() = default;          // 虚析构（有子类时必须！）
    virtual double area() const = 0;     // 纯虚函数：强制子类实现
};

class Circle : public Shape {
    double radius_;

public:
    Circle(double r, std::string color)
        : Shape(std::move(color)), radius_(r) {}

    double area() const override {       // 覆盖父类纯虚函数
        return 3.14159 * radius_ * radius_;
    }
};
```

#### 1.3.2 非虚函数：共用实现

非虚成员函数不能被子类覆盖（即使子类定义了同名函数，也是隐藏而非覆盖）。适合定义所有子类共用的逻辑。

```cpp
class Shape {
public:
    virtual double area() const = 0;
    virtual std::string type() const = 0;

    // 非虚函数：所有子类共用这个实现
    void describe() const {
        std::cout << type() << " 面积=" << area() << "\n";
    }
};
```

#### 1.3.3 protected 访问权限

`protected` 成员只能在类内部和子类中访问，类外部不能直接访问。介于 `public`（全可见）和 `private`（只有本类）之间。

```cpp
class Shape {
protected:
    std::string color_;   // 子类可以访问，外部不行
};

class Circle : public Shape {
    void showColor() {
        std::cout << color_;  // OK：子类可以访问 protected
    }
};

// Circle c;
// c.color_;   // 错误：protected 外部不能访问
```

### 1.4 多态

#### 1.4.1 多态：父类指针/引用调用子类的虚函数

通过父类指针或引用调用虚函数时，运行时根据实际对象类型决定调用哪个版本。这就是运行时多态（dynamic polymorphism），是面向对象编程的核心特性。

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual std::string type() const = 0;

    void describe() const {
        std::cout << type() << " 面积=" << area() << "\n";
    }
};
```

#### 1.4.2 父类指针数组，存放不同子类对象

一个 `Shape*` 数组可以存放 `Circle*`、`Square*` 等不同子类指针。遍历时调用虚函数，每个对象执行自己类的版本。

```cpp
Shape* shapes[] = {
    new Circle(3.0, "红"),
    new Square(4.0, "蓝"),
};

for (auto* s : shapes) {
    s->describe();  // 运行时决定：调 Circle 还是 Square 的方法
}

for (auto* s : shapes) {
    delete s;       // 通过虚析构正确释放
}
```

#### 1.4.3 虚析构的重要性

如果基类析构函数不是 `virtual`，通过基类指针 `delete` 子类对象时，子类的析构函数不会被调用，导致子类的资源泄漏。有继承关系时，基类析构函数必须声明为 `virtual`。

```cpp
class Base {
public:
    virtual ~Base() = default;   // 必须！有子类时析构要 virtual
};

class Derived : public Base {
    int* data_ = new int[100];
public:
    ~Derived() { delete[] data_; }  // 有虚析构才能被正确调用
};
```

### 1.5 Rule of 0/3/5

#### 1.5.1 三条规则概述

- **Rule of 0**：如果不管理资源（不手动 new/fopen），不要定义析构/拷贝/移动，用编译器默认的
- **Rule of 3**：如果自定义了析构函数，则必须同时定义拷贝构造和拷贝赋值运算符
- **Rule of 5**：在 Rule of 3 基础上，再加移动构造和移动赋值（C++11），共 5 个特殊成员函数

```cpp
// Rule of 0：不管理资源，什么都不定义
struct Point {
    double x, y;
    // 编译器自动生成所有特殊成员函数，完全够用
};
```

#### 1.5.2 拷贝构造（深拷贝）

当类管理裸指针等资源时，默认的拷贝构造是浅拷贝（复制指针地址），两个对象指向同一块内存，析构时 double free。必须自定义深拷贝：分配新内存，复制数据。

```cpp
#include <algorithm>

class Buffer {
    int* data_;
    size_t size_;

public:
    explicit Buffer(size_t n) : data_(new int[n]{}), size_(n) {}
    ~Buffer() { delete[] data_; }

    // 拷贝构造：深拷贝（分配新内存，复制数据）
    Buffer(const Buffer& other)
        : data_(new int[other.size_]{}), size_(other.size_)
    {
        std::copy(other.data_, other.data_ + size_, data_);
    }
};

int main() {
    Buffer b1(5);
    Buffer b2 = b1;   // 拷贝构造：b2 有独立的内存副本
    return 0;
}
```

#### 1.5.3 移动构造（转移所有权，C++11）

移动构造从源对象"窃取"资源（指针），而不是拷贝数据，效率远高于深拷贝。移动后源对象处于有效但不确定的状态（通常置空）。用 `std::move()` 将左值转为右值引用来触发移动。

```cpp
#include <utility>

class Buffer {
    int* data_;
    size_t size_;

public:
    explicit Buffer(size_t n) : data_(new int[n]{}), size_(n) {}
    ~Buffer() { delete[] data_; }

    // 移动构造：窃取资源，不复制数据
    Buffer(Buffer&& other) noexcept
        : data_(other.data_), size_(other.size_)
    {
        other.data_ = nullptr;   // 源对象置空，防止 double free
        other.size_ = 0;
    }

    size_t size() const { return size_; }
};

int main() {
    Buffer b1(5);
    Buffer b3 = std::move(b1);   // 移动构造：b1 的资源转移给 b3
    // b1.size() == 0（已被移动，资源归 b3）
    return 0;
}
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/oop.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-ninja\oop.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/oop.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-make\oop.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/oop.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-mc\Release\oop.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\oop.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-msvc-ninja\oop.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\oop.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-msvc-vs\Release\oop.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/oop
```

## 3. 英文及缩写说明

### 3.1 关键字

| 词汇           | 说明                                     |
|--------------|----------------------------------------|
| `struct`     | Structure：结构体，默认成员是 `public`           |
| `class`      | 类，默认成员是 `private`；通常用于有封装逻辑的对象         |
| `public`     | 公有：任何地方都能访问                            |
| `private`    | 私有：只有类内部可访问，外部和子类均不能直接访问               |
| `protected`  | 受保护：类内部和子类可访问，类外部不能                    |
| `virtual`    | 虚函数：允许子类覆盖，实现运行时多态；有子类时析构函数必须是 virtual |
| `override`   | 覆盖（C++11）：明确标注覆盖父类虚函数；父类无此虚函数时编译报错     |
| `final`      | 终止（C++11）：标在类上禁止继承；标在函数上禁止子类继续覆盖       |
| `explicit`   | 显式：禁止构造函数被隐式调用                         |
| `static`（成员） | 静态成员：属于类本身而非某个对象，所有对象共享                |

### 3.2 概念

| 概念                    | 说明                                               |
|-----------------------|--------------------------------------------------|
| OOP                   | Object-Oriented Programming：面向对象编程，三大特性：封装、继承、多态 |
| encapsulation         | 封装：数据和操作放在一起，并控制外部访问权限                           |
| inheritance           | 继承：子类继承父类的成员，`class Child : public Parent`       |
| polymorphism          | 多态：父类指针指向子类对象，调用虚函数时自动执行子类版本                     |
| constructor           | 构造函数：对象创建时自动调用，名字与类名相同                           |
| destructor            | 析构函数：对象销毁时自动调用，名字是 `~类名`                         |
| initializer list      | 初始化列表：构造函数 `:` 后的 `member(value)` 语法，比函数体内赋值更高效  |
| pure virtual function | 纯虚函数：`virtual type func() = 0`，强制子类实现            |
| abstract class        | 抽象类：含纯虚函数的类，不能直接创建对象                             |
| vtable                | Virtual Table：虚函数表，编译器为含虚函数的类生成的函数指针表            |
| virtual destructor    | 虚析构函数：`delete 基类指针` 时正确调用子类析构函数，防止资源泄漏           |
| `<cmath>`             | C math：包含 `sqrt`、`sin`、`cos`、`pow` 等数学函数         |
