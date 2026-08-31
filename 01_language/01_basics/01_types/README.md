# 01_types — 数据类型

演示 C++ 内置数据类型、字面量后缀、`auto`/`decltype` 类型推断、`const`/`constexpr` 常量。

## 1. 文件

| 文件               | 说明                                                       |
|------------------|----------------------------------------------------------|
| `main.cpp`       | 5 个 demo 函数，覆盖基本类型、字面量、初始化、auto/decltype、const/constexpr |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `types`                                     |

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
./build-mingw-ninja/types.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 运行
build-mingw-ninja\types.exe
```

---

## 3. 命令行 · MSVC（cmd）

### 方案 A：vcvarsall + Ninja（推荐）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

:: 激活 — 把 cl.exe / link.exe / ninja.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "%VCVARSALL%" x64

:: 配置
"%CMAKE%" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"%CMAKE%" --build build-msvc-ninja

:: 运行
build-msvc-ninja\types.exe
```

> 四步版（激活、配置、构建、运行各一行）：

```bat
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja

:: 运行
build-msvc-ninja\types.exe
```

---

## 4. CLion IDE

1. `File → Open` 选择 `01_types/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. 工具栏选择工具链（MinGW 或 Visual Studio）
4. **构建** `Ctrl+F9`**运行** `Shift+F10`

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
