// C++20
// 演示：特殊成员函数深入 — =default/=delete、编译器生成规则、copy-and-swap

#include <iostream>
#include <string>
#include <vector>
#include <memory>     // std::unique_ptr
#include <utility>    // std::move, std::swap, std::exchange
#include <algorithm>  // std::copy

// ═══════════════════════════════════════════════════════════════════
// ① 六个特殊成员函数
// ═══════════════════════════════════════════════════════════════════
//
// C++ 编译器最多可以为一个类自动生成以下 6 个特殊成员函数：
//
//   1. 默认构造函数       T()
//      作用：无参数创建对象，对成员执行默认初始化
//
//   2. 析构函数           ~T()
//      作用：对象生命周期结束时自动调用，释放资源
//
//   3. 拷贝构造函数       T(const T&)
//      作用：用另一个同类型对象初始化新对象（逐成员拷贝）
//
//   4. 拷贝赋值运算符     T& operator=(const T&)
//      作用：把另一个对象的值赋给已存在的对象（逐成员赋值）
//
//   5. 移动构造函数       T(T&&) noexcept
//      作用：从右值（临时对象）"偷走"资源，避免深拷贝
//
//   6. 移动赋值运算符     T& operator=(T&&) noexcept
//      作用：把右值的资源转移给已存在的对象
//

// 简单示例：所有特殊成员都打印日志
class Traced
{
    std::string name_;

public:
    // 1. 默认构造
    Traced()
        : name_("(unnamed)")
    {
        std::cout << "    [默认构造] " << name_ << "\n";
    }

    // 带参构造（不是特殊成员，但很常用）
    explicit Traced(std::string name)
        : name_(std::move(name))
    {
        std::cout << "    [带参构造] " << name_ << "\n";
    }

    // 2. 析构
    ~Traced()
    {
        std::cout << "    [析构] " << name_ << "\n";
    }

    // 3. 拷贝构造
    Traced(const Traced& other)
        : name_(other.name_ + "_copy")
    {
        std::cout << "    [拷贝构造] " << name_ << " <- " << other.name_ << "\n";
    }

    // 4. 拷贝赋值
    Traced& operator=(const Traced& other)
    {
        name_ = other.name_ + "_assigned";
        std::cout << "    [拷贝赋值] " << name_ << " <- " << other.name_ << "\n";
        return *this;
    }

    // 5. 移动构造
    Traced(Traced&& other) noexcept
        : name_(std::move(other.name_))
    {
        other.name_ = "(moved-from)";
        std::cout << "    [移动构造] " << name_ << "\n";
    }

    // 6. 移动赋值
    Traced& operator=(Traced&& other) noexcept
    {
        name_ = std::move(other.name_);
        other.name_ = "(moved-from)";
        std::cout << "    [移动赋值] " << name_ << "\n";
        return *this;
    }

    const std::string& name() const
    {
        return name_;
    }
};

void demo01_six_members()
{
    std::cout << "\n① 六个特殊成员函数\n";

    std::cout << "  创建 a（带参构造）:\n";
    Traced a("alpha");

    std::cout << "  创建 b（拷贝构造）:\n";
    Traced b(a);

    std::cout << "  创建 c（默认构造）:\n";
    Traced c;

    std::cout << "  c = a（拷贝赋值）:\n";
    c = a;

    std::cout << "  创建 d（移动构造）:\n";
    Traced d(std::move(a));

    std::cout << "  d = Traced(\"temp\")（移动赋值）:\n";
    d = Traced("temp");

    std::cout << "  ---- 函数结束，所有对象析构 ----\n";
}

// ═══════════════════════════════════════════════════════════════════
// ② 编译器何时自动生成
// ═══════════════════════════════════════════════════════════════════
//
// 规则汇总表（用户定义了某个 -> 对其他的影响）：
//
// ┌─────────────────────┬──────────┬──────────┬──────────┬──────────┐
// │ 用户定义了...        │ 默认构造 │ 拷贝构造 │ 拷贝赋值 │ 移动操作 │
// ├─────────────────────┼──────────┼──────────┼──────────┼──────────┤
// │ 任何构造函数         │ 不生成   │ 生成     │ 生成     │ 生成     │
// │ 析构函数             │ 生成     │ 生成*    │ 生成*    │ 不生成   │
// │ 拷贝构造函数         │ 不生成   │ -        │ 生成*    │ 不生成   │
// │ 拷贝赋值运算符       │ 生成     │ 生成*    │ -        │ 不生成   │
// │ 移动构造或移动赋值    │ 不生成   │ =delete  │ =delete  │ -       │
// └─────────────────────┴──────────┴──────────┴──────────┴──────────┘
//   * 标记的情况：编译器仍生成，但 C++ 标准标注为 deprecated（不推荐依赖）
//
// 核心要点：
//   - 定义了析构 -> 移动不再自动生成，move 退化为拷贝
//   - 定义了拷贝构造 -> 移动不再自动生成
//   - 定义了移动 -> 编译器把拷贝 delete 掉（不让你拷贝）

// 演示：只定义析构后，移动退化为拷贝
class HasDestructor
{
    std::string name_;

public:
    HasDestructor(std::string n) : name_(std::move(n))
    {
    }

    // 自定义析构 -> 编译器不再生成移动构造/移动赋值
    // 但拷贝构造/拷贝赋值仍然生成（deprecated 行为）
    ~HasDestructor()
    {
        std::cout << "    [析构] " << name_ << "\n";
    }

    const std::string& name() const
    {
        return name_;
    }
};

void demo02_generation_rules()
{
    std::cout << "\n② 编译器何时自动生成\n";

    HasDestructor h1("hello");

    // std::move(h1) 想触发移动构造，但移动构造未生成
    // 编译器退回到拷贝构造（const T& 可以绑定右值）
    // 结果：h1 没被"偷走"资源，h2 是 h1 的副本
    HasDestructor h2(std::move(h1));

    std::cout << "  h1.name = \"" << h1.name() << "\""
              << "  （期望被移走，但其实仍在，因为退化为拷贝）\n";
    std::cout << "  h2.name = \"" << h2.name() << "\"\n";
}

// ═══════════════════════════════════════════════════════════════════
// ③ = default：显式让编译器生成
// ═══════════════════════════════════════════════════════════════════

class Widget
{
    int id_;
    std::string label_;

public:
    // 自定义了带参构造函数
    Widget(int id, std::string label)
        : id_(id), label_(std::move(label))
    {
    }

    // 定义了任何构造函数后，默认构造函数不再自动生成
    // Widget w;  // 编译错误！没有默认构造
    // 用 = default 恢复编译器默认实现
    Widget() = default;

    // 对拷贝和移动也可以用 = default
    // 场景：自定义了析构，但拷贝/移动的默认实现够用
    // 显式写 = default 清晰表达意图："我知道规则，我要编译器版本"
    Widget(const Widget&) = default;
    Widget& operator=(const Widget&) = default;
    Widget(Widget&&) noexcept = default;
    Widget& operator=(Widget&&) noexcept = default;
    ~Widget() = default;

    void print() const
    {
        std::cout << "    Widget{id=" << id_ << ", label=\"" << label_ << "\"}\n";
    }
};

void demo03_default()
{
    std::cout << "\n③ = default\n";

    // 默认构造可用（被 = default 恢复）
    Widget w1;
    w1.print();

    Widget w2(42, "hello");
    w2.print();

    // 拷贝
    Widget w3 = w2;
    std::cout << "  拷贝 w3:\n";
    w3.print();

    // 移动
    Widget w4 = std::move(w2);
    std::cout << "  移动 w4:\n";
    w4.print();
}

// ═══════════════════════════════════════════════════════════════════
// ④ = delete：显式禁用
// ═══════════════════════════════════════════════════════════════════

// 不可拷贝的资源类（模拟文件句柄、网络连接等）
class UniqueResource
{
    std::string resource_name_;
    int handle_;

public:
    explicit UniqueResource(std::string name, int handle)
        : resource_name_(std::move(name)), handle_(handle)
    {
        std::cout << "    [获取资源] " << resource_name_
                  << " (handle=" << handle_ << ")\n";
    }

    ~UniqueResource()
    {
        if (handle_ >= 0)
        {
            std::cout << "    [释放资源] " << resource_name_
                      << " (handle=" << handle_ << ")\n";
        }
    }

    // ---- 禁止拷贝 ----
    // 资源句柄不能被复制，两个对象持有同一个 handle 会导致双重释放
    // = delete 让编译器在任何尝试拷贝的地方报错
    UniqueResource(const UniqueResource&) = delete;
    UniqueResource& operator=(const UniqueResource&) = delete;

    // ---- 允许移动 ----
    // 资源的所有权可以转移
    UniqueResource(UniqueResource&& other) noexcept
        : resource_name_(std::move(other.resource_name_)),
          handle_(std::exchange(other.handle_, -1))
    {
        std::cout << "    [转移资源] " << resource_name_
                  << " (handle=" << handle_ << ")\n";
    }

    UniqueResource& operator=(UniqueResource&& other) noexcept
    {
        if (this != &other)
        {
            // 释放自己的旧资源
            if (handle_ >= 0)
            {
                std::cout << "    [释放旧资源] " << resource_name_ << "\n";
            }
            resource_name_ = std::move(other.resource_name_);
            handle_ = std::exchange(other.handle_, -1);
        }
        return *this;
    }

    void print() const
    {
        std::cout << "    resource=\"" << resource_name_
                  << "\", handle=" << handle_ << "\n";
    }
};

// ---- delete 也可以用于普通函数，禁止某些重载 ----
// 只接受 double，禁止 int（避免隐式转换丢精度）
void process(double val)
{
    std::cout << "    process(double): " << val << "\n";
}

void process(int) = delete;  // 传 int 直接编译错误

void demo04_delete()
{
    std::cout << "\n④ = delete\n";

    UniqueResource r1("database", 100);

    // UniqueResource r2 = r1;     // 编译错误：拷贝构造被 delete
    // UniqueResource r3(r1);      // 编译错误：拷贝构造被 delete

    // 移动是允许的
    UniqueResource r2 = std::move(r1);
    std::cout << "  r1 移动后:\n";
    r1.print();
    std::cout << "  r2:\n";
    r2.print();

    // delete 普通函数重载
    std::cout << "  禁止 int 重载:\n";
    process(3.14);
    // process(42);   // 编译错误：use of deleted function
}

// ═══════════════════════════════════════════════════════════════════
// ⑤ Rule of Zero
// ═══════════════════════════════════════════════════════════════════
//
// 最佳实践：尽量不自定义任何特殊成员函数
//
// 原因：
//   - std::string、std::vector 等 RAII 容器自己管理资源
//   - 编译器默认生成的拷贝/移动/析构会逐成员调用对应操作
//   - 每个成员的操作已经正确，组合起来自然也正确
//   - 不写特殊成员 = 不出错 = 不需要维护 = 最安全

class Person
{
    std::string name_;
    int age_;
    std::vector<std::string> hobbies_;

    // 没有任何自定义特殊成员函数！
    // 编译器自动生成的全部正确：
    //   - 拷贝：string 和 vector 的拷贝构造做深拷贝
    //   - 移动：string 和 vector 的移动构造转移所有权
    //   - 析构：string 和 vector 的析构释放内存

public:
    Person(std::string name, int age, std::vector<std::string> hobbies)
        : name_(std::move(name)),
          age_(age),
          hobbies_(std::move(hobbies))
    {
    }

    void print() const
    {
        std::cout << "    " << name_ << " (age=" << age_ << ") hobbies: ";
        for (const auto& h : hobbies_)
        {
            std::cout << h << " ";
        }
        std::cout << "\n";
    }
};

// 用 unique_ptr 管理裸指针也符合 Rule of Zero
class ResourceOwner
{
    std::unique_ptr<int[]> data_;
    size_t size_;

    // unique_ptr 不可拷贝 -> ResourceOwner 也不可拷贝（自动推导）
    // unique_ptr 可移动 -> ResourceOwner 也可移动（自动推导）
    // 析构时 unique_ptr 自动释放 -> 不需要写析构

public:
    explicit ResourceOwner(size_t n)
        : data_(std::make_unique<int[]>(n)), size_(n)
    {
        for (size_t i = 0; i < n; ++i)
        {
            data_[i] = static_cast<int>(i * 100);
        }
    }

    void print() const
    {
        std::cout << "    ResourceOwner[" << size_ << "]: ";
        for (size_t i = 0; i < size_; ++i)
        {
            std::cout << data_[i] << " ";
        }
        std::cout << "\n";
    }
};

void demo05_rule_of_zero()
{
    std::cout << "\n⑤ Rule of Zero\n";

    Person p1("Alice", 30, {"reading", "coding", "hiking"});
    p1.print();

    // 拷贝：编译器默认生成，逐成员深拷贝
    Person p2 = p1;
    std::cout << "  拷贝 p2:\n";
    p2.print();

    // 移动：编译器默认生成，逐成员转移
    Person p3 = std::move(p1);
    std::cout << "  移动 p3:\n";
    p3.print();

    // unique_ptr 的 Rule of Zero
    ResourceOwner r1(3);
    r1.print();

    // ResourceOwner r2 = r1;  // 编译错误：unique_ptr 不可拷贝

    ResourceOwner r2 = std::move(r1);
    std::cout << "  移动 r2:\n";
    r2.print();
}

// ═══════════════════════════════════════════════════════════════════
// ⑥ Rule of Five 完整示例（copy-and-swap 惯用法）
// ═══════════════════════════════════════════════════════════════════
//
// 管理裸资源时，必须定义全部 5 个特殊成员：
//   析构 + 拷贝构造 + 拷贝赋值 + 移动构造 + 移动赋值
//
// copy-and-swap 惯用法：
//   1. 拷贝赋值的参数按值传入（触发拷贝构造）
//   2. 然后和自己 swap
//   好处：
//     - 异常安全：拷贝失败时 *this 不变（强异常保证）
//     - 自动处理自赋值：a = a 也安全
//     - 代码简洁：复用拷贝构造的逻辑

class DynamicArray
{
    int* data_;
    size_t size_;

    // 私有辅助函数：打印内容
    void print_data(const char* label) const
    {
        std::cout << "    [" << label << "] this=" << this
                  << " size=" << size_ << " data={";
        for (size_t i = 0; i < size_; ++i)
        {
            if (i > 0)
            {
                std::cout << ", ";
            }
            std::cout << data_[i];
        }
        std::cout << "}\n";
    }

public:
    // 构造函数
    explicit DynamicArray(size_t n = 0)
        : data_(n > 0 ? new int[n]{} : nullptr), size_(n)
    {
        std::cout << "    [构造] size=" << n << "\n";
    }

    // ---- 1. 析构函数 ----
    ~DynamicArray()
    {
        std::cout << "    [析构] size=" << size_ << "\n";
        delete[] data_;
    }

    // ---- 2. 拷贝构造函数 ----
    // 深拷贝：分配新内存，逐元素复制
    DynamicArray(const DynamicArray& other)
        : data_(other.size_ > 0 ? new int[other.size_] : nullptr),
          size_(other.size_)
    {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "    [拷贝构造] 从 size=" << other.size_ << " 深拷贝\n";
    }

    // ---- 3. 拷贝赋值运算符（copy-and-swap 惯用法）----
    //
    // 参数按值传入，而非 const T&
    // 按值传入时，编译器自动调用拷贝构造或移动构造来创建 other
    //   - 如果传入的是左值：调用拷贝构造
    //   - 如果传入的是右值：调用移动构造
    // 然后和自己 swap，旧数据交给 other，other 析构时释放
    //
    // 好处：
    //   - 异常安全：new 在拷贝构造中完成，失败时 *this 不变
    //   - 自赋值安全：a = a 时，other 是 a 的拷贝，swap 后 a 仍然正确
    //   - 同时处理拷贝赋值和移动赋值（传入右值时变成移动赋值）
    DynamicArray& operator=(DynamicArray other) // 注意：按值传入
    {
        std::cout << "    [赋值] copy-and-swap\n";
        swap(*this, other);
        // other 在函数结束时析构，释放旧数据
        return *this;
    }

    // ---- 4. 移动构造函数 ----
    // 从右值"偷走"资源，被移动的对象置为空
    //
    // noexcept 至关重要：
    //   std::vector 扩容时，如果元素的移动构造是 noexcept，
    //   vector 才会用移动而非拷贝。不标 noexcept = 性能退化。
    DynamicArray(DynamicArray&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)),
          size_(std::exchange(other.size_, 0))
    {
        std::cout << "    [移动构造] 偷走 size=" << size_ << "\n";
    }

    // ---- 移动赋值：不需要单独写 ----
    // copy-and-swap 的拷贝赋值参数按值传入，传入右值时自动变成移动赋值
    // 如果要单独写：
    //   DynamicArray& operator=(DynamicArray&& other) noexcept {
    //       swap(*this, other);
    //       return *this;
    //   }

    // ---- swap 函数 ----
    // 友元 swap：让 std::swap 和 ADL（参数依赖查找）都能找到
    friend void swap(DynamicArray& a, DynamicArray& b) noexcept
    {
        using std::swap;
        swap(a.data_, b.data_);
        swap(a.size_, b.size_);
    }

    // 赋值元素
    void set(size_t index, int value)
    {
        if (index < size_)
        {
            data_[index] = value;
        }
    }

    size_t size() const
    {
        return size_;
    }

    void print(const char* label = "DynamicArray") const
    {
        print_data(label);
    }
};

void demo06_rule_of_five()
{
    std::cout << "\n⑥ Rule of Five（copy-and-swap）\n";

    // 构造
    DynamicArray a(3);
    a.set(0, 10);
    a.set(1, 20);
    a.set(2, 30);
    a.print("a 初始");

    // 拷贝构造
    std::cout << "  --- 拷贝构造 b = a ---\n";
    DynamicArray b(a);
    b.print("b 拷贝");

    // 拷贝赋值（copy-and-swap）
    std::cout << "  --- 拷贝赋值 c = a ---\n";
    DynamicArray c(2);
    c.set(0, 99);
    c.set(1, 88);
    c.print("c 赋值前");
    c = a;  // 按值传入 -> 拷贝构造 other -> swap -> other 析构
    c.print("c 赋值后");

    // 移动构造
    std::cout << "  --- 移动构造 d = move(a) ---\n";
    DynamicArray d(std::move(a));
    d.print("d 移动后");
    std::cout << "    a.size() = " << a.size() << "（被移走，变为空）\n";

    // 移动赋值（通过 copy-and-swap：传入右值触发移动构造）
    std::cout << "  --- 移动赋值 b = DynamicArray(2) ---\n";
    DynamicArray temp(2);
    temp.set(0, 77);
    temp.set(1, 66);
    b = std::move(temp);  // 按值传入 -> 移动构造 other -> swap
    b.print("b 移动赋值后");

    // noexcept 验证
    std::cout << "  --- noexcept 验证 ---\n";
    std::cout << "    移动构造 noexcept = "
              << (std::is_nothrow_move_constructible_v<DynamicArray> ? "true" : "false")
              << "\n";

    std::cout << "  --- 函数结束，所有对象析构 ---\n";
}

// ═══════════════════════════════════════════════════════════════════

int main()
{
    std::cout << "=== 06_special_members: 特殊成员函数 ===";
    demo01_six_members();
    demo02_generation_rules();
    demo03_default();
    demo04_delete();
    demo05_rule_of_zero();
    demo06_rule_of_five();
    std::cout << "\n完成。\n";
    return 0;
}
