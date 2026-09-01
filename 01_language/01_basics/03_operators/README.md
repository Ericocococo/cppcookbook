# 03_operators — 运算符

演示算术、比较、逻辑、位运算、赋值、自增自减、三目运算符及 `sizeof`。

## 1. 文件

| 文件               | 说明                       |
|------------------|--------------------------|
| `main.cpp`       | 8 个 demo 函数，逐类演示各种运算符    |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `operators` |

---

## 2. 命令行 · MinGW（Git Bash）

### 方案 A — Ninja

```bash
cmake -G Ninja -B build-mingw-ninja -S .
cmake --build build-mingw-ninja
./build-mingw-ninja/operators.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-ninja\operators.exe
```

</details>

### 方案 B — MinGW Makefiles

```bash
cmake -G "MinGW Makefiles" -B build-mingw-make -S .
cmake --build build-mingw-make
./build-mingw-make/operators.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-make
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-make\operators.exe
```

</details>

### 方案 C — Ninja Multi-Config

```bash
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .
cmake --build build-mingw-mc --config Release
./build-mingw-mc/Release/operators.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-mc --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-mingw-mc\Release\operators.exe
```

</details>

---

## 3. 命令行 · MSVC（cmd）

### 方案 A — vcvarsall + Ninja

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B build-msvc-ninja -S .
cmake --build build-msvc-ninja
build-msvc-ninja\operators.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-msvc-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-msvc-ninja\operators.exe
```

</details>

### 方案 B — VS Generator

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .
cmake --build build-msvc-vs --config Release
build-msvc-vs\Release\operators.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-msvc-vs --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\03_operators\build-msvc-vs\Release\operators.exe
```

</details>

---

## 4. 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/operators
```

---

## 5. 英文及缩写说明

### 运算符术语

| 词汇                  | 说明                                           |
|---------------------|----------------------------------------------|
| arithmetic operator | 算术运算符：`+` `-` `*` `/` `%`，执行数学计算             |
| comparison operator | 比较运算符：`==` `!=` `<` `>` `<=` `>=`，结果为 `bool` |
| logical operator    | 逻辑运算符：`&&`（与）`\|\|`（或）`!`（非），组合多个条件          |
| bitwise operator    | 位运算符：直接操作二进制位                                |
| assignment operator | 赋值运算符：`=`；复合赋值 `+=` `-=` `*=` `/=` `%=`      |
| ternary operator    | 三目运算符：`条件 ? 真值 : 假值`，是 if-else 的紧凑写法         |
| `sizeof`            | size of：运算符，返回类型或变量占用的字节数，编译期确定              |
| `alignof`           | alignment of：运算符，返回对齐字节数                     |

### 重要概念

| 概念                       | 说明                                                    |
|--------------------------|-------------------------------------------------------|
| short-circuit evaluation | 短路求值：`&&` 左边为 `false` 时右边不执行；`\|\|` 左边为 `true` 时右边不执行 |
| integer division         | 整数除法：两个整数相除结果仍是整数，小数部分直接截断。`10/3 = 3`                 |
| modulo                   | 取余/取模：`%` 运算符，返回除法的余数。`10 % 3 = 1`                    |
| prefix vs postfix        | 前缀 `++i`：先加再用；后缀 `i++`：先用再加                           |
| bitwise AND              | 按位与 `&`：两位都为 1 才为 1，常用于检查/清除标志位                       |
| bitwise OR               | 按位或 `\|`：有一个 1 就为 1，常用于设置标志位                          |
| bitwise XOR              | 按位异或 `^`：两位不同才为 1，常用于翻转标志位                            |
| left shift               | 左移 `<<`：相当于乘以 2ⁿ。`1 << 3 = 8`                         |
| right shift              | 右移 `>>`：相当于除以 2ⁿ。`8 >> 1 = 4`                         |
