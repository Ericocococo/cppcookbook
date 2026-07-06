# 习惯约定

> 本仓库在编写/填充过程中形成的习惯，和 [目录设计文档](STRUCTURE.md) 里的
> "贯穿全库的约定"互补——那边讲**结构规则**，这边讲**动笔时的习惯**。

---

## 术语解释规范

专业名词、英文缩写必须解释，让读者不查文档也能读懂。解释方式按场景选一种：

| 场景 | 写法 | 示例 |
|------|------|------|
| 一句话能说清 | 行内注释 | `// RAII：资源获取即初始化` |
| 需要多句说明 | `.cpp` 文件顶部注释块 | 见下方模板 |
| 目录/库的术语表 | 该目录的 `README.md` 末尾 `## 术语速查` 表格 | 见 [01_language/README.md](01_language/README.md) |
| 全局通用缩写 | 本约定末尾 `## 全局术语表` | 见下方 |

**`.cpp` 文件顶部注释模板：**

```cpp
// C++20 | 依赖：<库名>
// 演示：<一句话说明演示内容>
// <专业名词>：<英文全称> — <中文解释>
// 构建：<构建命令>
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
6. **只有其子目录直接含 `.cpp` 的目录，才建 `README.md`**，中间层目录不加 —
   `README.md` 列出当前目录所有文件/子目录一行内容摘要，`cat README.md` 即可查导航。
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
"$CMAKE" -B build -G Ninja -DCMAKE_C_COMPILER="$GCC" -DCMAKE_CXX_COMPILER="$GXX"
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
       add_compile_options(/W4 /utf-8)
   else()
       add_compile_options(-Wall -Wextra -Wpedantic)
   endif()

   add_executable(示例名 main.cpp)
   ```

10. **需要三方库时，在本目录 `CMakeLists.txt` 里加 `find_package` / `target_link_libraries`**，
    并在 `vcpkg.json`（如有）里声明依赖；不依赖根目录的 `vcpkg.json`。

## 代码规范

8. **纯语法用可运行 `.cpp` + `main()` 展示** — 每个文件能直接编译运行，
   打印运行结果与目测结论（不用 `assert`）；出错时用 `return 1` 或 `std::exit(1)` 加打印说明，
   不让程序崩溃中断。
9. **打印显示要直观** — 结构体逐字段打印，并带上类型或说明：
   `std::cout << "  x: " << x << " (int)\n";`
   不要一行 dump 整个对象；多个用例用 `① ② ③` 小标题分隔。
10. **每段演示封装成 `demoXX_xxx()` 函数** — 如 `demo01_basic()` / `demo02_ranges()`，
    序号对应注释里的 ① ② ③；`main()` 只留串行调用，实现都在上面。
11. **demo 产出落到 `data/` 目录**（已被 `.gitignore` 忽略），不用 tmpfile。
12. **文件顶部注释写清楚**：所属标准库/三方库、C++ 标准版本（如 `// C++20`）、编译/安装方式。
13. **注释使用中文**。
14. **给出的 demo 必须是可执行的**，假定实际跑一遍确认输出正常后完成。

## 命名

15. **项目名 `cppcookbook`**（不是 cpp-cookbook）。
16. **别含顶层命名为 `cpp` 或 `test`** — 会与 ctest 或标准库 `<test>` 混淆概念。

## Git 提交信息

**生成提交说明前**：同时运行 `git diff` 和 `git diff --cached`，把所有改动（含删除文件）一起看，不遗漏。

格式：`<type>(<scope>): <描述>`

- **scope** 可选，填改动所在的库/模块名，如 `stl`、`cmake`、`language`。
- **描述**用中文，动词开头，不加句号，首行不超过 72 字。
- **正文**（body）可选，空行隔开，一两句说清楚"为什么"即可，不要复述"改了什么"。

| type | 含义 | 例子 |
|------|------|------|
| **feat** | 新功能，用户能感知的新能力 | `feat(stl): 新增 ranges 管道示例` |
| **fix** | 修 bug | `fix(cmake): 修复链接顺序错误` |
| docs | 只改文档、注释、README | `docs: 补充目录设计说明` |
| style | 格式、空格，不改逻辑 | `style: 统一 type 打印格式` |
| **refactor** | 重构，既不加功能也不修 bug | `refactor(stl): 拆分 demo 函数` |
| **perf** | 性能优化，功能不变 | `perf(hpc): 换用 SIMD 向量化` |
| test | 加/改测试代码 | `test: 补全 containers 边界用例` |
| build | 改构建系统、依赖 | `build: 升级 vcpkg baseline` |
| ci | 改 CI 流水线 | `ci: 新增 Windows 测试 job` |
| **chore** | 杂务，不在 src 和测试 | `chore: 删除旧目录设计文档` |
| revert | 回滚某次提交 | `revert: revert "feat: xxx"` |

加粗为高频 type，其他按需使用。

**工作方式**：提交说明由 Claude 起草文字，实际 `git commit` 由用户执行。

---

> 新习惯确立后追加到这里，保持一句话一条、可执行、不说废话。

---

## 全局术语表

遇到不认识的缩写先查这里，各目录 README 的 `## 术语速查` 里有更细分的术语。

### 语言与编译

| 缩写 | 全称 | 说明 |
|------|------|------|
| RAII | Resource Acquisition Is Initialization | 资源获取即初始化，析构时自动释放 |
| CRTP | Curiously Recurring Template Pattern | 奇异递归模板模式，实现静态多态 |
| SFINAE | Substitution Failure Is Not An Error | 模板替换失败不报错，用于编译期条件选择 |
| TMP | Template Metaprogramming | 模板元编程，编译期计算 |
| UB | Undefined Behavior | 未定义行为，编译器无任何保证 |
| ABI | Application Binary Interface | 二进制接口，名称修饰、调用约定等 |
| ODR | One Definition Rule | 单一定义规则 |
| POD | Plain Old Data | 无虚函数、无用户定义构造的简单数据 |
| CTAD | Class Template Argument Deduction | 类模板参数推导（C++17）|
| ADL | Argument-Dependent Lookup | 参数依赖查找 |
| NVI | Non-Virtual Interface | 非虚接口模式，模板方法的 C++ 惯用法 |
| EBO | Empty Base Optimization | 空基类优化 |

### 构建与工具链

| 缩写 | 全称 | 说明 |
|------|------|------|
| LTO / IPO | Link-Time / Interprocedural Optimization | 链接期优化，跨编译单元内联 |
| PGO | Profile-Guided Optimization | 基于运行时 profile 的优化 |
| ASan | AddressSanitizer | 内存错误检测（越界/UAF/double-free）|
| UBSan | UndefinedBehaviorSanitizer | 未定义行为检测 |
| TSan | ThreadSanitizer | 数据竞争检测 |
| MSan | MemorySanitizer | 未初始化内存读检测 |
| UAF | Use-After-Free | 释放后使用，常见内存安全漏洞 |

### 并发与系统

| 缩写 | 全称 | 说明 |
|------|------|------|
| NUMA | Non-Uniform Memory Access | 非统一内存访问，多 CPU 插槽架构 |
| CAS | Compare-And-Swap | 比较并交换，lock-free 的基础原语 |
| SIMD | Single Instruction Multiple Data | 单指令多数据，向量化计算 |
| IPC | Inter-Process Communication | 进程间通信（管道/共享内存/消息队列）|
| POSIX | Portable Operating System Interface | 可移植操作系统接口，Unix 标准 API |

### 领域

| 缩写 | 全称 | 说明 |
|------|------|------|
| ECS | Entity-Component-System | 实体-组件-系统，游戏引擎架构模式 |
| DDS | Data Distribution Service | 数据分发服务，ROS2 底层通信中间件 |
| ONNX | Open Neural Network Exchange | 开放神经网络交换格式 |
| FFT | Fast Fourier Transform | 快速傅里叶变换 |
| PBR | Physically Based Rendering | 基于物理的渲染 |
