# 习惯约定

> 本仓库在编写/填充过程中形成的习惯，和 [目录设计文档](STRUCTURE.md) 里的
> "贯穿全库的约定"互补——那边讲**结构规则**，这边讲**动笔时的习惯**。

---

## 术语解释规范

专业名词、英文缩写必须解释，让读者不查文档也能读懂。解释方式按场景选一种：

| 场景       | 写法                               | 示例                                               |
|----------|----------------------------------|--------------------------------------------------|
| 一句话能说清   | 行内注释                             | `// RAII：资源获取即初始化`                               |
| 需要多句说明   | `.cpp` 文件顶部注释块                   | 见下方模板                                            |
| 目录/库的术语表 | 该目录的 `README.md` 末尾 `## 术语速查` 表格 | 见 [01_language/README.md](01_language/README.md) |
| 全局通用缩写   | 本约定末尾 `## 全局术语表`                 | 见下方                                              |

**`.cpp` 文件顶部注释模板：**

```cpp
// C++20 | 依赖：<库名>
// 演示：<一句话说明演示内容>
// <专业名词>：<英文全称> — <中文解释>
// 构建：<构建命令>
```

### 代码中英文词汇的注释规则

面向小白读者，代码里出现的**英文关键字、标准库名称、API 名**，凡是名字本身不能自解释的，必须在行内注释中加中文说明。判断标准：初学者看到这个英文名，能否在
3 秒内猜出它的作用？不能就加注释。

**需要注释的典型场景：**

```cpp
// ① 标准库 API —— 函数名不直白的
auto p = std::make_unique<int>(10);   // make_unique：创建 unique_ptr（独占所有权的智能指针）
v.emplace_back(1, 2);                 // emplace_back：直接在末尾原地构造，比 push_back 少一次拷贝
s.shrink_to_fit();                    // shrink_to_fit：释放多余的预分配内存

// ② C++ 关键字 —— 初学者可能不熟悉的
int&& rref = std::move(x);           // &&：右值引用；move：转移所有权，x 之后不可再用
explicit Foo(int x);                 // explicit：禁止隐式类型转换，只能显式调用构造函数
[[nodiscard]] int compute();         // nodiscard：调用者若不使用返回值，编译器发出警告

// ③ 英文缩写 —— 看不出全称的
size_t n = v.size();                 // size_t：无符号整数类型，专门用于表示大小/长度
nullptr                              // nullptr：空指针常量（C++11），类型安全，替代 NULL/0

// ④ 模板/STL 术语
typename T                           // typename：占位符，T 可以是任意类型
std::forward<T>(arg)                 // forward：完美转发，保留实参的值类别（左值/右值）
```

**不需要注释的场景**（名字已经足够直白）：

```cpp
v.push_back(x);    // 不需要：push back = 推到后面，一看就懂
v.size();          // 不需要：size = 大小
v.empty();         // 不需要：empty = 是否为空
std::cout << x;    // 不需要：cout = 输出流
```

---

## 目录与结构

1. **用到再建，不建空壳** — 目录只在有文件时才创建，不预先遗留空目录占位
   （空目录 git 本来也不跟踪）。设计文档勾勒的是"规划"，不代表磁盘上都得建出来。
2. **所有层级目录带两位数字前缀**（`01_ 02_ ...`），阅读顺序即学习/工程顺序；
   工程约定目录（`tests/`、`.github/`）按惯例不加号。
3. **一个库/主题一个子目录，内部按功能拆多个带序号 `.cpp`** —
   以 `03_stl/01_containers/` 为范例（vector/map/queue/...），
   **不平铺成一个大文件**。
4. **子目录名要说明内容；缩写和非一般词在 `README.md` 里解释** —
   如 `01_sfinae`（SFINAE = Substitution Failure Is Not An Error）、
   `03_crtp`（CRTP = Curiously Recurring Template Pattern）。
5. **每个主题都放在自己的子目录里**，不直接把 `.cpp` 散放在上级目录 —
   以 `01_language/04_modern/01_move_semantics/` 为范例。
6. **每个含 `.cpp` 的目录（含叶子目录）都必须有 `README.md`**，中间层目录不加 —
   上级目录的 `README.md` 列出子目录一行摘要；叶子目录的 `README.md` 包含以下五个带序号的 `##` 章节：
    - **`## 1. 文件`**（或 `## 1. 知识点`）：列出文件用途或知识点总览
    - **`## 2. 命令行 · MinGW（Git Bash）`**：方案 A（Ninja）+ 方案 B（MinGW Makefiles）+ 方案 C（Ninja Multi-Config），每个附 cmd 三步版
    - **`## 3. 命令行 · MSVC（cmd）`**：方案 A（vcvarsall + Ninja）+ 方案 B（VS Generator），每个附四步版
    - **`## 4. 命令行 · Linux / WSL`**：Ninja 构建
    - **`## 5. CLion IDE`**：File→Open→加载→Ctrl+F9/Shift+F10
    - **`## 6. 英文及缩写说明`**（可选）：本文件代码中出现的英文关键字、API 名、缩写，逐条列表解释
    - pybind11 项目 MinGW 不适用时，该章节保留标题并注明原因
7. **英文缩写、缩写、非一般词必须解释** — 包括英文缩写（`RAII`/`CRTP`/`TMP`/`SFINAE`/`UB`/`ABI`），
   中文缩写、框架内术语（`ECS`/`DDS`）等，让读者不查文档也能读懂。
   解释位置：短的一句话放行内注释，较长放 `README.md` 里。

## 命令行工具使用

本机有两套工具路径体系，写命令时必须明确选一种，不要混用。

### 方式一：加 PATH（短命令，前提是系统 PATH 已含对应目录）

CLion 安装后自动将以下路径写入系统 PATH，打开任意终端即可直接使用短命令：

```
D:\ProgramData\JetBrains\CLion20260101\bin\mingw\bin        ← g++, gcc, mingw32-make
D:\ProgramData\JetBrains\CLion20260101\bin\ninja\win\x64    ← ninja
D:\Program Files\Microsoft Visual Studio\18\Community\
    Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin  ← cmake 4.3.1-msvc1（优先）
D:\ProgramData\JetBrains\CLion20260101\bin\cmake\win\x64\bin ← cmake 4.2.2（被上条覆盖）
D:\software\vcpkg                                           ← vcpkg
```

```bash
# Git Bash / cmd 直接用
g++ -std=c++20 main.cpp -o app.exe
cmake -B build -G Ninja
ninja -C build
```

> **例外**：`cl.exe`（MSVC 编译器）永远不在 PATH，必须先运行 `vcvarsall.bat` 激活。

---

### 方式二：不加 PATH（完整路径，不依赖系统 PATH 配置）

适合脚本、CI、或需要精确指定版本的场景。在命令中直接写绝对路径：

**Git Bash（MinGW 工具链）：**

```bash
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
CMAKE="D:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"
MAKE="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

"$GXX" -std=c++20 main.cpp -o app.exe
"$CMAKE" -B build -G Ninja -DCMAKE_CXX_COMPILER="$GXX"
"$CMAKE" --build build
```

**cmd（MSVC 工具链）：**

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

:: cl.exe 必须通过 vcvarsall.bat 激活，即使用完整路径也无法绕过（头文件/库路径由它注入）
call "%VCVARSALL%" x64

:: 激活后 cmake 自动检测 cl.exe
"%CMAKE%" -B build -G Ninja
"%CMAKE%" --build build
```

> `D:\software\cmake-4.3.3` 和 `D:\software\mingw64` 是本机自装版本，**不使用**，不写进任何命令或路径。

---

### README 部署命令标准格式

所有含构建步骤的 README 必须按以下结构写，参考完整范例：[
`10_ops/01_build_systems/01_hello/README.md`](10_ops/01_build_systems/01_hello/README.md)。

**路径常量（直接复制，不要改）：**

| 变量                | 路径                                                                                                                       |
|-------------------|--------------------------------------------------------------------------------------------------------------------------|
| `CMAKE`（MinGW 段用） | `D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe`                                                 |
| `GXX`             | `D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe`                                                           |
| `NINJA`           | `D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe`                                                     |
| `MAKE`            | `D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe`                                                  |
| `CMAKE`（MSVC 段用）  | `D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe` |
| `VCVARSALL`       | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat`                                 |
| MSVC Ninja        | `D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe`     |

#### MinGW 段模板（`## 2. 命令行 · MinGW（Git Bash）`）

````markdown
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
./build-mingw-ninja/<exe名>.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 运行
build-mingw-ninja\<exe名>.exe
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
./build-mingw-make/<exe名>.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-make -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-make

:: 运行
build-mingw-make\<exe名>.exe
```

### 方案 C：Ninja Multi-Config（一目录同时支持 Debug / Release）

```bash
# 配置
"$CMAKE" -B build-mingw-mc -G "Ninja Multi-Config" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA"

# 构建 Debug（多配置，构建时必须指定 --config）
"$CMAKE" --build build-mingw-mc --config Debug

# 运行 Debug
./build-mingw-mc/Debug/<exe名>.exe

# 构建 Release
"$CMAKE" --build build-mingw-mc --config Release

# 运行 Release
./build-mingw-mc/Release/<exe名>.exe
```
````

#### MSVC 段模板（`## 3. 命令行 · MSVC（cmd）`）

````markdown
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

:: 构建（Ninja 是单配置生成器，不需要 --config）
"%CMAKE%" --build build-msvc-ninja

:: 运行
build-msvc-ninja\<exe名>.exe
```

> 四步版（激活、配置、构建、运行各一行，全部内联路径，无变量依赖）：

```bat
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja

:: 运行
build-msvc-ninja\<exe名>.exe
```

### 方案 B：Visual Studio 生成器（多配置，无需激活 vcvarsall）

CMake 自动通过 `vswhere.exe` 检测 MSVC 工具链，生成 `.sln` 工程。VS Generator 是多配置，构建时须指定 `--config`，exe 输出到 `build-msvc-vs\Debug\` 或 `Release\` 子目录下。

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe

:: 配置（-A x64 指定目标 64 位，不加默认为 Win32）
"%CMAKE%" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64

:: 构建（多配置，必须指定 --config）
"%CMAKE%" --build build-msvc-vs --config Debug
```

> 三步版：

```bat
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-vs -G "Visual Studio 18 2026" -A x64

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-vs --config Debug
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Debug\<exe名>.exe
build-msvc-vs\Release\<exe名>.exe
```

### 方案 C：x64 Native Tools + Ninja（无需手动激活）

开始菜单搜索 `x64 Native Tools Command Prompt for VS 2026` 打开，窗口已自动注入环境。进去后三步运行：

```bat
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build-msvc-ninja -G Ninja -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build-msvc-ninja

:: 运行
build-msvc-ninja\<exe名>.exe
```
````

#### 生成器对比

|          | MinGW 方案 A<br>Ninja | MinGW 方案 B<br>MinGW Makefiles | MinGW 方案 C<br>Ninja Multi-Config |
|----------|---------------------|-------------------------------|----------------------------------|
| build 目录 | `build-mingw-ninja` | `build-mingw-make`            | `build-mingw-mc`                 |
| 构建工具     | ninja.exe           | mingw32-make.exe              | ninja.exe                        |
| 配置数      | 单配置                 | 单配置                           | 多配置（Debug/Release 共目录）           |
| 速度       | 最快                  | 慢                             | 快                                |
| 适用场景     | 日常首选                | 没有 ninja.exe 时备选              | 需频繁切换 Debug/Release              |

|          | MSVC 方案 A<br>vcvarsall + Ninja | MSVC 方案 B<br>VS Generator | MSVC 方案 C<br>Native Tools + Ninja |
|----------|--------------------------------|---------------------------|-----------------------------------|
| build 目录 | `build-msvc-ninja`             | `build-msvc-vs`           | `build-msvc-ninja`                |
| 需要激活     | 是（call vcvarsall）              | 否（CMake 自动检测）             | 否（窗口已激活）                          |
| 构建工具     | ninja.exe                      | msbuild.exe               | ninja.exe                         |
| 配置数      | 单配置（构建不用 `--config`）           | 多配置（构建必须 `--config`）      | 单配置                               |
| exe 路径   | `build-msvc-ninja\`            | `build-msvc-vs\Debug\`    | `build-msvc-ninja\`               |
| 适用场景     | 脚本/自动化首选                       | 不手动激活时用                   | 交互式操作                             |

#### Linux / WSL 段模板（`## 4. 命令行 · Linux / WSL`）

````markdown
```bash
cd <目录名>
mkdir build && cd build
cmake ..
cmake --build .
./<exe名>
```
````

> pybind11 项目需指定 pybind11 和 Python 路径：
>
> ```bash
> cmake -Dpybind11_DIR=$(python -c "import pybind11;print(pybind11.get_cmake_dir())") \
>       -DPython_EXECUTABLE=$(which python) ..
> ```
>
> pybind11 模块（.so）同样必须使用与 Python 相同的编译器。Linux 上的 Python 由 GCC 编译，因此 .so 用 GCC 构建即可。

#### CLion IDE 段模板（`## 5. CLion IDE`）

````markdown
1. `File → Open` 选择 `<目录名>/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. 工具栏选择工具链（MinGW 或 Visual Studio）
4. **构建** `Ctrl+F9`　**运行** `Shift+F10`
````

---

## 新章节模板（创建新知识点目录的 SOP）

新建一个知识点目录时，按以下三个文件模板创建，然后照 checklist 逐项检查。

### main.cpp 模板

```cpp
// C++20
// 演示：<一句话说明本文件演示什么>
// 构建：cmake -B build-mingw-ninja（详见 README.md）

#include <iostream>
// #include <其他需要的头文件>

// ============================================================
// ① <第一组主题名>
// ============================================================

// 知识点 1.1：<标题>
// <1-3 行说明：这是什么、为什么要用、不用会怎样>

<代码>

// 知识点 1.2：<标题>
// <说明>

<代码>

// ============================================================
// ② <第二组主题名>
// ============================================================

// 知识点 2.1：<标题>
// <说明>

<代码>

int main()
{
    std::cout << "=== <目录名>: <主题> ===\n";
    demo01_xxx();
    demo02_xxx();
    std::cout << "\n完成。\n";
    return 0;
}
```

**注释规则：**
- 每个知识点注释第一行格式：`// 知识点 X.Y：标题`（X = demo 序号，Y = 该 demo 内的知识点序号）
- 每组知识点注释后面空一行再写代码
- 不同知识点之间用空行隔开
- 注释要详细：参数含义、为什么这样写、不这样写会怎样
- 注释只描述当前状态，不引用其他章节

### README.md 模板

```markdown
# <目录名> — <主题名>

> <一句话介绍>

## 1. 知识点

### 1.1 <第一组主题>

#### 1.1.1 <标题>

<1-3 句中文说明>

\```cpp
// 独立可运行的代码示例，不看 main.cpp 也能学会
<代码片段>
\```

#### 1.1.2 <标题>

<说明>

\```cpp
<代码片段>
\```

### 1.2 <第二组主题>

#### 1.2.1 <标题>

...

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

**路径常量（绝对路径版使用）：**

| 工具 | 完整路径 |
|------|------|
| cmake | `D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe` |
| g++ | `D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe` |
| ninja | `D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe` |
| mingw32-make | `D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe` |

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

\```bash
# cd 到项目目录
cd <完整目录路径>

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/<EXE>.exe
\```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

\```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B <完整目录路径>\build-mingw-ninja -S <完整目录路径> -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build <完整目录路径>\build-mingw-ninja

:: 运行
<完整目录路径>\build-mingw-ninja\<EXE>.exe
\```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

\```bash
# cd 到项目目录
cd <完整目录路径>

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/<EXE>.exe
\```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

\```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B <完整目录路径>\build-mingw-make -S <完整目录路径> -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build <完整目录路径>\build-mingw-make

:: 运行
<完整目录路径>\build-mingw-make\<EXE>.exe
\```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

\```bash
# cd 到项目目录
cd <完整目录路径>

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/<EXE>.exe
\```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

\```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B <完整目录路径>\build-mingw-mc -S <完整目录路径> -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build <完整目录路径>\build-mingw-mc --config Release

:: 运行
<完整目录路径>\build-mingw-mc\Release\<EXE>.exe
\```

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

\```bat
:: 激活 — 把 cl.exe / link.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
\```

#### 方案 A — vcvarsall + Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

\```cmd
:: cd 到项目目录
cd /d <完整目录路径>

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\<EXE>.exe
\```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

\```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B <完整目录路径>\build-msvc-ninja -S <完整目录路径> -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build <完整目录路径>\build-msvc-ninja

:: 运行
<完整目录路径>\build-msvc-ninja\<EXE>.exe
\```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

\```cmd
:: cd 到项目目录
cd /d <完整目录路径>

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\<EXE>.exe
\```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

\```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B <完整目录路径>\build-msvc-vs -S <完整目录路径>

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build <完整目录路径>\build-msvc-vs --config Release

:: 运行
<完整目录路径>\build-msvc-vs\Release\<EXE>.exe
\```

</details>

### 2.3 命令行 · Linux / WSL

\```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/<EXE>
\```

## 3. 英文及缩写说明（可选）

### 3.1 <分类名>

| 词汇 | 说明 |
|------|------|
| ... | ... |
```

**README 格式规则：**
- 三级编号体系：`## 1. 知识点` → `### 1.1 大组` → `#### 1.1.1 具体知识点`
- `§ 2 构建` 合并为一章：`### 2.1 MinGW` / `### 2.2 MSVC` / `### 2.3 Linux`
- `§ 3 英文及缩写说明` 用 `### 3.1` 分组
- 知识点编号和 main.cpp 的 `// 知识点 X.Y：` 对应（README 的 `1.1.1` 对应 cpp 的 `知识点 1.1`）
- 每个知识点用 `####` 四级标题（不是加粗），必须有独立代码示例
- 构建命令每个方案两版折叠：`相对路径版（需先 cd）` + `绝对路径X步版（可任意目录粘贴）`
- **绝对路径版：涉及到的路径全部用绝对路径**，包括 cmake.exe、g++.exe、ninja.exe、mingw32-make.exe、vcvarsall.bat 等工具路径（参考 § 2.1 / § 2.2 下的路径常量表）
- 相对路径版：工具用短命令（假设在 PATH），只有 cd 用绝对路径
- 所有代码块内命令加步骤注释（bash 用 `#`，cmd 用 `::`）和空行分隔
- `<完整目录路径>` 替换为实际绝对路径
- `<EXE>` 替换为 CMakeLists.txt 里 `add_executable` 的名字

### CMakeLists.txt 模板

```cmake
cmake_minimum_required(VERSION 3.28)
project(<项目名> LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(<项目名> main.cpp)
```

### 创建新章节 Checklist

创建一个新知识点目录时，按顺序检查：

- [ ] 目录名带两位数字前缀（如 `05_functions`）
- [ ] `CMakeLists.txt`：按模板创建，项目名和目录名对应
- [ ] `main.cpp`：文件头注释（C++ 版本 + 演示内容 + 构建命令）
- [ ] `main.cpp`：每个知识点用 `// 知识点 X.Y：标题` 格式，后面空一行
- [ ] `main.cpp`：注释详细（参数含义、为什么这样写、不这样写会怎样）
- [ ] `main.cpp`：demo 函数按 `demo01_xxx()`、`demo02_xxx()` 命名
- [ ] `README.md`：§ 1 知识点用三级编号（`## 1.` → `### 1.1` → `#### 1.1.1`），`####` 四级标题，每个有代码示例
- [ ] `README.md`：§ 2 构建合并为一章（`### 2.1 MinGW` / `### 2.2 MSVC` / `### 2.3 Linux`）
- [ ] `README.md`：每个方案两版折叠（相对路径版 + 绝对路径 X 步版），代码块内加步骤注释和空行
- [ ] `README.md`：§ 3 英文及缩写说明用 `### 3.1` 分组（可选）
- [ ] 上级目录 README 更新子目录表
- [ ] STRUCTURE.md 目录树同步

---

## CMake 结构

8. **每个目录都是独立的 CMake 工程** — 任意层级的子目录均可在 CLion 中直接打开为独立项目，
   互不依赖，没有根目录聚合所有子目录的主工程。
9. **每个含 `.cpp` 的目录都有自己的 `CMakeLists.txt`** — 完全自包含，开箱即编译，
   无需上级配置。叶子目录标准模板：

   ```cmake
   cmake_minimum_required(VERSION 3.28)
   project(示例名 LANGUAGES CXX)

   set(CMAKE_CXX_STANDARD 20)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)
   set(CMAKE_CXX_EXTENSIONS OFF)

   if(MSVC)
       add_compile_options(
           /W4      # 警告级别 4（/W0=无，/W1-/W4 递增，最高常用级）
           /utf-8   # 源文件和执行字符集均为 UTF-8，避免中文乱码
       )
   else()
       add_compile_options(
           -Wall        # GCC 高价值警告集合（非全部，名字有误导）
           -Wextra      # Wall 之外的额外警告
           -Wpedantic   # 严格 C++ 标准，报所有 GCC 扩展用法
       )
   endif()

   add_executable(示例名 main.cpp)
   ```

10. **需要三方库时，在本目录 `CMakeLists.txt` 里加 `find_package` / `target_link_libraries`**，
    并在 `vcpkg.json`（如有）里声明依赖；不依赖根目录的 `vcpkg.json`。

## 代码规范

11. **纯语法用可运行 `.cpp` + `main()` 展示** — 每个文件能直接编译运行，
    打印运行结果与目测结论（不用 `assert`）；出错时用 `return 1` 或 `std::exit(1)` 加打印说明，
    不让程序崩溃中断。
12. **打印显示要直观** — 结构体逐字段打印，并带上类型或说明：
    `std::cout << "  x: " << x << " (int)\n";`
    不要一行 dump 整个对象；多个用例用 `① ② ③` 小标题分隔。
13. **每段演示封装成 `demoXX_xxx()` 函数** — 如 `demo01_basic()` / `demo02_ranges()`，
    序号对应注释里的 ① ② ③；`main()` 只留串行调用，实现都在上面。
14. **demo 产出落到 `data/` 目录**（已被 `.gitignore` 忽略），不用 tmpfile。
15. **文件顶部注释写清楚**：所属标准库/三方库、C++ 标准版本（如 `// C++20`）、编译/安装方式。
16. **注释使用中文，只描述当前状态** — 不引用前面章节（不写"和 vector 一样""前面讲过"），不写操作记录。每个知识点独立解释，保证跳过前面章节也能读懂。
17. **给出的 demo 必须是可执行的**，假定实际跑一遍确认输出正常后完成。
18. **目标受众是 C++ 小白** — 注释要解释"为什么"和"什么意思"，不只写"做了什么"；
    每个语法点附一句话说明常见陷阱或与直觉不符的行为（如整数除法截断、前后缀自增区别）。
    `README.md` 作详细原理解释，`.cpp` 是可运行的配套示例代码。
19. **代码优先简单易懂写法** —
    - for 循环体必须加花括号，即使只有一行
    - 防御性检查（如 `empty()`）提前 return，不要放在三元运算符里
    - 不在 return 中使用三元运算符，拆成 if/else
    - 不做多余的 `static_cast`（如 `double / size()` 已自动浮点除法）
    - **一行一条语句**，不把多条逻辑压在同一行
20. **`auto` 使用规则** —
    - 简单类型**显式写**：`size_t dot = symbol.find('.')`，不用 `auto`
    - 右侧类型很长且函数名已暗示类型时**用 auto**：`auto it = m_data.find(key)`（迭代器）、
      `auto table = ReadParquetTable(...)`（智能指针/Arrow）
    - 结构化绑定必须 `auto`：`auto [code, market] = split_symbol(...)`
    - 原则：读者能一眼看出类型就无所谓，看不出来就显式写

## 命名

21. **项目名 `cppcookbook`**（不是 cpp-cookbook）。
22. **别含顶层命名为 `cpp` 或 `test`** — 会与 ctest 或标准库 `<test>` 混淆概念。

## Git 提交信息

**生成提交说明前**：同时运行 `git diff` 和 `git diff --cached`，把所有改动（含删除文件）一起看，不遗漏。

```
<type>(<scope>): <subject>

<body>
```

**subject（首行）**

- 格式：`<type>(<scope>): <描述>`
- scope 可选，填改动所在目录名去掉数字前缀，如 `ops`、`stl`、`language`
- 描述用中文，动词开头，不加句号，不超过 72 字

**body（正文）**

- 可选，与 subject 空一行隔开
- 说清楚"为什么"，不复述"改了什么"（diff 已经说明了改了什么）
- 每行不超过 72 字

| type         | 含义              | 例子                           |
|--------------|-----------------|------------------------------|
| **feat**     | 新功能，用户能感知的新能力   | `feat(stl): 新增 ranges 管道示例`  |
| **fix**      | 修 bug           | `fix(cmake): 修复链接顺序错误`       |
| docs         | 只改文档、注释、README  | `docs: 补充目录设计说明`             |
| style        | 格式、空格，不改逻辑      | `style: 统一 type 打印格式`        |
| **refactor** | 重构，既不加功能也不修 bug | `refactor(stl): 拆分 demo 函数`  |
| **perf**     | 性能优化，功能不变       | `perf(hpc): 换用 SIMD 向量化`     |
| test         | 加/改测试代码         | `test: 补全 containers 边界用例`   |
| build        | 改构建系统、依赖        | `build: 升级 vcpkg baseline`   |
| ci           | 改 CI 流水线        | `ci: 新增 Windows 测试 job`      |
| **chore**    | 杂务，不在 src 和测试   | `chore: 删除旧目录设计文档`           |
| revert       | 回滚某次提交          | `revert: revert "feat: xxx"` |

加粗为高频 type，其他按需使用。

**工作方式**：提交说明由 Claude 起草文字，实际 `git commit` 由用户执行。

---

> 新习惯确立后追加到这里，保持一句话一条、可执行、不说废话。

---

## 全局术语表

遇到不认识的缩写先查这里，各目录 README 的 `## 术语速查` 里有更细分的术语。

### 语言与编译

| 缩写     | 全称                                     | 说明                   |
|--------|----------------------------------------|----------------------|
| RAII   | Resource Acquisition Is Initialization | 资源获取即初始化，析构时自动释放     |
| CRTP   | Curiously Recurring Template Pattern   | 奇异递归模板模式，实现静态多态      |
| SFINAE | Substitution Failure Is Not An Error   | 模板替换失败不报错，用于编译期条件选择  |
| TMP    | Template Metaprogramming               | 模板元编程，编译期计算          |
| UB     | Undefined Behavior                     | 未定义行为，编译器无任何保证       |
| ABI    | Application Binary Interface           | 二进制接口，名称修饰、调用约定等     |
| ODR    | One Definition Rule                    | 单一定义规则               |
| POD    | Plain Old Data                         | 无虚函数、无用户定义构造的简单数据    |
| CTAD   | Class Template Argument Deduction      | 类模板参数推导（C++17）       |
| ADL    | Argument-Dependent Lookup              | 参数依赖查找               |
| NVI    | Non-Virtual Interface                  | 非虚接口模式，模板方法的 C++ 惯用法 |
| EBO    | Empty Base Optimization                | 空基类优化                |

### 构建与工具链

| 缩写        | 全称                                       | 说明                         |
|-----------|------------------------------------------|----------------------------|
| LTO / IPO | Link-Time / Interprocedural Optimization | 链接期优化，跨编译单元内联              |
| PGO       | Profile-Guided Optimization              | 基于运行时 profile 的优化          |
| ASan      | AddressSanitizer                         | 内存错误检测（越界/UAF/double-free） |
| UBSan     | UndefinedBehaviorSanitizer               | 未定义行为检测                    |
| TSan      | ThreadSanitizer                          | 数据竞争检测                     |
| MSan      | MemorySanitizer                          | 未初始化内存读检测                  |
| UAF       | Use-After-Free                           | 释放后使用，常见内存安全漏洞             |

### 并发与系统

| 缩写    | 全称                                  | 说明                    |
|-------|-------------------------------------|-----------------------|
| NUMA  | Non-Uniform Memory Access           | 非统一内存访问，多 CPU 插槽架构    |
| CAS   | Compare-And-Swap                    | 比较并交换，lock-free 的基础原语 |
| SIMD  | Single Instruction Multiple Data    | 单指令多数据，向量化计算          |
| IPC   | Inter-Process Communication         | 进程间通信（管道/共享内存/消息队列）   |
| POSIX | Portable Operating System Interface | 可移植操作系统接口，Unix 标准 API |

### 领域

| 缩写   | 全称                           | 说明                  |
|------|------------------------------|---------------------|
| ECS  | Entity-Component-System      | 实体-组件-系统，游戏引擎架构模式   |
| DDS  | Data Distribution Service    | 数据分发服务，ROS2 底层通信中间件 |
| ONNX | Open Neural Network Exchange | 开放神经网络交换格式          |
| FFT  | Fast Fourier Transform       | 快速傅里叶变换             |
| PBR  | Physically Based Rendering   | 基于物理的渲染             |
