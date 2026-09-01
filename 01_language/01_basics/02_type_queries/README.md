# 02_type_queries — 类型查询运算符

`sizeof`（大小与填充）、`alignof`（对齐）、`auto`（深入）、`decltype`（保留 const/引用）。

## 1. 文件

| 文件               | 说明                                                                           |
|------------------|------------------------------------------------------------------------------|
| `main.cpp`       | 5 个 demo：sizeof 陷阱 / alignof 结构体填充 / auto 剥掉 const / decltype vs auto / 实际场景 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `type_queries`                                                  |

---

## 2. 命令行 · MinGW（Git Bash）

### 方案 A — Ninja

```bash
cmake -G Ninja -B build-mingw-ninja -S .
cmake --build build-mingw-ninja
./build-mingw-ninja/type_queries.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-ninja\type_queries.exe
```

</details>

### 方案 B — MinGW Makefiles

```bash
cmake -G "MinGW Makefiles" -B build-mingw-make -S .
cmake --build build-mingw-make
./build-mingw-make/type_queries.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-make
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-make\type_queries.exe
```

</details>

### 方案 C — Ninja Multi-Config

```bash
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .
cmake --build build-mingw-mc --config Release
./build-mingw-mc/Release/type_queries.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-mc --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-mingw-mc\Release\type_queries.exe
```

</details>

---

## 3. 命令行 · MSVC（cmd）

### 方案 A — vcvarsall + Ninja

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B build-msvc-ninja -S .
cmake --build build-msvc-ninja
build-msvc-ninja\type_queries.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-msvc-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-msvc-ninja\type_queries.exe
```

</details>

### 方案 B — VS Generator

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .
cmake --build build-msvc-vs --config Release
build-msvc-vs\Release\type_queries.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-msvc-vs --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\02_type_queries\build-msvc-vs\Release\type_queries.exe
```

</details>

---

## 4. 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/type_queries
```

---

## 5. 英文及缩写说明

| 词汇               | 说明                                                           |
|------------------|--------------------------------------------------------------|
| `sizeof`         | size of：运算符，返回类型/变量占用字节数；**不执行表达式**；对指针只返回指针大小（不是数组大小）       |
| `alignof`        | alignment of：返回类型的对齐要求（字节数），决定变量必须存放在几的倍数地址                  |
| alignment        | 对齐：CPU 读写内存的要求，`int` 要放在 4 的倍数地址，`double` 要放在 8 的倍数地址，否则性能下降 |
| padding          | 填充字节：结构体为满足对齐要求，由编译器自动插入的空白字节；字段顺序影响填充量                      |
| `alignas`        | alignment as（C++11）：手动指定对齐要求，如 `alignas(16)` 用于 SIMD 向量化指令   |
| `auto`           | 自动类型推断（C++11）：由初始值推断变量类型；**会剥掉 const 和引用**                   |
| `decltype`       | declared type：推断表达式类型但不执行；**保留 const 和引用**，与 auto 的关键区别      |
| `decltype(auto)` | C++14：兼具 auto 的简洁和 decltype 的保留能力，常用于完美转发返回类型                |
| `size_t`         | size type：无符号整数，sizeof/alignof 的返回类型，专门表示大小和索引               |
| `std::copy`      | 标准算法，将一个范围的元素复制到另一个位置，来自 `<algorithm>`                       |
