# 03_lint_format — clang-format 与 clang-tidy

> 两个工具一个管排版一个管质量，配合 CLion 实时生效。

## 1. 两个工具的区别

| | clang-format | clang-tidy |
|---|---|---|
| 管什么 | 代码**格式**（缩进、空格、花括号位置） | 代码**质量**（bug、性能、可读性、现代写法） |
| 配置文件 | `.clang-format` | `.clang-tidy` |
| 类比 | 排版员 | 代码审查员 |
| CLion 触发 | `Ctrl+Alt+L` 格式化 | 编辑器实时标黄提示 |
| 命令行 | `clang-format -i file.cpp` | `clang-tidy file.cpp` |

## 2. clang-format 配置详解

配置文件：项目根目录 `.clang-format`

```yaml
# ---- 基础 ----
BasedOnStyle: Google        # 基于 Google 风格，再覆盖自定义项
Standard: c++20             # 按 C++20 语法解析

# ---- 缩进 ----
IndentWidth: 4              # 缩进 4 空格（Google 默认是 2）
TabWidth: 4                 # Tab 宽度
UseTab: Never               # 禁用 Tab，全用空格
ContinuationIndentWidth: 4  # 续行缩进
IndentCaseLabels: true      # switch-case 的 case 标签缩进
IndentPPDirectives: BeforeHash  # 预处理指令 # 前加缩进

# ---- 行宽 ----
ColumnLimit: 120            # 单行最多 120 字符，超过自动折行

# ---- 花括号 ----
BreakBeforeBraces: Attach   # K&R 风格：{ 跟在同一行
# BreakBeforeBraces: Allman # Allman 风格：{ 独占下一行

# ---- 短语句折叠 ----
AllowShortFunctionsOnASingleLine: Inline  # 内联短函数允许单行（类内的）
AllowShortIfStatementsOnASingleLine: Never # if 不允许压成单行
AllowShortLoopsOnASingleLine: false        # for/while 不允许压成单行
AllowShortLambdasOnASingleLine: Inline     # 短 lambda 允许单行

# ---- 指针/引用 ----
PointerAlignment: Left      # int* p（靠左），不是 int *p 或 int * p
ReferenceAlignment: Left    # int& r（靠左）

# ---- 括号前空格 ----
SpaceBeforeParens: ControlStatements  # if/for/while 后加空格，函数名后不加
# if (x)  ✓
# foo(x)  ✓（函数名后无空格）

# ---- include 排序 ----
SortIncludes: CaseSensitive  # include 按字母排序
IncludeBlocks: Regroup       # 标准库 → 三方库 → 项目内 分组

# ---- 参数换行 ----
BinPackArguments: false      # 函数参数不挤在一行，每个参数独占一行
BinPackParameters: false     # 函数声明的参数也是
AlignAfterOpenBracket: AlwaysBreak  # 左括号后立即换行

# ---- 注释 ----
ReflowComments: false        # 不自动重排注释（保留手动排版）
SpacesBeforeTrailingComments: 2  # 行尾注释前留 2 个空格
```

## 3. clang-tidy 配置详解

配置文件：项目根目录 `.clang-tidy`

### 3.1 四大检查类别

| 类别 | 管什么 | 举例 |
|---|---|---|
| **modernize** | 催你用新语法 | `NULL` → `nullptr`，传统 for → range-for |
| **bugprone** | 找潜在 bug | 用了 move 后继续使用、无限循环、整数除法截断 |
| **readability** | 代码可读性 | `size() == 0` → `empty()`、字符串比较用 `==` |
| **performance** | 性能优化 | 不必要的拷贝、`std::endl` → `"\n"` |

### 3.2 本项目启用的规则

#### modernize（现代化）

```yaml
modernize-use-nullptr          # NULL → nullptr
modernize-use-auto             # 显式类型 → auto（迭代器等场景）
modernize-use-override         # 虚函数加 override
modernize-use-default-member-init  # 成员变量在声明处给默认值
modernize-use-emplace          # push_back({}) → emplace_back()
modernize-loop-convert          # 传统 for(int i=0;...) → range-for
modernize-avoid-bind           # std::bind → lambda
modernize-replace-auto-ptr     # auto_ptr → unique_ptr（auto_ptr 已废弃）
modernize-use-nodiscard        # 返回值不应忽略的函数加 [[nodiscard]]
modernize-use-ranges           # STL 算法 → ranges 写法（C++20）
```

#### bugprone（易出 bug）

```yaml
bugprone-use-after-move            # std::move 后继续使用对象 → 未定义行为
bugprone-dangling-handle           # 悬挂引用/指针（对象已销毁但引用还在）
bugprone-infinite-loop             # 检测可能的无限循环
bugprone-integer-division          # 整数除法截断：5/2=2 不是 2.5
bugprone-suspicious-memset-usage   # memset 参数顺序错误
bugprone-unchecked-optional-access # 没检查 optional 是否有值就取用
```

#### readability（可读性）

```yaml
readability-const-return-type      # 返回 const 值通常没意义，去掉
readability-container-size-empty   # v.size() == 0 → v.empty()（更清晰）
readability-string-compare         # str.compare("x") == 0 → str == "x"
```

#### performance（性能）

```yaml
performance-unnecessary-copy-initialization  # 不必要的拷贝初始化
# auto s = getString();     → 拷贝
# const auto& s = getString(); → 不拷贝

performance-move-const-arg   # std::move(const 对象) 无意义，move 要求非 const

performance-avoid-endl       # std::endl 会刷新缓冲区，"\n" 不会
# std::cout << x << std::endl;  → 慢（每次刷新）
# std::cout << x << "\n";       → 快（不刷新）
```

#### cppcoreguidelines（C++ 核心指南）

```yaml
cppcoreguidelines-avoid-magic-numbers    # 魔法数字要定义为常量
# if (x > 3.14)  → 3.14 是什么？
# constexpr double PI = 3.14; if (x > PI) → 清楚
# IgnoredIntegerValues: 0;1;2;-1  → 这几个常用数字不报警

cppcoreguidelines-prefer-member-initializer  # 成员变量优先用初始化列表
# Stock(string c) { m_code = c; }        → 先默认构造再赋值，两步
# Stock(string c) : m_code(std::move(c)) {} → 直接构造，一步
```

### 3.3 命名规则（CheckOptions）

```yaml
ClassCase: CamelCase       # 类名大驼峰：MyClass
FunctionCase: lower_case   # 函数名蛇形：get_price
VariableCase: lower_case   # 变量名蛇形：total_amount
ConstantCase: UPPER_CASE   # 常量全大写：MAX_SIZE
```

### 3.4 其他配置

```yaml
WarningsAsErrors: ""       # 不把警告当错误（空 = 不阻断编译）
HeaderFilterRegex: ""      # 检查哪些头文件（空 = 只检查源文件）
FormatStyle: file          # 修复时用 .clang-format 的格式
```

## 4. CLion 中使用

### clang-format

1. **Settings → Editor → 代码样式 → C/C++**
2. 勾选 **"从 .clang-format 文件中读取代码样式"**
3. `Ctrl+Alt+L` 格式化当前文件

### clang-tidy

CLion 自动读取项目根目录的 `.clang-tidy`，编辑器实时标黄提示，鼠标悬停查看建议。

也可以在 **Settings → Editor → 检查 → C/C++ → Clang-Tidy** 中配置哪些规则启用/禁用。

## 5. 命令行使用

```bash
# 格式化单个文件（-i 原地修改）
clang-format -i main.cpp

# 格式化整个目录
find . -name "*.cpp" -o -name "*.h" | xargs clang-format -i

# 静态分析单个文件
clang-tidy main.cpp

# 静态分析并自动修复
clang-tidy --fix main.cpp
```
