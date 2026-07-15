# 01_basics — C++ 基础语法

每个子目录是一个独立的 CMake 工程，用 CLion 直接打开即可编译运行。

| 子目录 | 内容 |
|--------|------|
| [01_types](01_types/) | 内置类型、字面量后缀、auto/decltype、const/constexpr |
| [02_operators](02_operators/) | 算术、比较、逻辑、位运算、自增自减、三目、sizeof |
| [03_control_flow](03_control_flow/) | if/else、switch、while、do-while、for、范围 for、break/continue |
| [04_functions](04_functions/) | 函数定义、默认参数、重载、内联、函数指针、lambda |
| [05_pointers_refs](05_pointers_refs/) | 引用、指针、nullptr、const 指针、指针算术、new/delete、智能指针 |
| [06_arrays_strings](06_arrays_strings/) | 原始数组、std::array、std::vector、std::string |
| [07_oop](07_oop/) | struct、class 封装、继承、多态（虚函数） |
| [08_misc](08_misc/) | 枚举（enum/enum class）、命名空间、类型转换 |
| [09_templates_exceptions](09_templates_exceptions/) | 函数模板、类模板、模板特化、try/catch/throw |

---

## 1. 数据类型

### 1.1 内置类型速查

| 类型 | 字节 | 用途 |
|------|------|------|
| `bool` | 1 | 只有 true/false，常用于条件判断 |
| `char` | 1 | 单个字符，本质是 -128~127 的小整数（ASCII 码）|
| `int` | 4 | 最常用的整数，-2³¹ ~ 2³¹-1 |
| `long long` | 8 | 需要存非常大的整数时用 |
| `double` | 8 | 小数，精度高（推荐）|
| `float` | 4 | 小数，精度低，省内存（图形/音频处理常用）|
| `unsigned int` | 4 | 无符号整数，0 ~ 2³²-1（不能存负数）|

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

| | `const` | `constexpr` |
|---|---|---|
| 何时确定值 | 运行时 | 编译时 |
| 可用于数组大小 | 可以（大多数编译器支持）| 可以（标准保证）|
| 推荐场景 | 运行时才能确定的常量 | 编译期就能算出来的常量 |

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

| 写法 | 含义 |
|------|------|
| `[x]` | 值捕获 x（副本，lambda 内修改不影响外部）|
| `[&x]` | 引用捕获 x（直接用外部的 x）|
| `[=]` | 值捕获所有外部变量 |
| `[&]` | 引用捕获所有外部变量 |

---

## 5. 指针与引用

### 5.1 引用 vs 指针

| | 引用 | 指针 |
|---|---|---|
| 声明时 | 必须初始化 | 可以先不初始化 |
| 能否为空 | 不能（不存在空引用）| 可以（nullptr）|
| 能否换绑定 | 不能（绑定后固定）| 可以（随时改指向）|
| 语法 | 和普通变量一样 | 需要 `*` 和 `&` |
| 推荐场景 | 函数参数传递、别名 | 动态内存、可空、数组遍历 |

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

| | 原始数组 | `std::array` | `std::vector` |
|---|---|---|---|
| 大小 | 固定（编译期）| 固定（编译期）| 动态可变 |
| 越界检查 | 无（危险）| `.at()` 有检查 | `.at()` 有检查 |
| 传函数时 | 退化为指针（丢失长度）| 完整传递 | 完整传递 |
| 推荐场景 | 与 C 接口交互 | 固定大小集合 | 大多数场景 |

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

| | `enum`（传统）| `enum class`（C++11，推荐）|
|---|---|---|
| 枚举名作用域 | 污染外部命名空间 | 限定在枚举类内 |
| 隐式转为整数 | 可以 | 不可以（需要 `static_cast`）|

### 8.2 类型转换优先级

1. `static_cast`：最常用，编译期检查（整数/浮点/枚举互转）
2. `dynamic_cast`：多态向下转型，运行时检查安全性
3. `const_cast`：去除/添加 const，很少用
4. `reinterpret_cast`：重新解释内存，危险，几乎不用

**不推荐 C 风格 `(int)x`**：看不出转换意图，编译器也不会报警。

---

## 9. 模板与异常

### 9.1 模板的本质

模板是"代码的模具"，编译器根据你实际调用的类型生成对应的函数/类，不会有运行时开销。

### 9.2 异常使用原则

- 只在**真正异常的情况**下抛异常（不是用来做正常控制流）
- `catch` 的顺序：**具体类型先，通用类型后**（`runtime_error` 在 `exception` 前面）
- 函数保证不抛异常时，加 `noexcept`，让编译器做更多优化

### 9.3 标准异常继承关系（简化）

```
std::exception
├── std::logic_error
│   ├── std::invalid_argument
│   └── std::out_of_range
└── std::runtime_error
    ├── std::overflow_error
    └── std::underflow_error
```
