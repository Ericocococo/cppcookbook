# 01_types — 数据类型

演示 C++ 内置数据类型、字面量后缀、`auto`/`decltype` 类型推断、`const`/`constexpr` 常量。

## 1. 文件

| 文件               | 说明                                                       |
|------------------|----------------------------------------------------------|
| `main.cpp`       | 5 个 demo 函数，覆盖基本类型、字面量、初始化、auto/decltype、const/constexpr |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `types`                                     |

---

## 2. 命令行 · MinGW（Git Bash）

### 方案 A — Ninja

```bash
cmake -G Ninja -B build-mingw-ninja -S .
cmake --build build-mingw-ninja
./build-mingw-ninja/types.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-ninja\types.exe
```

</details>

### 方案 B — MinGW Makefiles

```bash
cmake -G "MinGW Makefiles" -B build-mingw-make -S .
cmake --build build-mingw-make
./build-mingw-make/types.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-make
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-make\types.exe
```

</details>

### 方案 C — Ninja Multi-Config

```bash
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .
cmake --build build-mingw-mc --config Release
./build-mingw-mc/Release/types.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-mc --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-mingw-mc\Release\types.exe
```

</details>

---

## 3. 命令行 · MSVC（cmd）

### 方案 A — vcvarsall + Ninja

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B build-msvc-ninja -S .
cmake --build build-msvc-ninja
build-msvc-ninja\types.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-msvc-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-msvc-ninja\types.exe
```

</details>

### 方案 B — VS Generator

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .
cmake --build build-msvc-vs --config Release
build-msvc-vs\Release\types.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-msvc-vs --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\01_types\build-msvc-vs\Release\types.exe
```

</details>

---

## 4. 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/types
```

---

## 5. 英文及缩写说明

### C++ 关键字

| 词汇          | 全称 / 读法                         | 说明                                              |
|-------------|---------------------------------|-------------------------------------------------|
| `bool`      | Boolean                         | 布尔类型，只有 `true`（真）和 `false`（假）两个值                |
| `char`      | Character                       | 字符类型，存单个字符，本质是 -128~127 的小整数（对应 ASCII 码）        |
| `short`     | Short integer                   | 短整数，2 字节                                        |
| `int`       | Integer                         | 整数，最常用的整数类型，4 字节                                |
| `long`      | Long integer                    | 长整数，大小平台相关（Windows 上 4 字节，Linux 上 8 字节）         |
| `long long` | Long long integer               | 更长的整数，8 字节，用于存非常大的整数                            |
| `float`     | Floating point                  | 单精度浮点数，4 字节，精度约 7 位有效数字                         |
| `double`    | Double precision floating point | 双精度浮点数，8 字节，精度约 15 位有效数字，推荐用                    |
| `unsigned`  | Unsigned integer                | 无符号整数，不能存负数，但正数范围翻倍（如 `unsigned int`：0 ~ 2³²-1） |
| `const`     | Constant                        | 常量，声明后不可修改；运行时确定值                               |
| `constexpr` | Constant Expression             | 常量表达式，编译期就计算好，可用作数组大小、模板参数                      |
| `volatile`  | （拉丁语：易变的）                       | 告知编译器不要优化此变量，每次都从内存读写；常用于硬件寄存器、多线程              |
| `auto`      | Automatic（自动）                   | 让编译器根据右边的值自动推断变量类型（C++11）                       |
| `decltype`  | Declared type（声明类型）             | 推断表达式的类型但不执行它（C++11）                            |

### 字面量后缀

| 后缀        | 说明                | 示例            |
|-----------|-------------------|---------------|
| `L`       | long              | `42L`         |
| `LL`      | long long         | `42LL`        |
| `U` / `u` | unsigned          | `42U`         |
| `F` / `f` | float             | `3.14F`       |
| `0x` 前缀   | 十六进制（Hexadecimal） | `0xFF` = 255  |
| `0b` 前缀   | 二进制（Binary，C++14） | `0b1010` = 10 |

### 运算符与头文件

| 词汇            | 说明                                                       |
|---------------|----------------------------------------------------------|
| `sizeof`      | size of：运算符，返回类型或变量占用的字节数（1 字节 = 8 位）                    |
| `alignof`     | alignment of：运算符，返回类型的对齐要求（字节数）                          |
| `static_cast` | static cast：编译期类型转换，最安全最常用的 C++ 风格转换                     |
| `<climits>`   | C limits：包含整数类型的范围常量，如 `INT_MAX`（int 最大值）、`INT_MIN`（最小值） |
