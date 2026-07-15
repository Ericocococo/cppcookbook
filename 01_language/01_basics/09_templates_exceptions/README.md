# 09_templates_exceptions — 模板与异常

演示函数模板、类模板、模板特化，以及 `try`/`catch`/`throw` 异常处理机制。

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 4 个 demo 函数：函数模板 → 类模板 → 特化 → 异常 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `templates_exceptions` |

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
./build-mingw/templates_exceptions.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw

:: 运行
build-mingw\templates_exceptions.exe
```

---

## 3. 命令行 · MSVC（cmd）

### 方案 A：vcvarsall + Ninja（推荐）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

call "%VCVARSALL%" x64

"%CMAKE%" -B build-msvc -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

"%CMAKE%" --build build-msvc

build-msvc\templates_exceptions.exe
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
build-msvc\templates_exceptions.exe
```

---

## 4. CLion IDE

1. `File → Open` 选择 `09_templates_exceptions/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. **构建** `Ctrl+F9`　**运行** `Shift+F10`

---

## 5. 英文及缩写说明

### 模板

| 词汇 | 说明 |
|------|------|
| `template` | 模板：定义"代码的模具"，让函数或类对任意类型通用；编译器根据实际类型生成代码 |
| `typename` | 类型名：模板参数列表中的占位符，与 `class` 在此处等价 |
| type parameter | 类型参数：模板中的 T 等占位符，调用时由编译器推断或显式指定 |
| template instantiation | 模板实例化：编译器根据实际类型生成具体函数/类，发生在编译期 |
| template specialization | 模板特化：针对特定类型提供专门实现，覆盖通用版本 |
| full specialization | 全特化：`template<>` 为某个具体类型提供完全不同的实现 |
| TMP | Template Metaprogramming：模板元编程，利用模板在编译期执行计算 |

### 异常处理

| 词汇 | 说明 |
|------|------|
| `try` | 尝试：标记可能抛出异常的代码块 |
| `catch` | 捕获：捕获 `try` 块中抛出的异常 |
| `throw` | 抛出：主动抛出一个异常对象，函数立即终止，跳到最近的 `catch` |
| `noexcept` | no exception：承诺函数不会抛出异常，编译器可做更多优化 |
| `std::exception` | 标准库所有异常类的基类，`what()` 返回错误描述字符串 |
| `std::runtime_error` | runtime error：运行时错误（如除零、文件不存在）|
| `std::logic_error` | logic error：逻辑错误（程序设计问题，如无效参数）|
| `std::out_of_range` | out of range：越界错误，下标超出有效范围 |
| `std::invalid_argument` | invalid argument：无效参数 |
| `what()` | `std::exception` 的成员函数，返回异常描述（`const char*`）|
| `catch(...)` | 捕获任意类型异常，作为最后的兜底 catch |
| stack unwinding | 栈展开：异常抛出后自动销毁 try 块内已构造的局部对象，保证析构函数被调用 |
| `<stdexcept>` | standard exceptions：包含 `runtime_error`、`logic_error`、`out_of_range` 等 |
