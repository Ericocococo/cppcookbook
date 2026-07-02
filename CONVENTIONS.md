# 习惯约定

> 本仓库在编写/填充过程中形成的习惯，和 [目录设计文档](STRUCTURE.md) 里的
> "贯穿全库的约定"互补——那边讲**结构规则**，这边讲**动笔时的习惯**。

---

## 目录与结构

1. **用到再建，不建空壳** — 目录只在有文件时才创建，不预先遗留空目录占位
   （空目录 git 本来也不跟踪）。设计文档勾勒的是"规划"，不代表磁盘上都得建出来。
2. **所有层级目录带两位数字前缀**（`01_ 02_ ...`），阅读顺序即学习/工程顺序；
   工程约定目录（`tests/`、`.github/`）按惯例不加号。
3. **一个库/主题一个子目录，内部按功能拆多个带序号 `.cpp`** —
   以 [`03_stl/01_containers/`](03_stl/01_containers/) 为范例（vector/map/queue/...），
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

## CMake 结构

8. **每个目录都是独立的 CMake 工程** — 任意层级的子目录均可在 CLion 中直接打开为独立项目，
   互不依赖，没有根目录聚合所有子目录的主工程。
9. **每个含 `.cpp` 的目录都有自己的 `CMakeLists.txt`** — 包含完整的
   `cmake_minimum_required` + `project()` + `add_executable()`，开箱即编译，无需上级配置。
10. **共享设置通过 `cmake/common.cmake` 引入** — 编译标准、警告级别等公共选项写在根目录
    `cmake/common.cmake`，各子目录 `include()` 引用，改一处全局生效。

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
