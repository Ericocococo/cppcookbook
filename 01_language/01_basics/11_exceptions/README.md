# 11_exceptions — 异常处理

`try`/`catch`/`throw`、标准异常层次、自定义异常、`noexcept`、RAII 异常安全。

## 1. 知识点

| 文件 | 说明 |
|---|---|
| `main.cpp` | 6 个 demo：基本 try/catch / 标准异常层次 / 自定义异常 / noexcept / RAII+异常 / catch 顺序 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `exceptions` |

### 1.1 基本的 try / catch / throw

`try` 块包裹可能抛出异常的代码，`throw` 抛出异常对象后函数立即终止，跳到最近匹配的 `catch` 块。

```cpp
#include <iostream>
#include <stdexcept>

double safeDivide(double a, double b) {
    if (b == 0.0) {
        throw std::runtime_error("除数不能为零");
    }
    return a / b;
}

try {
    double r = safeDivide(10.0, 0.0);   // throw 后直接跳到 catch
    std::cout << "这行不会执行\n";
}
catch (const std::exception& e) {
    std::cout << "异常: " << e.what() << "\n";  // "除数不能为零"
}
```

#### 1.1.1 捕获具体类型

多个 `catch` 块按从具体到通用的顺序排列。如果把 `std::exception` 放在前面，具体类型的 `catch` 永远匹配不到。

```cpp
try {
    throw std::runtime_error("运行时错误");
}
catch (const std::runtime_error& e) {
    // 先匹配具体类型（放在 exception 前面！）
    std::cout << "runtime_error: " << e.what() << "\n";
}
catch (const std::exception& e) {
    // 再匹配基类（兜底标准异常）
    std::cout << "exception: " << e.what() << "\n";
}
```

#### 1.1.2 捕获任何类型（兜底）

`catch(...)` 用省略号语法捕获所有类型的异常，包括非 `std::exception` 派生的类型。放在所有 `catch` 的最后作为兜底。

```cpp
try {
    throw std::string("非标准异常");  // string 不继承 std::exception
}
catch (const std::exception& e) {
    std::cout << "标准异常\n";
}
catch (...) {
    std::cout << "未知类型异常（非 std::exception）\n";  // 走这里
}
```

### 1.2 标准异常层次

C++ 标准库异常类形成一棵继承树，所有异常都继承自 `std::exception`，用 `.what()` 获取错误描述。

```
std::exception（基类）
├── std::logic_error（程序逻辑错误）
│   ├── invalid_argument（无效参数）
│   ├── out_of_range（越界）
│   ├── length_error（长度超限）
│   └── domain_error（定义域错误）
└── std::runtime_error（运行时错误）
    ├── overflow_error（上溢）
    ├── underflow_error（下溢）
    └── range_error（范围错误）
```

#### 1.2.1 常见标准异常演示

`std::vector::at()` 越界时抛 `out_of_range`，`std::invalid_argument` 用于参数不合法的场景。头文件 `<stdexcept>` 包含所有标准异常类。

```cpp
#include <stdexcept>
#include <vector>

// out_of_range：越界访问
std::vector<int> v = {1, 2, 3};
try {
    v.at(10);  // at() 越界时抛 out_of_range（operator[] 不会抛）
}
catch (const std::out_of_range& e) {
    std::cout << "out_of_range: " << e.what() << "\n";
}

// invalid_argument：参数不合法
try {
    throw std::invalid_argument("参数不合法");
}
catch (const std::logic_error& e) {
    // invalid_argument 继承自 logic_error，所以基类 catch 也能捕获
    std::cout << "logic_error: " << e.what() << "\n";
}
```

### 1.3 自定义异常类

#### 1.3.1 继承标准异常类

自定义异常类继承 `std::runtime_error`（或其他标准异常），可以携带额外信息（如错误码）。构造函数通过 `std::runtime_error(msg)` 传递错误描述，`.what()` 自动可用。

```cpp
#include <stdexcept>
#include <string>

class DatabaseError : public std::runtime_error {
    int code_;
public:
    DatabaseError(int code, const std::string& msg)
        : std::runtime_error(msg), code_(code) {}
    int code() const { return code_; }
};

try {
    throw DatabaseError(404, "表不存在: users");
}
catch (const DatabaseError& e) {
    std::cout << "DatabaseError[" << e.code() << "]: "
              << e.what() << "\n";
    // 输出：DatabaseError[404]: 表不存在: users
}
catch (const std::exception& e) {
    std::cout << "其他异常: " << e.what() << "\n";
}
```

### 1.4 noexcept：承诺函数不抛出异常

标记 `noexcept` 的函数承诺不抛出异常。如果实际抛出，程序调用 `std::terminate()` 直接终止（不会进入 `catch`）。

```cpp
int safeAdd(int a, int b) noexcept {
    return a + b;  // 确实不会抛出
}
```

#### 1.4.1 noexcept 的条件形式

`noexcept(条件)` 根据编译期条件决定是否标记 `noexcept`。常配合 `type_traits` 使用，让 `noexcept` 跟随类型特性。

```cpp
#include <type_traits>
#include <utility>   // std::move

template <typename T>
void swapSafe(T& a, T& b) noexcept(std::is_nothrow_move_constructible_v<T>) {
    // 如果 T 的移动构造不抛异常，swapSafe 也不抛
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

int a = 10, b = 20;
swapSafe(a, b);   // a=20, b=10
```

#### 1.4.2 noexcept 的作用

`noexcept` 有两个重要作用：让编译器去掉异常处理代码生成更小更快的二进制；让 `vector` 扩容时使用移动而非拷贝（移动构造必须标 `noexcept`，否则 `vector` 为了异常安全会退回拷贝）。

```cpp
class MyClass {
public:
    // 移动构造标 noexcept，vector 扩容时才会用移动（否则用拷贝）
    MyClass(MyClass&&) noexcept = default;

    // noexcept 函数如果真的抛出异常：std::terminate() 直接终止程序
};
```

### 1.5 异常安全与 RAII

#### 1.5.1 RAII 保证资源释放

RAII（Resource Acquisition Is Initialization）：在构造函数获取资源，在析构函数释放资源。即使抛出异常，C++ 也会自动调用已构造对象的析构函数（栈展开），保证资源不泄漏。

```cpp
#include <string>
#include <stdexcept>

class File {
    std::string name_;
    bool open_;
public:
    explicit File(const std::string& name) : name_(name), open_(true) {
        std::cout << "[File 打开] " << name_ << "\n";
    }
    ~File() {
        if (open_) {
            std::cout << "[File 关闭] " << name_ << "（RAII 保证关闭）\n";
            open_ = false;
        }
    }
};

try {
    File f("data.txt");                           // 构造时打开
    std::cout << "处理文件中...\n";
    throw std::runtime_error("处理失败");           // 抛出异常
}
catch (const std::exception& e) {
    // 虽然抛了异常，f 的析构函数已自动调用，文件已关闭
    std::cout << "捕获异常: " << e.what() << "\n";
}
```

#### 1.5.2 不用 RAII 的危险写法

手动 `new`/`delete` 管理资源时，异常路径中的 `delete` 会被跳过，导致内存泄漏。解决方案是用 `unique_ptr` / `shared_ptr` 代替裸指针。

```cpp
// 危险写法：异常路径跳过 delete
void dangerous() {
    int* p = new int(42);
    // ... 如果这里抛出异常，下面的 delete 不会执行
    delete p;  // 异常时永远不会到达这里
}

// 安全写法：用 unique_ptr，析构时自动释放
#include <memory>
void safe() {
    auto p = std::make_unique<int>(42);
    // ... 即使抛出异常，unique_ptr 析构时也会自动 delete
}
```

### 1.6 多层 catch 的顺序

#### 1.6.1 catch 从具体到通用排列

多个 `catch` 块必须从最具体的类型排到最通用的类型。如果基类 `catch` 放在前面，派生类 `catch` 永远不会被匹配到。`catch(...)` 放在最后兜底。

```cpp
for (int i = 0; i < 3; ++i) {
    try {
        if (i == 0) { throw std::out_of_range("越界"); }
        if (i == 1) { throw std::runtime_error("运行时错误"); }
        if (i == 2) { throw std::string("非标准异常"); }
    }
    catch (const std::out_of_range& e) {
        // 最具体的类型放最前面
        std::cout << "[" << i << "] out_of_range: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        // 基类 catch 放后面
        std::cout << "[" << i << "] exception: " << e.what() << "\n";
    }
    catch (...) {
        // 兜底：捕获所有非 std::exception 的异常
        std::cout << "[" << i << "] 未知类型异常\n";
    }
}
// 输出：
// [0] out_of_range: 越界
// [1] exception: 运行时错误
// [2] 未知类型异常
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/exceptions.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-ninja\exceptions.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/exceptions.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-make\exceptions.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/exceptions.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-mc\Release\exceptions.exe
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
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\exceptions.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-msvc-ninja\exceptions.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\exceptions.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-msvc-vs\Release\exceptions.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/exceptions
```

## 3. 英文及缩写说明

| 词汇                      | 说明                                                                                       |
|-------------------------|------------------------------------------------------------------------------------------|
| `try`                   | 尝试：标记可能抛出异常的代码块                                                                          |
| `catch`                 | 捕获：捕获 try 块中抛出的指定类型异常；多个 catch 从具体到通用排列                                                  |
| `throw`                 | 抛出：主动抛出异常对象，函数立即终止跳到最近的 catch；`throw;`（无参）重新抛出当前异常                                       |
| `noexcept`              | no exception：承诺函数不抛出异常；若实际抛出则 `std::terminate()` 直接终止；移动构造标 noexcept，vector 扩容时才会用移动而非拷贝 |
| `std::exception`        | 所有标准异常的基类；`.what()` 返回错误描述字符串（`const char*`）；需要 `#include <stdexcept>`                   |
| `std::runtime_error`    | runtime error：运行时错误，程序运行时才能检测到                                                           |
| `std::logic_error`      | logic error：逻辑错误，程序设计问题，理论上在开发期可避免                                                       |
| `std::out_of_range`     | out of range：越界访问；`std::vector::at()` 越界时抛出此异常                                           |
| `std::invalid_argument` | invalid argument：传入了不合法的参数                                                               |
| `catch(...)`            | 捕获任意类型异常（省略号语法），作为最后的兜底 catch                                                            |
| stack unwinding         | 栈展开：异常抛出后，C++ 运行时自动销毁 try 块内已构造的局部对象（调用析构函数），保证资源释放                                      |
| RAII                    | Resource Acquisition Is Initialization：资源获取即初始化，析构时自动释放；即使抛出异常析构也会执行，是异常安全的基础            |
| exception safety        | 异常安全：代码在异常路径下不泄漏资源、不留下不一致状态的能力                                                           |
| `<stdexcept>`           | standard exceptions：包含 `runtime_error`、`logic_error`、`out_of_range`、`invalid_argument` 等 |
