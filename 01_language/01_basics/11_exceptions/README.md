# 11_exceptions — 异常处理

`try`/`catch`/`throw`、标准异常层次、自定义异常、`noexcept`、RAII 异常安全。

## 1. 文件

| 文件               | 说明                                                                     |
|------------------|------------------------------------------------------------------------|
| `main.cpp`       | 6 个 demo：基本 try/catch / 标准异常层次 / 自定义异常 / noexcept / RAII+异常 / catch 顺序 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `exceptions`                                              |

---

## 2. 命令行 · MinGW（Git Bash）

### 方案 A — Ninja

```bash
cmake -G Ninja -B build-mingw-ninja -S .
cmake --build build-mingw-ninja
./build-mingw-ninja/exceptions.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-ninja\exceptions.exe
```

</details>

### 方案 B — MinGW Makefiles

```bash
cmake -G "MinGW Makefiles" -B build-mingw-make -S .
cmake --build build-mingw-make
./build-mingw-make/exceptions.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-make
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-make\exceptions.exe
```

</details>

### 方案 C — Ninja Multi-Config

```bash
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .
cmake --build build-mingw-mc --config Release
./build-mingw-mc/Release/exceptions.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-mc --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-mingw-mc\Release\exceptions.exe
```

</details>

## 3. 命令行 · MSVC（cmd）

### 方案 A — vcvarsall + Ninja

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B build-msvc-ninja -S .
cmake --build build-msvc-ninja
build-msvc-ninja\exceptions.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-msvc-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-msvc-ninja\exceptions.exe
```

</details>

### 方案 B — VS Generator

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .
cmake --build build-msvc-vs --config Release
build-msvc-vs\Release\exceptions.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-msvc-vs --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\11_exceptions\build-msvc-vs\Release\exceptions.exe
```

</details>

## 4. 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/exceptions
```

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
