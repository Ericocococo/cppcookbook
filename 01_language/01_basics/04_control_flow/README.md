# 03_control_flow — 控制流

演示 `if/else`、`switch`、`while`、`do-while`、`for`、范围 `for`、`break`/`continue`。

## 1. 文件

| 文件               | 说明                          |
|------------------|-----------------------------|
| `main.cpp`       | 7 个 demo 函数，覆盖全部控制流结构       |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `control_flow` |

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
./build-mingw-ninja/control_flow.exe
```

> cmd 三步版：

```bat
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build build-mingw-ninja

:: 运行
build-mingw-ninja\control_flow.exe
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

build-msvc-ninja\control_flow.exe
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
build-msvc-ninja\control_flow.exe
```

---

## 4. CLion IDE

1. `File → Open` 选择 `03_control_flow/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击**加载**
3. **构建** `Ctrl+F9`**运行** `Shift+F10`

---

## 5. 英文及缩写说明

### 关键字

| 词汇            | 说明                                  |
|---------------|-------------------------------------|
| `if` / `else` | 条件分支：`if(条件)` 为真走 if 块，否则走 `else` 块 |
| `else if`     | 多分支链：在 if 不满足时继续判断下一个条件             |
| `switch`      | 开关语句：根据一个整数/枚举值跳转到对应 `case`         |
| `case`        | switch 内的分支标签，值必须是编译期常量             |
| `default`     | switch 内的默认分支，所有 case 都不匹配时执行       |
| `break`       | 跳出：立即退出当前的 `switch` 或循环             |
| `continue`    | 跳过：跳过本次循环剩余代码，直接进入下一次迭代             |
| `while`       | 当型循环：先判断条件，为真才执行循环体                 |
| `do-while`    | 直到型循环：先执行一次循环体，再判断条件，至少执行一次         |
| `for`         | 计次循环：`for(初始化; 条件; 步进)`，适合已知循环次数的场景 |

### 概念

| 概念                       | 说明                                                   |
|--------------------------|------------------------------------------------------|
| range-based for          | 范围 for（C++11）：`for(auto x : 集合)` 自动遍历数组/容器的每个元素      |
| fall-through             | 穿透：switch 中某个 case 没有 `break`，继续执行下一个 case，常见 bug 来源 |
| iteration                | 迭代：循环中每次执行循环体称为一次迭代                                  |
| short-circuit evaluation | 短路求值：`&&` 左边 false 则右边不执行，可防止空指针访问等危险操作              |
| nested loop              | 嵌套循环：循环内部还有循环，如九九乘法表用两层 for                          |
