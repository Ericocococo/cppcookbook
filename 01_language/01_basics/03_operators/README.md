# 02_operators — 运算符

演示算术、比较、逻辑、位运算、赋值、自增自减、三目运算符及 `sizeof`。

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 8 个 demo 函数，逐类演示各种运算符 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `operators` |

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
./build-mingw-ninja/operators.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 运行
build-mingw-ninja\operators.exe
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

build-msvc-ninja\operators.exe
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
build-msvc-ninja\operators.exe
```

---

## 4. CLion IDE

1. `File → Open` 选择 `02_operators/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. **构建** `Ctrl+F9`　**运行** `Shift+F10`

---

## 5. 英文及缩写说明

### 运算符术语

| 词汇 | 说明 |
|------|------|
| arithmetic operator | 算术运算符：`+` `-` `*` `/` `%`，执行数学计算 |
| comparison operator | 比较运算符：`==` `!=` `<` `>` `<=` `>=`，结果为 `bool` |
| logical operator | 逻辑运算符：`&&`（与）`\|\|`（或）`!`（非），组合多个条件 |
| bitwise operator | 位运算符：直接操作二进制位 |
| assignment operator | 赋值运算符：`=`；复合赋值 `+=` `-=` `*=` `/=` `%=` |
| ternary operator | 三目运算符：`条件 ? 真值 : 假值`，是 if-else 的紧凑写法 |
| `sizeof` | size of：运算符，返回类型或变量占用的字节数，编译期确定 |
| `alignof` | alignment of：运算符，返回对齐字节数 |

### 重要概念

| 概念 | 说明 |
|------|------|
| short-circuit evaluation | 短路求值：`&&` 左边为 `false` 时右边不执行；`\|\|` 左边为 `true` 时右边不执行 |
| integer division | 整数除法：两个整数相除结果仍是整数，小数部分直接截断。`10/3 = 3` |
| modulo | 取余/取模：`%` 运算符，返回除法的余数。`10 % 3 = 1` |
| prefix vs postfix | 前缀 `++i`：先加再用；后缀 `i++`：先用再加 |
| bitwise AND | 按位与 `&`：两位都为 1 才为 1，常用于检查/清除标志位 |
| bitwise OR | 按位或 `\|`：有一个 1 就为 1，常用于设置标志位 |
| bitwise XOR | 按位异或 `^`：两位不同才为 1，常用于翻转标志位 |
| left shift | 左移 `<<`：相当于乘以 2ⁿ。`1 << 3 = 8` |
| right shift | 右移 `>>`：相当于除以 2ⁿ。`8 >> 1 = 4` |
