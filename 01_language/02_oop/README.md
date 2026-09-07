# 02_oop — 面向对象深化专章

> 前置要求：已完成 `01_basics`（主要依赖 08_oop 入门概念；04_polymorphism 的 CRTP 需要 10_templates 模板基础；异常/RAII 概念见 11_exceptions）。
> 本章在每个主题上深入展开，basics 已讲的入门内容只给"回顾"，只展开新细节与惯用法（详情见 [01_language/README.md](../README.md) 的两层学习路径）。

## 1. 知识点分级大纲

<details><summary><a href="01_classes_objects/">01_classes_objects</a> — 类深入（6 个主题 / 8 个小节）</summary>

- 1.1 友元函数（friend function）
  - 1.1.1 友元函数
- 1.2 友元类（friend class）
  - 1.2.1 友元类
- 1.3 嵌套类（nested class）
  - 1.3.1 嵌套类
- 1.4 this 指针
  - 1.4.1 this 指针
  - 1.4.2 链式调用
- 1.5 mutable 关键字
  - 1.5.1 mutable 关键字
- 1.6 static 成员深入
  - 1.6.1 static 成员变量与函数
  - 1.6.2 Meyers' Singleton
</details>

<details><summary><a href="02_constructors_raii/">02_constructors_raii</a> — 构造与 RAII（5 个主题 / 14 个小节）</summary>

- 1.1 构造函数全家族
  - 1.1.1 参数构造函数
  - 1.1.2 委托构造函数（C++11）
  - 1.1.3 拷贝构造函数
  - 1.1.4 移动构造函数（C++11）
  - 1.1.5 转换构造 + explicit
- 1.2 初始化列表 vs 函数体赋值
  - 1.2.1 回顾与对比表
  - 1.2.2 初始化顺序
- 1.3 析构顺序
  - 1.3.1 析构顺序
  - 1.3.2 继承体系的析构顺序
- 1.4 RAII（Resource Acquisition Is Initialization）
  - 1.4.1 RAII 核心思想（回顾）
  - 1.4.2 LockGuard 模式
- 1.5 copy-and-swap 惯用法
  - 1.5.1 copy-and-swap 惯用法
  - 1.5.2 swap 函数
  - 1.5.3 赋值运算符（copy-and-swap）
</details>

<details><summary><a href="03_inheritance/">03_inheritance</a> — 继承深入（6 个主题 / 9 个小节）</summary>

- 1.1 多继承（multiple inheritance）
  - 1.1.1 多继承
- 1.2 菱形继承问题（diamond problem）
  - 1.2.1 菱形继承问题
- 1.3 virtual 继承
  - 1.3.1 virtual 继承
- 1.4 final 关键字
  - 1.4.1 final 的两种用法
- 1.5 隐藏（hiding）vs 覆盖（overriding）
  - 1.5.1 隐藏（name hiding）
  - 1.5.2 覆盖（overriding）
  - 1.5.3 using 声明恢复被隐藏的函数
- 1.6 继承中的访问控制
  - 1.6.1 三种继承方式的访问控制
  - 1.6.2 private 继承
</details>

<details><summary><a href="04_polymorphism/">04_polymorphism</a> — 多态深入（5 个主题 / 9 个小节）</summary>

- 1.1 虚函数表（vtable）原理
  - 1.1.1 vtable 结构
  - 1.1.2 虚函数调用过程
- 1.2 dynamic_cast 安全向下转型
  - 1.2.1 向下转型
- 1.3 抽象类与接口模式
  - 1.3.1 抽象类（回顾）
  - 1.3.2 接口模式
- 1.4 协变返回类型（covariant return types）
  - 1.4.1 协变返回规则
  - 1.4.2 典型应用——clone() 方法
- 1.5 多态的性能考量
  - 1.5.1 虚函数调用的开销
  - 1.5.2 优化手段——CRTP
</details>

<details><summary><a href="05_operator_overload/">05_operator_overload</a> — 运算符重载（7 个主题 / 11 个小节）</summary>

- 1.1 算术运算符（+, -, *, /）
  - 1.1.1 operator+= 返回引用
  - 1.1.2 标量乘法的两种写法
  - 1.1.3 operator+ 用非成员函数的原因
  - 1.1.4 基于 += 实现 + 的惯用法
- 1.2 比较运算符（==, <=>）
  - 1.2.1 太空船运算符 <=> 的用法（C++20）
- 1.3 流运算符（<<, >>）
  - 1.3.1 流运算符必须用友元函数
- 1.4 下标运算符（[]）
  - 1.4.1 下标运算符的 const 版本
- 1.5 函数调用运算符（()）— 仿函数
  - 1.5.1 仿函数（Functor）
- 1.6 自增 / 自减运算符（++, --）
  - 1.6.1 前缀自增
  - 1.6.2 后缀自增 vs 前缀自增
- 1.7 类型转换运算符
  - 1.7.1 explicit operator bool()
</details>

<details><summary><a href="06_special_members/">06_special_members</a> — 特殊成员函数（6 个主题 / 11 个小节）</summary>

- 1.1 六个特殊成员函数
  - 1.1.1 六个特殊成员函数总览
- 1.2 编译器何时自动生成
  - 1.2.1 编译器自动生成规则
  - 1.2.2 移动退化为拷贝
- 1.3 = default：显式让编译器生成
  - 1.3.1 = default 恢复默认构造
  - 1.3.2 = default 用于拷贝和移动
- 1.4 = delete：显式禁用
  - 1.4.1 = delete 禁止拷贝
- 1.5 Rule of Zero
  - 1.5.1 Rule of Zero
  - 1.5.2 unique_ptr 与 Rule of Zero
- 1.6 Rule of Five 完整示例（copy-and-swap 惯用法）
  - 1.6.1 三步法回顾（详讲见 02_constructors_raii）
  - 1.6.2 noexcept 与 vector 扩容（回顾）
  - 1.6.3 std::exchange：一步完成"取走旧值并置空"
</details>

## 2. 知识点总览

| 子目录 | 主题 | 先修（01_basics） | 本章新增 |
|---|---|---|---|
| [01_classes_objects](01_classes_objects/) | 类深入 | struct/class、private/public（08_oop） | 友元、嵌套类、this 指针、mutable、static 深入 |
| [02_constructors_raii](02_constructors_raii/) | 构造与 RAII | 构造/初始化列表/委托（08_oop 1.2）、RAII 概念（11_exceptions 1.5） | 全构造家族、explicit、析构顺序、LockGuard、copy-and-swap |
| [03_inheritance](03_inheritance/) | 继承深入 | 单继承、virtual、protected（08_oop 1.3） | 多继承、菱形继承、virtual 继承、final、隐藏 vs 覆盖 |
| [04_polymorphism](04_polymorphism/) | 多态深入 | 纯虚、父类指针调子类（08_oop 1.4）、模板基础（10_templates 1.2） | vtable 原理、dynamic_cast、接口模式、协变返回、CRTP |
| [05_operator_overload](05_operator_overload/) | 运算符重载 | lambda/std::function（05_functions 1.6–1.7） | 算术/比较/流/下标/仿函数/自增/太空船(C++20) |
| [06_special_members](06_special_members/) | 特殊成员函数 | Rule 0/3/5 概念（08_oop 1.5） | =default/=delete、编译器生成规则、copy-and-swap 完整实现 |

## 3. 构建

> 六个子目录都是独立 CMake 工程，每个子目录的 README 自带完整构建命令（MinGW 三方案 / MSVC 三方案 / Linux），进入子目录复制即用。
> CMake 与 CMakeLists.txt 的首次详细讲解在 [01_basics/00_hello_world](../01_basics/00_hello_world/README.md) 的构建章节（本仓库第一个 CMake 工程）。

| 子目录 | 可执行文件名 |
|---|---|
| 01_classes_objects | classes_objects |
| 02_constructors_raii | constructors_raii |
| 03_inheritance | inheritance |
| 04_polymorphism | polymorphism |
| 05_operator_overload | operator_overload |
| 06_special_members | special_members |
