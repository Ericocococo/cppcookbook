# 04_control_flow — 控制流

演示 `if/else`、`switch`、`while`、`do-while`、`for`、范围 `for`、`break`/`continue`。

## 1. 文件

| 文件               | 说明                          |
|------------------|-----------------------------|
| `main.cpp`       | 7 个 demo 函数，覆盖全部控制流结构       |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `control_flow` |

---

## 2. 命令行 · MinGW（Git Bash）

### 方案 A — Ninja

```bash
cmake -G Ninja -B build-mingw-ninja -S .
cmake --build build-mingw-ninja
./build-mingw-ninja/control_flow.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-ninja\control_flow.exe
```

</details>

### 方案 B — MinGW Makefiles

```bash
cmake -G "MinGW Makefiles" -B build-mingw-make -S .
cmake --build build-mingw-make
./build-mingw-make/control_flow.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-make
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-make\control_flow.exe
```

</details>

### 方案 C — Ninja Multi-Config

```bash
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .
cmake --build build-mingw-mc --config Release
./build-mingw-mc/Release/control_flow.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-mc --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-mc\Release\control_flow.exe
```

</details>

---

## 3. 命令行 · MSVC（cmd）

### 方案 A — vcvarsall + Ninja

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B build-msvc-ninja -S .
cmake --build build-msvc-ninja
build-msvc-ninja\control_flow.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-msvc-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-msvc-ninja\control_flow.exe
```

</details>

### 方案 B — VS Generator

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .
cmake --build build-msvc-vs --config Release
build-msvc-vs\Release\control_flow.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-msvc-vs --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-msvc-vs\Release\control_flow.exe
```

</details>

---

## 4. 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/control_flow
```

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
