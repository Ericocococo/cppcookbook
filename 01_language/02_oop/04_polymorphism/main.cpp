// C++20
// 演示：多态深入 — vtable、dynamic_cast、抽象类、接口模式、协变返回

#include <iostream>
#include <string>
#include <vector>
#include <memory>

// ============================================================================
// ① 虚函数表（vtable）原理
//
//   有虚函数的类，编译器自动生成一张虚函数表（vtable）：
//   - vtable 是一个函数指针数组，每个虚函数占一个槽位
//   - 每个对象多一个隐藏成员 vptr（虚函数表指针），指向所属类的 vtable
//   - vptr 通常占 8 字节（64 位平台）
//
//   虚函数调用过程：
//   obj->func()  →  通过 obj 的 vptr 找到 vtable
//                →  在 vtable 中查找 func 对应的槽位
//                →  取出函数指针，跳转调用
//   所以虚函数调用比普通函数调用多一次指针间接跳转。
// ============================================================================

// 没有虚函数的类：大小 = 数据成员大小
class PlainClass
{
public:
    int value = 0;

    void doSomething()
    {
        std::cout << "  PlainClass::doSomething\n";
    }
};

// 有虚函数的类：大小 = 数据成员 + vptr（8 字节）
class VirtualClass
{
public:
    int value = 0;

    virtual void doSomething()
    {
        std::cout << "  VirtualClass::doSomething\n";
    }

    virtual ~VirtualClass() = default;
};

// 子类共享 vtable 结构，但指向自己的函数实现
class DerivedVirtual : public VirtualClass
{
public:
    void doSomething() override
    {
        std::cout << "  DerivedVirtual::doSomething\n";
    }
};

void demo01_vtable()
{
    std::cout << "\n① 虚函数表（vtable）原理\n";

    // sizeof 对比：看 vptr 带来的额外开销
    std::cout << "  sizeof(int)          = " << sizeof(int) << "\n";
    std::cout << "  sizeof(PlainClass)   = " << sizeof(PlainClass)
              << "（只有 int，无 vptr）\n";
    std::cout << "  sizeof(VirtualClass) = " << sizeof(VirtualClass)
              << "（int + vptr，有对齐填充）\n";
    std::cout << "  sizeof(DerivedVirtual) = " << sizeof(DerivedVirtual)
              << "（继承 vptr，大小相同）\n";

    // 运行时多态：通过基类指针调用，走 vtable 查找
    VirtualClass* ptr = new DerivedVirtual();
    ptr->doSomething();  // → vptr → DerivedVirtual 的 vtable → DerivedVirtual::doSomething
    delete ptr;

    std::cout << "  （虚函数调用路径：obj→vptr→vtable→函数指针→调用）\n";
}

// ============================================================================
// ② dynamic_cast 安全向下转型
//
//   向下转型：把父类指针/引用转为子类指针/引用。
//   dynamic_cast 在运行时检查类型是否匹配：
//   - 指针版本：失败返回 nullptr
//   - 引用版本：失败抛出 std::bad_cast 异常
//
//   前提条件：父类必须有虚函数（编译器需要 RTTI 信息来做类型检查）。
//   没有虚函数的类不能用 dynamic_cast。
// ============================================================================

class Shape
{
public:
    virtual ~Shape() = default;

    virtual std::string type() const = 0;

    virtual double area() const = 0;
};

class Circle : public Shape
{
    double radius_;

public:
    explicit Circle(double r) : radius_(r)
    {
    }

    std::string type() const override
    {
        return "Circle";
    }

    double area() const override
    {
        return 3.14159 * radius_ * radius_;
    }

    // Circle 特有的方法
    double radius() const
    {
        return radius_;
    }
};

class Rectangle : public Shape
{
    double w_, h_;

public:
    Rectangle(double w, double h) : w_(w), h_(h)
    {
    }

    std::string type() const override
    {
        return "Rectangle";
    }

    double area() const override
    {
        return w_ * h_;
    }

    double width() const
    {
        return w_;
    }

    double height() const
    {
        return h_;
    }
};

void try_as_circle(Shape* s)
{
    // 指针版 dynamic_cast：失败返回 nullptr
    Circle* c = dynamic_cast<Circle*>(s);
    if (c != nullptr) {
        std::cout << "  转型成功：Circle, radius=" << c->radius()
                  << ", area=" << c->area() << "\n";
    } else {
        std::cout << "  转型失败：" << s->type() << " 不是 Circle → 返回 nullptr\n";
    }
}

void try_as_circle_ref(Shape& s)
{
    // 引用版 dynamic_cast：失败抛出 std::bad_cast
    try {
        Circle& c = dynamic_cast<Circle&>(s);
        std::cout << "  引用转型成功：radius=" << c.radius() << "\n";
    } catch (const std::bad_cast& e) {
        std::cout << "  引用转型失败：抛出 bad_cast → " << e.what() << "\n";
    }
}

void demo02_dynamic_cast()
{
    std::cout << "\n② dynamic_cast 安全向下转型\n";

    Circle circle(5.0);
    Rectangle rect(3.0, 4.0);

    // 指针版本
    std::cout << "  --- 指针版 dynamic_cast ---\n";
    try_as_circle(&circle);  // 成功
    try_as_circle(&rect);    // 失败，返回 nullptr

    // 引用版本
    std::cout << "  --- 引用版 dynamic_cast ---\n";
    try_as_circle_ref(circle);  // 成功
    try_as_circle_ref(rect);    // 失败，抛 bad_cast
}

// ============================================================================
// ③ 抽象类与接口模式
//
//   抽象类：至少有一个纯虚函数（= 0），不能直接实例化。
//   子类必须实现所有纯虚函数，否则子类也是抽象类。
//
//   接口模式：全部是纯虚函数的类（类似 Java 的 interface）。
//   只定义"能做什么"，不包含任何实现和数据成员。
//   一个类可以实现多个接口（通过多继承）。
//
//   设计取舍：
//   - 抽象类可以包含部分实现和数据 → 代码复用
//   - 接口只有契约 → 更灵活，不引入耦合
// ============================================================================

// 接口：只有纯虚函数，定义"可序列化"的能力
class ISerializable
{
public:
    virtual ~ISerializable() = default;

    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
};

// 接口：定义"可打印"的能力
class IPrintable
{
public:
    virtual ~IPrintable() = default;

    virtual void print() const = 0;
};

// 抽象类：包含部分实现和数据成员
class Document
{
protected:
    std::string title_;
    std::string content_;

public:
    Document(std::string title, std::string content)
        : title_(std::move(title)), content_(std::move(content))
    {
    }

    virtual ~Document() = default;

    // 纯虚函数：子类必须实现
    virtual std::string format() const = 0;

    // 非虚函数：共用实现
    std::string title() const
    {
        return title_;
    }
};

// 实现多个接口 + 继承抽象类
class JsonDocument : public Document, public ISerializable, public IPrintable
{
public:
    JsonDocument(std::string title, std::string content)
        : Document(std::move(title), std::move(content))
    {
    }

    // 实现 Document 的纯虚函数
    std::string format() const override
    {
        return "{\"title\":\"" + title_ + "\",\"content\":\"" + content_ + "\"}";
    }

    // 实现 ISerializable 接口
    std::string serialize() const override
    {
        return format();
    }

    void deserialize(const std::string& data) override
    {
        content_ = data;
        std::cout << "  JsonDocument::deserialize → 内容更新为: " << data << "\n";
    }

    // 实现 IPrintable 接口
    void print() const override
    {
        std::cout << "  [JsonDocument] " << format() << "\n";
    }
};

class XmlDocument : public Document, public ISerializable, public IPrintable
{
public:
    XmlDocument(std::string title, std::string content)
        : Document(std::move(title), std::move(content))
    {
    }

    std::string format() const override
    {
        return "<doc><title>" + title_ + "</title><content>" + content_ + "</content></doc>";
    }

    std::string serialize() const override
    {
        return format();
    }

    void deserialize(const std::string& data) override
    {
        content_ = data;
        std::cout << "  XmlDocument::deserialize → 内容更新为: " << data << "\n";
    }

    void print() const override
    {
        std::cout << "  [XmlDocument] " << format() << "\n";
    }
};

void demo03_abstract_and_interface()
{
    std::cout << "\n③ 抽象类与接口模式\n";

    // Document doc("test", "test");  // 编译错误：抽象类不能实例化

    JsonDocument jdoc("配置", "hello");
    XmlDocument xdoc("报告", "world");

    // 通过抽象类指针使用
    std::cout << "  --- 通过 Document* 调用 ---\n";
    std::vector<Document*> docs = {&jdoc, &xdoc};
    for (Document* d : docs) {
        std::cout << "  " << d->title() << " → " << d->format() << "\n";
    }

    // 通过接口指针使用
    std::cout << "  --- 通过 ISerializable* 调用 ---\n";
    std::vector<ISerializable*> items = {&jdoc, &xdoc};
    for (ISerializable* s : items) {
        std::cout << "  serialize → " << s->serialize() << "\n";
    }

    // 通过 IPrintable 接口
    std::cout << "  --- 通过 IPrintable* 调用 ---\n";
    std::vector<IPrintable*> printables = {&jdoc, &xdoc};
    for (IPrintable* p : printables) {
        p->print();
    }
}

// ============================================================================
// ④ 协变返回类型（covariant return types）
//
//   子类 override 虚函数时，返回类型可以是父类返回类型的派生类指针/引用。
//   编译器允许这种"放宽"：Base* → Derived*，Base& → Derived&。
//
//   典型应用：clone() 方法（原型模式 Prototype Pattern）。
//   父类 clone() 返回 Base*，子类 clone() 返回 Derived*。
//   好处：通过子类对象调用 clone() 时，直接得到子类指针，不用手动转型。
// ============================================================================

class Prototype
{
public:
    virtual ~Prototype() = default;

    // 返回 Prototype*
    virtual Prototype* clone() const = 0;

    virtual void describe() const = 0;
};

class ConcreteA : public Prototype
{
    int data_;

public:
    explicit ConcreteA(int d) : data_(d)
    {
    }

    // 协变返回：Prototype* → ConcreteA*
    // 编译器允许，因为 ConcreteA* 可以隐式转为 Prototype*
    ConcreteA* clone() const override
    {
        return new ConcreteA(*this);
    }

    void describe() const override
    {
        std::cout << "  ConcreteA(data=" << data_ << ")\n";
    }

    int data() const
    {
        return data_;
    }
};

class ConcreteB : public Prototype
{
    std::string label_;

public:
    explicit ConcreteB(std::string label) : label_(std::move(label))
    {
    }

    // 协变返回：Prototype* → ConcreteB*
    ConcreteB* clone() const override
    {
        return new ConcreteB(*this);
    }

    void describe() const override
    {
        std::cout << "  ConcreteB(label=\"" << label_ << "\")\n";
    }
};

void demo04_covariant_return()
{
    std::cout << "\n④ 协变返回类型（covariant return types）\n";

    ConcreteA original(42);
    original.describe();

    // 通过子类对象调用 clone()：直接得到 ConcreteA*，不用 dynamic_cast
    ConcreteA* copy_a = original.clone();
    std::cout << "  clone 后: ";
    copy_a->describe();
    std::cout << "  直接访问子类方法: data=" << copy_a->data() << "\n";

    // 通过父类指针调用 clone()：返回 Prototype*（协变在编译期被擦除）
    Prototype* base_ptr = &original;
    Prototype* copy_base = base_ptr->clone();
    std::cout << "  通过 Prototype* clone: ";
    copy_base->describe();

    // ConcreteB 同理
    ConcreteB b("hello");
    ConcreteB* copy_b = b.clone();  // 直接得到 ConcreteB*
    std::cout << "  ConcreteB clone: ";
    copy_b->describe();

    delete copy_a;
    delete copy_base;
    delete copy_b;
}

// ============================================================================
// ⑤ 多态的性能考量
//
//   虚函数调用的开销：
//   - 普通函数调用：编译器直接生成 call 指令，地址已知
//   - 虚函数调用：先读 vptr → 再读 vtable → 再跳转，多两次内存访问
//   - 还会阻止内联优化（编译器不知道最终调哪个函数，无法内联）
//
//   优化手段：
//   - final 关键字：编译器知道不会再被覆盖，可以去虚拟化（devirtualize）
//   - CRTP（Curiously Recurring Template Pattern）：编译期多态，零运行时开销
//     模板在编译期展开，没有 vptr / vtable，函数调用可以被内联
// ============================================================================

// --- 虚函数方式（运行时多态） ---
class AnimalVirtual
{
public:
    virtual ~AnimalVirtual() = default;

    virtual void speak() const = 0;
};

class DogVirtual : public AnimalVirtual
{
public:
    void speak() const override
    {
        std::cout << "  [虚函数] Dog: 汪!\n";
    }
};

class CatVirtual : public AnimalVirtual
{
public:
    void speak() const override
    {
        std::cout << "  [虚函数] Cat: 喵!\n";
    }
};

// --- CRTP 方式（编译期多态） ---

// 基类模板：Derived 通过模板参数传入自身类型
template <typename Derived>
class AnimalCRTP
{
public:
    void speak() const
    {
        // static_cast 在编译期确定类型，直接调用 Derived 的方法
        // 没有 vptr 查找，可以被内联
        static_cast<const Derived*>(this)->speak_impl();
    }
};

class DogCRTP : public AnimalCRTP<DogCRTP>
{
public:
    void speak_impl() const
    {
        std::cout << "  [CRTP]   Dog: 汪!\n";
    }
};

class CatCRTP : public AnimalCRTP<CatCRTP>
{
public:
    void speak_impl() const
    {
        std::cout << "  [CRTP]   Cat: 喵!\n";
    }
};

// CRTP 的使用方式：模板函数，编译期决定类型
template <typename T>
void make_speak(const AnimalCRTP<T>& animal)
{
    animal.speak();  // 编译期展开，无虚函数开销
}

void demo05_performance()
{
    std::cout << "\n⑤ 多态的性能考量\n";

    // 虚函数方式
    std::cout << "  --- 运行时多态（虚函数） ---\n";
    DogVirtual dog_v;
    CatVirtual cat_v;
    AnimalVirtual* animals[] = {&dog_v, &cat_v};
    for (const AnimalVirtual* a : animals) {
        a->speak();  // 运行时通过 vtable 查找
    }

    // CRTP 方式
    std::cout << "  --- 编译期多态（CRTP） ---\n";
    DogCRTP dog_c;
    CatCRTP cat_c;
    make_speak(dog_c);  // 编译期展开为 DogCRTP::speak_impl
    make_speak(cat_c);  // 编译期展开为 CatCRTP::speak_impl

    // 大小对比：CRTP 没有 vptr
    std::cout << "\n  --- sizeof 对比 ---\n";
    std::cout << "  sizeof(DogVirtual) = " << sizeof(DogVirtual)
              << "（有 vptr）\n";
    std::cout << "  sizeof(DogCRTP)    = " << sizeof(DogCRTP)
              << "（无 vptr，零开销）\n";

    // CRTP 的局限：不能用同一个基类指针指向不同子类
    // AnimalCRTP<DogCRTP> 和 AnimalCRTP<CatCRTP> 是不同的类型
    // 所以不能放进同一个数组（不像虚函数那样灵活）
    std::cout << "\n  CRTP 权衡：\n";
    std::cout << "  优势 → 零开销，可内联，性能最优\n";
    std::cout << "  劣势 → 不能用基类指针统一管理不同子类（类型不同）\n";
    std::cout << "  场景 → 性能敏感的库代码、策略模式、混入（mixin）\n";
}

// ============================================================================
// main
// ============================================================================

int main()
{
    std::cout << "=== 04_polymorphism: 多态深入 ===";
    demo01_vtable();
    demo02_dynamic_cast();
    demo03_abstract_and_interface();
    demo04_covariant_return();
    demo05_performance();
    std::cout << "\n完成。\n";
    return 0;
}
