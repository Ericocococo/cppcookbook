// C++20
// 演示：类深入 — 友元、嵌套类、this 指针、mutable、const 成员函数
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <string>
#include <utility>   // std::move
#include <vector>
#include <algorithm> // std::for_each

// ============================================================
// ① 友元函数（friend function）
// ============================================================

// 知识点 1.1：友元函数
// 不是类的成员函数，但可以访问类的 private / protected 成员。
// 典型用途：重载 operator<< 输出运算符，需要读取私有数据但又不属于类接口。

class Temperature
{
private:
    double celsius_;

public:
    explicit Temperature(double c)
        : celsius_(c)
    {
    }

    // 知识点 1.2：声明友元函数
    // 允许这个普通函数访问 private 成员
    // friend 只是"授权"，不是成员函数声明

    friend std::ostream& operator<<(std::ostream& os, const Temperature& t);

    // 知识点 1.3：非运算符友元
    // 友元函数也可以不是运算符，普通函数也行

    friend double to_fahrenheit(const Temperature& t);
};

// 知识点 1.4：友元函数的实现
// 写在类外面，不需要 Temperature:: 前缀
// 因为它不是成员函数，不通过对象调用（t.operator<< 是错的）

std::ostream& operator<<(std::ostream& os, const Temperature& t)
{
    os << t.celsius_ << " C";  // 直接访问 private 成员 celsius_
    return os;
}

double to_fahrenheit(const Temperature& t)
{
    return t.celsius_ * 9.0 / 5.0 + 32.0;  // 直接访问 private 成员
}

void demo01_friend_function()
{
    std::cout << "\n==============================\n";
    std::cout << "  1  friend function\n";
    std::cout << "==============================\n";

    Temperature t(100.0);

    // operator<< 是友元函数，通过 std::cout << t 调用
    std::cout << "  " << t << "\n";

    // to_fahrenheit 也是友元函数，直接调用
    double f = to_fahrenheit(t);
    std::cout << "  -> " << f << " F\n";

    // 注意：友元函数 不是 成员函数
    // t.to_fahrenheit()  // 编译错误！
    // to_fahrenheit(t)   // 正确：像普通函数一样调用
}

// ============================================================
// ② 友元类（friend class）
// ============================================================

// 知识点 2.1：友元类
// 一个类可以把另一个类声明为友元，友元类的所有成员函数都能访问该类的 private 成员。
// 注意：友元关系不可传递（A 是 B 的友元，B 是 C 的友元，不代表 A 是 C 的友元）
// 注意：友元关系不可继承（A 是 B 的友元，不代表 A 的子类也是 B 的友元）

class Car
{
private:
    std::string model_;
    int horsepower_;

    // 知识点 2.2：声明友元类
    // Engine 的所有成员函数都能访问 Car 的 private 成员

    friend class Engine;

public:
    Car(std::string model, int hp)
        : model_(std::move(model)), horsepower_(hp)
    {
    }

    void print() const
    {
        std::cout << "  Car: " << model_ << ", " << horsepower_ << " hp\n";
    }
};

class Engine
{
public:
    // Engine 是 Car 的友元，可以直接读写 Car 的 private 成员
    void tune(Car& car, int extra_hp) const
    {
        std::cout << "  [Engine] " << car.model_
                  << " +" << extra_hp << " hp\n";
        car.horsepower_ += extra_hp;  // 直接修改 private 成员
    }

    void diagnose(const Car& car) const
    {
        std::cout << "  [Engine] " << car.model_
                  << ": " << car.horsepower_ << " hp\n";
    }
};

void demo02_friend_class()
{
    std::cout << "\n==============================\n";
    std::cout << "  2  friend class\n";
    std::cout << "==============================\n";

    Car car("Tesla Model S", 670);
    Engine engine;

    car.print();
    engine.diagnose(car);   // Engine 可以读取 Car 的 private 成员
    engine.tune(car, 100);  // Engine 可以修改 Car 的 private 成员
    car.print();

    // 反过来不行：Car 不能访问 Engine 的 private 成员
    // 友元是单向的，除非双方互相声明
}

// ============================================================
// ③ 嵌套类（nested class）
// ============================================================

// 知识点 3.1：嵌套类
// 在类内部定义的类，用于封装"只给外部类使用"的实现细节。
// C++11 起，嵌套类可以访问外部类的 private 成员。
// 外部类不能自动访问嵌套类的 private 成员（需要嵌套类主动声明友元或提供公开接口）。

class LinkedList
{
private:
    // 知识点 3.2：嵌套类 Node
    // 外部不需要知道链表内部用了什么结构
    // Node 定义在 private 区域，外部代码无法直接使用 LinkedList::Node

    struct Node
    {
        int data;
        Node* next;

        Node(int val, Node* nxt)
            : data(val), next(nxt)
        {
        }
    };

    Node* head_;
    size_t size_;

public:
    LinkedList()
        : head_(nullptr), size_(0)
    {
    }

    // 析构时释放所有节点
    ~LinkedList()
    {
        Node* current = head_;
        while (current != nullptr)
        {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    // 禁止拷贝（简化示例，避免浅拷贝问题）
    // = delete 声明"这个函数被删除"：任何调用的地方都编译报错——详细讲解见 06_special_members 1.4
    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    // 头插法
    void push_front(int val)
    {
        head_ = new Node(val, head_);
        ++size_;
    }

    void print() const
    {
        std::cout << "  LinkedList[" << size_ << "]: ";
        Node* current = head_;
        while (current != nullptr)
        {
            std::cout << current->data;
            if (current->next != nullptr)
            {
                std::cout << " -> ";
            }
            current = current->next;
        }
        std::cout << "\n";
    }
};

void demo03_nested_class()
{
    std::cout << "\n==============================\n";
    std::cout << "  3  nested class\n";
    std::cout << "==============================\n";

    LinkedList list;
    list.push_front(30);
    list.push_front(20);
    list.push_front(10);
    list.print();  // 10 -> 20 -> 30

    // LinkedList::Node n(42, nullptr);  // 编译错误：Node 是 private 的
    // 外部代码只能通过 LinkedList 的公开接口操作，不需要知道内部用了 Node
}

// ============================================================
// ④ this 指针详解
// ============================================================

// 知识点 4.1：this 指针
// this 是编译器自动传入的隐式参数，指向"调用该成员函数的那个对象"。
// 类型：T*（非 const 成员函数）或 const T*（const 成员函数）。
// 常见用途：链式调用（return *this）、消除成员与参数同名歧义。

class Builder
{
private:
    std::string name_;
    int width_;
    int height_;
    std::string color_;

public:
    Builder()
        : name_("default"), width_(0), height_(0), color_("white")
    {
    }

    // 知识点 4.2：链式调用
    // 每个 setter 返回 *this（当前对象的引用）
    // 返回类型必须是引用 Builder&，不是 Builder（否则会拷贝）

    Builder& setName(const std::string& name)
    {
        name_ = name;   // 这里 name 是参数，name_ 是成员
        return *this;    // *this 就是当前对象本身
    }

    Builder& setSize(int width, int height)
    {
        width_ = width;
        height_ = height;
        return *this;
    }

    Builder& setColor(const std::string& color)
    {
        color_ = color;
        return *this;
    }

    // 知识点 4.3：const 成员函数
    // this 的类型变成 const Builder*
    // 意味着不能修改任何成员变量

    void print() const
    {
        std::cout << "  Builder: " << name_
                  << " " << width_ << "x" << height_
                  << " " << color_ << "\n";
    }

    // 演示 this 指针的地址
    void showThis() const
    {
        std::cout << "  this = " << this << "\n";
    }
};

void demo04_this_pointer()
{
    std::cout << "\n==============================\n";
    std::cout << "  4  this\n";
    std::cout << "==============================\n";

    Builder b;

    // 链式调用：setName 返回 *this，可以继续调 setSize，再继续调 setColor
    b.setName("Window")
     .setSize(800, 600)
     .setColor("blue");

    b.print();

    // 验证 this 就是对象的地址
    std::cout << "  &b   = " << &b << "\n";
    b.showThis();  // 输出相同地址
}

// ============================================================
// ⑤ mutable 关键字
// ============================================================

// 知识点 5.1：mutable 关键字
// const 成员函数中，所有成员变量默认不可修改。
// mutable 关键字标记的成员变量例外：即使在 const 函数中也允许修改。
// 适用场景：缓存结果、访问计数器、互斥锁（std::mutex 在 const 函数中需要 lock）。

class DataStore
{
private:
    std::vector<int> data_;

    // 知识点 5.2：mutable 声明
    // 在 const 成员函数中也允许修改
    // 因为 access_count_ 是"观测行为的副作用"，不影响对象的逻辑状态

    mutable int access_count_;

    // 缓存：避免重复计算
    mutable bool cache_valid_;
    mutable double cached_average_;

public:
    explicit DataStore(std::vector<int> data)
        : data_(std::move(data)),
          access_count_(0),
          cache_valid_(false),
          cached_average_(0.0)
    {
    }

    void addValue(int val)
    {
        data_.push_back(val);
        cache_valid_ = false;  // 数据变了，缓存失效
    }

    // 知识点 5.3：const 函数中修改 mutable
    // 承诺不修改对象的逻辑状态
    // 但 mutable 成员（access_count_, cached_average_）允许修改

    double average() const
    {
        ++access_count_;  // mutable：const 函数中可以修改

        if (cache_valid_)
        {
            std::cout << "  [cache hit]\n";
            return cached_average_;
        }

        if (data_.empty())
        {
            return 0.0;
        }

        // 计算平均值
        double sum = 0.0;
        for (int val : data_)
        {
            sum += val;
        }

        // 更新缓存（mutable 允许在 const 函数中修改）
        cached_average_ = sum / data_.size();
        cache_valid_ = true;
        std::cout << "  [computed]\n";
        return cached_average_;
    }

    int accessCount() const
    {
        return access_count_;
    }
};

void demo05_mutable()
{
    std::cout << "\n==============================\n";
    std::cout << "  5  mutable\n";
    std::cout << "==============================\n";

    DataStore ds({10, 20, 30, 40, 50});

    // 即使通过 const 引用访问，mutable 成员也能被修改
    const DataStore& ref = ds;

    double avg1 = ref.average();  // 第一次：计算并缓存
    std::cout << "  avg = " << avg1 << "\n";

    double avg2 = ref.average();  // 第二次：命中缓存
    std::cout << "  avg = " << avg2 << "\n";

    std::cout << "  access count = " << ref.accessCount() << "\n";

    // 修改数据后缓存失效
    ds.addValue(60);
    double avg3 = ds.average();   // 重新计算
    std::cout << "  avg(+60) = " << avg3 << "\n";
}

// ============================================================
// ⑥ static 成员深入
// ============================================================

// 知识点 6.1：static 成员
// static 成员变量：属于类本身，所有对象共享同一份，在类外初始化。
// static 成员函数：没有 this 指针，不能访问非 static 成员，通过 类名:: 调用。
// 典型用途：对象计数、工厂方法、单例模式。

class Connection
{
private:
    int id_;
    std::string host_;

    // static 成员变量：所有 Connection 对象共享
    static int total_count_;  // 总共创建过多少个
    static int alive_count_;  // 当前还存活的数量

    // C++17 内联静态变量：可以直接在类内初始化，不需要类外定义
    static inline int next_id_ = 0;

public:
    explicit Connection(std::string host)
        : id_(++next_id_), host_(std::move(host))
    {
        ++total_count_;
        ++alive_count_;
        std::cout << "  [Connection #" << id_ << "] " << host_
                  << " (alive=" << alive_count_ << ")\n";
    }

    ~Connection()
    {
        --alive_count_;
        std::cout << "  [~Connection #" << id_ << "] " << host_
                  << " (alive=" << alive_count_ << ")\n";
    }

    // 禁止拷贝（连接资源不应被拷贝）
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    // 知识点 6.2：static 成员函数
    // 没有 this 指针，只能访问 static 成员
    // 通过 Connection::aliveCount() 调用，不需要对象

    static int aliveCount()
    {
        return alive_count_;
    }

    static int totalCount()
    {
        return total_count_;
    }

    // 工厂方法：static 函数创建对象，封装构造逻辑
    static Connection* create(const std::string& host)
    {
        std::cout << "  [Factory] " << host << "\n";
        return new Connection(host);
    }

    void print() const
    {
        std::cout << "  Connection #" << id_ << " -> " << host_ << "\n";
    }
};

// 知识点 6.3：static 变量类外初始化
// static 成员变量必须在类外初始化（除非用 inline，见 next_id_）
// 不写这两行会链接错误：undefined reference to Connection::total_count_

int Connection::total_count_ = 0;
int Connection::alive_count_ = 0;

// --- 单例模式（Meyers' Singleton） ---

class Logger
{
private:
    // 知识点 6.4：private 构造函数
    // 外部无法直接 new 或创建对象

    Logger()
    {
        std::cout << "  [Logger] singleton\n";
    }

    // 禁止拷贝和移动
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

public:
    // 知识点 6.5：Meyers' Singleton
    // static 成员函数返回唯一实例的引用
    // 局部 static 变量，线程安全（C++11 保证）

    static Logger& instance()
    {
        static Logger inst;  // 第一次调用时构造，程序结束时析构
        return inst;
    }

    void log(const std::string& msg) const
    {
        std::cout << "  [LOG] " << msg << "\n";
    }
};

void demo06_static_member()
{
    std::cout << "\n==============================\n";
    std::cout << "  6  static member\n";
    std::cout << "==============================\n";

    // --- 对象计数 ---
    std::cout << "  --- object counting ---\n";
    {
        Connection c1("192.168.1.1");
        Connection c2("192.168.1.2");
        std::cout << "  alive = " << Connection::aliveCount() << "\n";

        {
            Connection c3("10.0.0.1");
            std::cout << "  alive = " << Connection::aliveCount() << "\n";
        }
        // c3 析构
        std::cout << "  after c3 destroyed: alive = " << Connection::aliveCount() << "\n";
    }
    // c1, c2 析构
    std::cout << "  total = " << Connection::totalCount()
              << ", alive = " << Connection::aliveCount() << "\n";

    // --- 工厂方法 ---
    std::cout << "\n  --- factory ---\n";
    Connection* conn = Connection::create("db.example.com");
    conn->print();
    delete conn;

    // --- 单例模式 ---
    std::cout << "\n  --- singleton ---\n";
    Logger& log1 = Logger::instance();  // 第一次：构造
    Logger& log2 = Logger::instance();  // 第二次：返回同一个实例

    log1.log("first");
    log2.log("second");

    // 验证是同一个对象
    std::cout << "  &log1 = " << &log1 << "\n";
    std::cout << "  &log2 = " << &log2 << "\n";
    std::cout << "  same? " << (&log1 == &log2 ? "yes" : "no") << "\n";
}

// ============================================================

int main()
{
    std::cout << "=== 01_classes_objects ===";
    demo01_friend_function();
    demo02_friend_class();
    demo03_nested_class();
    demo04_this_pointer();
    demo05_mutable();
    demo06_static_member();
    std::cout << "\n done.\n";
    return 0;
}
