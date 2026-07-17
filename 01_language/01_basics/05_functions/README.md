# 04_functions — 函数

演示函数定义、默认参数、重载、内联函数、函数指针、lambda 表达式。

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 6 个 demo 函数，覆盖函数的各种用法 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `functions` |

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
./build-mingw/functions.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw

:: 运行
build-mingw\functions.exe
```

---

## 3. 命令行 · MSVC（cmd）

### 方案 A：vcvarsall + Ninja（推荐）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

call "%VCVARSALL%" x64

"%CMAKE%" -B build-msvc -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

"%CMAKE%" --build build-msvc

build-msvc\functions.exe
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
build-msvc\functions.exe
```

---

## 4. CLion IDE

1. `File → Open` 选择 `04_functions/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. **构建** `Ctrl+F9`　**运行** `Shift+F10`

---

## 5. 英文及缩写说明

### 关键字

| 词汇 | 说明 |
|------|------|
| `inline` | 内联：建议编译器把函数调用展开为函数体，避免调用开销；适合短小、频繁调用的函数 |
| `return` | 返回：退出函数并返回一个值；`void` 函数也可写 `return;` 提前退出 |
| `void` | 空：作为返回类型时表示函数没有返回值 |

### 概念

| 概念 | 说明 |
|------|------|
| pass by value | 值传递：函数拿到参数的副本，函数内修改不影响外部变量 |
| pass by reference | 引用传递：参数写 `int& x`，函数内修改直接影响外部变量 |
| pass by const reference | 常量引用传递：`const int& x`，不拷贝也不修改，适合传大对象（`string`、`vector`）|
| default argument | 默认参数：调用时可省略；只能从右往左设置 |
| function overload | 函数重载：同名函数有不同参数类型或数量，编译器自动选择 |
| function pointer | 函数指针：把函数当变量传递；`int (*fp)(int,int)` 声明函数指针 |
| lambda expression | lambda 表达式（C++11）：匿名函数，在用到的地方直接定义 |
| capture list | 捕获列表：lambda 的 `[]` 部分，指定从外部捕获哪些变量 |
| closure | 闭包：lambda 加上它捕获的外部变量，整体打包成可调用对象 |
| value capture | 值捕获 `[x]`：拿到副本，外部修改不影响 lambda 内的值 |
| reference capture | 引用捕获 `[&x]`：直接引用外部变量，外部修改立即反映 |
