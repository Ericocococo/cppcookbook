// C++20
// 演示：struct、class（封装/构造/析构）、继承、多态（虚函数）
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <string>
#include <cmath>   // sqrt

// ========== ① struct：简单数据聚合 ==========
struct Point {
    double x;
    double y;

    // struct 里也可以有成员函数
    // const 表示这个函数不会修改成员变量
    double distToOrigin() const {
        return std::sqrt(x*x + y*y);
    }

    void print() const {
        std::cout << "  Point(" << x << ", " << y << ")"
                  << "  到原点距离=" << distToOrigin() << "\n";
    }
};

void demo01_struct() {
    std::cout << "\n① struct\n";

    // struct 默认成员是 public（外部可访问）
    Point p1 = {3.0, 4.0};      // 聚合初始化
    Point p2{1.0, 2.0};         // 列表初始化（C++11）
    p1.print();
    p2.print();

    // 修改成员
    p1.x = 0;
    p1.y = 5;
    p1.print();
}

// ========== ② class：封装（数据+操作，控制访问权限）==========
class Rectangle {
private:
    // private：只有类内部能访问
    double width;
    double height;

public:
    // public：外部可以访问

    // 构造函数：创建对象时自动调用，用于初始化
    // 初始化列表（: width(w)）比在函数体内赋值更高效
    Rectangle(double w, double h) : width(w), height(h) {
        std::cout << "  [Rectangle 构造] " << w << "x" << h << "\n";
    }

    // 默认构造函数（无参数）
    Rectangle() : width(0), height(0) {
        std::cout << "  [Rectangle 默认构造]\n";
    }

    // 析构函数：对象销毁时自动调用，用于清理资源
    ~Rectangle() {
        std::cout << "  [Rectangle 析构] " << width << "x" << height << "\n";
    }

    // Getter（读取私有成员）
    double getWidth()  const { return width; }
    double getHeight() const { return height; }

    // Setter（修改私有成员，可以加验证）
    void setWidth(double w) {
        if (w < 0) w = 0;   // 防止负值
        width = w;
    }

    double area()      const { return width * height; }
    double perimeter() const { return 2 * (width + height); }

    void print() const {
        std::cout << "  Rectangle " << width << "x" << height
                  << "  面积=" << area() << "  周长=" << perimeter() << "\n";
    }

    // 静态成员：属于类本身，不属于某个对象
    static int count;   // 记录创建了多少个 Rectangle
};

int Rectangle::count = 0;   // 静态成员在类外初始化

void demo02_class() {
    std::cout << "\n② class 封装\n";

    Rectangle r1(3.0, 4.0);
    r1.print();

    // 不能访问 private 成员
    // r1.width = 10;  // 编译错误！

    // 通过 public 接口访问
    std::cout << "  getWidth()=" << r1.getWidth() << "\n";
    r1.setWidth(10);
    r1.print();

    {
        Rectangle r2(5.0, 6.0);
        r2.print();
        // r2 在此处超出作用域，析构函数被调用
    }
    std::cout << "  r2 已析构\n";
}

// ========== ③ 继承：子类继承父类的成员 ==========
// 基类（父类）
class Shape {
protected:
    // protected：类内和子类可访问，外部不可访问
    std::string color;

public:
    explicit Shape(const std::string& c) : color(c) {}
    virtual ~Shape() {}   // 有子类时，析构函数必须是 virtual

    // 纯虚函数：没有实现，强制子类实现
    // 有纯虚函数的类叫"抽象类"，不能直接创建对象
    virtual double area() const = 0;

    // 普通虚函数：有默认实现，子类可以覆盖也可以不覆盖
    virtual void describe() const {
        std::cout << "  我是一个 " << color << " 的形状，面积=" << area() << "\n";
    }

    const std::string& getColor() const { return color; }
};

// 子类：继承 Shape
class Circle : public Shape {
    double radius;
public:
    Circle(double r, const std::string& c) : Shape(c), radius(r) {}

    // override：明确告诉编译器"这是在覆盖父类的虚函数"
    // 如果父类没有这个函数，编译器会报错（安全）
    double area() const override {
        return 3.14159 * radius * radius;
    }

    void describe() const override {
        std::cout << "  我是半径=" << radius << " 的"
                  << color << "圆，面积=" << area() << "\n";
    }
};

class Square : public Shape {
    double side;
public:
    Square(double s, const std::string& c) : Shape(c), side(s) {}

    double area() const override {
        return side * side;
    }
};

void demo03_inheritance() {
    std::cout << "\n③ 继承\n";

    Circle  c(5.0, "红色");
    Square  s(4.0, "蓝色");

    c.describe();
    s.describe();   // 用父类的 describe()，子类没有覆盖

    // 通过父类指针访问子类对象（多态的基础）
    std::cout << "  c.getColor()=" << c.getColor() << "（继承自 Shape）\n";
}

// ========== ④ 多态：父类指针调用子类的函数 ==========
void demo04_polymorphism() {
    std::cout << "\n④ 多态（父类指针，子类行为）\n";

    // 父类指针可以指向子类对象
    Shape* shapes[3];
    shapes[0] = new Circle(3.0, "红色");
    shapes[1] = new Square(4.0, "蓝色");
    shapes[2] = new Circle(1.5, "绿色");

    // 调用时自动选择正确的子类版本（运行时决定）
    for (int i = 0; i < 3; ++i) {
        shapes[i]->describe();   // 自动调用 Circle::describe 或 Square::describe
    }

    // 释放内存（因为用了虚析构，delete 会正确调用子类析构函数）
    for (int i = 0; i < 3; ++i) {
        delete shapes[i];
    }
}

int main() {
    std::cout << "=== 07_oop: 面向对象 ===";
    demo01_struct();
    demo02_class();
    demo03_inheritance();
    demo04_polymorphism();
    std::cout << "\n完成。\n";
    return 0;
}
