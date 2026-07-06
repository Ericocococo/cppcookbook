# 01_language — C++ 语言本身

按难度递进学习 C++ 核心语言特性。

| 目录 | 内容 |
|------|------|
| 01_basics | 内置类型、值类别、引用/指针、控制流、函数、命名空间 |
| 02_oop | 类、构造/析构、继承、多态、运算符重载、Rule of 0/3/5 |
| 03_templates | 函数模板、类模板、全/偏特化、变参模板、SFINAE |
| 04_modern | 移动语义、lambda、结构化绑定、Concepts、Modules、Ranges 入门 |
| 05_memory | 智能指针、内存布局、自定义分配器、内存模型、placement new |
| 06_expert | type_traits、constexpr/consteval、静态反射、UB 与陷阱、ABI |

## 术语速查

| 术语 | 全称 / 含义 |
|------|-------------|
| RAII | Resource Acquisition Is Initialization — 资源在构造时获取，在析构时释放 |
| CRTP | Curiously Recurring Template Pattern — 奇异递归模板模式，静态多态 |
| SFINAE | Substitution Failure Is Not An Error — 模板替换失败不是错误（用于编译期条件选择）|
| UB | Undefined Behavior — 未定义行为，编译器对此无任何保证 |
| ABI | Application Binary Interface — 二进制接口，决定函数调用约定、名称修饰等 |
| TMP | Template Metaprogramming — 模板元编程，在编译期执行计算 |
| POD | Plain Old Data — 无虚函数、无用户定义构造的简单数据结构 |
| ODR | One Definition Rule — 单一定义规则，每个符号只能有一个定义 |
