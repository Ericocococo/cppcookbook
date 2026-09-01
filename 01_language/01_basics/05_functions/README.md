# 05_functions — 函数

演示函数定义、默认参数、重载、内联函数、函数指针、lambda 表达式。

## 1. 文件

| 文件               | 说明                       |
|------------------|--------------------------|
| `main.cpp`       | 6 个 demo 函数，覆盖函数的各种用法    |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `functions` |

---

## 2. 命令行 · MinGW（Git Bash）

### 方案 A — Ninja

```bash
cmake -G Ninja -B build-mingw-ninja -S .
cmake --build build-mingw-ninja
./build-mingw-ninja/functions.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-ninja\functions.exe
```

</details>

### 方案 B — MinGW Makefiles

```bash
cmake -G "MinGW Makefiles" -B build-mingw-make -S .
cmake --build build-mingw-make
./build-mingw-make/functions.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-make
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-make\functions.exe
```

</details>

### 方案 C — Ninja Multi-Config

```bash
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .
cmake --build build-mingw-mc --config Release
./build-mingw-mc/Release/functions.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-mc --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-mingw-mc\Release\functions.exe
```

</details>

---

## 3. 命令行 · MSVC（cmd）

### 方案 A — vcvarsall + Ninja

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B build-msvc-ninja -S .
cmake --build build-msvc-ninja
build-msvc-ninja\functions.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-msvc-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-msvc-ninja\functions.exe
```

</details>

### 方案 B — VS Generator

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .
cmake --build build-msvc-vs --config Release
build-msvc-vs\Release\functions.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-msvc-vs --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\05_functions\build-msvc-vs\Release\functions.exe
```

</details>

---

## 4. 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/functions
```

---

## 5. 英文及缩写说明

### 关键字

| 词汇       | 说明                                        |
|----------|-------------------------------------------|
| `inline` | 内联：建议编译器把函数调用展开为函数体，避免调用开销；适合短小、频繁调用的函数   |
| `return` | 返回：退出函数并返回一个值；`void` 函数也可写 `return;` 提前退出 |
| `void`   | 空：作为返回类型时表示函数没有返回值                        |

### 概念

| 概念                      | 说明                                                      |
|-------------------------|---------------------------------------------------------|
| pass by value           | 值传递：函数拿到参数的副本，函数内修改不影响外部变量                              |
| pass by reference       | 引用传递：参数写 `int& x`，函数内修改直接影响外部变量                         |
| pass by const reference | 常量引用传递：`const int& x`，不拷贝也不修改，适合传大对象（`string`、`vector`） |
| default argument        | 默认参数：调用时可省略；只能从右往左设置                                    |
| function overload       | 函数重载：同名函数有不同参数类型或数量，编译器自动选择                             |
| function pointer        | 函数指针：把函数当变量传递；`int (*fp)(int,int)` 声明函数指针               |
| lambda expression       | lambda 表达式（C++11）：匿名函数，在用到的地方直接定义                       |
| capture list            | 捕获列表：lambda 的 `[]` 部分，指定从外部捕获哪些变量                       |
| closure                 | 闭包：lambda 加上它捕获的外部变量，整体打包成可调用对象                         |
| value capture           | 值捕获 `[x]`：拿到副本，外部修改不影响 lambda 内的值                       |
| reference capture       | 引用捕获 `[&x]`：直接引用外部变量，外部修改立即反映                           |
