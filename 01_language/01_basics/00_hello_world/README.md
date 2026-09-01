# 00_hello_world — 程序结构

每个 C++ 程序都有的"必须件"：`#include`、`main()`、变量声明、`cout` 打印、注释、作用域。

## 1. 知识点

| 序号 | 主题 | 说明 |
|------|------|------|
| ① | 程序结构 | `#include`、`main()`、`return 0`、`;`、`{}` |
| ② | 注释 | `//` 单行、`/* */` 多行，写"为什么"不写"做了什么" |
| ③ | 变量 | 类型 变量名 = 初始值；int/double/bool/char/string |
| ④ | cout 打印 | `std::cout << "文字" << 变量 << "\n";` |
| ⑤ | 作用域 | `{}` 内声明的变量出了 `{}` 就消失 |
| ⑥ | cin 输入 | `std::cin >> 变量`、`std::getline` 读整行 |

## 2. 命令行 · MinGW（Git Bash）

### 方案 A — Ninja

```bash
cmake -G Ninja -B build-mingw-ninja -S .
cmake --build build-mingw-ninja
./build-mingw-ninja/hello_world.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-ninja\hello_world.exe
```

</details>

### 方案 B — MinGW Makefiles

```bash
cmake -G "MinGW Makefiles" -B build-mingw-make -S .
cmake --build build-mingw-make
./build-mingw-make/hello_world.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-make
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-make\hello_world.exe
```

</details>

### 方案 C — Ninja Multi-Config

```bash
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .
cmake --build build-mingw-mc --config Release
./build-mingw-mc/Release/hello_world.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-mc --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-mingw-mc\Release\hello_world.exe
```

</details>

## 3. 命令行 · MSVC（cmd）

### 方案 A — vcvarsall + Ninja

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B build-msvc-ninja -S .
cmake --build build-msvc-ninja
build-msvc-ninja\hello_world.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-msvc-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-msvc-ninja\hello_world.exe
```

</details>

### 方案 B — VS Generator

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .
cmake --build build-msvc-vs --config Release
build-msvc-vs\Release\hello_world.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-msvc-vs --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\00_hello_world\build-msvc-vs\Release\hello_world.exe
```

</details>

## 4. 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/hello_world
```

## 5. 英文及缩写说明

### 程序结构关键字

| 词汇 | 全称 / 读法 | 说明 |
|------|------|------|
| `#include` | include（包含） | 预处理指令：把指定头文件的内容复制进来，相当于"引入工具箱" |
| `#`（井号） | preprocessor directive（预处理指令） | 以 `#` 开头的行由预处理器在编译前处理，不是普通 C++ 语句，结尾不加 `;` |
| `<iostream>` | input/output stream（输入输出流） | 提供 `std::cout`（打印到终端）和 `std::cin`（从终端读取） |
| `<string>` | — | 提供 `std::string` 字符串类型 |
| `main` | — | 程序入口函数，固定名字，操作系统从这里开始执行；每个程序有且只有一个 |
| `return 0` | — | 退出 `main` 并返回 0；0 表示"程序正常结束"，非零表示出错 |
| `{` `}` | braces / curly braces（花括号） | 划定"作用域"，里面声明的变量只在 `{}` 内有效 |
| `;` | semicolon（分号） | 每条语句的结束符，必须有，缺少会编译报错 |

### 变量与类型

| 词汇 | 说明 |
|------|------|
| `int` | Integer：整数，如 `18`、`-5`、`0` |
| `double` | Double precision float：双精度小数，如 `1.75`、`3.14` |
| `bool` | Boolean：只有 `true`（真/1）和 `false`（假/0） |
| `char` | Character：单个字符，用**单引号**，如 `'A'`、`'1'` |
| `std::string` | 一段文字（字符串），用**双引号**，如 `"Hello"` |

### 输出相关

| 词汇 | 说明 |
|------|------|
| `std::` | Standard（标准）的命名空间前缀；表示"去 C++ 标准库里找" |
| `cout` | Console output：控制台输出，向终端打印内容 |
| `<<` | 流插入运算符（stream insertion）：把右边的内容"送进"输出流 |
| `"\n"` | 换行符（newline），推荐用这个 |
| `std::endl` | end line：换行并强制刷新输出缓冲区；比 `"\n"` 慢 |
