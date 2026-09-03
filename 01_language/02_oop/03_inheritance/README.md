# 03_inheritance — 继承深入 — 多继承、菱形、virtual 继承、final

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 演示代码 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `inheritance` |

---

## 知识点详解

### 1.1 多继承（multiple inheritance）

#### 1.1.1 多继承

一个类可以同时继承多个父类，获得所有父类的成员。风险：如果多个父类有同名函数，调用时编译器无法决定用哪个，报 `ambiguous` 错误，必须用 `类名::函数名` 手动消歧义。

```cpp
#include <iostream>

class LandAnimal
{
public:
    void move()    { std::cout << "在陆地上奔跑\n"; }
    void breathe() { std::cout << "用肺呼吸\n"; }
};

class WaterAnimal
{
public:
    void swim()    { std::cout << "在水中游泳\n"; }
    void breathe() { std::cout << "用鳃呼吸\n"; }  // 与 LandAnimal 同名
};

// 同时继承两个父类
class Amphibian : public LandAnimal, public WaterAnimal
{
public:
    void show()
    {
        move();   // 只在 LandAnimal 中，不歧义
        swim();   // 只在 WaterAnimal 中，不歧义

        // breathe();  // 编译错误：ambiguous，两个父类都有
        LandAnimal::breathe();   // 用 类名:: 消歧义
        WaterAnimal::breathe();
    }
};

int main()
{
    Amphibian frog;
    frog.show();
    // frog.breathe();            // 编译错误
    frog.LandAnimal::breathe();   // 指定调哪个版本
}
```

### 1.2 菱形继承问题（diamond problem）

#### 1.2.1 菱形继承问题

A 是顶层基类，B 和 C 各自继承 A，D 同时继承 B 和 C。结果：D 对象内部有两份 A 的数据副本（一份来自 B，一份来自 C）。访问 A 的成员时编译器不知道该用哪一份，报歧义错误。两份数据互相独立，修改一份不影响另一份，这通常不是想要的行为。

```cpp
#include <iostream>
#include <string>

class Animal
{
public:
    std::string name = "动物";
};

// 普通继承（非 virtual）
class Bird : public Animal
{
public:
    Bird() { name = "鸟类"; }
};

class Horse : public Animal
{
public:
    Horse() { name = "马类"; }
};

// Pegasus 内部有两份 Animal
class Pegasus : public Bird, public Horse {};

int main()
{
    Pegasus p;
    // p.name;  // 编译错误：ambiguous，两份 Animal 都有 name

    // 必须指定走哪条路径
    std::cout << p.Bird::name << "\n";   // 输出: 鸟类
    std::cout << p.Horse::name << "\n";  // 输出: 马类

    // 两份是独立的，修改一份不影响另一份
    p.Bird::name = "飞马（鸟面）";
    p.Horse::name = "飞马（马面）";
}
```

### 1.3 virtual 继承

#### 1.3.1 virtual 继承

`class B : virtual public A` 告诉编译器：无论 A 被继承多少次，在最终派生类中只保留一份 A 的数据。代价是每个 virtual 继承路径多一个指针（vbptr），且构造函数链变复杂：最终派生类必须直接调用虚基类 A 的构造函数，中间类对 A 构造函数的调用会被编译器跳过。

```cpp
#include <iostream>
#include <string>

class Animal
{
public:
    std::string name;

    explicit Animal(std::string n = "动物") : name(std::move(n))
    {
        std::cout << "Animal 构造: " << name << "\n";
    }
};

// virtual public：声明虚继承
class Bird : virtual public Animal
{
public:
    // Bird 传给 Animal 的构造参数只在 Bird 是最终类时生效
    Bird() : Animal("鸟类") { std::cout << "Bird 构造\n"; }
};

class Horse : virtual public Animal
{
public:
    Horse() : Animal("马类") { std::cout << "Horse 构造\n"; }
};

// 最终派生类必须直接调用虚基类 Animal 的构造函数
class Pegasus : public Bird, public Horse
{
public:
    // Animal("飞马") 是唯一生效的 Animal 构造调用
    // Bird() 和 Horse() 中对 Animal 的调用被编译器跳过
    Pegasus() : Animal("飞马"), Bird(), Horse()
    {
        std::cout << "Pegasus 构造\n";
    }
};

int main()
{
    Pegasus p;
    // 只有一份 Animal，不再歧义
    std::cout << p.name << "\n";     // 输出: 飞马

    p.name = "天马";
    p.Bird::name;   // 和 p.Horse::name 是同一份
}
```

### 1.4 final 关键字

#### 1.4.1 final 的两种用法

`final` 有两种用法：final 类（`class X final {}`）禁止任何类继承它；final 虚函数（`void foo() override final`）禁止子类覆盖该函数。当确定不需要再派生或覆盖时加 final，编译器可以做去虚拟化优化（devirtualize），把虚函数调用变成普通调用，提升性能。

```cpp
#include <iostream>

class BaseWidget
{
public:
    virtual ~BaseWidget() = default;
    virtual void draw()   { std::cout << "BaseWidget::draw\n"; }
    virtual void resize() { std::cout << "BaseWidget::resize\n"; }
};

class Button : public BaseWidget
{
public:
    // override final：实现了 draw，并且禁止子类再覆盖
    void draw() override final
    {
        std::cout << "Button::draw（final，子类不能再覆盖）\n";
    }

    void resize() override
    {
        std::cout << "Button::resize\n";
    }
};

// final 类：禁止任何类继承 IconButton
class IconButton final : public Button
{
public:
    // void draw() override {}  // 编译错误：draw 被 Button 标记为 final
    void resize() override      // resize 没有 final，可以覆盖
    {
        std::cout << "IconButton::resize\n";
    }
};

// class FancyIconButton : public IconButton {};  // 编译错误：IconButton 是 final 类

int main()
{
    IconButton btn;
    btn.draw();    // Button::draw（final 版本）
    btn.resize();  // IconButton::resize

    BaseWidget* ptr = &btn;
    ptr->draw();   // 运行时仍走 Button::draw
}
```

### 1.5 隐藏（hiding）vs 覆盖（overriding）

#### 1.5.1 隐藏（name hiding）

子类定义了与父类同名的非虚函数，父类的所有同名重载都被"藏起来"。注意不是只隐藏签名相同的那个，而是隐藏所有名为该函数名的重载。通过子类对象调用时始终调子类版本，这不是多态，是编译期的名字查找规则。

#### 1.5.2 覆盖（overriding）

子类用 `override` 覆盖父类的虚函数。通过父类指针/引用调用时，运行时根据对象实际类型决定调哪个版本，这才是真正的多态。

#### 1.5.3 using 声明恢复被隐藏的函数

子类中写 `using Base::func;` 可以把被隐藏的父类函数"拉回"子类作用域，让父类的其他重载仍然可用。

```cpp
#include <iostream>

class Printer
{
public:
    void print(int x)    { std::cout << "Printer::print(int) " << x << "\n"; }
    void print(double x) { std::cout << "Printer::print(double) " << x << "\n"; }

    virtual void describe() { std::cout << "Printer::describe\n"; }
    virtual ~Printer() = default;
};

class ColorPrinter : public Printer
{
public:
    // 定义了 print(int) → 隐藏了父类所有 print 重载（包括 print(double)）
    void print(int x)
    {
        std::cout << "ColorPrinter::print(int) " << x << "\n";
    }

    void describe() override  // 覆盖虚函数 → 真正的多态
    {
        std::cout << "ColorPrinter::describe\n";
    }
};

class AdvancedPrinter : public Printer
{
public:
    using Printer::print;  // 用 using 把父类的 print 拉回来

    void print(int x)  // 子类版本覆盖 int 版本
    {
        std::cout << "AdvancedPrinter::print(int) " << x << "\n";
    }
    // print(double) 因为 using 声明，仍然可用
};

int main()
{
    ColorPrinter cp;
    cp.print(42);         // ColorPrinter::print(int)
    // cp.print(3.14);    // 编译错误：print(double) 被隐藏了
    cp.Printer::print(3.14);  // 显式指定调父类版本

    // 隐藏 vs 覆盖的区别
    Printer* ptr = &cp;
    ptr->print(42);       // Printer::print(int)  → 非虚函数看指针类型
    ptr->describe();      // ColorPrinter::describe → 虚函数看实际类型（多态）

    // using 声明恢复被隐藏的函数
    AdvancedPrinter ap;
    ap.print(42);    // AdvancedPrinter::print(int)
    ap.print(3.14);  // Printer::print(double)（using 拉回来的）
}
```

### 1.6 继承中的访问控制

#### 1.6.1 三种继承方式的访问控制

- `public` 继承：父类 public -> public，protected -> protected。最常用，表达"is-a"关系。
- `protected` 继承：父类 public -> protected，protected -> protected。外部不能通过子类对象访问，但子类和孙子类内部可以。
- `private` 继承：父类 public/protected -> 全变 private。外部和孙子类都不能访问。表达"用 A 实现 B"的关系（has-a 的替代写法）。

无论哪种继承方式，父类的 private 成员子类永远不能直接访问。

#### 1.6.2 private 继承

private 继承把父类的 public/protected 成员在子类中变成 private。外部不能通过子类对象调用父类方法，只能通过子类自己的公开接口间接使用。如果想选择性暴露某个方法，可以用 `using Base::method;`。

```cpp
#include <iostream>

class Engine
{
public:
    void start() { std::cout << "Engine::start\n"; }
    void stop()  { std::cout << "Engine::stop\n"; }

protected:
    int rpm_ = 0;
};

// public 继承：外部可以通过 Car 调用 Engine 的 public 方法
class Car : public Engine
{
public:
    void drive()
    {
        start();      // public -> public
        rpm_ = 3000;  // protected -> protected
    }
};

// private 继承：外部不能通过 Robot 调用 start()/stop()
class Robot : private Engine
{
public:
    void operate()
    {
        start();      // 类内部可以访问
        rpm_ = 5000;
    }
};

int main()
{
    Car car;
    car.start();    // 可以：public 继承保持 public

    Robot robot;
    robot.operate();
    // robot.start();  // 编译错误：start() 在 Robot 中是 private
}
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/inheritance.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-mingw-ninja\inheritance.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/inheritance.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-mingw-make\inheritance.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/inheritance.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-mingw-mc\Release\inheritance.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\inheritance.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-msvc-ninja\inheritance.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\inheritance.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\03_inheritance\build-msvc-vs\Release\inheritance.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/inheritance
```
