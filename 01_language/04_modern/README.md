# 04_modern — 现代 C++（C++11-23 核心特性）

> 状态：🚧 建设中（目前仅 `01_lambda` 建成）。
> 定位：**补充章集合**——basics 入门线讲过的现代特性（lambda 基础等）不在此重讲，只收 basics 没有的新内容与实战应用。计划目录见 [STRUCTURE.md](../../../STRUCTURE.md)。

## 知识点分级大纲

<details><summary><a href="01_lambda/">01_lambda</a> — lambda 表达式（补充章：6 个主题 / 11 个小节）</summary>

- 1.1 基本语法
  - 1.1.1 lambda 基本语法
  - 1.1.2 最简 lambda、带参数、显式返回类型（回顾）
- 1.2 捕获列表
  - 1.2.1 捕获列表语法一览
  - 1.2.2 按值 / 按引用捕获 x（回顾）
  - 1.2.3 按值捕获所有
  - 1.2.4 按引用捕获所有
- 1.3 mutable 与泛型 lambda（回顾）
  - 1.3.1 mutable 与泛型 lambda（回顾）
- 1.4 lambda 与 STL 算法
  - 1.4.1 lambda 作为函数参数（总起）
  - 1.4.2 sort — 自定义比较规则
  - 1.4.3 count_if — 统计满足条件的元素个数
  - 1.4.4 for_each — 对每个元素执行一次操作
  - 1.4.5 find_if — 查找第一个满足条件的元素
- 1.5 std::function 批量回调
  - 1.5.1 std::function 是什么（回顾）
  - 1.5.2 存到容器批量执行
- 1.6 IIFE（回顾）
  - 1.6.1 立即调用与复杂 const 初始化（回顾）
</details>

## 目录总览

| 子目录 | 状态 | 内容 |
|--------|------|------|
| [01_lambda](01_lambda/) | ✅ | lambda 补充章：捕获一览/默认捕获/STL 算法/批量回调（基础见 01_basics 05_functions 1.7） |
| 02_move_semantics | ⏳ 规划中 | 移动语义 / std::move / 完美转发 |
| 03_structured_bindings | ⏳ 规划中 | 结构化绑定（C++17） |
| 04_if_switch_init | ⏳ 规划中 | if/switch 初始化语句（C++17） |
| 05_concepts | ⏳ 规划中 | Concepts（C++20） |
| 06_modules | ⏳ 规划中 | Modules（C++20） |
| 07_ranges_brief | ⏳ 规划中 | Ranges 入门（C++20） |

## 说明

- 本章所有知识目录遵循"补充章"规则：与 01_basics 重复的知识点只给一句回顾（标"回顾"小节），不重复展开；新增实战内容才是详解正文。
- 学习路径见 [01_language/README.md](../README.md)：本章属于第二遍深化，先完成 01_basics 再进。
