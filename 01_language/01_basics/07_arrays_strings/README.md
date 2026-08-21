# 06_arrays_strings — 数组与字符串

演示原始数组、`std::array`（固定大小）、`std::vector`（动态数组）、`std::string` 的基本用法。

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 4 个 demo 函数，覆盖原始数组到 string 的常用操作 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `arrays_strings` |

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
./build-mingw-ninja/arrays_strings.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 运行
build-mingw-ninja\arrays_strings.exe
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

build-msvc-ninja\arrays_strings.exe
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
build-msvc-ninja\arrays_strings.exe
```

---

## 4. CLion IDE

1. `File → Open` 选择 `06_arrays_strings/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. **构建** `Ctrl+F9`　**运行** `Shift+F10`

---

## 5. 英文及缩写说明

### 数组与容器

| 词汇 | 说明 |
|------|------|
| array | 数组：同类型元素的连续序列，下标从 0 开始 |
| index / subscript | 下标/索引：访问元素的位置编号，从 0 开始，越界访问是未定义行为 |
| `std::array` | 标准库固定大小数组（C++11），有 `size()`/`at()` 方法，比原始数组安全 |
| `std::vector` | 标准库动态数组：大小运行时可变，是最常用的容器 |

### vector 常用方法

| 方法 | 说明 |
|------|------|
| `push_back(x)` | push back：在末尾追加元素 x |
| `pop_back()` | pop back：移除末尾元素（不返回值）|
| `emplace_back(...)` | emplace back：在末尾直接构造元素，比 `push_back` 少一次拷贝，效率更高 |
| `size()` | 当前元素数量 |
| `capacity()` | 容量：当前预分配内存能容纳的元素数；超出时自动扩容 |
| `empty()` | 是否为空，返回 `bool` |
| `front()` / `back()` | 第一个 / 最后一个元素的引用 |
| `at(i)` | 带越界检查的下标访问，越界抛 `std::out_of_range` |
| `fill(v)` | `std::array` 专用：把所有元素设置为 v |

### string 常用方法

| 方法 | 说明 |
|------|------|
| `length()` / `size()` | 字符串长度（字节数）|
| `substr(pos, len)` | substring：从 pos 位置取 len 个字符 |
| `find(s)` | 查找子串 s 第一次出现的位置，找不到返回 `string::npos` |
| `npos` | not a position：`string::npos` 表示"没找到"，值为 `size_t` 最大值 |
| `stoi(s)` | string to integer：字符串转 `int` |
| `stod(s)` | string to double：字符串转 `double` |
| `to_string(n)` | 数值转字符串 |
| `c_str()` | 返回 C 风格字符串（`const char*`），与 C 接口交互时用 |

### 类型与头文件

| 词汇 | 说明 |
|------|------|
| `size_t` | size type：无符号整数类型，专门表示大小/长度 |
| `<array>` | 包含 `std::array` |
| `<vector>` | 包含 `std::vector` |
| `<string>` | 包含 `std::string`、`stoi`、`stod`、`to_string` |
