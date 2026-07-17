# 02_type_queries — 类型查询运算符

`sizeof`（大小与填充）、`alignof`（对齐）、`auto`（深入）、`decltype`（保留 const/引用）。

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 5 个 demo：sizeof 陷阱 / alignof 结构体填充 / auto 剥掉 const / decltype vs auto / 实际场景 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `type_queries` |

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
./build-mingw/type_queries.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw

:: 运行
build-mingw\type_queries.exe
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

build-msvc\type_queries.exe
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
build-msvc\type_queries.exe
```

---

## 4. CLion IDE

1. `File → Open` 选择 `02_type_queries/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. **构建** `Ctrl+F9`　**运行** `Shift+F10`

---

## 5. 英文及缩写说明

| 词汇 | 说明 |
|------|------|
| `sizeof` | size of：运算符，返回类型/变量占用字节数；**不执行表达式**；对指针只返回指针大小（不是数组大小）|
| `alignof` | alignment of：返回类型的对齐要求（字节数），决定变量必须存放在几的倍数地址 |
| alignment | 对齐：CPU 读写内存的要求，`int` 要放在 4 的倍数地址，`double` 要放在 8 的倍数地址，否则性能下降 |
| padding | 填充字节：结构体为满足对齐要求，由编译器自动插入的空白字节；字段顺序影响填充量 |
| `alignas` | alignment as（C++11）：手动指定对齐要求，如 `alignas(16)` 用于 SIMD 向量化指令 |
| `auto` | 自动类型推断（C++11）：由初始值推断变量类型；**会剥掉 const 和引用** |
| `decltype` | declared type：推断表达式类型但不执行；**保留 const 和引用**，与 auto 的关键区别 |
| `decltype(auto)` | C++14：兼具 auto 的简洁和 decltype 的保留能力，常用于完美转发返回类型 |
| `size_t` | size type：无符号整数，sizeof/alignof 的返回类型，专门表示大小和索引 |
| `std::copy` | 标准算法，将一个范围的元素复制到另一个位置，来自 `<algorithm>` |
