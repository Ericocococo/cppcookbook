# 10_templates — 模板

函数模板、类模板、全特化、非类型模板参数、Concepts（C++20）、`if constexpr`。

## 1. 文件

| 文件               | 说明                                                                  |
|------------------|---------------------------------------------------------------------|
| `main.cpp`       | 6 个 demo：函数模板推断 / 类模板（Stack）/ 全特化 / 非类型参数 / Concepts / if constexpr |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `templates`                                            |

---

## 2. 命令行 · MinGW（Git Bash）

### 方案 A — Ninja

```bash
cmake -G Ninja -B build-mingw-ninja -S .
cmake --build build-mingw-ninja
./build-mingw-ninja/templates.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-ninja\templates.exe
```

</details>

### 方案 B — MinGW Makefiles

```bash
cmake -G "MinGW Makefiles" -B build-mingw-make -S .
cmake --build build-mingw-make
./build-mingw-make/templates.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-make
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-make\templates.exe
```

</details>

### 方案 C — Ninja Multi-Config

```bash
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .
cmake --build build-mingw-mc --config Release
./build-mingw-mc/Release/templates.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-mc --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-mingw-mc\Release\templates.exe
```

</details>

## 3. 命令行 · MSVC（cmd）

### 方案 A — vcvarsall + Ninja

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B build-msvc-ninja -S .
cmake --build build-msvc-ninja
build-msvc-ninja\templates.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-msvc-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-msvc-ninja\templates.exe
```

</details>

### 方案 B — VS Generator

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .
cmake --build build-msvc-vs --config Release
build-msvc-vs\Release\templates.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-msvc-vs --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\10_templates\build-msvc-vs\Release\templates.exe
```

</details>

## 4. 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/templates
```

## 5. 英文及缩写说明

| 词汇                          | 说明                                                       |
|-----------------------------|----------------------------------------------------------|
| `template`                  | 模板：定义代码的"模具"，让函数/类对任意类型通用；编译器根据实际类型生成对应代码，零运行时开销         |
| `typename`                  | 类型名：模板参数列表中的占位符关键字；与 `class` 在此处完全等价                     |
| type parameter              | 类型参数：模板中的 T 等占位符，调用时由编译器自动推断或显式指定                        |
| template instantiation      | 模板实例化：编译器根据实际类型参数生成具体函数/类，发生在编译期                         |
| full specialization         | 全特化：`template<>` 为某个具体类型提供完全不同的实现，覆盖通用版本                 |
| non-type template parameter | 非类型模板参数：用编译期常量（如 `size_t N`）作为模板参数                       |
| `requires`                  | Concepts（C++20）约束关键字：指定模板类型必须满足的条件；不满足则编译报错（而不是难懂的模板错误）  |
| `std::is_arithmetic_v<T>`   | T 是算术类型（int/float 等）时为 true；来自 `<type_traits>`           |
| `if constexpr`              | 编译期条件（C++17）：条件为 false 的分支在编译期丢弃；普通 if 两个分支都要能编译         |
| TMP                         | Template Metaprogramming：模板元编程，利用模板在编译期执行计算              |
| CTAD                        | Class Template Argument Deduction：类模板参数推断（C++17），可省略模板参数 |
