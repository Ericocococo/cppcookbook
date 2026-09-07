# 10_templates — 模板

函数模板、类模板、全特化、非类型模板参数、Concepts（C++20）、`if constexpr`。

## 1. 知识点

| 文件 | 说明 |
|---|---|
| `main.cpp` | 6 个 demo：函数模板推断 / 类模板（Stack）/ 全特化 / 非类型参数 / Concepts / if constexpr |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `templates` |

### 1.1 函数模板：一份代码适用多种类型

函数模板用 `template <typename T>` 定义，编译器根据调用时的实参类型自动生成对应的函数版本，零运行时开销。

```cpp
template <typename T>
T maxVal(T a, T b) {
    return (a > b) ? a : b;
}

std::cout << maxVal(3, 5);         // T=int，输出 5
std::cout << maxVal(3.14, 2.71);   // T=double，输出 3.14
std::cout << maxVal('a', 'z');     // T=char，输出 z
```

#### 1.1.1 编译器自动推断 T

调用函数模板时，编译器根据实参类型自动推断 `T`，不需要手动指定。每次调用只要实参类型一致，编译器就能推断成功。

```cpp
template <typename T>
T maxVal(T a, T b) {
    return (a > b) ? a : b;
}

maxVal(3, 5);         // 自动推断 T=int
maxVal(3.14, 2.71);   // 自动推断 T=double
// maxVal(3, 5.0);    // 编译错误：T 推断矛盾（int vs double）
```

#### 1.1.2 多个类型参数

函数模板可以有多个类型参数，用逗号分隔。编译器分别推断每个类型参数，互不干扰，两个实参类型完全不同也能匹配。

```cpp
template <typename T1, typename T2>
void printPair(const T1& a, const T2& b) {
    std::cout << "(" << a << ", " << b << ")\n";
}

printPair(1, 3.14);                       // T1=int, T2=double
printPair(std::string("hello"), 42);      // T1=string, T2=int
```

#### 1.1.3 显式指定类型

当需要强制指定类型（例如两个实参类型不同需要统一）时，在函数名后用尖括号写明类型。

```cpp
std::cout << maxVal<double>(3, 5);  // 显式指定 T=double，3 隐式转为 3.0
```

#### 1.1.4 类型推断失败：实参类型不一致怎么办

模板只有一个类型参数 `T` 时，两个实参必须是同一个类型（或能隐式转换到同一个 `T`）；实参类型不一致时，编译器无法唯一确定 `T`，直接报错。

```cpp
// maxVal(3, 5.0);   // 编译错误！T 推断矛盾：3 是 int，5.0 是 double
// 编译器不知道该生成 maxVal<int> 还是 maxVal<double>
```

两种解决思路，取决于真实意图：

1. **两个值本来该是同一个类型**（如 3 和 5.0 表示同一种数量）——显式指定一个类型，另一个自动转换：`maxVal<double>(3, 5.0)`，输出 5
2. **两个值本来就是不同类型**（如一对坐标、名字+年龄）——模板本身就该用两个类型参数，`T` 一个不够用：定义成 `template <typename T1, typename T2>`（见 1.1.2）

### 1.2 类模板

类模板让一个类可以处理任意类型。使用时用 `Stack<int>` 或 `Stack<string>` 指定类型参数，编译器生成对应版本。

```cpp
#include <vector>
#include <stdexcept>

template <typename T>
class Stack {
    std::vector<T> data_;
public:
    void push(const T& val) { data_.push_back(val); }

    T pop() {
        if (data_.empty()) {
            throw std::runtime_error("栈为空");
        }
        T val = data_.back();
        data_.pop_back();
        return val;
    }

    const T& top() const {
        if (data_.empty()) {
            throw std::runtime_error("栈为空");
        }
        return data_.back();
    }

    bool empty() const { return data_.empty(); }
    size_t size() const { return data_.size(); }
};

Stack<int> si;
si.push(1);
si.push(2);
si.push(3);
std::cout << si.top();   // 3
std::cout << si.pop();   // 3，剩余 2 个

Stack<std::string> ss;
ss.push("hello");
ss.push("world");
std::cout << ss.top();   // "world"
```

#### 1.2.1 C++17 类模板参数推断（CTAD）

C++17 引入 CTAD（Class Template Argument Deduction），可以像函数模板一样从构造函数参数推断类型，不需要显式写 `<int>`。标准库容器已支持，自定义类需要提供推断规则（deduction guide）。

```cpp
std::vector v = {1, 2, 3};   // CTAD 推断为 vector<int>
std::pair p = {1, 3.14};     // CTAD 推断为 pair<int, double>
```

### 1.3 全特化：为特定类型提供完全不同的实现

#### 1.3.1 函数模板全特化

全特化用 `template<>` 为某个具体类型提供完全不同的实现。编译器优先匹配特化版本，找不到才用通用版本。

```cpp
#include <string>

// 通用版本
template <typename T>
std::string describe(T) { return "未知类型"; }

// 全特化：T = bool
template <>
std::string describe<bool>(bool val) {
    return val ? "布尔值:真" : "布尔值:假";
}

// 全特化：T = std::string
template <>
std::string describe<std::string>(std::string val) {
    return "字符串:\"" + val + "\"  长度=" + std::to_string(val.size());
}

std::cout << describe(42);                     // "未知类型"（走通用版本）
std::cout << describe(true);                   // "布尔值:真"（走 bool 特化）
std::cout << describe(std::string("hi"));      // 字符串:"hi"  长度=2
```

### 1.4 非类型模板参数：用编译期常量作为模板参数

#### 1.4.1 编译期常量作模板参数

除了类型参数 `typename T`，模板参数还可以是编译期常量（如 `size_t N`）。数组大小在编译期确定，不需要动态内存分配，比 `vector` 更高效。

```cpp
template <typename T, size_t N>
class FixedArray {
    T data_[N];
public:
    FixedArray() : data_{} {}
    T& operator[](size_t i) { return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }
    size_t size() const { return N; }
};

FixedArray<int, 5> arr;                         // 编译期确定大小为 5
for (size_t i = 0; i < arr.size(); ++i) {
    arr[i] = static_cast<int>(i) * 10;
}
// arr 内容：0 10 20 30 40
// 大小在编译期确定，比 vector 更高效（无动态内存分配）
```

### 1.5 Concepts（C++20）：约束模板参数

#### 1.5.1 requires 约束模板参数

C++20 的 `requires` 关键字指定模板类型必须满足的条件。不满足时编译器给出清晰的错误信息，而不是传统模板那种难懂的报错。

```cpp
#include <type_traits>

template <typename T>
    requires std::is_arithmetic_v<T>    // T 必须是算术类型（int/float 等）
T average(T a, T b) {
    return (a + b) / 2;
}

std::cout << average(3, 5);       // 4（int 整除）
std::cout << average(1.5, 2.5);   // 2.0
// average("hi", "lo");           // 编译错误：string 不满足 is_arithmetic_v
```

### 1.6 if constexpr（C++17）：编译期条件分支

#### 1.6.1 if constexpr + 类型特性

`if constexpr` 在编译期求值条件，条件为 `false` 的分支直接丢弃，不生成代码也不需要能编译。普通 `if` 两个分支都必须能编译通过。配合 `<type_traits>` 可以让不同类型走不同的代码路径。

```cpp
#include <type_traits>
#include <string>

template <typename T>
void process(T val) {
    if constexpr (std::is_integral_v<T>) {
        // 只有 T 是整数类型时才编译这段代码
        std::cout << "整数，低8位=0x"
                  << std::hex << (val & 0xFF) << std::dec << "\n";
    }
    else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "浮点，取整=" << static_cast<long long>(val) << "\n";
    }
    else {
        std::cout << "其他类型: " << val << "\n";
    }
}

process(255);                       // "整数，低8位=0xff"
process(3.14);                      // "浮点，取整=3"
process(std::string("hello"));      // "其他类型: hello"
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/templates.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-ninja\templates.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/templates.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-make\templates.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/templates.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-mc\Release\templates.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\templates.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-msvc-ninja\templates.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\templates.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-msvc-vs\Release\templates.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/templates
```

## 3. 英文及缩写说明

| 词汇                          | 说明                                                       |
|-----------------------------|----------------------------------------------------------|
| `template`                  | 模板：定义代码的"模具"，让函数/类对任意类型通用；编译器根据实际类型生成对应代码，零运行时开销         |
| `typename`                  | 类型名：模板参数列表中的占位符关键字；与 `class` 在此处完全等价                     |
| type parameter              | 类型参数：模板中的 T 等占位符，调用时由编译器自动推断或显式指定                        |
| template instantiation      | 模板实例化：编译器根据实际类型参数生成具体函数/类，发生在编译期                         |
| full specialization         | 全特化：`template<>` 为某个具体类型提供完全不同的实现，覆盖通用版本                 |
| non-type template parameter | 非类型模板参数：用编译期常量（如 `size_t N`）作为模板参数                       |
| `requires`                  | Concepts（C++20）约束关键字：指定模板类型必须满足的条件；不满足则编译报错（而不是难懂的模板错误）  |
| `std::is_arithmetic_v<T>`   | T 是算术类型（int/float 等）时为 true；来自 `<type_traits>`           |
| `if constexpr`              | 编译期条件（C++17）：条件为 false 的分支在编译期丢弃；普通 if 两个分支都要能编译         |
| TMP                         | Template Metaprogramming：模板元编程，利用模板在编译期执行计算              |
| CTAD                        | Class Template Argument Deduction：类模板参数推断（C++17），可省略模板参数 |
