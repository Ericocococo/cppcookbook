# 01_language — C++ 语言本身

按难度递进学习 C++ 核心语言特性。

| 目录           | 内容                                              | 状态 |
|--------------|-------------------------------------------------|------|
| 01_basics    | 内置类型、值类别、引用/指针、控制流、函数、命名空间                      | ✅ 入门主线 |
| 02_oop       | 类、构造/析构、继承、多态、运算符重载、Rule of 0/3/5               | ⏳ 深化专章 |
| 03_templates | 函数模板、类模板、全/偏特化、变参模板、SFINAE                      | ⏳ 深化专章 |
| 04_modern    | 移动语义、lambda、结构化绑定、Concepts、Modules、Ranges 入门    | 🚧 建设中 |
| 05_memory    | 智能指针、内存布局、自定义分配器、内存模型、placement new             | ⏳ 深化专章 |
| 06_expert    | type_traits、constexpr/consteval、静态反射、UB 与陷阱、ABI | ⏳ 深化专章 |

## 学习路径：两层结构，先广后深

C++ 太大，一次深挖会淹死在细节里。所以分**两遍**：

- **第一遍 · `01_basics`（广度 · 入门主线）**：从 hello world 到模板/异常，每个主题讲到"看得懂、写得出"就够。按 `00 → 12` 顺序走一遍，建立全局认知。**小白从这里开始。**
- **第二遍 · `02_oop` ~ `06_expert`（深度 · 专题深化）**：基础扎实后，哪个主题想吃透就进对应专章。比 basics 讲得更透——设计哲学、陷阱、底层原理。

**同一主题，两层分工**（以 OOP 为例）：

| | `01_basics/08_oop`（入门） | `02_oop`（深化） |
|---|---|---|
| 目标 | 会写 class、会用继承多态 | 吃透面向对象设计 |
| 深度 | struct/class、初始化列表、虚函数、Rule 0/3/5 **是什么** | 每个特殊成员函数**细节**、运算符重载**全形式**、友元、RAII、组合 vs 继承 |

> `01_basics/08_oop`（入门）和 `02_oop`（深化）不是重复，是"先会用、再精通"两个层次。
> 现阶段专注走完 `01_basics`，`⏳ 深化专章` 等 basics 全部学完再回头填充。

## 术语速查

| 术语     | 全称 / 含义                                                      |
|--------|--------------------------------------------------------------|
| RAII   | Resource Acquisition Is Initialization — 资源在构造时获取，在析构时释放     |
| CRTP   | Curiously Recurring Template Pattern — 奇异递归模板模式，静态多态         |
| SFINAE | Substitution Failure Is Not An Error — 模板替换失败不是错误（用于编译期条件选择） |
| UB     | Undefined Behavior — 未定义行为，编译器对此无任何保证                        |
| ABI    | Application Binary Interface — 二进制接口，决定函数调用约定、名称修饰等          |
| TMP    | Template Metaprogramming — 模板元编程，在编译期执行计算                    |
| POD    | Plain Old Data — 无虚函数、无用户定义构造的简单数据结构                         |
| ODR    | One Definition Rule — 单一定义规则，每个符号只能有一个定义                     |
