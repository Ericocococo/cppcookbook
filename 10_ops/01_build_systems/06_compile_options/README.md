# 06_compile_options — 编译选项与宏定义

`target_compile_options` + 生成器表达式，跨编译器统一管理

## 1. CMakeLists.txt 写法解读

### 1.1 `if(MSVC)` — 最简跨编译器写法

本章 01_build_systems 各子目录的 CMakeLists 都用这套模板：

```cmake
if(MSVC)                                          # 如果当前编译器是 MSVC（Visual Studio 的 cl.exe）
    add_compile_options(/W4 /utf-8)               #   → 用 MSVC 语法的选项
else()                                            # 否则（MinGW / GCC / Clang）
    add_compile_options(-Wall -Wextra -Wpedantic) #   → 用 GCC 语法的选项
endif()                                           # if 结束
```

**为什么要区分两套**：MSVC 和 GCC 是两家公司的编译器，选项语法不兼容：

| 编译器                | 前缀  | 示例                |
|--------------------|-----|-------------------|
| MSVC（`cl.exe`）     | `/` | `/W4`、`/utf-8`    |
| GCC / Clang（`g++`） | `-` | `-Wall`、`-Wextra` |

把 GCC 的 `-Wall` 传给 MSVC 会报错，反之亦然。`if(MSVC)` 自动判断，不用手动切换。

### 1.2 `add_compile_options` vs `target_compile_options`

| 命令                                       | 作用范围                    | 适合场景                                  |
|------------------------------------------|-------------------------|---------------------------------------|
| `add_compile_options(...)`               | 当前 CMakeLists.txt 内所有目标 | 只有一个 `add_executable` 的简单工程（即本项目各子目录） |
| `target_compile_options(目标 PRIVATE ...)` | 指定的单个目标                 | 多目标工程，需要各自独立控制选项                      |

本项目各子目录只有一个可执行文件，两者效果相同，用 `add_compile_options` 更简洁。

### 1.3 本目录的进阶写法：生成器表达式

本目录 `CMakeLists.txt` 用的是**生成器表达式**，比 `if(MSVC)` 更精细，可以在单条命令里同时处理编译器类型和 Debug/Release
配置：

```cmake
# 生成器表达式：$<条件:值>  — 条件为真时展开为值，否则为空
target_compile_options(app PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>            # MSVC 时加这些
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>  # 非 MSVC 时加这些
)
```

`if(MSVC)` 是**配置阶段**判断（cmake 运行时），生成器表达式是**构建阶段**判断（ninja/make 运行时），后者能区分
Debug/Release，前者不行。入门阶段用 `if(MSVC)` 即可。

## 2. 标志速查

### GCC/Clang 警告标志（`-W` 前缀）

| 标志           | 说明                                |
|--------------|-----------------------------------|
| `-Wall`      | 高价值警告集合（未使用变量、类型不匹配等），名字骗人，并非全部警告 |
| `-Wextra`    | `-Wall` 之外的额外警告，误报稍多，单独一档         |
| `-Wpedantic` | 严格 C++ 标准模式，报所有 GCC 扩展用法          |
| `-Werror`    | 将所有警告提升为错误，强制修复                   |

### 优化级别（`-O` 大写 O）vs 输出文件名（`-o` 小写 o）

> **易混淆！** 大写 `-O` 控制优化，小写 `-o` 指定输出文件名。

| 标志           | 类别       | 说明              |
|--------------|----------|-----------------|
| `-O0`        | 优化（大写 O） | 不优化，默认，调试最准确    |
| `-O1`        | 优化       | 基础优化            |
| `-O2`        | 优化       | 中等优化，Release 常用 |
| `-O3`        | 优化       | 激进优化，可能增大二进制体积  |
| `-Os`        | 优化       | 优先减小体积          |
| `-o app.exe` | 输出（小写 o） | 指定生成文件的名称       |

### MSVC 等价标志

| GCC/Clang       | MSVC           | 说明                                        |
|-----------------|----------------|-------------------------------------------|
| `-Wall -Wextra` | `/W4`          | 高警告级别（MSVC 体系：/W0=无，/W1-/W4 递增，/W4 最高常用级） |
| `-Werror`       | `/WX`          | 警告视为错误                                    |
| `-O0`           | `/Od`          | 关闭优化（d = Disable）                         |
| `-O2`           | `/O2`          | 中等优化                                      |
| `-O3`           | `/Ox`          | 最大优化                                      |
| `-g` / `-g3`    | `/Zi`          | 生成调试信息；GCC 写入二进制，MSVC 生成独立 `.pdb` 文件      |
| `-DNDEBUG`      | `/DNDEBUG`     | 定义 NDEBUG 宏，关闭 `assert()`，Release 构建标准做法  |
| `-o app.exe`    | `/Fe:app.exe`  | 指定输出文件名（Fe = File Executable）             |
| `-EHsc`（无对应）    | `/EHsc`        | 启用标准 C++ 异常处理（MSVC 独有，GCC 默认已启用）          |
| 无对应             | `/permissive-` | 关闭 MSVC 历史兼容宽松模式，强制严格 C++ 标准              |
| 无对应             | `/utf-8`       | 源文件和执行字符集均为 UTF-8，避免中文乱码                  |

## 3. 构建与运行

完整六套方案（MinGW 三方案 / MSVC 三方案，含对比与 CLion）见 [章 README § 5 通用构建流程](../README.md)，本目录把 `<exe_name>` 换成 `app`。由于本目录需对比 Debug / Release 两种编译选项的效果，单配置方案每个 build 目录带 `-debug` / `-release` 后缀，并加 `-DCMAKE_BUILD_TYPE`（原因见下）。日常最常用——Ninja 方案 A，Debug（Git Bash 三步）：

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

"$CMAKE" -B build-mingw-ninja-debug -G Ninja -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_MAKE_PROGRAM="$NINJA" -DCMAKE_BUILD_TYPE=Debug
"$CMAKE" --build build-mingw-ninja-debug
./build-mingw-ninja-debug/app.exe
```

> 本目录专属（讲解见上文 §1.3）：Ninja / MinGW Makefiles 是单配置生成器，一次只编一种类型，编译选项的 Debug / Release 分支由 `-DCMAKE_BUILD_TYPE=Debug` 或 `Release` 触发，所以两者各占一个 build 目录、各配置一次。构建后可用 `cat build-mingw-ninja-debug/compile_commands.json` 查看每个源文件的完整编译命令，验证 `-O0`、`-g`、`-DDEBUG` 等选项是否注入。
>
> cmd 版与 MSVC/Linux 版：同样把通用流程里的 exe 名换成 `app`，并追加上述 `-DCMAKE_BUILD_TYPE` 与目录后缀规则即可。
