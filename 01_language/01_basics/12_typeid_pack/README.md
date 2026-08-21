# 12_typeid_pack — typeid 与变参包

`typeid`（运行时类型）、`sizeof...`（变参包大小）、变参模板、折叠表达式、`type_traits`。

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 4 个 demo：typeid 多态查询 / sizeof... 统计 / 变参模板展开与折叠 / type_traits 类型特性 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `typeid_pack` |

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
./build-mingw-ninja/typeid_pack.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 运行
build-mingw-ninja\typeid_pack.exe
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

build-msvc-ninja\typeid_pack.exe
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
build-msvc-ninja\typeid_pack.exe
```

---

## 4. CLion IDE

1. `File → Open` 选择 `12_typeid_pack/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. **构建** `Ctrl+F9`　**运行** `Shift+F10`

---

## 5. 英文及缩写说明

| 词汇 | 说明 |
|------|------|
| `typeid` | type id：返回 `std::type_info` 对象；用于多态时查实际类型（运行期）；需要 `#include <typeinfo>` |
| `type_info` | C++ 标准类，`typeid` 的返回类型；`.name()` 返回类型名（GCC 返回缩写，MSVC 返回全名） |
| `sizeof...` | sizeof 的变参版本（C++11）：统计变参模板参数包的数量，编译期求值 |
| parameter pack | 参数包（C++11）：`typename... Args` 或 `Args... args`，可接收任意数量任意类型的参数 |
| fold expression | 折叠表达式（C++17）：`(args + ...)` = a1+a2+...，简洁地展开参数包；支持 + - * / & | && \|\| , 等运算符 |
| `<type_traits>` | 类型特性头文件：提供编译期类型查询和变换工具，全部以 `_v`（变量模板）或 `_t`（类型别名）结尾 |
| `std::is_same_v<T1,T2>` | 两个类型是否完全相同，编译期 `bool` |
| `std::is_integral_v<T>` | T 是整数类型（bool/char/int/long 等）时为 true |
| `std::is_floating_point_v<T>` | T 是浮点类型（float/double/long double）时为 true |
| `std::is_pointer_v<T>` | T 是指针类型时为 true |
| `std::is_reference_v<T>` | T 是引用类型时为 true |
| `std::remove_const_t<T>` | 移除 T 的 const 限定，如 `remove_const_t<const int>` = `int` |
| `std::add_pointer_t<T>` | 把 T 变为指针，如 `add_pointer_t<int>` = `int*` |
| IIFE | Immediately Invoked Function Expression：定义 lambda 后立刻调用，常用于初始化复杂的 const 变量 |
| `<typeinfo>` | 包含 `std::type_info` 类；不包含此头文件时，`typeid` 可能无法编译 |
