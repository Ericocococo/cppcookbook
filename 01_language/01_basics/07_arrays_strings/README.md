# 07_arrays_strings — 数组与字符串

演示原始数组、`std::array`（固定大小）、`std::vector`（动态数组）、`std::string` 的基本用法。

## 1. 文件

| 文件               | 说明                               |
|------------------|----------------------------------|
| `main.cpp`       | 4 个 demo 函数，覆盖原始数组到 string 的常用操作 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `arrays_strings`    |

---

## 2. 命令行 · MinGW（Git Bash）

### 方案 A — Ninja

```bash
cmake -G Ninja -B build-mingw-ninja -S .
cmake --build build-mingw-ninja
./build-mingw-ninja/arrays_strings.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-ninja\arrays_strings.exe
```

</details>

### 方案 B — MinGW Makefiles

```bash
cmake -G "MinGW Makefiles" -B build-mingw-make -S .
cmake --build build-mingw-make
./build-mingw-make/arrays_strings.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-make
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-make\arrays_strings.exe
```

</details>

### 方案 C — Ninja Multi-Config

```bash
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .
cmake --build build-mingw-mc --config Release
./build-mingw-mc/Release/arrays_strings.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-mc --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-mingw-mc\Release\arrays_strings.exe
```

</details>

## 3. 命令行 · MSVC（cmd）

### 方案 A — vcvarsall + Ninja

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B build-msvc-ninja -S .
cmake --build build-msvc-ninja
build-msvc-ninja\arrays_strings.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-msvc-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-msvc-ninja\arrays_strings.exe
```

</details>

### 方案 B — VS Generator

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .
cmake --build build-msvc-vs --config Release
build-msvc-vs\Release\arrays_strings.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-msvc-vs --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\07_arrays_strings\build-msvc-vs\Release\arrays_strings.exe
```

</details>

## 4. 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/arrays_strings
```

## 5. 英文及缩写说明

### 数组与容器

| 词汇                | 说明                                            |
|-------------------|-----------------------------------------------|
| array             | 数组：同类型元素的连续序列，下标从 0 开始                        |
| index / subscript | 下标/索引：访问元素的位置编号，从 0 开始，越界访问是未定义行为             |
| `std::array`      | 标准库固定大小数组（C++11），有 `size()`/`at()` 方法，比原始数组安全 |
| `std::vector`     | 标准库动态数组：大小运行时可变，是最常用的容器                       |

### vector 常用方法

| 方法                   | 说明                                              |
|----------------------|-------------------------------------------------|
| `push_back(x)`       | push back：在末尾追加元素 x                             |
| `pop_back()`         | pop back：移除末尾元素（不返回值）                           |
| `emplace_back(...)`  | emplace back：在末尾直接构造元素，比 `push_back` 少一次拷贝，效率更高 |
| `size()`             | 当前元素数量                                          |
| `capacity()`         | 容量：当前预分配内存能容纳的元素数；超出时自动扩容                       |
| `empty()`            | 是否为空，返回 `bool`                                  |
| `front()` / `back()` | 第一个 / 最后一个元素的引用                                 |
| `at(i)`              | 带越界检查的下标访问，越界抛 `std::out_of_range`              |
| `fill(v)`            | `std::array` 专用：把所有元素设置为 v                      |

### string 常用方法

| 方法                    | 说明                                                    |
|-----------------------|-------------------------------------------------------|
| `length()` / `size()` | 字符串长度（字节数）                                            |
| `substr(pos, len)`    | substring：从 pos 位置取 len 个字符                           |
| `find(s)`             | 查找子串 s 第一次出现的位置，找不到返回 `string::npos`                  |
| `npos`                | not a position：`string::npos` 表示"没找到"，值为 `size_t` 最大值 |
| `stoi(s)`             | string to integer：字符串转 `int`                          |
| `stod(s)`             | string to double：字符串转 `double`                        |
| `to_string(n)`        | 数值转字符串                                                |
| `c_str()`             | 返回 C 风格字符串（`const char*`），与 C 接口交互时用                  |

### 类型与头文件

| 词汇         | 说明                                         |
|------------|--------------------------------------------|
| `size_t`   | size type：无符号整数类型，专门表示大小/长度                |
| `<array>`  | 包含 `std::array`                            |
| `<vector>` | 包含 `std::vector`                           |
| `<string>` | 包含 `std::string`、`stoi`、`stod`、`to_string` |
