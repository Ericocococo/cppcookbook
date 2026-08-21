# 10_templates — 模板

函数模板、类模板、全特化、非类型模板参数、Concepts（C++20）、`if constexpr`。

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 6 个 demo：函数模板推断 / 类模板（Stack）/ 全特化 / 非类型参数 / Concepts / if constexpr |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `templates` |

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
./build-mingw-ninja/templates.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 运行
build-mingw-ninja\templates.exe
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

build-msvc-ninja\templates.exe
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
build-msvc-ninja\templates.exe
```

---

## 4. CLion IDE

1. `File → Open` 选择 `10_templates/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. **构建** `Ctrl+F9`　**运行** `Shift+F10`

---

## 5. 英文及缩写说明

| 词汇 | 说明 |
|------|------|
| `template` | 模板：定义代码的"模具"，让函数/类对任意类型通用；编译器根据实际类型生成对应代码，零运行时开销 |
| `typename` | 类型名：模板参数列表中的占位符关键字；与 `class` 在此处完全等价 |
| type parameter | 类型参数：模板中的 T 等占位符，调用时由编译器自动推断或显式指定 |
| template instantiation | 模板实例化：编译器根据实际类型参数生成具体函数/类，发生在编译期 |
| full specialization | 全特化：`template<>` 为某个具体类型提供完全不同的实现，覆盖通用版本 |
| non-type template parameter | 非类型模板参数：用编译期常量（如 `size_t N`）作为模板参数 |
| `requires` | Concepts（C++20）约束关键字：指定模板类型必须满足的条件；不满足则编译报错（而不是难懂的模板错误）|
| `std::is_arithmetic_v<T>` | T 是算术类型（int/float 等）时为 true；来自 `<type_traits>` |
| `if constexpr` | 编译期条件（C++17）：条件为 false 的分支在编译期丢弃；普通 if 两个分支都要能编译 |
| TMP | Template Metaprogramming：模板元编程，利用模板在编译期执行计算 |
| CTAD | Class Template Argument Deduction：类模板参数推断（C++17），可省略模板参数 |
