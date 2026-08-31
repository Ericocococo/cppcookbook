# 02_oop — 面向对象深化专章

> 前置要求：已完成 `01_basics/08_oop`（入门：struct/class、继承、多态、Rule 0/3/5 基本概念）。
> 本章在每个主题上深入展开，覆盖入门未涉及的细节和惯用法。

## 1. 知识点总览

| 子目录 | 主题 | 入门已讲 | 本章深化 |
|---|---|---|---|
| 01_classes_objects | 类深入 | struct/class、private/public | 友元、嵌套类、this 指针、mutable、static 深入 |
| 02_constructors_raii | 构造与 RAII | 初始化列表、委托构造 | 全构造家族、explicit、析构顺序、RAII 模式、copy-and-swap |
| 03_inheritance | 继承深入 | 单继承、virtual、protected | 多继承、菱形继承、virtual 继承、final、隐藏 vs 覆盖 |
| 04_polymorphism | 多态深入 | 纯虚、父类指针调子类 | vtable 原理、dynamic_cast、接口模式、协变返回、CRTP |
| 05_operator_overload | 运算符重载 | 无 | 算术/比较/流/下标/仿函数/自增/太空船(C++20) |
| 06_special_members | 特殊成员函数 | Rule 0/3/5 概念 | =default/=delete、编译器生成规则、copy-and-swap 完整实现 |

## 2. 命令行 · MinGW（Git Bash）

每个子目录独立编译，以 `01_classes_objects` 为例（其他子目录替换目录名和可执行文件名即可）：

### 方案 A — Ninja

```bash
cmake -G Ninja -B build-mingw-ninja -S .
cmake --build build-mingw-ninja
./build-mingw-ninja/classes_objects.exe
```

### 方案 B — MinGW Makefiles

```bash
cmake -G "MinGW Makefiles" -B build-mingw-make -S .
cmake --build build-mingw-make
./build-mingw-make/classes_objects.exe
```

### 方案 C — Ninja Multi-Config

```bash
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .
cmake --build build-mingw-mc --config Release
./build-mingw-mc/Release/classes_objects.exe
```

## 3. 命令行 · MSVC（cmd）

### 方案 A — vcvarsall + Ninja

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B build-msvc-ninja -S .
cmake --build build-msvc-ninja
build-msvc-ninja\classes_objects.exe
```

### 方案 B — VS Generator

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .
cmake --build build-msvc-vs --config Release
build-msvc-vs\Release\classes_objects.exe
```

## 4. 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/classes_objects
```

## 5. 各子目录可执行文件名

| 子目录 | 可执行文件名 |
|---|---|
| 01_classes_objects | classes_objects |
| 02_constructors_raii | constructors_raii |
| 03_inheritance | inheritance |
| 04_polymorphism | polymorphism |
| 05_operator_overload | operator_overload |
| 06_special_members | special_members |
