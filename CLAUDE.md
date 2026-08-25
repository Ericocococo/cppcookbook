# CLAUDE.md

## 文档同步

- 代码或目录有新增/修改/删除时，必须同步更新对应的 README.md、STRUCTURE.md 等相关文档，不能只改代码不改文档。

## README 格式

- 每个含 .cpp 的目录的 README.md 必须包含 `## 2. 命令行 · MinGW（Git Bash）` 和 `## 3. 命令行 · MSVC（cmd）` 和 `## 4. 命令行 · Linux / WSL` 章节。
- MSVC 包含方案 A（vcvarsall + Ninja）和方案 B（VS Generator），每个方案附自己的四步版，紧跟各自方案不堆在末尾。
- MinGW 包含方案 A（Ninja）、方案 B（MinGW Makefiles）、方案 C（Ninja Multi-Config），每个附 cmd 三步版。
- 所有内联版全部用绝对路径，无变量依赖，可直接复制粘贴。
- pybind11 项目 MinGW 不适用时，该章节保留标题并注明原因。

## Python 验证脚本

- 验证脚本（`test_*.py`）计算和打印必须分开：先赋值到变量，再单独 print。
- 不在 f-string 里直接调用函数。
- 不用 assert，打印结果给人眼看即可。

## 注释规范

- 注释只描述当前状态，不引用前面章节（不写"和 vector 一样""前面讲过""如上所述"）。
- 每个知识点在自己的位置独立解释，保证跳过前面章节也能读懂。
- 不同知识点的注释之间用空行隔开，不要挤成一大块连续注释。

## 代码可读性

- for 循环体必须加花括号，即使只有一行。
- 防御性检查（如 `empty()`）提前 return，不要放在三元运算符里。
- 不在 return 中使用三元运算符，拆成 if/else。
- 不做多余的 `static_cast`（如 `double / size()` 已自动浮点除法）。
- 一行一条语句，不把多条逻辑压在同一行。
- 函数体不写单行（`int foo() { return 1; }`），展开成多行带花括号。
- `auto`：简单类型显式写（`size_t dot = s.find('.')`），右侧很长时用 auto（迭代器、智能指针、Arrow 类型）。

## Git 提交说明

- subject + body 放在同一个代码块里输出，不要分成两块分别标注。
- 格式：`<type>(<scope>): <subject>`，body 空一行跟在后面。
- 只输出提交说明文本，不要执行 `git commit`，由用户手动提交。
- 不加 `Co-Authored-By` 或任何署名行。
