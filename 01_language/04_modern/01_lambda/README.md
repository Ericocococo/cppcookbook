# 01_lambda — lambda 表达式

> C++11 引入，C++14/20 持续增强。把函数写成"就地定义的匿名函数"，最常配合 STL 算法使用。

## 1. 知识点

| 序号 | 主题            | 说明                                               |
|----|---------------|--------------------------------------------------|
| ①  | 基本语法          | `[捕获](参数) -> 返回类型 { 函数体 }`                       |
| ②  | 捕获列表          | `[]` `[x]` `[&x]` `[=]` `[&]` `[=, &x]` `[&, x]` |
| ③  | mutable       | 允许修改按值捕获的变量（改的是拷贝）                               |
| ④  | 泛型 lambda     | 参数用 `auto`，类似函数模板（C++14）                         |
| ⑤  | lambda + STL  | sort / count_if / for_each / find_if             |
| ⑥  | std::function | 存到变量 / 容器，做回调                                    |
| ⑦  | IIFE          | 立即调用，常用于复杂 const 初始化                             |

## 2. 命令行 · MinGW（Git Bash）

### 方案 A — Ninja

```bash
cmake -G Ninja -B build-mingw-ninja -S .
cmake --build build-mingw-ninja
./build-mingw-ninja/lambda.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-ninja\lambda.exe
```

</details>

### 方案 B — MinGW Makefiles

```bash
cmake -G "MinGW Makefiles" -B build-mingw-make -S .
cmake --build build-mingw-make
./build-mingw-make/lambda.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-make
D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-make\lambda.exe
```

</details>

### 方案 C — Ninja Multi-Config

```bash
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .
cmake --build build-mingw-mc --config Release
./build-mingw-mc/Release/lambda.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-mc --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-mingw-mc\Release\lambda.exe
```

</details>

## 3. 命令行 · MSVC（cmd）

### 方案 A — vcvarsall + Ninja

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B build-msvc-ninja -S .
cmake --build build-msvc-ninja
build-msvc-ninja\lambda.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-msvc-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-msvc-ninja\lambda.exe
```

</details>

### 方案 B — VS Generator

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .
cmake --build build-msvc-vs --config Release
build-msvc-vs\Release\lambda.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-msvc-vs --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\04_modern\01_lambda\build-msvc-vs\Release\lambda.exe
```

</details>

## 4. 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/lambda
```
