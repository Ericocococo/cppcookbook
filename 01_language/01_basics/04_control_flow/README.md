# 04_control_flow — 控制流

演示 `if/else`、`switch`、`while`、`do-while`、`for`、范围 `for`、`break`/`continue`。

## 1. 知识点

### 1.1 if / else

`if` 根据条件是否为真来决定执行哪段代码。可以用 `else if` 链式判断多个条件，`else` 处理所有不满足的情况。

```cpp
int score = 75;
if (score >= 90) {
    std::cout << "优秀\n";
} else if (score >= 75) {
    std::cout << "良好\n";
} else if (score >= 60) {
    std::cout << "及格\n";
} else {
    std::cout << "不及格\n";
}
```

#### 1.1.1 悬空 else 陷阱

`else` 总是和最近的未配对 `if` 配对，不看缩进。不加花括号时容易产生歧义，建议始终加 `{}`。

```cpp
int a = 1, b = -1;
if (a > 0)
    if (b > 0)
        std::cout << "a>0 且 b>0\n";
    else                                    // 属于内层 if(b>0)，不是外层！
        std::cout << "b<=0\n";

// 加花括号消除歧义：
if (a > 0) {
    if (b > 0) {
        std::cout << "a>0 且 b>0\n";
    }
} else {
    std::cout << "a<=0\n";                  // 现在 else 配对外层 if
}
```

#### 1.1.2 C++17 初始化 if

C++17 允许在 `if` 条件前声明变量，变量作用域限于 if/else 块内，不会泄漏到外面。适合只在条件判断中使用的临时变量。

```cpp
// C++17 写法：变量 x 只在 if/else 块内可见
if (int x = score - 60; x >= 0) {
    std::cout << "高于及格线 " << x << " 分\n";
} else {
    std::cout << "低于及格线 " << (-x) << " 分\n";
}
// x 在这里不可见，不污染外部作用域
```

### 1.2 switch

`switch` 根据一个整数或枚举值跳转到匹配的 `case` 标签。每个 `case` 末尾通常需要 `break`，否则会"穿透"执行下一个 case。`default` 处理所有未匹配的值。

```cpp
int day = 3;
switch (day) {
    case 1: std::cout << "周一\n"; break;
    case 2: std::cout << "周二\n"; break;
    case 3:
    case 4:
    case 5: std::cout << "工作日\n"; break;   // 3、4、5 共用同一个处理
    case 6:
    case 7: std::cout << "周末\n"; break;
    default: std::cout << "非法\n";
}
```

#### 1.2.1 fall-through 穿透与 `[[fallthrough]]`（C++17）

忘写 `break` 会导致执行下一个 case 的代码（穿透），这是常见的 bug 来源。如果确实需要穿透，C++17 提供 `[[fallthrough]]` 属性标注，告诉编译器和读者这是故意的。

```cpp
int x = 1;
switch (x) {
    case 1:
        std::cout << "case 1 执行\n";
        [[fallthrough]];              // 告知编译器：故意穿透
    case 2:
        std::cout << "case 2 也执行\n";
        break;
    // 不加 [[fallthrough]] 时，编译器会发出 warning 提醒可能忘了 break
}
```

#### 1.2.2 C++17 初始化 switch

和初始化 if 类似，C++17 允许在 `switch` 前声明变量，变量作用域限于 switch 块内。

```cpp
switch (int v = day * 2; v) {
    case 6:
        std::cout << "v=" << v << "\n";
        break;
    default:
        break;
}
// v 在这里不可见
```

### 1.3 while / do-while

#### 1.3.1 while 循环

`while` 先判断条件，为真才执行循环体。如果条件一开始就为假，循环体一次都不执行。

```cpp
int n = 5;
while (n > 0) {
    std::cout << n << " ";
    --n;
}
// 输出：5 4 3 2 1
```

#### 1.3.2 do-while 循环

`do-while` 先执行循环体，再判断条件。保证循环体至少执行一次，适用于"先做再检查"的场景（如菜单交互、输入验证）。

```cpp
int n = 0;
do {
    std::cout << "执行！";
    ++n;
} while (n < 0);
// 输出：执行！（虽然 n=0 不满足 n<0，但循环体已经执行了一次）
```

#### 1.3.3 while(true) + break

当循环退出条件不在开头也不在末尾（在循环体中间）时，可以用 `while(true)` 配合 `break` 退出。比硬凑成 while/do-while 的条件更清晰。

```cpp
int i = 0;
while (true) {
    if (i >= 3) {
        break;            // 满足条件时退出循环
    }
    std::cout << i << " ";
    ++i;
}
// 输出：0 1 2
```

### 1.4 for 循环

`for(初始化; 条件; 步进)` 适合已知循环次数的场景。三个部分都可以省略（分号不能省），省略条件等价于 `true`（无限循环）。

```cpp
// 正序 0~4
for (int i = 0; i < 5; ++i) {
    std::cout << i << " ";
}
// 输出：0 1 2 3 4

// 倒序 4~0
for (int i = 4; i >= 0; --i) {
    std::cout << i << " ";
}

// 步长为 2
for (int i = 0; i <= 10; i += 2) {
    std::cout << i << " ";
}
// 输出：0 2 4 6 8 10
```

#### 1.4.1 for 可以省略任何部分

`for` 的初始化、条件、步进三个部分都可以省略，但两个分号必须保留。省略条件等价于 `while(true)`。

```cpp
int i = 0;
for (; i < 3; ++i) {         // 省略初始化（i 已在外部声明）
    std::cout << i << " ";
}

for (;;) {                    // 全部省略 = 无限循环
    break;                    // 需要 break 退出
}
```

#### 1.4.2 多变量 for 循环

`for` 的初始化和步进部分可以用逗号分隔多个变量，实现同时操作多个变量。常用于双指针算法。

```cpp
for (int a = 0, b = 10; a < b; ++a, --b) {
    std::cout << "(" << a << "," << b << ") ";
}
// 输出：(0,10) (1,9) (2,8) (3,7) (4,6)
```

### 1.5 范围 for（C++11）

范围 for 自动遍历数组或容器的每个元素，不需要手写索引，更简洁且不会越界。

#### 1.5.1 值遍历

`for (auto x : arr)` 每次循环 `x` 是元素的副本，修改 `x` 不影响原数组。适用于只读且元素是基本类型的场景。

```cpp
int arr[] = {1, 2, 3, 4, 5};
for (auto x : arr) {
    std::cout << x << " ";     // 1 2 3 4 5
}
// x 是副本，修改 x 不影响 arr
```

#### 1.5.2 引用遍历

`for (auto& x : arr)` 中 `x` 是元素的引用，可以直接修改原数组。

```cpp
int arr[] = {1, 2, 3, 4, 5};
for (auto& x : arr) {
    x *= 2;                    // 修改原数组
}
// arr 变为 {2, 4, 6, 8, 10}
```

#### 1.5.3 const 引用遍历

`for (const auto& x : arr)` 只读引用，不复制也不修改。对大对象（如 `std::string`、`std::vector`）推荐用这种方式，避免不必要的拷贝开销。

```cpp
#include <string>
#include <vector>

std::vector<std::string> words = {"hello", "world", "cpp"};
for (const auto& w : words) {
    std::cout << w << " ";     // 只读，不复制 string
}
```

#### 1.5.4 C++17 结构化绑定遍历 pair

C++17 的结构化绑定 `auto& [key, value]` 可以直接拆解 `pair`、`tuple` 或结构体的成员，让遍历 map 等容器更简洁。

```cpp
#include <vector>
#include <string>
#include <utility>    // std::pair

std::vector<std::pair<std::string, int>> data = {
    {"Alice", 95}, {"Bob", 82}
};
for (const auto& [name, score] : data) {
    std::cout << name << ": " << score << "\n";
}
// 输出：Alice: 95
//       Bob: 82
```

### 1.6 break / continue

`break` 立即退出当前循环或 switch；`continue` 跳过本次循环剩余代码，直接进入下一次迭代。

```cpp
// break：找到 5 就停止
for (int i = 0; i < 10; ++i) {
    if (i == 5) {
        break;
    }
    std::cout << i << " ";
}
// 输出：0 1 2 3 4

// continue：跳过偶数
for (int i = 0; i < 10; ++i) {
    if (i % 2 == 0) {
        continue;
    }
    std::cout << i << " ";
}
// 输出：1 3 5 7 9
```

#### 1.6.1 break 只退出最近一层循环

嵌套循环中 `break` 只退出最内层循环，外层循环不受影响。如果需要退出多层循环，可以用标志变量、将循环封装为函数后 `return`，或用 `goto`。

```cpp
for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
        if (j == 1) {
            break;               // 只退出内层 for(j)
        }
        std::cout << "(" << i << "," << j << ")\n";
    }
    // break 后从这里继续外层循环
}
// 输出：(0,0) (1,0) (2,0)
```

#### 1.6.2 goto 退出多层循环

`goto` 跳转到指定标签，是退出多层嵌套循环的少数合理使用场景之一。除此之外应避免使用 `goto`，它会破坏代码结构使其难以阅读。

```cpp
for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
        if (i == 1 && j == 1) {
            goto done;           // 直接跳出两层循环
        }
        std::cout << "(" << i << "," << j << ")\n";
    }
}
done:
std::cout << "跳出两层循环\n";
// 输出：(0,0) (0,1) (0,2) (1,0) 跳出两层循环
```

## 2. 构建

### 2.1 命令行 · MinGW（Git Bash）

#### 方案 A — Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow

# 配置
cmake -G Ninja -B build-mingw-ninja -S .

# 构建
cmake --build build-mingw-ninja

# 运行
./build-mingw-ninja/control_flow.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-ninja\control_flow.exe
```

</details>

#### 方案 B — MinGW Makefiles

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow

# 配置
cmake -G "MinGW Makefiles" -B build-mingw-make -S .

# 构建
cmake --build build-mingw-make

# 运行
./build-mingw-make/control_flow.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-make

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-make\control_flow.exe
```

</details>

#### 方案 C — Ninja Multi-Config

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```bash
# cd 到项目目录
cd D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow

# 配置
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .

# 构建
cmake --build build-mingw-mc --config Release

# 运行
./build-mingw-mc/Release/control_flow.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

:: 构建
D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-mc --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-mingw-mc\Release\control_flow.exe
```

</details>

### 2.2 命令行 · MSVC（cmd）

> MSVC 路径常量与 vcvarsall 注入原理（4 个环境变量、为什么必须 `call`）见首次详解 [00_hello_world §2.2](../00_hello_world/README.md)，以下命令直接用。

```bat
:: 激活 — 把 cl.exe / link.exe 加入当前会话 PATH，并注入 INCLUDE / LIB / LIBPATH
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```

#### 方案 A — vcvarsall + Ninja

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow

:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
cmake -G Ninja -B build-msvc-ninja -S .

:: 构建
cmake --build build-msvc-ninja

:: 运行
build-msvc-ninja\control_flow.exe
```

</details>

<details><summary>绝对路径四步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 激活
call "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow -DCMAKE_MAKE_PROGRAM="D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-msvc-ninja

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-msvc-ninja\control_flow.exe
```

</details>

#### 方案 B — VS Generator

<details><summary>相对路径版（需先 cd 到项目目录）</summary>

```cmd
:: cd 到项目目录
cd /d D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow

:: 配置
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .

:: 构建
cmake --build build-msvc-vs --config Release

:: 运行
build-msvc-vs\Release\control_flow.exe
```

</details>

<details><summary>绝对路径三步版（可在任意目录直接粘贴运行）</summary>

```cmd
:: 配置
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow

:: 构建
"D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-msvc-vs --config Release

:: 运行
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\04_control_flow\build-msvc-vs\Release\control_flow.exe
```

</details>

### 2.3 命令行 · Linux / WSL

```bash
# 配置
cmake -G Ninja -B build-linux -S .

# 构建
cmake --build build-linux

# 运行
./build-linux/control_flow
```

## 3. 英文及缩写说明

### 3.1 关键字

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

### 3.2 概念

| 概念                       | 说明                                                   |
|--------------------------|------------------------------------------------------|
| range-based for          | 范围 for（C++11）：`for(auto x : 集合)` 自动遍历数组/容器的每个元素      |
| fall-through             | 穿透：switch 中某个 case 没有 `break`，继续执行下一个 case，常见 bug 来源 |
| iteration                | 迭代：循环中每次执行循环体称为一次迭代                                  |
| short-circuit evaluation | 短路求值：`&&` 左边 false 则右边不执行，可防止空指针访问等危险操作              |
| nested loop              | 嵌套循环：循环内部还有循环，如九九乘法表用两层 for                          |
