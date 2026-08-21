# 07_oop — 面向对象

演示 `struct`、`class` 封装（构造/析构/访问控制）、继承、多态（虚函数）。

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 4 个 demo 函数：struct → class封装 → 继承 → 多态 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `oop` |

---

## 2. 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"
```

### 方案 A：Ninja（推荐）

```bash
# 配置
"$CMAKE" -B build-mingw-ninja -G Ninja \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA"

# 构建
"$CMAKE" --build build-mingw-ninja

# 运行
./build-mingw-ninja/oop.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 运行
build-mingw-ninja\oop.exe
```

---

## 3. 命令行 · MSVC（cmd）

### 方案 A：vcvarsall + Ninja（推荐）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

call "%VCVARSALL%" x64

"%CMAKE%" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

"%CMAKE%" --build build-msvc-ninja

build-msvc-ninja\oop.exe
```

> 四步版：

```bat
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja

:: 运行
build-msvc-ninja\oop.exe
```

---

## 4. CLion IDE

1. `File → Open` 选择 `07_oop/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. **构建** `Ctrl+F9`　**运行** `Shift+F10`

---

## 5. 英文及缩写说明

### 关键字

| 词汇 | 说明 |
|------|------|
| `struct` | Structure：结构体，默认成员是 `public` |
| `class` | 类，默认成员是 `private`；通常用于有封装逻辑的对象 |
| `public` | 公有：任何地方都能访问 |
| `private` | 私有：只有类内部可访问，外部和子类均不能直接访问 |
| `protected` | 受保护：类内部和子类可访问，类外部不能 |
| `virtual` | 虚函数：允许子类覆盖，实现运行时多态；有子类时析构函数必须是 virtual |
| `override` | 覆盖（C++11）：明确标注覆盖父类虚函数；父类无此虚函数时编译报错 |
| `final` | 终止（C++11）：标在类上禁止继承；标在函数上禁止子类继续覆盖 |
| `explicit` | 显式：禁止构造函数被隐式调用 |
| `static`（成员）| 静态成员：属于类本身而非某个对象，所有对象共享 |

### 概念

| 概念 | 说明 |
|------|------|
| OOP | Object-Oriented Programming：面向对象编程，三大特性：封装、继承、多态 |
| encapsulation | 封装：数据和操作放在一起，并控制外部访问权限 |
| inheritance | 继承：子类继承父类的成员，`class Child : public Parent` |
| polymorphism | 多态：父类指针指向子类对象，调用虚函数时自动执行子类版本 |
| constructor | 构造函数：对象创建时自动调用，名字与类名相同 |
| destructor | 析构函数：对象销毁时自动调用，名字是 `~类名` |
| initializer list | 初始化列表：构造函数 `:` 后的 `member(value)` 语法，比函数体内赋值更高效 |
| pure virtual function | 纯虚函数：`virtual type func() = 0`，强制子类实现 |
| abstract class | 抽象类：含纯虚函数的类，不能直接创建对象 |
| vtable | Virtual Table：虚函数表，编译器为含虚函数的类生成的函数指针表 |
| virtual destructor | 虚析构函数：`delete 基类指针` 时正确调用子类析构函数，防止资源泄漏 |
| `<cmath>` | C math：包含 `sqrt`、`sin`、`cos`、`pow` 等数学函数 |
