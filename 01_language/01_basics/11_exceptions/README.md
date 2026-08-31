# 11_exceptions — 异常处理

`try`/`catch`/`throw`、标准异常层次、自定义异常、`noexcept`、RAII 异常安全。

## 1. 文件

| 文件               | 说明                                                                     |
|------------------|------------------------------------------------------------------------|
| `main.cpp`       | 6 个 demo：基本 try/catch / 标准异常层次 / 自定义异常 / noexcept / RAII+异常 / catch 顺序 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `exceptions`                                              |

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
./build-mingw-ninja/exceptions.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 运行
build-mingw-ninja\exceptions.exe
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

build-msvc-ninja\exceptions.exe
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
build-msvc-ninja\exceptions.exe
```

---

## 4. CLion IDE

1. `File → Open` 选择 `11_exceptions/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. **构建** `Ctrl+F9`**运行** `Shift+F10`

---

## 5. 英文及缩写说明

| 词汇                      | 说明                                                                                       |
|-------------------------|------------------------------------------------------------------------------------------|
| `try`                   | 尝试：标记可能抛出异常的代码块                                                                          |
| `catch`                 | 捕获：捕获 try 块中抛出的指定类型异常；多个 catch 从具体到通用排列                                                  |
| `throw`                 | 抛出：主动抛出异常对象，函数立即终止跳到最近的 catch；`throw;`（无参）重新抛出当前异常                                       |
| `noexcept`              | no exception：承诺函数不抛出异常；若实际抛出则 `std::terminate()` 直接终止；移动构造标 noexcept，vector 扩容时才会用移动而非拷贝 |
| `std::exception`        | 所有标准异常的基类；`.what()` 返回错误描述字符串（`const char*`）；需要 `#include <stdexcept>`                   |
| `std::runtime_error`    | runtime error：运行时错误，程序运行时才能检测到                                                           |
| `std::logic_error`      | logic error：逻辑错误，程序设计问题，理论上在开发期可避免                                                       |
| `std::out_of_range`     | out of range：越界访问；`std::vector::at()` 越界时抛出此异常                                           |
| `std::invalid_argument` | invalid argument：传入了不合法的参数                                                               |
| `catch(...)`            | 捕获任意类型异常（省略号语法），作为最后的兜底 catch                                                            |
| stack unwinding         | 栈展开：异常抛出后，C++ 运行时自动销毁 try 块内已构造的局部对象（调用析构函数），保证资源释放                                      |
| RAII                    | Resource Acquisition Is Initialization：资源获取即初始化，析构时自动释放；即使抛出异常析构也会执行，是异常安全的基础            |
| exception safety        | 异常安全：代码在异常路径下不泄漏资源、不留下不一致状态的能力                                                           |
| `<stdexcept>`           | standard exceptions：包含 `runtime_error`、`logic_error`、`out_of_range`、`invalid_argument` 等 |
