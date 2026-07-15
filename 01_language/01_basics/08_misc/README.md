# 08_misc — 枚举、命名空间、类型转换

演示 `enum`/`enum class`、`namespace`、四种 C++ 类型转换运算符。

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 3 个 demo 函数：枚举 → 命名空间 → 类型转换 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `misc` |

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
./build-mingw/misc.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw

:: 运行
build-mingw\misc.exe
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

build-msvc\misc.exe
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
build-msvc\misc.exe
```

---

## 4. CLion IDE

1. `File → Open` 选择 `08_misc/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. **构建** `Ctrl+F9`　**运行** `Shift+F10`

---

## 5. 英文及缩写说明

### 枚举

| 词汇 | 说明 |
|------|------|
| `enum` | Enumeration：枚举，定义一组具名整数常量；名字暴露在外部命名空间，可能冲突 |
| `enum class` | 枚举类（C++11，推荐）：名字限定在枚举类内（需写 `Color::RED`），类型安全 |
| enumerator | 枚举值：枚举中的每个具名常量，如 `RED`、`GREEN` |
| underlying type | 底层类型：枚举实际存储的整数类型，默认 `int`，可指定 `enum class E : uint8_t {}` |
| `uint8_t` | unsigned integer 8-bit type：无符号 8 位整数（0~255），定义在 `<cstdint>` |

### 命名空间

| 词汇 | 说明 |
|------|------|
| `namespace` | 命名空间：将名字划分到独立作用域，避免不同库之间的名字冲突 |
| `using` | 引入声明：`using math::add` 引入单个名字；`using namespace std` 引入整个命名空间 |
| `std` | Standard：C++ 标准库的命名空间 |
| `::` | scope resolution operator：作用域解析运算符，访问命名空间或类的成员 |
| nested namespace | 嵌套命名空间：C++17 可写 `namespace A::B::C {}` |

### 类型转换

| 词汇 | 说明 |
|------|------|
| `static_cast` | static cast：编译期类型转换，最安全最常用；整数/浮点/枚举互转 |
| `dynamic_cast` | dynamic cast：运行时类型检查转换；多态向下转型，失败返回 `nullptr` |
| `const_cast` | const cast：去除或添加 `const`/`volatile` 限定；修改真正 const 变量是未定义行为 |
| `reinterpret_cast` | reinterpret cast：将内存二进制直接重新解释为另一种类型；非常危险，几乎不用 |
| implicit conversion | 隐式转换：编译器自动进行的类型转换，可能导致精度丢失 |
| narrowing conversion | 窄化转换：从大范围转到小范围（`double` → `int`），列表初始化 `{}` 会拒绝 |
