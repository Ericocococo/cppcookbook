# 08_oop — 面向对象

演示 `struct`、`class` 封装（构造/析构/访问控制）、继承、多态（虚函数）。

## 1. 文件

| 文件               | 说明                                     |
|------------------|----------------------------------------|
| `main.cpp`       | 4 个 demo 函数：struct → class封装 → 继承 → 多态 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `oop`                     |

---

## 2. 命令行 · MinGW（Git Bash）

### 方案 A — Ninja

```bash
cmake -G Ninja -B build-mingw-ninja -S .
cmake --build build-mingw-ninja
./build-mingw-ninja/oop.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-ninja\oop.exe
```

</details>

### 方案 B — MinGW Makefiles

```bash
cmake -G "MinGW Makefiles" -B build-mingw-make -S .
cmake --build build-mingw-make
./build-mingw-make/oop.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-make
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-make\oop.exe
```

</details>

### 方案 C — Ninja Multi-Config

```bash
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .
cmake --build build-mingw-mc --config Release
./build-mingw-mc/Release/oop.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-mc --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-mingw-mc\Release\oop.exe
```

</details>

## 3. 命令行 · MSVC（cmd）

### 方案 A — vcvarsall + Ninja

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B build-msvc-ninja -S .
cmake --build build-msvc-ninja
build-msvc-ninja\oop.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-msvc-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-msvc-ninja\oop.exe
```

</details>

### 方案 B — VS Generator

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .
cmake --build build-msvc-vs --config Release
build-msvc-vs\Release\oop.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-msvc-vs --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\08_oop\build-msvc-vs\Release\oop.exe
```

</details>

## 4. 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/oop
```

## 5. 英文及缩写说明

### 关键字

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

### 概念

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
