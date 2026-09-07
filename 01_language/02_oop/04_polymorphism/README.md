# 04_polymorphism — 多态深入 — vtable、dynamic_cast、CRTP

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 演示代码 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `polymorphism` |

---

## 知识点详解

### 1.1 虚函数表（vtable）原理

#### 1.1.1 vtable 结构

有虚函数的类，编译器自动生成一张虚函数表（vtable），是一个函数指针数组，每个虚函数占一个槽位。每个对象多一个隐藏成员 vptr（虚函数表指针），指向所属类的 vtable，通常占 8 字节（64 位平台）。

#### 1.1.2 虚函数调用过程

`obj->func()` 的调用路径：通过 obj 的 vptr 找到 vtable -> 在 vtable 中查找 func 对应的槽位 -> 取出函数指针跳转调用。所以虚函数调用比普通函数调用多一次指针间接跳转。

```cpp
#include <iostream>

// 没有虚函数：大小 = 数据成员大小
class PlainClass
{
public:
    int value = 0;
    void doSomething() {}
};

// 有虚函数：大小 = 数据成员 + vptr（8 字节，有对齐填充）
class VirtualClass
{
public:
    int value = 0;
    virtual void doSomething() { std::cout << "VirtualClass\n"; }
    virtual ~VirtualClass() = default;
};

class DerivedVirtual : public VirtualClass
{
public:
    void doSomething() override { std::cout << "DerivedVirtual\n"; }
};

int main()
{
    std::cout << "sizeof(PlainClass)   = " << sizeof(PlainClass) << "\n";   // 4
    std::cout << "sizeof(VirtualClass) = " << sizeof(VirtualClass) << "\n"; // 16（int+vptr+对齐）

    VirtualClass* ptr = new DerivedVirtual();
    ptr->doSomething();  // 走 vtable：→ DerivedVirtual::doSomething
    delete ptr;
}
```

### 1.2 dynamic_cast 安全向下转型

#### 1.2.1 向下转型

把父类指针/引用转为子类指针/引用。`dynamic_cast` 在运行时检查类型是否匹配，指针版和引用版失败时的表现不同：

| 版本 | 失败时 | 使用建议 |
|------|--------|---------|
| `dynamic_cast<子类*>(父类指针)` | 返回 `nullptr` | 先判空再用 |
| `dynamic_cast<子类&>(父类引用)` | 抛出 `std::bad_cast` | 用 try/catch 包住 |

父类必须有虚函数（编译器需要 RTTI 信息做类型检查），没有虚函数的类不能用 `dynamic_cast`。

```cpp
#include <iostream>
#include <string>

class Shape
{
public:
    virtual ~Shape() = default;
    virtual std::string type() const = 0;
};

class Circle : public Shape
{
    double radius_;

public:
    explicit Circle(double r) : radius_(r) {}
    std::string type() const override { return "Circle"; }
    double radius() const { return radius_; }
};

class Rectangle : public Shape
{
    double w_, h_;

public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    std::string type() const override { return "Rectangle"; }
};

int main()
{
    Circle circle(5.0);
    Rectangle rect(3.0, 4.0);

    Shape* s1 = &circle;
    Shape* s2 = &rect;

    // 指针版：失败返回 nullptr
    Circle* c1 = dynamic_cast<Circle*>(s1);  // 成功
    Circle* c2 = dynamic_cast<Circle*>(s2);  // 失败，c2 == nullptr

    if (c1)
    {
        std::cout << "转型成功: radius=" << c1->radius() << "\n";
    }
    if (!c2)
    {
        std::cout << "转型失败: " << s2->type() << " 不是 Circle\n";
    }

    // 引用版：失败抛出 std::bad_cast
    try
    {
        Circle& ref = dynamic_cast<Circle&>(*s2);
    }
    catch (const std::bad_cast& e)
    {
        std::cout << "引用转型失败: " << e.what() << "\n";
    }
}
```

### 1.3 抽象类与接口模式

#### 1.3.1 抽象类（回顾）

抽象类——含至少一个纯虚函数（`= 0`）的类，不能直接实例化，子类必须实现所有纯虚函数否则也是抽象类——在 01_basics 08_oop 的多态演示里已经用过了（`Shape` 的 `area() = 0`）。本目录直接往下走：抽象类还可以包含部分实现和数据成员提供代码复用（见下），而 1.3.2 的接口模式是它的极限形态。

#### 1.3.2 接口模式

全部是纯虚函数的类（类似 Java 的 interface），只定义"能做什么"，不包含任何实现和数据成员。一个类可以通过多继承实现多个接口，比抽象类更灵活，不引入耦合。

```cpp
#include <iostream>
#include <string>
#include <vector>

// 接口：只有纯虚函数
class ISerializable
{
public:
    virtual ~ISerializable() = default;
    virtual std::string serialize() const = 0;
};

class IPrintable
{
public:
    virtual ~IPrintable() = default;
    virtual void print() const = 0;
};

// 抽象类：包含部分实现和数据
class Document
{
protected:
    std::string title_;

public:
    explicit Document(std::string title) : title_(std::move(title)) {}
    virtual ~Document() = default;
    virtual std::string format() const = 0;  // 纯虚函数
    std::string title() const { return title_; }  // 非虚函数，共用实现
};

// 实现多个接口 + 继承抽象类
class JsonDocument : public Document, public ISerializable, public IPrintable
{
    std::string content_;

public:
    JsonDocument(std::string title, std::string content)
        : Document(std::move(title)), content_(std::move(content)) {}

    std::string format() const override
    {
        return "{\"title\":\"" + title_ + "\",\"content\":\"" + content_ + "\"}";
    }

    std::string serialize() const override { return format(); }

    void print() const override
    {
        std::cout << "[JsonDocument] " << format() << "\n";
    }
};

int main()
{
    // Document doc("test");  // 编译错误：抽象类不能实例化

    JsonDocument jdoc("配置", "hello");

    // 通过不同的接口指针使用同一个对象
    Document* dp = &jdoc;
    std::cout << dp->format() << "\n";

    ISerializable* sp = &jdoc;
    std::cout << sp->serialize() << "\n";

    IPrintable* pp = &jdoc;
    pp->print();
}
```

### 1.4 协变返回类型（covariant return types）

#### 1.4.1 协变返回规则

子类 override 虚函数时，返回类型可以是父类返回类型的派生类指针/引用。编译器允许这种"放宽"：`Base*` -> `Derived*`，`Base&` -> `Derived&`。

#### 1.4.2 典型应用——clone() 方法

原型模式（Prototype Pattern）中，父类 `clone()` 返回 `Base*`，子类 `clone()` 返回 `Derived*`。好处：通过子类对象调用 `clone()` 时直接得到子类指针，不用手动 `dynamic_cast` 转型。

```cpp
#include <iostream>

class Prototype
{
public:
    virtual ~Prototype() = default;
    virtual Prototype* clone() const = 0;  // 返回 Prototype*
    virtual void describe() const = 0;
};

class ConcreteA : public Prototype
{
    int data_;

public:
    explicit ConcreteA(int d) : data_(d) {}

    // 协变返回：Prototype* → ConcreteA*
    // 编译器允许，因为 ConcreteA* 可以隐式转为 Prototype*
    ConcreteA* clone() const override
    {
        return new ConcreteA(*this);
    }

    void describe() const override
    {
        std::cout << "ConcreteA(data=" << data_ << ")\n";
    }

    int data() const { return data_; }
};

int main()
{
    ConcreteA original(42);

    // 通过子类对象调用：直接得到 ConcreteA*，不用 dynamic_cast
    ConcreteA* copy = original.clone();
    std::cout << "data=" << copy->data() << "\n";  // 直接访问子类方法

    // 通过父类指针调用：返回 Prototype*
    Prototype* base_ptr = &original;
    Prototype* copy2 = base_ptr->clone();
    copy2->describe();

    delete copy;
    delete copy2;
}
```

### 1.5 多态的性能考量

#### 1.5.1 虚函数调用的开销

普通函数调用编译器直接生成 call 指令（地址已知）。虚函数调用先读 vptr -> 再读 vtable -> 再跳转，多两次内存访问，还会阻止内联优化（编译器不知道最终调哪个函数）。

#### 1.5.2 优化手段——CRTP

CRTP（Curiously Recurring Template Pattern）是编译期多态方案，零运行时开销。它需要类模板基础（01_basics 10_templates 1.2）——基类是模板，子类把自身类型作为模板参数传入。`static_cast` 在编译期确定类型，没有 vptr / vtable，函数调用可以被内联。

局限：`AnimalCRTP<Dog>` 和 `AnimalCRTP<Cat>` 是不同类型，不能放进同一个数组用基类指针统一管理。适合性能敏感的库代码、策略模式、混入（mixin）。

```cpp
#include <iostream>

// --- 虚函数方式（运行时多态） ---
class AnimalVirtual
{
public:
    virtual ~AnimalVirtual() = default;
    virtual void speak() const = 0;
};

class DogVirtual : public AnimalVirtual
{
public:
    void speak() const override { std::cout << "[虚函数] Dog: 汪!\n"; }
};

// --- CRTP 方式（编译期多态） ---
template <typename Derived>
class AnimalCRTP
{
public:
    void speak() const
    {
        // static_cast 在编译期确定类型，直接调用，可以被内联
        static_cast<const Derived*>(this)->speak_impl();
    }
};

class DogCRTP : public AnimalCRTP<DogCRTP>
{
public:
    void speak_impl() const { std::cout << "[CRTP] Dog: 汪!\n"; }
};

// CRTP 使用方式：模板函数
template <typename T>
void make_speak(const AnimalCRTP<T>& animal)
{
    animal.speak();  // 编译期展开，无虚函数开销
}

int main()
{
    DogVirtual dog_v;
    dog_v.speak();

    DogCRTP dog_c;
    make_speak(dog_c);

    std::cout << "sizeof(DogVirtual) = " << sizeof(DogVirtual)
              << "（有 vptr）\n";
    std::cout << "sizeof(DogCRTP) = " << sizeof(DogCRTP)
              << "（无 vptr，零开销）\n";
}
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/polymorphism.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-mingw-ninja\polymorphism.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/polymorphism.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-mingw-make\polymorphism.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/polymorphism.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-mingw-mc\Release\polymorphism.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\polymorphism.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-msvc-ninja\polymorphism.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\polymorphism.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\04_polymorphism\build-msvc-vs\Release\polymorphism.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/polymorphism
```
