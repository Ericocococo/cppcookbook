# 00_hello_world — 程序结构

每个 C++ 程序都有的"必须件"：`#include`、`main()`、变量声明、`cout` 打印、注释、作用域。

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 5 个 demo 函数，逐一拆解程序每个组成部分 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `hello_world` |

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
"$CMAKE" -B build-mingw -G Ninja \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA"

# 构建
"$CMAKE" --build build-mingw

# 运行
./build-mingw/hello_world.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw

:: 运行
build-mingw\hello_world.exe
```

---

## 3. 命令行 · MSVC（cmd）

### 方案 A：vcvarsall + Ninja（推荐）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

:: 激活 — 把 cl.exe / link.exe / ninja.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "%VCVARSALL%" x64

"%CMAKE%" -B build-msvc -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

"%CMAKE%" --build build-msvc

build-msvc\hello_world.exe
```

> 四步版：

```bat
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc

:: 运行
build-msvc\hello_world.exe
```

---

## 4. CLion IDE

1. `File → Open` 选择 `00_hello_world/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. **构建** `Ctrl+F9`　**运行** `Shift+F10`

---

## 5. 英文及缩写说明

### 程序结构关键字

| 词汇 | 全称 / 读法 | 说明 |
|------|-------------|------|
| `#include` | include（包含）| 预处理指令：把指定头文件的内容复制进来，相当于"引入工具箱"；不写就用不了对应的类型和函数 |
| `#`（井号）| preprocessor directive（预处理指令）| 以 `#` 开头的行由预处理器在编译前处理，不是普通 C++ 语句，结尾不加 `;` |
| `<iostream>` | input/output stream（输入输出流）| 提供 `std::cout`（打印到终端）和 `std::cin`（从终端读取）|
| `<string>` | — | 提供 `std::string` 字符串类型 |
| `main` | — | 程序入口函数，固定名字，操作系统从这里开始执行；每个程序有且只有一个 |
| `int`（main 前）| integer（整数）| `main` 的返回类型，表示它向操作系统返回一个整数 |
| `return 0` | — | 退出 `main` 并返回 0；0 表示"程序正常结束"，非零表示出错 |
| `{` `}` | braces / curly braces（花括号）| 划定"作用域"，里面声明的变量只在 `{}` 内有效；函数体、if、for 等都用它 |
| `;` | semicolon（分号）| 每条语句的结束符，必须有，缺少会编译报错 |

### 变量与类型

| 词汇 | 说明 |
|------|------|
| `int` | Integer：整数，如 `18`、`-5`、`0` |
| `double` | Double precision float：双精度小数，如 `1.75`、`3.14` |
| `bool` | Boolean：只有 `true`（真/1）和 `false`（假/0）|
| `char` | Character：单个字符，用**单引号**，如 `'A'`、`'1'` |
| `std::string` | 一段文字（字符串），用**双引号**，如 `"Hello"` |
| `=`（赋值）| 把右边的值存入左边的变量；不是"相等"，相等判断用 `==` |

### 输出相关

| 词汇 | 说明 |
|------|------|
| `std::` | Standard（标准）的命名空间前缀；表示"去 C++ 标准库里找" |
| `cout` | Console output：控制台输出，向终端打印内容 |
| `<<` | 流插入运算符（stream insertion）：把右边的内容"送进"输出流；可以连续用多个 `<<` |
| `"\n"` | 换行符（newline），在打印中产生换行；推荐用这个 |
| `std::endl` | end line：换行并强制刷新输出缓冲区；比 `"\n"` 慢，一般不需要 |

### 注释

| 写法 | 说明 |
|------|------|
| `// 文字` | 单行注释：从 `//` 到行尾都被编译器忽略，只给人看 |
| `/* 文字 */` | 多行注释：`/*` 到 `*/` 之间都被忽略，可跨多行 |

### 作用域

| 词汇 | 说明 |
|------|------|
| scope | 作用域：变量"存活"的范围，由最近的一对 `{}` 决定；出了 `{}` 变量就消失，访问报错 |
