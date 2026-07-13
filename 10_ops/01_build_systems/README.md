# 01_build_systems — C++ 构建系统演进

从最原始的手动编译，到现代主流的 CMake + vcpkg 全流程。

每个子目录是独立工程，在 CLion 中直接打开对应目录即可构建（CMake 阶段起）。

---

## 工具路径（完整路径，不依赖 PATH）

所有命令行示例均使用以下完整路径，无需修改系统 PATH。

### 工具链 A · MinGW（CLion 内置）

| 工具 | 完整路径 |
|------|----------|
| g++.exe | `D:\ProgramData\JetBrains\CLion20260101\bin\mingw\bin\g++.exe` |
| cmake.exe | `D:\ProgramData\JetBrains\CLion20260101\bin\cmake\win\x64\bin\cmake.exe` |
| ninja.exe | `D:\ProgramData\JetBrains\CLion20260101\bin\ninja\win\x64\ninja.exe` |

**Git Bash 里定义变量（可粘贴到 ~/.bashrc 长期生效）：**

```bash
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

# 验证
"$GXX"   --version   # → GCC 13.1.0
"$CMAKE" --version   # → cmake version 4.2.2
"$NINJA" --version   # → 1.13.2
```

---

### 工具链 B · MSVC（VS 自带）

| 工具 | 完整路径 |
|------|----------|
| cl.exe | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\cl.exe` |
| link.exe | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\link.exe` |
| cmake.exe | `D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe` |
| ninja.exe | `D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe` |
| vcvarsall.bat | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat` |

**cl.exe 必须通过 vcvarsall.bat 激活环境后才能使用**（头文件、库路径、link.exe 路径均由它注入）。

**cmd 里定义变量：**

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

:: 激活 MSVC 环境（当前 cmd 会话有效，cl/link/ninja 均自动加入 PATH）
call "%VCVARSALL%" x64

:: 验证
cl              & :: → Microsoft (R) C/C++ Optimizing Compiler Version 19.51...
"%CMAKE%" --version & :: → cmake version 4.3.1-msvc1
```

---

## 演进路线

```
手动 g++/cl.exe  →  手写 Makefile  →  CMake 基础  →  CMake 现代化  →  CMake + vcpkg  →  企业级多模块
```

| 阶段 | 目录 | 核心知识点 |
|------|------|------------|
| **原始** | [00_compiler_cli](00_compiler_cli/README.md) | 直接调用 g++/cl.exe，理解编译器在做什么 |
| **Makefile** | [00_makefile](00_makefile/README.md) | 手写规则、模式匹配、增量编译、依赖追踪 |
| **CMake 入门** | [01_hello](01_hello/README.md) | 最简工程：`cmake_minimum_required` + `project` + `add_executable` |
| | [02_sources](02_sources/README.md) | 多源文件：显式列举 + `target_include_directories` |
| | [03_library](03_library/README.md) | 静态库与动态库：`add_library(STATIC/SHARED)` |
| **CMake 现代化** | [04_targets](04_targets/README.md) | target 属性传播：`PRIVATE / PUBLIC / INTERFACE`，接口库 |
| | [05_compile_options](05_compile_options/README.md) | 编译选项与宏：跨编译器生成器表达式 |
| | [06_find_package](06_find_package/README.md) | 查找依赖：Module 模式 vs Config 模式，可选依赖 |
| **包管理** | [07_vcpkg](07_vcpkg/README.md) | vcpkg 集成：`vcpkg.json` 声明，toolchain 注入 |
| **工程质量** | [08_install](08_install/README.md) | 安装规则：`GNUInstallDirs` + 导出 CMake 包配置 |
| | [09_ctest](09_ctest/README.md) | 测试集成：`enable_testing` + `add_test` + 标签/超时 |
| **高阶** | [10_generator_expr](10_generator_expr/README.md) | 生成器表达式：`$<CONFIG>` / `$<CXX_COMPILER_ID>` |
| | [11_presets](11_presets/README.md) | CMakePresets.json：debug / release / asan 多配置预设 |
| **企业级** | [12_enterprise](12_enterprise/README.md) | 多模块：`project_options` + `add_subdirectory` 依赖链 + ctest |
