# 01_classes_objects — 类深入 — 友元、嵌套类、this、mutable、static

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 演示代码 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `classes_objects` |

---

## 知识点详解

### 1.1 友元函数（friend function）

#### 1.1.1 友元函数

友元函数不是类的成员函数，但被授权访问类的 private / protected 成员。典型用途是重载 `operator<<` 输出运算符——左操作数是 `std::ostream`，不属于我们的类，所以不能写成成员函数。友元只是"授权"，调用方式和普通函数一样，不通过对象调用。

```cpp
#include <iostream>

class Temperature
{
private:
    double celsius_;

public:
    explicit Temperature(double c) : celsius_(c) {}

    // 声明友元函数：允许这个普通函数访问 private 成员
    // friend 只是"授权"，不是成员函数声明
    friend std::ostream& operator<<(std::ostream& os, const Temperature& t);

    // 友元也可以不是运算符，普通函数也行
    friend double to_fahrenheit(const Temperature& t);
};

// 友元函数的实现写在类外面，不需要 Temperature:: 前缀
// 因为它不是成员函数
std::ostream& operator<<(std::ostream& os, const Temperature& t)
{
    os << t.celsius_ << " C";  // 直接访问 private 成员 celsius_
    return os;
}

double to_fahrenheit(const Temperature& t)
{
    return t.celsius_ * 9.0 / 5.0 + 32.0;  // 直接访问 private 成员
}

int main()
{
    Temperature t(100.0);
    std::cout << t << "\n";              // 输出: 100 C
    double f = to_fahrenheit(t);         // f = 212.0
    std::cout << f << " F\n";
    // t.to_fahrenheit();  // 编译错误！友元不是成员函数，不能通过对象调用
}
```

### 1.2 友元类（friend class）

#### 1.2.1 友元类

一个类可以把另一个类声明为友元，友元类的所有成员函数都能访问该类的 private 成员。友元关系的三个特性：

| 特性 | 含义 |
|------|------|
| 单向 | A 声明 B 为友元，B 不能反过来自动访问 A 的 private |
| 不可传递 | A 是 B 的友元、B 是 C 的友元，不代表 A 是 C 的友元 |
| 不可继承 | A 是 B 的友元，不代表 A 的子类也是 B 的友元 |

```cpp
#include <iostream>
#include <string>

class Car
{
private:
    std::string model_;
    int horsepower_;

    // 声明友元类：Engine 的所有成员函数都能访问 Car 的 private 成员
    friend class Engine;

public:
    Car(std::string model, int hp) : model_(std::move(model)), horsepower_(hp) {}

    void print() const
    {
        std::cout << "Car: " << model_ << ", " << horsepower_ << " hp\n";
    }
};

class Engine
{
public:
    // Engine 是 Car 的友元，可以直接读写 Car 的 private 成员
    void tune(Car& car, int extra_hp) const
    {
        car.horsepower_ += extra_hp;  // 直接修改 private 成员
    }
};

int main()
{
    Car car("Tesla Model S", 670);
    Engine engine;
    engine.tune(car, 100);
    car.print();  // 输出: Car: Tesla Model S, 770 hp
}
```

### 1.3 嵌套类（nested class）

#### 1.3.1 嵌套类

在类内部定义的类，用于封装"只给外部类使用"的实现细节。C++11 起嵌套类可以访问外部类的 private 成员。外部类不能自动访问嵌套类的 private 成员。定义在 private 区域的嵌套类，外部代码无法直接使用，只能通过外部类的公开接口间接操作。

```cpp
#include <iostream>

class LinkedList
{
private:
    // 嵌套类 Node 定义在 private 区域，外部代码无法直接使用
    struct Node
    {
        int data;
        Node* next;
        Node(int val, Node* nxt) : data(val), next(nxt) {}
    };

    Node* head_ = nullptr;
    size_t size_ = 0;

public:
    ~LinkedList()
    {
        Node* curr = head_;
        while (curr)
        {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
    }

    void push_front(int val)
    {
        head_ = new Node(val, head_);
        ++size_;
    }

    void print() const
    {
        Node* curr = head_;
        while (curr)
        {
            std::cout << curr->data;
            if (curr->next)
            {
                std::cout << " -> ";
            }
            curr = curr->next;
        }
        std::cout << "\n";
    }
};

int main()
{
    LinkedList list;
    list.push_front(30);
    list.push_front(20);
    list.push_front(10);
    list.print();  // 输出: 10 -> 20 -> 30
    // LinkedList::Node n(42, nullptr);  // 编译错误：Node 是 private 的
}
```

### 1.4 this 指针

#### 1.4.1 this 指针

`this` 是编译器自动传入的隐式参数，指向"调用该成员函数的那个对象"。类型为 `T*`（非 const 成员函数）或 `const T*`（const 成员函数）。常见用途：消除成员与参数同名的歧义、链式调用（`return *this`）。

#### 1.4.2 链式调用

每个 setter 返回 `*this`（当前对象的引用），调用方可以连续调用多个方法。返回类型必须是引用 `Builder&`，如果返回 `Builder` 值类型会发生拷贝，后续调用修改的是副本而非原对象。

```cpp
#include <iostream>
#include <string>

class Builder
{
    std::string name_ = "default";
    int width_ = 0;
    int height_ = 0;
    std::string color_ = "white";

public:
    // 返回 *this 实现链式调用
    Builder& setName(const std::string& name)
    {
        name_ = name;
        return *this;
    }

    Builder& setSize(int w, int h)
    {
        width_ = w;
        height_ = h;
        return *this;
    }

    Builder& setColor(const std::string& color)
    {
        color_ = color;
        return *this;
    }

    // const 成员函数：this 的类型变成 const Builder*，不能修改成员
    void print() const
    {
        std::cout << name_ << " " << width_ << "x" << height_
                  << " " << color_ << "\n";
    }
};

int main()
{
    Builder b;
    b.setName("Window").setSize(800, 600).setColor("blue");
    b.print();  // 输出: Window 800x600 blue
}
```

### 1.5 mutable 关键字

#### 1.5.1 mutable 关键字

const 成员函数中，所有成员变量默认不可修改。`mutable` 标记的成员例外——即使在 const 函数中也允许修改。适用场景：缓存计算结果、访问计数器、互斥锁（`std::mutex` 在 const 函数中需要 lock）。`mutable` 成员是"观测行为的副作用"，不影响对象的逻辑状态。

```cpp
#include <iostream>
#include <vector>

class DataStore
{
    std::vector<int> data_;
    mutable int access_count_ = 0;        // const 函数中也允许修改
    mutable bool cache_valid_ = false;
    mutable double cached_average_ = 0.0;

public:
    explicit DataStore(std::vector<int> data)
        : data_(std::move(data)) {}

    void addValue(int val)
    {
        data_.push_back(val);
        cache_valid_ = false;  // 数据变了，缓存失效
    }

    // const 函数：承诺不修改对象的逻辑状态
    // 但 mutable 成员（access_count_, cached_average_）允许修改
    double average() const
    {
        ++access_count_;  // mutable：const 函数中可以修改

        if (cache_valid_)
        {
            return cached_average_;
        }

        if (data_.empty())
        {
            return 0.0;
        }

        double sum = 0.0;
        for (int val : data_)
        {
            sum += val;
        }
        cached_average_ = sum / data_.size();
        cache_valid_ = true;
        return cached_average_;
    }

    int accessCount() const { return access_count_; }
};

int main()
{
    DataStore ds({10, 20, 30, 40, 50});
    const DataStore& ref = ds;
    double avg1 = ref.average();  // 第一次：计算并缓存
    double avg2 = ref.average();  // 第二次：命中缓存
    std::cout << "avg=" << avg2 << " access=" << ref.accessCount() << "\n";
}
```

### 1.6 static 成员深入

#### 1.6.1 static 成员变量与函数

static 成员变量属于类本身，所有对象共享同一份，必须在类外初始化（否则链接错误）。C++17 的 `inline` 关键字允许在类内直接初始化。static 成员函数没有 `this` 指针，不能访问非 static 成员，通过 `类名::` 调用，不需要对象。典型用途：对象计数、工厂方法。

```cpp
#include <iostream>
#include <string>

class Connection
{
    int id_;
    std::string host_;

    static int total_count_;              // 类外初始化
    static int alive_count_;              // 类外初始化
    static inline int next_id_ = 0;      // C++17 内联静态，可在类内初始化

public:
    explicit Connection(std::string host)
        : id_(++next_id_), host_(std::move(host))
    {
        ++total_count_;
        ++alive_count_;
    }

    ~Connection() { --alive_count_; }

    // static 成员函数：没有 this，只能访问 static 成员
    static int aliveCount() { return alive_count_; }
    static int totalCount() { return total_count_; }

    // 工厂方法：static 函数创建对象，封装构造逻辑
    static Connection* create(const std::string& host)
    {
        return new Connection(host);
    }
};

// static 成员变量必须在类外初始化，不写会链接错误
int Connection::total_count_ = 0;
int Connection::alive_count_ = 0;

int main()
{
    Connection c1("192.168.1.1");
    Connection c2("192.168.1.2");
    std::cout << "alive=" << Connection::aliveCount() << "\n";  // 输出: 2
}
```

#### 1.6.2 Meyers' Singleton

利用局部 static 变量实现线程安全的单例模式。构造函数设为 private 阻止外部创建对象，拷贝和移动用 `= delete` 禁止——`= delete` 声明一个成员函数不可调用，任何调用它的地方都会编译报错（这里用它禁止复制，保证 `Logger` 不会出现第二份）。`= delete` 的完整讲解在 06_special_members 1.4。C++11 保证局部 static 变量初始化的线程安全。

```cpp
#include <iostream>
#include <string>

class Logger
{
    Logger() {}                        // private 构造：外部不能创建
    Logger(const Logger&) = delete;    // 禁止拷贝
    Logger& operator=(const Logger&) = delete;

public:
    static Logger& instance()
    {
        static Logger inst;  // 第一次调用时构造，程序结束时析构
        return inst;         // C++11 保证线程安全
    }

    void log(const std::string& msg) const
    {
        std::cout << "[LOG] " << msg << "\n";
    }
};

int main()
{
    Logger& log1 = Logger::instance();
    Logger& log2 = Logger::instance();  // 返回同一个实例
    log1.log("hello");
    std::cout << (&log1 == &log2 ? "same" : "different") << "\n";  // 输出: same
}
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/classes_objects.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-mingw-ninja\classes_objects.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/classes_objects.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-mingw-make\classes_objects.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/classes_objects.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-mingw-mc\Release\classes_objects.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\classes_objects.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-msvc-ninja\classes_objects.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\classes_objects.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\01_classes_objects\build-msvc-vs\Release\classes_objects.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/classes_objects
```
