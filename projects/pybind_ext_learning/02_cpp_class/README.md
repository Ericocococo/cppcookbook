# 02_cpp_class — 第二步：C++ 类的基础

学会用 class 把数据和函数打包在一起。和 Python 没有任何关系，纯 C++。

## 1. 文件

| 文件 | 说明 |
|:---|:---|
| `stock.h` | Stock 类声明（头文件 = 菜单） |
| `stock.cpp` | Stock 类实现（厨房 = 实际做菜） |
| `main.cpp` | 创建对象、调方法、多文件编译 demo |
| `CMakeLists.txt` | 多文件 CMake 工程（`add_executable`） |

---

## 2. 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"
MAKE="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"
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
./build-mingw-ninja/stock_demo.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 运行
build-mingw-ninja\stock_demo.exe
```

### 方案 B：MinGW Makefiles（无需 ninja.exe）

```bash
# 配置
"$CMAKE" -B build-mingw-make -G "MinGW Makefiles" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$MAKE"

# 构建
"$CMAKE" --build build-mingw-make

# 运行
./build-mingw-make/stock_demo.exe
```

---

## 3. 命令行 · MSVC（cmd）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

:: 激活 — 把 cl.exe / link.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "%VCVARSALL%" x64
```

### 方案 A：vcvarsall + Ninja（推荐，单配置）

```bat
:: 配置
"%CMAKE%" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建（Ninja 是单配置生成器，配置时已锁定模式，--build 不需要 --config）
"%CMAKE%" --build build-msvc-ninja

:: 运行
build-msvc-ninja\stock_demo.exe
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
build-msvc-ninja\stock_demo.exe
```

### 方案 B：Visual Studio 生成器（多配置）

```bat
:: 配置（-G "Visual Studio 18 2026" 生成 .sln 工程，-A x64 指定 64 位）
"%CMAKE%" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64

:: 构建（VS 生成器一个工程含 Debug + Release，必须用 --config 指定；不写默认 Debug）
"%CMAKE%" --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\stock_demo.exe
```

> 四步版：

```bat
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\stock_demo.exe
```

---

## 4. 命令行 · Linux / WSL

```bash
cd 02_cpp_class
mkdir build && cd build
cmake ..
cmake --build .
./stock_demo
```

---

## 5. CLion IDE

1. `File → Open` 选择 `02_cpp_class/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. 工具栏选择工具链（MinGW 或 Visual Studio）
4. **构建** `Ctrl+F9`　**运行** `Shift+F10`

---

## 6. 预期输出

```
========== 第二步：C++ 类的基础 ==========

① 股票代码: 600519.SH
   收盘价: 1500

② set_close(1600) 后: 1600

③ 涨停价: 1760, 跌停价: 1440

④ 多个对象:
   000001.SZ close=12.5
   共 3 只股票

========== 类基础完成 ==========
```

---

## 7. 本步要点

| 概念 | 说明 |
|:---|:---|
| class | 把数据 + 操作数据的方法打包在一起 |
| 构造函数 | 创建对象时自动调用，必须和类同名 |
| 成员变量 | 对象里的数据（默认 private） |
| 成员函数 | 对象能做的事 |
| public / private | 控制外部能否访问 |
| .h / .cpp 分离 | 头文件声明"有什么"，cpp 实现"怎么做" |
| 初始化列表 | `: m_code(...)` 构造时直接初始化，比在 {} 里赋值快 |
| const & 参数 | 只读引用，不拷贝不修改 |
| 末尾 const | 函数不修改成员变量 |
| std::move | 转移所有权，避免拷贝字符串 |

---

## 8. 英文及缩写说明

| 词汇 | 说明 |
|:---|:---|
| class | 类，定义对象的蓝图/模具 |
| object / instance | 对象/实例，按类创建的具体东西 |
| constructor | 构造函数，创建对象时自动调用 |
| member | 成员（成员变量 = 数据，成员函数 = 方法） |
| public | 公有，外部可访问 |
| private | 私有，只有类内部能访问 |
| `::` | 作用域运算符，`Stock::code` 表示 Stock 类的 code 函数 |
| `std::move` | 移动语义，转移资源所有权而非拷贝 |
| initializer list | 初始化列表，`:` 后面直接初始化成员变量 |
