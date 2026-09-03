// C++20
// 演示：struct、class封装、构造/析构/初始化列表、继承、多态、Rule of 0/3/5
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>  // std::copy
#include <utility>    // std::move

// 知识点 1.1：struct：数据聚合（默认 public）

struct Point
{
    double x, y;

    double distance() const
    {
        return std::sqrt(x * x + y * y);
    }

    void print() const
    {
        std::cout << "  Point(" << x << ", " << y
            << ")  距原点=" << distance() << "\n";
    }
};

void demo01_struct()
{
    std::cout << "\n① struct\n";
    Point p1{3.0, 4.0}; // 聚合初始化
    Point p2 = {1.0, 2.0};
    p1.print();
    p2.print();
    p1.x = 0;
    p1.y = 5;
    p1.print();
}

// 知识点 2.1：class：封装（默认 private）

class Rectangle
{
private:
    double width_, height_;
    static int count_; // 静态成员：属于类，所有对象共享

public:
    // 知识点 2.2：构造函数 + 初始化列表（推荐：直接初始化，不是赋值）

    Rectangle(double w, double h)
        : width_(w), height_(h)
    {
        // : 后面是初始化列表
        ++count_;
        std::cout << "  [Rectangle 构造] " << w << "x" << h
            << "  共创建=" << count_ << "\n";
    }

    // 知识点 2.3：默认构造

    Rectangle() : Rectangle(0, 0)
    {
    } // 委托构造（C++11）

    // 知识点 2.4：析构函数：对象销毁时自动调用

    ~Rectangle()
    {
        --count_;
        std::cout << "  [Rectangle 析构] " << width_ << "x" << height_
            << "  剩余=" << count_ << "\n";
    }

    double area() const { return width_ * height_; }
    double perimeter() const { return 2 * (width_ + height_); }
    double width() const { return width_; }
    double height() const { return height_; }

    void setWidth(double w)
    {
        if (w < 0)
        {
            std::cout << "  宽度不能为负，忽略\n";
            return;
        }
        width_ = w;
    }

    void print() const
    {
        std::cout << "  Rectangle " << width_ << "x" << height_
            << "  面积=" << area() << "  周长=" << perimeter() << "\n";
    }

    static int count() { return count_; } // 静态成员函数
};

int Rectangle::count_ = 0; // 静态成员在类外初始化

void demo02_class()
{
    std::cout << "\n② class 封装\n";
    Rectangle r1(3.0, 4.0);
    r1.print();
    r1.setWidth(-1); // 负数被拒绝
    r1.setWidth(10);
    r1.print();
    {
        Rectangle r2(5.0, 6.0);
        std::cout << "  当前 Rectangle 数量=" << Rectangle::count() << "\n";
    } // r2 析构
    std::cout << "  r2 析构后数量=" << Rectangle::count() << "\n";
}

// 知识点 3.1：继承

class Shape
{
protected:
    std::string color_;

public:
    explicit Shape(std::string color) : color_(std::move(color))
    {
    }

    virtual ~Shape() = default; // 虚析构（有子类时必须！）

    virtual double area() const = 0; // 纯虚函数：强制子类实现
    virtual std::string type() const = 0;

    // 知识点 3.2：非虚函数：共用实现，子类不覆盖

    void describe() const
    {
        std::cout << "  " << type() << "（" << color_ << "）"
            << "  面积=" << area() << "\n";
    }
};

class Circle : public Shape
{
    double radius_;

public:
    Circle(double r, std::string color)
        : Shape(std::move(color)), radius_(r)
    {
    }

    double area() const override { return 3.14159 * radius_ * radius_; }
    std::string type() const override { return "圆形"; }
};

class Square : public Shape
{
    double side_;

public:
    Square(double s, std::string color)
        : Shape(std::move(color)), side_(s)
    {
    }

    double area() const override { return side_ * side_; }
    std::string type() const override { return "正方形"; }
};

void demo03_inheritance()
{
    std::cout << "\n③ 继承\n";
    Circle c(5.0, "红色");
    Square s(4.0, "蓝色");
    c.describe();
    s.describe();

    // 知识点 3.3：子类可以访问 protected 成员（父类设置的 color_）
    // c.color_ = "绿色";  // 错误：protected 只有类内和子类能访问
}

// 知识点 4.1：多态：父类指针/引用调用子类的虚函数

void demo04_polymorphism()
{
    std::cout << "\n④ 多态（运行时决定调用哪个版本）\n";

    // 知识点 4.2：父类指针数组，存放不同子类对象

    Shape* shapes[] = {
        new Circle(3.0, "红"),
        new Square(4.0, "蓝"),
        new Circle(1.5, "绿"),
    };

    for (auto* s : shapes)
    {
        s->describe(); // 运行时决定：调 Circle::area 还是 Square::area
    }

    // 知识点 4.3：必须通过虚析构正确释放

    for (auto* s : shapes) delete s;
    std::cout << "  （虚析构确保子类析构函数被正确调用）\n";
}

// 知识点 5.1：Rule of 0/3/5
// Rule of 0：如果不需要自定义析构/拷贝/移动，就都不定义，用编译器默认的
// Rule of 3：如果自定义了析构，则必须同时定义拷贝构造和拷贝赋值
// Rule of 5：如果定义了移动，则需要定义全部5个（析构/拷贝构造/拷贝赋值/移动构造/移动赋值）

class Buffer
{
    int* data_;
    size_t size_;

public:
    explicit Buffer(size_t n) : data_(new int[n]{}), size_(n)
    {
        std::cout << "  [Buffer 构造] size=" << n << "\n";
    }

    ~Buffer()
    {
        delete[] data_;
        std::cout << "  [Buffer 析构] size=" << size_ << "\n";
    }

    // 知识点 5.2：拷贝构造（深拷贝）

    Buffer(const Buffer& other) : data_(new int[other.size_]{}), size_(other.size_)
    {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "  [Buffer 拷贝构造]\n";
    }

    // 知识点 5.3：移动构造（转移所有权，不复制）

    Buffer(Buffer&& other) noexcept
        : data_(other.data_), size_(other.size_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "  [Buffer 移动构造]\n";
    }

    size_t size() const { return size_; }
};

void demo05_rule_of_five()
{
    std::cout << "\n⑤ Rule of 0/3/5\n";
    Buffer b1(5);
    Buffer b2 = b1; // 拷贝构造（深拷贝）
    Buffer b3 = std::move(b1); // 移动构造（转移所有权）
    std::cout << "  b1.size=" << b1.size()
        << "（已被移动，资源转移给 b3）\n";
    std::cout << "  b2.size=" << b2.size() << "  b3.size=" << b3.size() << "\n";
}

int main()
{
    std::cout << "=== 08_oop: 面向对象 ===";
    demo01_struct();
    demo02_class();
    demo03_inheritance();
    demo04_polymorphism();
    demo05_rule_of_five();
    std::cout << "\n完成。\n";
    return 0;
}
