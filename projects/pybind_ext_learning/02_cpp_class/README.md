# 02_cpp_class — 第二步：C++ 类的基础

学会用 class 把数据和函数打包在一起。和 Python 没有任何关系，纯 C++。

## 1. 文件

| 文件               | 说明                             |
|:-----------------|:-------------------------------|
| `stock.h`        | 写法 ① Stock 类声明（头文件 = 菜单）       |
| `stock.cpp`      | 写法 ① Stock 类实现（厨房 = 实际做菜）      |
| `bond.h`         | 写法 ② Bond 类，声明 + 实现全在 .h 里     |
| `main.cpp`       | 写法 ③ Fund 类直接定义在文件里 + 三种写法演示   |
| `CMakeLists.txt` | 多文件 CMake 工程（`add_executable`） |

共 6 套部署方案（MinGW 3 + MSVC 2 + Linux 1）：

| 平台        | 方案                      | 生成器 | build 目录            |
|:----------|:------------------------|:----|:--------------------|
| MinGW A   | Ninja（推荐）               | 单配置 | `build-mingw-ninja` |
| MinGW B   | MinGW Makefiles         | 单配置 | `build-mingw-make`  |
| MinGW C   | Ninja Multi-Config      | 多配置 | `build-mingw-mc`    |
| MSVC A    | vcvarsall + Ninja（推荐）   | 单配置 | `build-msvc-ninja`  |
| MSVC B    | Visual Studio Generator | 多配置 | `build-msvc-vs`     |
| Linux/WSL | cmake + make            | 单配置 | `build/`            |

> **pybind11 注意**：Windows 上的 Python（python.org / Anaconda）都是 MSVC 编译的，pybind11 编译出的 `.pyd` 扩展必须和
> Python 解释器用同一套编译器，否则 ABI 不兼容会崩溃。前面 01、02 步是纯 C++，MinGW/MSVC 都能跑；后面涉及 pybind11 的步骤 MinGW
> 不适用，必须走 MSVC。

---

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/stock_demo.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-mingw-ninja\stock_demo.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/stock_demo.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-mingw-make\stock_demo.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Debug

# 运行
./build-mingw-mc/Debug/stock_demo.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-mingw-mc --config Debug

:: 运行
D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-mingw-mc\Debug\stock_demo.exe
```

</details>

### 2.2 命令行 · MSVC（cmd）

**路径常量（绝对路径版使用）：**

| 工具 | 完整路径 |
|------|------|
| cmake | `D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe` |
| ninja | `D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe` |
| vcvarsall | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat` |
| cl.exe | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\cl.exe` |

> cl.exe 一般不用在命令里指定：`call vcvarsall.bat` 会把它注入 PATH，CMake 自动找到。
> 列出仅供参考；版本号 14.51.36231 随 VS 更新可能变化。

#### vcvarsall 注入的 4 个环境变量

| 变量 | 给谁用 | 找什么 | 示例路径（MSVC 14.51 / Win10 SDK 26100） |
|------|--------|--------|------|
| PATH | cmd 命令 | cl.exe / link.exe / ninja / cmake | `...\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\` |
| INCLUDE | cl.exe（编译器） | 头文件 | `...\VC\Tools\MSVC\14.51.36231\include\`<br>`...\Windows Kits\10\Include\10.0.26100.0\ucrt\` 等 |
| LIB | link.exe（链接器） | .lib 库文件 | `...\VC\Tools\MSVC\14.51.36231\lib\x64\`<br>`...\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64\` 等 |
| LIBPATH | .NET 工具 | 程序集 | 本项目用不到 |

- `call` 必须写在当前 cmd 会话里——`call` 让变量修改留在当前窗口；直接运行则只存在临时进程，退出就没了
- 不激活直接调 cl.exe 会报"找不到头文件"：cl.exe 找到 cl 自身但 INCLUDE 没注入，`#include <iostream>` 无从解析
- link.exe 依赖 LIB 找 `libcmt.lib` 等库文件，LIB 没注入则链接失败

```bat
:: 激活 — 把 cl.exe / link.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```

#### 方案 A — vcvarsall + Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建（Ninja 是单配置生成器，配置时已锁定模式，--build 不需要 --config）
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\stock_demo.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-msvc-ninja\stock_demo.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class

:: 配置（-G "Visual Studio 18 2026" 生成 .sln 工程，-A x64 指定 64 位）
cmake -B build-msvc-vs -G "Visual Studio 18 2026" -A x64 -S .

:: 构建（VS 生成器一个工程含 Debug + Release，必须用 --config 指定；不写默认 Debug）
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\stock_demo.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\projects\pybind_ext_learning\02_cpp_class\build-msvc-vs\Release\stock_demo.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# cd 到项目目录
cd 02_cpp_class

# 配置
mkdir build && cd build
cmake ..

# 构建
cmake --build .

# 运行
./stock_demo
```

---

## 3. CLion IDE

1. `File → Open` 选择 `02_cpp_class/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. 工具栏选择工具链（MinGW 或 Visual Studio）
4. **构建** `Ctrl+F9`**运行** `Shift+F10`

---

## 4. 预期输出

```
========== 第二步：C++ 类的基础 ==========

① 写法 ①  Stock（.h + .cpp 分离）
   代码: 600519.SH, 收盘价: 1500
   set_close(1600) 后: 1600
   涨停价: 1760, 跌停价: 1440

② 写法 ②  Bond（全写在 .h 里）
   名称: 24国债01, 利率: 0.025
   面值 10000 的年利息: 250

③ 写法 ③  Fund（直接写在 main.cpp 里）
   名称: 沪深300ETF, 净值: 3.85
   10000 元买入份额: 2597.4

④ 多个对象:
   共 3 只股票

========== 类基础完成 ==========
```

---

## 5. 栈与堆

物理上都是同一条内存条（RAM），区别是操作系统怎么管理：

```
┌──────────────────────────────────┐  高地址
│            栈（Stack）            │  ← 从高地址往下长
│  系统自动管理，固定大小（约 1-8 MB）│
│  函数调用时压入，返回时弹出         │
│              ↓ 往下长              │
├──────────────────────────────────┤
│           空闲区域                │
├──────────────────────────────────┤
│              ↑ 往上长              │
│            堆（Heap）             │  ← 从低地址往上长
│  程序员手动管理（new/delete）      │
├──────────────────────────────────┤
│         代码 + 全局变量            │
└──────────────────────────────────┘  低地址
```

**为什么栈快：**

| 操作 | 栈             | 堆                      |
|:---|:--------------|:-----------------------|
| 分配 | 栈指针减 8（1 条指令） | 找空闲区域、记录大小、标记占用（几十条指令） |
| 释放 | 栈指针加 8（1 条指令） | 标记释放、合并相邻空闲块（几十条指令）    |
| 类比 | 桌上放东西，伸手就拿    | 去仓库领东西，要登记、找货架         |

**和 std::move 的关系：**

```
double m_nav = 3.85;
  → 8 字节直接存在栈上，拷贝就是复制 8 字节，已经最快，move 没意义

std::string m_name = "沪深300ETF";
  → 栈上存指针+长度+容量，实际字符数组在堆上
  → 拷贝要在堆上新开内存、逐字节复制（慢）
  → move 只把指针转给新主人，不动堆内存（快）
```

基本类型（int/double/bool）不需要 move，string/vector/map 等管理堆内存的类型用 move 更快。

---

## 6. 定义类的三种写法

| 写法              | 本项目示例                      | 适用场景                |
|:----------------|:---------------------------|:--------------------|
| .h 声明 + .cpp 实现 | Stock（stock.h + stock.cpp） | 多文件共用，改实现只重编一个 .cpp |
| 全写在 .h 里        | Bond（bond.h）               | 模板类必须这样；小类也常用       |
| 直接写在 .cpp 里     | Fund（main.cpp 里）           | 只有当前文件用，不需要拆文件      |

`.h` 里能写实现，不是只能写声明——"只写声明"是约定习惯，不是语法限制。`#include` 本质就是把 `.h` 的内容复制粘贴到 `.cpp` 里。

---

## 7. 本步要点

| 概念               | 说明                                  |
|:-----------------|:------------------------------------|
| class            | 把数据 + 操作数据的方法打包在一起                  |
| 构造函数             | 创建对象时自动调用，必须和类同名                    |
| 成员变量             | 对象里的数据（默认 private）                  |
| 成员函数             | 对象能做的事                              |
| public / private | 控制外部能否访问                            |
| .h / .cpp 分离     | 头文件声明"有什么"，cpp 实现"怎么做"              |
| 初始化列表            | `: m_code(...)` 构造时直接初始化，比在 {} 里赋值快 |
| const & 参数       | 只读引用，不拷贝不修改                         |
| 末尾 const         | 函数不修改成员变量                           |
| std::move        | 转移所有权，避免拷贝字符串                       |

---

## 8. 英文及缩写说明

| 词汇                | 说明                                       |
|:------------------|:-----------------------------------------|
| class             | 类，定义对象的蓝图/模具                             |
| object / instance | 对象/实例，按类创建的具体东西                          |
| constructor       | 构造函数，创建对象时自动调用                           |
| member            | 成员（成员变量 = 数据，成员函数 = 方法）                  |
| public            | 公有，外部可访问                                 |
| private           | 私有，只有类内部能访问                              |
| `::`              | 作用域运算符，`Stock::code` 表示 Stock 类的 code 函数 |
| `std::move`       | 移动语义，转移资源所有权而非拷贝                         |
| initializer list  | 初始化列表，`:` 后面直接初始化成员变量                    |
