# 01_basics — C++ 基础语法

## 知识点分级大纲

> 学习地图：**一级** = 知识目录（按学习顺序 00 → 12），**二级** = `###` 主题，**三级** = `####` 小节；编号沿用各目录 README 内部编号，点目录名进详细页。

<details><summary><a href="00_hello_world/">00_hello_world</a> — 程序结构（6 个主题 / 17 个小节）</summary>

- 1.1 程序结构
  - 1.1.1 `#include` 引入工具箱
  - 1.1.2 `std` 命名空间
  - 1.1.3 标准库常用头文件
  - 1.1.4 最小可运行程序（main 函数）
- 1.2 注释
  - 1.2.1 单行注释 `//`
- 1.3 变量
  - 1.3.1 声明格式
  - 1.3.2 未初始化变量
  - 1.3.3 命名规则
- 1.4 cout 打印
  - 1.4.1 基本用法
  - 1.4.2 链式调用 + `"\n"` vs `std::endl`
  - 1.4.3 转义字符
  - 1.4.4 `std::cerr` 错误输出
- 1.5 作用域
  - 1.5.1 作用域 `{}`
  - 1.5.2 变量屏蔽（shadowing）
- 1.6 cin 输入
  - 1.6.1 `cin >>` 读单个值
  - 1.6.2 `cin.ignore()` 清缓冲区
  - 1.6.3 `std::getline` 读整行
  </details>

<details><summary><a href="01_types/">01_types</a> — 数据类型（9 个主题 / 22 个小节）</summary>

- 1.1 sizeof 运算符
  - 1.1.1 sizeof 与数组
- 1.2 整数类型
  - 1.2.1 有符号整数
  - 1.2.2 无符号整数
  - 1.2.3 整数溢出
- 1.3 浮点类型
  - 1.3.1 浮点精度问题
  - 1.3.2 浮点特殊值
- 1.4 bool 与 char
  - 1.4.1 bool 类型
  - 1.4.2 char 类型
  - 1.4.3 字符类型家族：char / signed char / unsigned char / wchar_t / charXX_t
  - 1.4.4 转义字符（char 场景）
- 1.5 字面量写法
  - 1.5.1 不同进制
  - 1.5.2 数字分隔符（C++14）
  - 1.5.3 科学计数法
  - 1.5.4 字面量后缀
- 1.6 初始化方式
  - 1.6.1 列表初始化防窄化
  - 1.6.2 数组零初始化
  - 1.6.3 结构体部分初始化
- 1.7 const / constexpr / volatile
  - 1.7.1 const 运行时常量
  - 1.7.2 constexpr 编译期常量
  - 1.7.3 volatile
- 1.8 定宽整数类型
- 1.9 类型别名
  - 1.9.1 using 与 typedef
  - 1.9.2 size_t
  </details>

<details><summary><a href="02_type_queries/">02_type_queries</a> — 类型查询运算符（5 个主题 / 13 个小节）</summary>

- 1.1 sizeof（进阶）
  - 1.1.1 sizeof 不执行表达式
  - 1.1.2 指针和数组的区别
  - 1.1.3 数组长度：优先用 std::size（C++17）
- 1.2 alignof 与结构体填充
  - 1.2.1 结构体填充
  - 1.2.2 alignas
- 1.3 auto（类型推断）
  - 1.3.1 基本推断
  - 1.3.2 auto 剥掉 const 和引用
  - 1.3.3 保留 const/引用要显式写
  - 1.3.4 auto 在范围 for 中的用法
- 1.4 decltype（推断类型，保留 const/引用）
  - 1.4.1 decltype vs auto 的关键区别
  - 1.4.2 decltype(auto)（C++14）
- 1.5 实际使用场景
  - 1.5.1 decltype 推断函数返回类型
  - 1.5.2 验证结构体对齐
  </details>

<details><summary><a href="03_operators/">03_operators</a> — 运算符（8 个主题 / 11 个小节）</summary>

- 1.1 算术运算符
  - 1.1.1 负数取余
  - 1.1.2 浮点除法
  - 1.1.3 整数提升
- 1.2 比较运算符
  - 1.2.1 浮点比较陷阱
  - 1.2.2 链式比较陷阱
- 1.3 逻辑运算符与短路求值
  - 1.3.1 短路求值
  - 1.3.2 短路求值防崩溃
- 1.4 位运算符
  - 1.4.1 标志位操作
- 1.5 自增自减
- 1.6 三目运算符
  - 1.6.1 用三目初始化 const
  - 1.6.2 两分支类型不同时会提升
- 1.7 运算符优先级
  - 1.7.1 位运算优先级陷阱
- 1.8 逗号运算符
</details>

<details><summary><a href="04_control_flow/">04_control_flow</a> — 控制流（7 个主题 / 15 个小节）</summary>

- 1.1 if / else
  - 1.1.1 悬空 else 陷阱
  - 1.1.2 C++17 初始化 if
- 1.2 switch
  - 1.2.1 fall-through 穿透与 `[[fallthrough]]`（C++17）
  - 1.2.2 C++17 初始化 switch
- 1.3 while / do-while
  - 1.3.1 while 循环
  - 1.3.2 do-while 循环
  - 1.3.3 while(true) + break
- 1.4 for 循环
  - 1.4.1 for 可以省略任何部分
  - 1.4.2 多变量 for 循环
- 1.5 范围 for（C++11）
  - 1.5.1 值遍历
  - 1.5.2 引用遍历
  - 1.5.3 const 引用遍历
  - 1.5.4 C++17 结构化绑定遍历 pair
- 1.6 break / continue
  - 1.6.1 break 只退出最近一层循环
  - 1.6.2 goto 退出多层循环
  </details>

<details><summary><a href="05_functions/">05_functions</a> — 函数（8 个主题 / 23 个小节）</summary>

- 1.1 基本函数
  - 1.1.1 基本函数：定义、调用、返回值
  - 1.1.2 函数声明（前向声明）
- 1.2 参数传递
  - 1.2.1 参数传递方式：值、引用、const 引用、指针
  - 1.2.2 const 引用可以绑定临时值
- 1.3 默认参数
  - 1.3.1 默认参数
  - 1.3.2 规则：默认参数只能从右往左设置
- 1.4 函数重载
  - 1.4.1 函数重载：同名函数，参数不同
  - 1.4.2 返回类型不同不构成重载
- 1.5 inline / constexpr 函数
  - 1.5.1 inline 内联函数
  - 1.5.2 constexpr 函数：编译期可求值
- 1.6 函数指针
  - 1.6.1 函数指针基础
  - 1.6.2 函数指针类型声明
  - 1.6.3 函数指针作为参数（回调）
  - 1.6.4 using 简化函数指针类型
  - 1.6.5 std::function：更灵活的可调用对象包装
- 1.7 Lambda 表达式
  - 1.7.1 Lambda 表达式（C++11）
  - 1.7.2 Lambda 基本语法
  - 1.7.3 捕获外部变量
  - 1.7.4 mutable lambda：修改值捕获的副本
  - 1.7.5 泛型 lambda（C++14）：参数用 auto
  - 1.7.6 立即调用 lambda（IIFE）
- 1.8 返回类型
  - 1.8.1 尾置返回类型（trailing return type，C++11）
  - 1.8.2 返回引用的危险：不要返回局部变量的引用
  </details>

<details><summary><a href="06_pointers_refs/">06_pointers_refs</a> — 指针与引用（7 个主题 / 26 个小节）</summary>

- 1.1 引用
  - 1.1.1 引用：变量的别名
  - 1.1.2 引用 vs 指针
  - 1.1.3 const 引用：能绑定临时值
  - 1.1.4 右值引用（C++11）：绑定临时值，用于移动语义
- 1.2 指针
  - 1.2.1 指针：存储地址的变量
  - 1.2.2 未初始化指针：危险
  - 1.2.3 nullptr（C++11）：类型安全的空指针
  - 1.2.4 指针的指针
- 1.3 const 指针
  - 1.3.1 const 指针四种形式
  - 1.3.2 const int* p —— 指向 const 的指针
  - 1.3.3 int* const p —— const 指针
  - 1.3.4 const int* const p —— 指向 const 的 const 指针
  - 1.3.5 助记：从右往左读
- 1.4 指针算术
  - 1.4.1 指针加减：+1 移动一个元素大小
  - 1.4.2 用指针遍历数组
  - 1.4.3 指针差：两指针相减得到元素个数
  - 1.4.4 注意：指针算术只在同一数组内有效
- 1.5 void* 指针
  - 1.5.1 void* 万能指针
- 1.6 new / delete
  - 1.6.1 动态内存分配基础
  - 1.6.2 分配单个对象
  - 1.6.3 分配数组
  - 1.6.4 常见错误
- 1.7 智能指针
  - 1.7.1 智能指针概述（C++11）
  - 1.7.2 unique_ptr：独占所有权
  - 1.7.3 shared_ptr：共享所有权，引用计数
  - 1.7.4 weak_ptr：弱引用，解决循环引用
  </details>

<details><summary><a href="07_arrays_strings/">07_arrays_strings</a> — 数组与字符串（5 个主题 / 29 个小节）</summary>

- 1.1 原始数组
  - 1.1.1 原始数组声明与初始化
  - 1.1.2 自动推断长度
  - 1.1.3 零初始化
  - 1.1.4 二维数组
  - 1.1.5 数组的陷阱：传给函数时退化为指针
- 1.2 std::array（C++11，固定大小）
  - 1.2.1 std::array 基本用法
  - 1.2.2 at() 带越界检查
  - 1.2.3 传给函数不退化
  - 1.2.4 排序
- 1.3 std::vector（动态数组）
  - 1.3.1 std::vector 基本用法
  - 1.3.2 push_back 与自动扩容
  - 1.3.3 reserve：预留空间，避免反复扩容
  - 1.3.4 常用操作
  - 1.3.5 emplace_back：直接在末尾构造
  - 1.3.6 insert / erase
  - 1.3.7 多种初始化方式
- 1.4 std::string
  - 1.4.1 std::string 基本用法
  - 1.4.2 拼接
  - 1.4.3 访问字符
  - 1.4.4 子串 substr
  - 1.4.5 查找 find
  - 1.4.6 替换 replace
  - 1.4.7 数值与字符串互转
  - 1.4.8 字符串比较
  - 1.4.9 C 字符串互转
- 1.5 std::string_view（C++17，只读视图）
  - 1.5.1 string_view 基本用法
  - 1.5.2 string_view 比 const string& 更灵活
  - 1.5.3 string_view 的子串也不复制
  - 1.5.4 注意：string_view 不拥有数据
  </details>

<details><summary><a href="08_oop/">08_oop</a> — 面向对象（5 个主题 / 14 个小节）</summary>

- 1.1 struct
  - 1.1.1 struct：数据聚合，默认 public
- 1.2 class 封装
  - 1.2.1 class 与 struct 的区别
  - 1.2.2 构造函数 + 初始化列表
  - 1.2.3 默认构造与委托构造（C++11）
  - 1.2.4 析构函数：对象销毁时自动调用
- 1.3 继承
  - 1.3.1 继承基础：子类继承父类的成员和方法
  - 1.3.2 非虚函数：共用实现
  - 1.3.3 protected 访问权限
- 1.4 多态
  - 1.4.1 多态：父类指针/引用调用子类的虚函数
  - 1.4.2 父类指针数组，存放不同子类对象
  - 1.4.3 虚析构的重要性
- 1.5 Rule of 0/3/5
  - 1.5.1 三条规则概述
  - 1.5.2 拷贝构造（深拷贝）
  - 1.5.3 移动构造（转移所有权，C++11）
  </details>

<details><summary><a href="09_misc/">09_misc</a> — 枚举、命名空间、类型转换（3 个主题 / 14 个小节）</summary>

- 1.1 枚举：enum（传统）vs enum class（推荐）
  - 1.1.1 传统 enum
  - 1.1.2 enum class（C++11，推荐）
  - 1.1.3 switch 配合 enum class
- 1.2 命名空间：避免名字冲突
  - 1.2.1 嵌套命名空间
  - 1.2.2 C++17 嵌套命名空间简写
  - 1.2.3 using 声明
  - 1.2.4 using namespace
  - 1.2.5 访问 C++17 嵌套命名空间
- 1.3 四种 C++ 类型转换
  - 1.3.1 static_cast
  - 1.3.2 enum class 转 int
  - 1.3.3 dynamic_cast
  - 1.3.4 const_cast
  - 1.3.5 reinterpret_cast
  - 1.3.6 C 风格转换（不推荐）
  </details>

<details><summary><a href="10_templates/">10_templates</a> — 模板（6 个主题 / 9 个小节）</summary>

- 1.1 函数模板：一份代码适用多种类型
  - 1.1.1 编译器自动推断 T
  - 1.1.2 多个类型参数
  - 1.1.3 显式指定类型
  - 1.1.4 类型推断失败：实参类型不一致怎么办
- 1.2 类模板
  - 1.2.1 C++17 类模板参数推断（CTAD）
- 1.3 全特化：为特定类型提供完全不同的实现
  - 1.3.1 函数模板全特化
- 1.4 非类型模板参数：用编译期常量作为模板参数
  - 1.4.1 编译期常量作模板参数
- 1.5 Concepts（C++20）：约束模板参数
  - 1.5.1 requires 约束模板参数
- 1.6 if constexpr（C++17）：编译期条件分支
  - 1.6.1 if constexpr + 类型特性
  </details>

<details><summary><a href="11_exceptions/">11_exceptions</a> — 异常处理（6 个主题 / 9 个小节）</summary>

- 1.1 基本的 try / catch / throw
  - 1.1.1 捕获具体类型
  - 1.1.2 捕获任何类型（兜底）
- 1.2 标准异常层次
  - 1.2.1 常见标准异常演示
- 1.3 自定义异常类
  - 1.3.1 继承标准异常类
- 1.4 noexcept：承诺函数不抛出异常
  - 1.4.1 noexcept 的条件形式
  - 1.4.2 noexcept 的作用
- 1.5 异常安全与 RAII
  - 1.5.1 RAII 保证资源释放
  - 1.5.2 不用 RAII 的危险写法
- 1.6 多层 catch 的顺序
  - 1.6.1 catch 从具体到通用排列
  </details>

<details><summary><a href="12_typeid_pack/">12_typeid_pack</a> — typeid 与变参包（4 个主题 / 10 个小节）</summary>

- 1.1 typeid：查运行时类型
  - 1.1.1 typeid().name()
  - 1.1.2 比较类型是否相同
  - 1.1.3 typeid 在多态中的真正用途
- 1.2 sizeof...：统计变参模板参数数量
  - 1.2.1 sizeof... 统计参数包大小
- 1.3 变参模板基础（C++11）
  - 1.3.1 递归展开
  - 1.3.2 C++17 折叠表达式
- 1.4 type_traits（编译期类型查询）
  - 1.4.1 is_same_v
  - 1.4.2 is_integral_v / is_floating_point_v
  - 1.4.3 is_pointer_v / is_reference_v
  - 1.4.4 add/remove 系列：类型变换
  </details>

## 目录总览

每个子目录是一个独立的 CMake 工程，用 CLion 直接打开即可编译运行。

| 子目录                                     | 内容                                                                                                    |
|-----------------------------------------|-------------------------------------------------------------------------------------------------------|
| [00_hello_world](00_hello_world/)       | 程序结构：#include、main、变量、cout、注释、作用域                                                                     |
| [01_types](01_types/)                   | sizeof（基础）、整数/浮点/bool/char、字面量后缀、初始化、const/constexpr、定宽整数、类型别名                                   |
| [02_type_queries](02_type_queries/)     | sizeof（进阶：不执行/指针退化）、alignof（对齐）、auto（深入）、decltype                                    |
| [03_operators](03_operators/)           | 算术、比较、逻辑（短路）、位运算（标志位）、自增自减、三目、优先级、逗号                                                                  |
| [04_control_flow](04_control_flow/)     | if/else（悬空else）、switch（fall-through）、while、do-while、for、范围for（结构化绑定）、break/continue/goto |
| [05_functions](05_functions/)           | 值/引用/指针传参、默认参数、重载、inline、constexpr函数、函数指针、std::function、lambda（mutable/泛型/IIFE）                       |
| [06_pointers_refs](06_pointers_refs/)   | 引用/右值引用、指针、nullptr、const指针四种、指针算术、void*、new/delete、unique_ptr/shared_ptr/weak_ptr                     |
| [07_arrays_strings](07_arrays_strings/) | 原始数组（退化陷阱）、std::array、std::vector（扩容/reserve）、std::string、string_view（C++17）                          |
| [08_oop](08_oop/)                       | struct、class（封装/初始化列表/委托构造）、继承、多态、纯虚函数、Rule of 0/3/5、移动语义                                             |
| [09_misc](09_misc/)                     | enum/enum class（底层类型）、namespace（嵌套/using）、static_cast/dynamic_cast/const_cast/reinterpret_cast        |
| [10_templates](10_templates/)           | 函数模板、类模板、全特化、非类型参数、Concepts（C++20）、if constexpr                                                       |
| [11_exceptions](11_exceptions/)         | try/catch/throw、标准异常层次、自定义异常、noexcept、RAII异常安全、catch顺序                                                |
| [12_typeid_pack](12_typeid_pack/)       | typeid（多态实际类型）、sizeof...、变参模板（折叠表达式）、type_traits                                                      |

---

## 0. 程序结构

### 0.1 一个程序的"必须件"

```cpp
#include <iostream>   // 引入工具箱，不写就用不了 cout

int main() {          // 程序入口，有且只有一个
    // 代码写在这里
    return 0;         // 返回 0 = 正常退出
}
```

| 部分               | 说明                        |
|------------------|---------------------------|
| `#include <xxx>` | 引入工具箱（头文件），需要什么先包含什么      |
| `int main()`     | 程序入口，操作系统从这里开始执行，固定写法     |
| `{}`             | 作用域边界，函数体、if、for 等都用它划定范围 |
| `;`              | 每条语句结束符，缺少会编译报错           |
| `return 0`       | 向操作系统返回 0，表示程序正常结束        |

### 0.2 `#include` — 引入工具箱

C++ 自带很多工具箱（标准库），但默认什么都不带，用什么先 `#include` 什么：

```cpp
#include <iostream>   // 提供 cout（打印）、cin（读输入）
#include <string>     // 提供 std::string 字符串类型
#include <vector>     // 提供 std::vector 动态数组
#include <cmath>      // 提供 sqrt、sin、pow 等数学函数
```

### 0.3 `std::cout` — 向终端打印

```cpp
std::cout << "文字" << 变量 << "\n";
```

| 部分      | 说明                         |
|---------|----------------------------|
| `std::` | 标准库命名空间前缀，表示"去标准库里找"       |
| `cout`  | console output，控制台输出       |
| `<<`    | 流插入运算符，把右边内容"送进"输出流，可以连续用  |
| `"\n"`  | 换行符，推荐用这个（比 `std::endl` 快） |

### 0.4 变量声明

```cpp
// 格式：类型 变量名 = 初始值;
int    age    = 18;
double height = 1.75;
bool   passed = true;
char   grade  = 'A';          // 单个字符用单引号
std::string name = "小白";    // 字符串用双引号
```

### 0.5 注释

```cpp
// 单行注释：从 // 到行尾，编译器完全忽略

/* 多行注释：
   从 /* 到 */ 之间都被忽略 */
```

注释只给人看，写"为什么"这样写，不写"做了什么"（代码本身已经说明了做了什么）。

### 0.6 作用域 `{}`

变量只在声明它的 `{}` 内有效，出了 `{}` 就消失：

```cpp
int outer = 10;
{
    int inner = 20;   // inner 只在这个 {} 内有效
    // outer 在这里也能访问
}
// 这里访问 inner 会编译报错
```

---

## 1. 数据类型

### 1.1 内置类型速查

| 类型             | 字节 | 用途                              |
|----------------|----|---------------------------------|
| `bool`         | 1  | 只有 true/false，常用于条件判断           |
| `char`         | 1  | 单个字符，本质是 -128~127 的小整数（ASCII 码） |
| `int`          | 4  | 最常用的整数，-2³¹ ~ 2³¹-1             |
| `long long`    | 8  | 需要存非常大的整数时用                     |
| `double`       | 8  | 小数，精度高（推荐）                      |
| `float`        | 4  | 小数，精度低，省内存（图形/音频处理常用）           |
| `unsigned int` | 4  | 无符号整数，0 ~ 2³²-1（不能存负数）          |

### 1.2 初始化方式

```cpp
int a = 10;    // 拷贝初始化（C 风格）
int b(10);     // 直接初始化
int c{10};     // 列表初始化（C++11，推荐）
```

**推荐用 `{}`**：列表初始化会拒绝精度丢失的转换，`int x{3.14}` 编译报错，更安全。

### 1.3 auto 与 decltype

- `auto`：让编译器根据右边的值推断类型，省去手写复杂类型名
- `decltype(expr)`：推断表达式的类型，但不执行表达式

### 1.4 const vs constexpr

|         | `const`      | `constexpr` |
|---------|--------------|-------------|
| 何时确定值   | 运行时          | 编译时         |
| 可用于数组大小 | 可以（大多数编译器支持） | 可以（标准保证）    |
| 推荐场景    | 运行时才能确定的常量   | 编译期就能算出来的常量 |

---

## 2. 运算符

### 2.1 常见陷阱

**整数除法截断**：`10 / 3 = 3`，不是 3.33。想要小数结果，需要至少一个浮点数：`(double)10 / 3`。

**前缀 vs 后缀自增**：

```cpp
int a = 5;
int b = ++a;  // 先加：a=6, b=6
int c = a++;  // 后用：a=7, c=6（c 得到加1前的值）
```

**短路求值**：`&&` 左边为 false 则右边不执行；`||` 左边为 true 则右边不执行。利用这个特性可以避免除零、空指针等错误：

```cpp
if (p != nullptr && *p > 0) { ... }  // p 为空时不会执行 *p
```

---

## 3. 控制流

### 3.1 if 的 C++17 扩展

```cpp
if (int v = getValue(); v > 0) {
    // v 只在这个 if-else 块内有效
}
```

### 3.2 switch 注意 break

switch 中不加 `break` 会"穿透"到下一个 case，这是常见 bug 来源。除非有意共用代码，否则每个 case 都要加 `break`。

### 3.3 范围 for 的引用

```cpp
for (auto& x : vec) { x *= 2; }   // & 引用，修改原数组
for (auto  x : vec) { x *= 2; }   // 值副本，不影响原数组
```

---

## 4. 函数

### 4.1 值传递 vs 引用传递

```cpp
void byValue(int x)  { x = 999; }   // 修改副本，外部不变
void byRef(int& x)   { x = 999; }   // 直接修改外部变量
```

### 4.2 默认参数规则

默认参数只能从右往左声明：

```cpp
void f(int a, int b = 2, int c = 3);  // 合法
void g(int a = 1, int b, int c = 3);  // 错误！b 没有默认值，但左边有
```

### 4.3 Lambda 捕获

| 写法     | 含义                        |
|--------|---------------------------|
| `[x]`  | 值捕获 x（副本，lambda 内修改不影响外部） |
| `[&x]` | 引用捕获 x（直接用外部的 x）          |
| `[=]`  | 值捕获所有外部变量                 |
| `[&]`  | 引用捕获所有外部变量                |

---

## 5. 指针与引用

### 5.1 引用 vs 指针

|       | 引用         | 指针           |
|-------|------------|--------------|
| 声明时   | 必须初始化      | 可以先不初始化      |
| 能否为空  | 不能（不存在空引用） | 可以（nullptr）  |
| 能否换绑定 | 不能（绑定后固定）  | 可以（随时改指向）    |
| 语法    | 和普通变量一样    | 需要 `*` 和 `&` |
| 推荐场景  | 函数参数传递、别名  | 动态内存、可空、数组遍历 |

### 5.2 const 指针四种形式（从右往左读）

```cpp
const int* p    // 指向 const 的指针：不能改值，能改指向
int* const p    // const 指针：能改值，不能改指向
const int* const p  // 两者都不能改
```

### 5.3 智能指针（推荐替代裸指针）

```cpp
auto p = std::make_unique<int>(42);  // 离开作用域自动 delete
auto p = std::make_shared<int>(42);  // 引用计数，最后一个 shared_ptr 销毁时释放
```

裸指针 `new/delete` 容易忘记释放（内存泄漏）或重复释放（崩溃），优先用智能指针。

---

## 6. 数组与字符串

### 6.1 三种数组对比

|      | 原始数组        | `std::array` | `std::vector` |
|------|-------------|--------------|---------------|
| 大小   | 固定（编译期）     | 固定（编译期）      | 动态可变          |
| 越界检查 | 无（危险）       | `.at()` 有检查  | `.at()` 有检查   |
| 传函数时 | 退化为指针（丢失长度） | 完整传递         | 完整传递          |
| 推荐场景 | 与 C 接口交互    | 固定大小集合       | 大多数场景         |

### 6.2 string 常用操作

```cpp
s.length()          // 长度
s += " world"       // 拼接
s.substr(0, 5)      // 子串
s.find("world")     // 查找，返回 string::npos 表示没找到
stoi(s) / stod(s)   // string -> 数值
to_string(42)       // 数值 -> string
```

---

## 7. 面向对象（OOP）

### 7.1 struct vs class

唯一区别：`struct` 默认成员 `public`，`class` 默认成员 `private`。

习惯上：

- `struct` 用于纯数据聚合（没有复杂逻辑）
- `class` 用于有封装逻辑的对象

### 7.2 构造函数初始化列表

```cpp
class Foo {
    int x; double y;
public:
    Foo(int a, double b) : x(a), y(b) {}  // 初始化列表（推荐）
    // 而不是在函数体里 x = a; y = b;（那是赋值，不是初始化）
};
```

### 7.3 多态三要素

1. **虚函数** `virtual`：父类声明，允许子类覆盖
2. **override** 关键字：子类明确标注覆盖，编译器帮你验证
3. **虚析构函数**：父类有子类时必须加，否则 `delete 父类指针` 不会调用子类析构

---

## 8. 枚举、命名空间、类型转换

### 8.1 enum vs enum class

|        | `enum`（传统） | `enum class`（C++11，推荐） |
|--------|------------|------------------------|
| 枚举名作用域 | 污染外部命名空间   | 限定在枚举类内                |
| 隐式转为整数 | 可以         | 不可以（需要 `static_cast`）  |

### 8.2 类型转换优先级

1. `static_cast`：最常用，编译期检查（整数/浮点/枚举互转）
2. `dynamic_cast`：多态向下转型，运行时检查安全性
3. `const_cast`：去除/添加 const，很少用
4. `reinterpret_cast`：重新解释内存，危险，几乎不用

**不推荐 C 风格 `(int)x`**：看不出转换意图，编译器也不会报警。

---

## 9. 模板

### 9.1 模板的本质

模板是"代码的模具"，编译器根据实际调用的类型生成对应的函数/类，**零运行时开销**（代价在编译期）。

```cpp
template<typename T>
T maxVal(T a, T b) { return (a > b) ? a : b; }

maxVal(3, 5);       // 编译器生成 maxVal<int>
maxVal(3.14, 2.71); // 编译器生成 maxVal<double>
```

### 9.2 函数模板 vs 类模板

|      | 函数模板     | 类模板           |
|------|----------|---------------|
| 类型推断 | 自动（根据实参） | 需显式写（C++17 前） |
| 特化   | 全特化      | 全特化 + 偏特化     |
| 典型用途 | 通用算法     | 通用容器          |

### 9.3 非类型模板参数

```cpp
template<typename T, size_t N>
class FixedArray { T data_[N]; };   // N 在编译期确定，无动态内存

FixedArray<int, 5> arr;   // 类似 std::array<int,5>
```

### 9.4 Concepts（C++20）约束

```cpp
template<typename T>
    requires std::is_arithmetic_v<T>
T average(T a, T b) { return (a + b) / 2; }
// average("a","b") → 编译错误，报错信息更友好
```

### 9.5 if constexpr（C++17）

```cpp
template<typename T>
void process(T val) {
    if constexpr (std::is_integral_v<T>) {
        // 只有 T 是整数时才编译这段
    } else {
        // 其他类型走这里
    }
}
```

与普通 `if` 的区别：两个分支分别在编译期裁剪，不要求两段代码都能对当前 T 编译通过。

---

## 10. 异常

### 10.1 基本语法

```cpp
try {
    if (bad) throw std::runtime_error("出错了");
} catch (const std::runtime_error& e) {
    std::cout << e.what();
} catch (const std::exception& e) {
    // 兜底：具体类型先，通用类型后（顺序不能反）
}
```

### 10.2 标准异常继承关系（简化）

```
std::exception
├── std::logic_error          // 程序逻辑错误，可预防
│   ├── std::invalid_argument
│   └── std::out_of_range
└── std::runtime_error        // 运行时无法预防的错误
    ├── std::overflow_error
    └── std::underflow_error
```

### 10.3 使用原则

- 只在**真正异常的情况**下抛异常，不用作正常控制流
- `catch` 顺序：具体类型先，`std::exception` 兜底后
- 确保不抛异常的函数标 `noexcept`，编译器可以做更多优化
- 用 RAII（对象析构自动释放资源）而不是 try/catch 来保证异常安全

### 10.4 自定义异常

```cpp
class AppError : public std::runtime_error {
    int code_;
public:
    AppError(int code, const std::string& msg)
        : std::runtime_error(msg), code_(code) {}
    int code() const { return code_; }
};
```

---

## 11. typeid / 变参包

### 11.1 typeid

```cpp
#include <typeinfo>
typeid(expr).name()   // 返回类型名（格式因编译器而异）
typeid(a) == typeid(b)  // 比较两个表达式的类型是否相同
```

**多态场景**：`typeid(*ptr)` 返回运行期实际类型（需要 virtual 函数存在）。
适合调试；生产代码优先用虚函数多态，不用 typeid 做类型分发。

### 11.2 sizeof...

```cpp
template<typename... Args>
void f(Args... args) {
    std::cout << sizeof...(args);  // 参数包里有多少个参数
}
f(1, 2.0, "hi");  // 输出 3
```

### 11.3 折叠表达式（C++17）

```cpp
template<typename... Args>
auto sum(Args... args) { return (args + ...); }  // a1 + a2 + a3 + ...

template<typename... Args>
void print(Args... args) { ((std::cout << args << " "), ...); }
```

| 写法                                | 展开结果                         |
|-----------------------------------|------------------------------|
| `(args + ...)`                    | `a1 + (a2 + (a3 + ...))` 右折叠 |
| `(... + args)`                    | `((a1 + a2) + a3) + ...` 左折叠 |
| `(std::cout << args << " ", ...)` | 逐个打印                         |

### 11.4 type_traits 速查

```cpp
std::is_same_v<T, U>          // T 和 U 类型完全相同？
std::is_integral_v<T>         // T 是整数类型？
std::is_floating_point_v<T>   // T 是浮点类型？
std::is_pointer_v<T>          // T 是指针？
std::remove_const_t<T>        // 去掉 const
std::add_pointer_t<T>         // 加指针：T → T*
```
