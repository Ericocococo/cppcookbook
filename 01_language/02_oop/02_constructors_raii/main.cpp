// C++20
// 演示：构造函数全形式、析构顺序、RAII、copy-and-swap 惯用法
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <string>
#include <utility>   // std::move, std::swap
#include <algorithm> // std::copy

// ============================================================
// ① 构造函数全家族
// ============================================================

// 演示所有构造函数形式，每种都打印日志，方便观察调用顺序。
// 包括：默认构造、参数构造、拷贝构造、移动构造、委托构造、转换构造 + explicit。

class Widget
{
private:
    std::string name_;
    int value_;

public:
    // --- 参数构造函数 ---
    // 接收所有参数来初始化对象
    Widget(std::string name, int value)
        : name_(std::move(name)), value_(value)
    {
        std::cout << "  [param ctor] " << name_ << "=" << value_ << "\n";
    }

    // --- 委托构造函数（C++11） ---
    // 一个构造函数调用另一个构造函数，避免重复初始化代码
    // 委托目标写在初始化列表中，不能再初始化其他成员
    Widget()
        : Widget("default", 0)  // 委托给参数构造函数
    {
        // 委托构造执行完后，才执行这里的函数体
        std::cout << "  [delegating ctor] (default)\n";
    }

    // --- 拷贝构造函数 ---
    // 参数必须是 const 引用（const Widget&），不能按值传递
    // 如果按值传递，调用拷贝构造时又需要拷贝构造，无限递归
    Widget(const Widget& other)
        : name_(other.name_), value_(other.value_)
    {
        std::cout << "  [copy ctor] " << name_ << "\n";
    }

    // --- 移动构造函数（C++11） ---
    // 参数是右值引用（Widget&&），"窃取"源对象的资源而不是复制
    // noexcept 很重要：容器（如 vector）扩容时只有 noexcept 移动构造才会被使用
    Widget(Widget&& other) noexcept
        : name_(std::move(other.name_)), value_(other.value_)
    {
        other.value_ = 0;  // 把源对象置为安全状态
        std::cout << "  [move ctor] " << name_ << "\n";
    }

    ~Widget()
    {
        std::cout << "  [dtor] " << name_ << "\n";
    }

    void print() const
    {
        std::cout << "  Widget{" << name_ << ", " << value_ << "}\n";
    }
};

// --- 转换构造 + explicit ---
// 只有一个参数的构造函数可以做隐式类型转换，explicit 阻止这种转换。

class Meter
{
    double value_;

public:
    // explicit 阻止 double -> Meter 的隐式转换
    // 没有 explicit 时，double 可以悄悄变成 Meter，容易出 bug
    explicit Meter(double val)
        : value_(val)
    {
    }

    double value() const
    {
        return value_;
    }
};

class Inch
{
    double value_;

public:
    // 没有 explicit：允许 double -> Inch 的隐式转换（故意的，用于对比）
    Inch(double val)
        : value_(val)
    {
    }

    double value() const
    {
        return value_;
    }
};

void printMeter(Meter m)
{
    std::cout << "  Meter: " << m.value() << "\n";
}

void printInch(Inch i)
{
    std::cout << "  Inch: " << i.value() << "\n";
}

void demo01_constructors()
{
    std::cout << "\n==============================\n";
    std::cout << "  1  constructors\n";
    std::cout << "==============================\n";

    std::cout << "  --- param ctor ---\n";
    Widget w1("alpha", 10);

    std::cout << "\n  --- delegating ctor ---\n";
    Widget w2;  // 调用默认构造 -> 委托给参数构造

    std::cout << "\n  --- copy ctor ---\n";
    Widget w3 = w1;  // 拷贝构造（等号初始化，不是赋值）
    w3.print();

    std::cout << "\n  --- move ctor ---\n";
    Widget w4 = std::move(w1);  // 移动构造：w1 的资源被转移给 w4
    w4.print();
    w1.print();  // w1 被移动后处于"有效但未指定"的状态

    std::cout << "\n  --- explicit ---\n";
    // printMeter(3.14);       // 编译错误！explicit 阻止了 double->Meter 隐式转换
    printMeter(Meter(3.14));   // 正确：显式构造
    printInch(2.54);           // 正确：Inch 没有 explicit，允许隐式转换

    std::cout << "\n  --- dtors follow (LIFO) ---\n";
}

// ============================================================
// ② 初始化列表 vs 函数体赋值
// ============================================================

// 初始化列表（推荐）：在成员构造时直接赋值，一步完成。
// 函数体赋值：成员先默认构造，再赋值，两步，效率低。
// const 成员和引用成员 必须 用初始化列表，因为它们只能初始化一次。

class InitDemo
{
private:
    const int id_;         // const 成员：必须在初始化列表中初始化
    std::string name_;

    // 注意：初始化顺序由成员声明顺序决定，不是列表中的书写顺序
    // 这里 first_ 声明在 second_ 前面，所以先初始化 first_
    int first_;
    int second_;

public:
    // 推荐：初始化列表方式
    // 成员直接构造成目标值，高效
    InitDemo(int id, std::string name, int a, int b)
        : id_(id),                   // const 成员，必须在这里初始化
          name_(std::move(name)),     // string 直接移动构造，不是先默认构造再赋值
          first_(a),
          second_(b)
    {
        std::cout << "  [InitDemo] id=" << id_
                  << " name=" << name_
                  << " first=" << first_
                  << " second=" << second_ << "\n";
    }

    // 对比：函数体赋值方式（效率低，这里仅演示区别）
    // 注意：这个构造函数和上面的参数不同，不会冲突
    InitDemo(int id, const std::string& name)
        : id_(id),        // const 成员仍然必须在初始化列表
          first_(0),      // 非 const 可以在函数体赋值，但初始化列表更好
          second_(0)
    {
        // name_ 先默认构造成空字符串，再赋值 —— 两步操作
        name_ = name;
        std::cout << "  [InitDemo body-assign] name=" << name_ << "\n";
    }
};

void demo02_init_list()
{
    std::cout << "\n==============================\n";
    std::cout << "  2  init list vs body assign\n";
    std::cout << "==============================\n";

    std::cout << "  --- init list ---\n";
    InitDemo d1(1, "Alice", 10, 20);

    std::cout << "\n  --- body assign ---\n";
    InitDemo d2(2, "Bob");

    // 初始化顺序陷阱：
    // 如果写 : second_(b), first_(second_ + 1)
    // first_ 声明在 second_ 前面，所以 first_ 先初始化
    // 但此时 second_ 还没初始化 —— 未定义行为！
    // 编译器会警告 (-Wreorder)，但不会报错
    std::cout << "\n  [note] member init order = declaration order, not list order\n";
}

// ============================================================
// ③ 析构顺序
// ============================================================

// 析构顺序：
// - 栈对象：后构造的先析构（LIFO，像弹栈）
// - 继承：先析构子类，再析构父类
// - 成员变量：按声明的逆序析构

class Component
{
    std::string name_;

public:
    explicit Component(std::string name)
        : name_(std::move(name))
    {
        std::cout << "  [Component ctor] " << name_ << "\n";
    }

    ~Component()
    {
        std::cout << "  [Component dtor] " << name_ << "\n";
    }
};

class Base
{
public:
    Base()
    {
        std::cout << "  [Base ctor]\n";
    }

    virtual ~Base()
    {
        std::cout << "  [Base dtor]\n";
    }
};

class Derived : public Base
{
    // 成员变量按声明顺序构造：first_ -> second_
    // 析构时逆序：second_ -> first_
    Component first_;
    Component second_;

public:
    Derived()
        : Base(),                     // 先构造父类
          first_("member-1st"),       // 再按声明顺序构造成员
          second_("member-2nd")
    {
        std::cout << "  [Derived ctor]\n";
    }

    ~Derived() override
    {
        std::cout << "  [Derived dtor]\n";
        // 析构顺序：
        // 1. Derived 的析构函数体执行
        // 2. second_ 析构（逆序）
        // 3. first_ 析构（逆序）
        // 4. Base 析构
    }
};

void demo03_destruction_order()
{
    std::cout << "\n==============================\n";
    std::cout << "  3  destruction order\n";
    std::cout << "==============================\n";

    // --- 栈对象 LIFO ---
    std::cout << "  --- stack LIFO ---\n";
    {
        Component a("A");
        Component b("B");
        Component c("C");
        std::cout << "  (scope end -> LIFO)\n";
    }
    // 析构顺序：C -> B -> A

    // --- 继承 + 成员 ---
    std::cout << "\n  --- inheritance + members ---\n";
    {
        Derived d;
        std::cout << "  (scope end)\n";
    }
    // 构造：Base -> first_ -> second_ -> Derived 函数体
    // 析构：Derived 函数体 -> second_ -> first_ -> Base
}

// ============================================================
// ④ RAII（Resource Acquisition Is Initialization）
// ============================================================

// 核心思想：资源（文件、锁、内存、网络连接）在构造时获取，在析构时释放。
// 无论正常退出还是异常退出，析构函数都会被调用（栈展开），保证资源不泄漏。
// 标准库的 RAII 实例：unique_ptr、shared_ptr、lock_guard、fstream。

// --- 模拟 FileGuard ---
// 用构造/析构模拟文件打开/关闭，不真正操作文件

class FileGuard
{
    std::string filename_;
    bool is_open_;

public:
    explicit FileGuard(std::string filename)
        : filename_(std::move(filename)), is_open_(true)
    {
        // 构造时"打开文件"（模拟）
        std::cout << "  [FileGuard] " << filename_ << "\n";
    }

    ~FileGuard()
    {
        // 析构时"关闭文件"（模拟）
        // 无论函数正常返回还是抛异常，析构都会执行
        if (is_open_)
        {
            std::cout << "  [~FileGuard] " << filename_ << "\n";
            is_open_ = false;
        }
    }

    // 禁止拷贝（文件句柄不应被拷贝）
    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;

    void write(const std::string& content) const
    {
        std::cout << "  [FileGuard write] " << filename_ << ": " << content << "\n";
    }
};

// --- 模拟 LockGuard ---
// 用构造/析构模拟加锁/解锁

class FakeMutex
{
    std::string name_;
    bool locked_;

public:
    explicit FakeMutex(std::string name)
        : name_(std::move(name)), locked_(false)
    {
    }

    void lock()
    {
        locked_ = true;
        std::cout << "  [Mutex] " << name_ << " locked\n";
    }

    void unlock()
    {
        locked_ = false;
        std::cout << "  [Mutex] " << name_ << " unlocked\n";
    }

    const std::string& name() const
    {
        return name_;
    }
};

class LockGuard
{
    FakeMutex& mtx_;  // 引用：不拥有 mutex，只管理锁定状态

public:
    // 构造时加锁
    explicit LockGuard(FakeMutex& mtx)
        : mtx_(mtx)
    {
        mtx_.lock();
    }

    // 析构时解锁 —— 保证不管怎么退出作用域，锁都会被释放
    ~LockGuard()
    {
        mtx_.unlock();
    }

    // 禁止拷贝和赋值
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};

void demo04_raii()
{
    std::cout << "\n==============================\n";
    std::cout << "  4  RAII\n";
    std::cout << "==============================\n";

    // --- FileGuard ---
    std::cout << "  --- FileGuard ---\n";
    {
        FileGuard file("data.txt");
        file.write("hello");
        file.write("world");
        // 离开作用域时，FileGuard 析构函数自动"关闭文件"
        // 即使中间抛异常，析构也会执行（栈展开）
    }
    std::cout << "  (file auto-closed)\n";

    // --- LockGuard ---
    std::cout << "\n  --- LockGuard ---\n";
    FakeMutex mtx("resource_mutex");
    {
        LockGuard guard(mtx);  // 构造时自动加锁
        std::cout << "  (critical section)\n";
        // 离开作用域时，LockGuard 析构函数自动解锁
    }
    std::cout << "  (lock auto-released)\n";

    // --- 为什么 RAII 比手动管理好 ---
    std::cout << "\n  --- why RAII > manual ---\n";
    std::cout << "  manual: file.open() ... file.close()  // easy to forget close\n";
    std::cout << "  manual: if error, need try/catch to close  // verbose\n";
    std::cout << "  RAII:   FileGuard f(\"x.txt\");  // auto close on scope exit\n";
    std::cout << "  RAII:   exception safe, no leak, no forget\n";
}

// ============================================================
// ⑤ copy-and-swap 惯用法
// ============================================================

// 问题：手写拷贝赋值运算符容易出错
// - 自赋值（a = a）时，如果先 delete 再 new，会访问已释放的内存
// - 异常安全：new 失败时，对象已经被 delete，处于损坏状态
//
// copy-and-swap 三步法：
// 1. 写好拷贝构造函数（深拷贝）
// 2. 写 swap 函数（交换所有成员，不会抛异常）
// 3. 赋值运算符：按值传参（触发拷贝构造），然后 swap
//
// 优点：自赋值安全、异常安全、代码简洁

class IntArray
{
private:
    int* data_;
    size_t size_;

public:
    // 参数构造
    explicit IntArray(size_t size = 0)
        : data_(size > 0 ? new int[size]{} : nullptr),
          size_(size)
    {
        std::cout << "  [IntArray ctor] size=" << size_ << "\n";
    }

    // 析构
    ~IntArray()
    {
        std::cout << "  [IntArray dtor] size=" << size_ << "\n";
        delete[] data_;
    }

    // --- 步骤 1：拷贝构造（深拷贝） ---
    IntArray(const IntArray& other)
        : data_(other.size_ > 0 ? new int[other.size_] : nullptr),
          size_(other.size_)
    {
        if (size_ > 0)
        {
            std::copy(other.data_, other.data_ + size_, data_);
        }
        std::cout << "  [IntArray copy ctor] size=" << size_ << "\n";
    }

    // --- 移动构造（窃取资源） ---
    IntArray(IntArray&& other) noexcept
        : data_(other.data_), size_(other.size_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "  [IntArray move ctor]\n";
    }

    // --- 步骤 2：swap 函数 ---
    // 交换两个对象的所有成员，noexcept 保证不抛异常
    // 使用 friend 让它可以被 std::swap 找到（ADL）
    friend void swap(IntArray& a, IntArray& b) noexcept
    {
        using std::swap;
        swap(a.data_, b.data_);
        swap(a.size_, b.size_);
    }

    // --- 步骤 3：赋值运算符（copy-and-swap） ---
    // 参数按值传递：
    //   - 如果传入左值，触发拷贝构造 -> other 是副本
    //   - 如果传入右值，触发移动构造 -> other 接管资源
    // 然后 swap：把副本的资源交给自己，把自己的旧资源交给副本
    // 副本在函数结束时析构，自动释放旧资源
    IntArray& operator=(IntArray other)  // 注意：按值传参，不是引用
    {
        std::cout << "  [IntArray operator=] swap\n";
        swap(*this, other);  // 交换后 other 持有旧数据，函数结束时析构旧数据
        return *this;
    }

    // 下标访问
    int& operator[](size_t index)
    {
        return data_[index];
    }

    const int& operator[](size_t index) const
    {
        return data_[index];
    }

    size_t size() const
    {
        return size_;
    }

    void print(const std::string& label) const
    {
        std::cout << "  " << label << "[" << size_ << "]: ";
        for (size_t i = 0; i < size_; ++i)
        {
            std::cout << data_[i];
            if (i + 1 < size_)
            {
                std::cout << ", ";
            }
        }
        std::cout << "\n";
    }
};

void demo05_copy_and_swap()
{
    std::cout << "\n==============================\n";
    std::cout << "  5  copy-and-swap\n";
    std::cout << "==============================\n";

    // 创建并填充
    IntArray a(4);
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;
    a[3] = 40;
    a.print("a");

    // --- 拷贝赋值 ---
    std::cout << "\n  --- copy assign (b = a) ---\n";
    IntArray b(2);
    b[0] = 99;
    b[1] = 88;
    b.print("b(before)");

    b = a;  // 拷贝赋值：先拷贝构造 other，再 swap
    b.print("b(after) ");
    a.print("a(intact)");  // a 没有被修改

    // --- 自赋值安全 ---
    std::cout << "\n  --- self assign (a = a) ---\n";
    a = a;  // copy-and-swap 天然安全：拷贝了一份副本再 swap
    a.print("a(self)  ");

    // --- 移动赋值 ---
    std::cout << "\n  --- move assign (c = move(a)) ---\n";
    IntArray c(1);
    c = std::move(a);  // 移动构造 other（不拷贝），再 swap
    c.print("c(moved) ");
    std::cout << "  a.size=" << a.size() << " (moved-from)\n";

    std::cout << "\n  --- dtors ---\n";
}

// ============================================================

int main()
{
    std::cout << "=== 02_constructors_raii ===";
    demo01_constructors();
    demo02_init_list();
    demo03_destruction_order();
    demo04_raii();
    demo05_copy_and_swap();
    std::cout << "\n done.\n";
    return 0;
}
