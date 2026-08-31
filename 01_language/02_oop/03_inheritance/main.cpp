// C++20
// 演示：继承深入 — 多继承、菱形继承、virtual 继承、final、隐藏 vs 覆盖

#include <iostream>
#include <string>

// ============================================================================
// ① 多继承（multiple inheritance）
//
//   一个类可以同时继承多个父类，获得所有父类的成员。
//   风险：如果多个父类有同名函数，调用时编译器无法决定用哪个，
//        必须用 类名::函数名 手动消歧义。
// ============================================================================

class LandAnimal
{
public:
    void move()
    {
        std::cout << "  在陆地上奔跑\n";
    }

    void breathe()
    {
        std::cout << "  用肺呼吸（LandAnimal）\n";
    }
};

class WaterAnimal
{
public:
    void swim()
    {
        std::cout << "  在水中游泳\n";
    }

    // 与 LandAnimal 有同名函数 breathe()
    void breathe()
    {
        std::cout << "  用鳃呼吸（WaterAnimal）\n";
    }
};

// 同时继承两个父类
class Amphibian : public LandAnimal, public WaterAnimal
{
public:
    void show()
    {
        // move() 只在 LandAnimal 中，不歧义
        move();

        // swim() 只在 WaterAnimal 中，不歧义
        swim();

        // breathe() 两个父类都有 → 编译器报错 "ambiguous"
        // breathe();  // 错误：不知道调哪个

        // 消歧义：用 类名:: 指定
        LandAnimal::breathe();
        WaterAnimal::breathe();
    }
};

void demo01_multiple_inheritance()
{
    std::cout << "\n① 多继承（multiple inheritance）\n";
    Amphibian frog;
    frog.show();

    // 外部调用也需要消歧义
    // frog.breathe();          // 错误：ambiguous
    frog.LandAnimal::breathe();  // 指定调 LandAnimal 版本
}

// ============================================================================
// ② 菱形继承问题（diamond problem）
//
//   A 是顶层基类，B 和 C 各自继承 A，D 同时继承 B 和 C。
//   结果：D 对象内部有两份 A 的数据副本（一份来自 B，一份来自 C）。
//   访问 A 的成员时，编译器不知道该用哪一份 → 报歧义错误。
// ============================================================================

namespace diamond {

class Animal
{
public:
    std::string name = "动物";

    void identify()
    {
        std::cout << "  Animal::identify → " << name << "\n";
    }
};

// 普通继承（非 virtual）：B 和 C 各自包含一份完整的 Animal
class Bird : public Animal
{
public:
    Bird()
    {
        name = "鸟类";
    }
};

class Horse : public Animal
{
public:
    Horse()
    {
        name = "马类";
    }
};

// Pegasus 同时继承 Bird 和 Horse → 内部有两份 Animal
class Pegasus : public Bird, public Horse
{
};

}  // namespace diamond

void demo02_diamond_problem()
{
    std::cout << "\n② 菱形继承问题（diamond problem）\n";
    diamond::Pegasus p;

    // p.name;        // 错误：ambiguous，两份 Animal 都有 name
    // p.identify();  // 错误：ambiguous

    // 必须指定走哪条路径
    std::cout << "  Bird 路径的 name = " << p.Bird::name << "\n";
    std::cout << "  Horse 路径的 name = " << p.Horse::name << "\n";

    // 两份是独立的，修改一份不影响另一份
    p.Bird::name = "飞马（鸟面）";
    p.Horse::name = "飞马（马面）";
    std::cout << "  修改后 Bird::name = " << p.Bird::name << "\n";
    std::cout << "  修改后 Horse::name = " << p.Horse::name << "\n";
    std::cout << "  （两份 Animal 互相独立，这通常不是我们想要的）\n";
}

// ============================================================================
// ③ virtual 继承
//
//   class B : virtual public A → 告诉编译器：无论 A 被继承多少次，
//   在最终的派生类中只保留一份 A 的数据。
//
//   代价：
//   - 每个 virtual 继承路径多一个指针（vbptr，virtual base pointer）
//   - 构造函数链变复杂：最终派生类必须直接调用虚基类 A 的构造函数，
//     中间类（B、C）的构造函数中对 A 的调用会被忽略
// ============================================================================

namespace virt_inherit {

class Animal
{
public:
    std::string name;

    // 虚基类的构造函数由最终派生类负责调用
    explicit Animal(std::string n = "动物") : name(std::move(n))
    {
        std::cout << "  Animal 构造: name=" << name << "\n";
    }

    void identify()
    {
        std::cout << "  Animal::identify → " << name << "\n";
    }
};

// virtual public：声明虚继承
class Bird : virtual public Animal
{
public:
    // Bird 传给 Animal 的构造参数只在 Bird 是最终类时生效
    // 如果 Bird 不是最终类，这里对 Animal("鸟类") 的调用被跳过
    Bird() : Animal("鸟类")
    {
        std::cout << "  Bird 构造\n";
    }
};

class Horse : virtual public Animal
{
public:
    Horse() : Animal("马类")
    {
        std::cout << "  Horse 构造\n";
    }
};

// 最终派生类必须直接调用虚基类 Animal 的构造函数
class Pegasus : public Bird, public Horse
{
public:
    // 这里的 Animal("飞马") 是唯一生效的 Animal 构造调用
    // Bird() 和 Horse() 中对 Animal 的调用被编译器跳过
    Pegasus() : Animal("飞马"), Bird(), Horse()
    {
        std::cout << "  Pegasus 构造\n";
    }
};

}  // namespace virt_inherit

void demo03_virtual_inheritance()
{
    std::cout << "\n③ virtual 继承\n";
    virt_inherit::Pegasus p;

    // 只有一份 Animal，不再歧义
    p.identify();
    std::cout << "  name = " << p.name << "（只有一份，不歧义）\n";

    // 修改一次就影响全部路径
    p.name = "天马";
    p.Bird::identify();   // 同一份 Animal
    p.Horse::identify();  // 同一份 Animal
    std::cout << "  （Bird 和 Horse 路径看到的是同一个 name）\n";

    // sizeof 对比：virtual 继承的对象更大（多了 vbptr 指针）
    std::cout << "  sizeof(Animal)  = " << sizeof(virt_inherit::Animal) << "\n";
    std::cout << "  sizeof(Bird)    = " << sizeof(virt_inherit::Bird) << "\n";
    std::cout << "  sizeof(Pegasus) = " << sizeof(virt_inherit::Pegasus) << "\n";
}

// ============================================================================
// ④ final 关键字
//
//   final 有两种用法：
//   1. final 类：禁止任何类继承它  →  class X final { };
//   2. final 虚函数：禁止子类覆盖  →  void foo() override final;
//
//   使用场景：当确定不需要再派生或覆盖时加 final，
//   编译器可以做去虚拟化优化（devirtualize），把虚函数调用变成普通调用。
// ============================================================================

class BaseWidget
{
public:
    virtual ~BaseWidget() = default;

    virtual void draw()
    {
        std::cout << "  BaseWidget::draw\n";
    }

    virtual void resize()
    {
        std::cout << "  BaseWidget::resize\n";
    }
};

class Button : public BaseWidget
{
public:
    // override final：Button 实现了 draw，并且禁止子类再覆盖
    void draw() override final
    {
        std::cout << "  Button::draw（final，子类不能再覆盖）\n";
    }

    void resize() override
    {
        std::cout << "  Button::resize\n";
    }
};

// final 类：禁止任何类继承 IconButton
class IconButton final : public Button
{
public:
    // draw() 已被 Button 标记为 final，这里不能覆盖 draw
    // void draw() override { }  // 编译错误

    // resize 没有被 final，可以覆盖
    void resize() override
    {
        std::cout << "  IconButton::resize\n";
    }
};

// 不能继承 final 类
// class FancyIconButton : public IconButton { };  // 编译错误

void demo04_final()
{
    std::cout << "\n④ final 关键字\n";
    IconButton btn;
    btn.draw();    // Button::draw（final 版本）
    btn.resize();  // IconButton::resize

    // 通过基类指针调用，运行时仍然走正确的版本
    BaseWidget* ptr = &btn;
    ptr->draw();    // Button::draw
    ptr->resize();  // IconButton::resize
    std::cout << "  （final 帮助编译器做 devirtualize 优化）\n";
}

// ============================================================================
// ⑤ 隐藏（hiding）vs 覆盖（overriding）
//
//   隐藏（name hiding）：
//     子类定义了与父类同名的非虚函数，父类的版本被"藏起来"。
//     通过子类对象调用时，始终调子类版本，与指针/引用类型无关。
//     不是多态，是编译期的名字查找规则。
//
//   覆盖（overriding）：
//     子类用 override 覆盖父类的虚函数。
//     通过父类指针/引用调用时，运行时决定调哪个版本。
//     这才是真正的多态。
//
//   using 声明：
//     子类中写 using Base::func; 可以把被隐藏的父类函数"拉回"子类作用域。
// ============================================================================

class Printer
{
public:
    // 非虚函数
    void print(int x)
    {
        std::cout << "  Printer::print(int) → " << x << "\n";
    }

    void print(double x)
    {
        std::cout << "  Printer::print(double) → " << x << "\n";
    }

    // 虚函数
    virtual void describe()
    {
        std::cout << "  Printer::describe\n";
    }

    virtual ~Printer() = default;
};

class ColorPrinter : public Printer
{
public:
    // 定义了 print(int) → 隐藏了父类所有 print 重载（包括 print(double)）
    // 注意：不是只隐藏 print(int)，而是隐藏所有名为 print 的函数
    void print(int x)
    {
        std::cout << "  ColorPrinter::print(int) → " << x << " (彩色)\n";
    }

    // override 覆盖虚函数 → 真正的多态
    void describe() override
    {
        std::cout << "  ColorPrinter::describe\n";
    }
};

class AdvancedPrinter : public Printer
{
public:
    // 用 using 把父类的 print 拉回来，不被隐藏
    using Printer::print;

    // 再定义子类自己的 print(int) 版本
    void print(int x)
    {
        std::cout << "  AdvancedPrinter::print(int) → " << x << " (高级)\n";
    }

    // print(double) 因为 using 声明，仍然可用
};

void demo05_hiding_vs_overriding()
{
    std::cout << "\n⑤ 隐藏 vs 覆盖\n";

    ColorPrinter cp;

    // 隐藏的效果：子类对象只能看到子类的 print
    cp.print(42);       // ColorPrinter::print(int)
    // cp.print(3.14);  // 编译错误：ColorPrinter 隐藏了 print(double)

    // 要调父类被隐藏的版本，必须显式指定
    cp.Printer::print(3.14);  // Printer::print(double)
    cp.Printer::print(42);    // Printer::print(int)

    std::cout << "\n  --- 隐藏 vs 覆盖的区别 ---\n";
    Printer* base_ptr = &cp;

    // 非虚函数 print：看指针类型（Printer*），调 Printer::print → 不是多态
    base_ptr->print(42);

    // 虚函数 describe：看实际对象类型（ColorPrinter），调 ColorPrinter::describe → 多态
    base_ptr->describe();

    std::cout << "\n  --- using 声明恢复被隐藏的函数 ---\n";
    AdvancedPrinter ap;
    ap.print(42);    // AdvancedPrinter::print(int)（子类版本覆盖）
    ap.print(3.14);  // Printer::print(double)（using 拉回来的父类版本）
}

// ============================================================================
// ⑥ 继承中的访问控制
//
//   三种继承方式控制父类成员在子类中的访问级别：
//
//   public 继承：父类 public → public，protected → protected
//                最常用，表达"is-a"关系（Dog is a Animal）
//
//   protected 继承：父类 public → protected，protected → protected
//                   外部不能通过子类对象访问父类的 public 成员
//
//   private 继承：父类 public/protected → 全变 private
//                 外部和孙子类都不能访问
//                 表达"用 A 实现 B"的关系（has-a 的替代写法）
//
//   无论哪种继承，父类的 private 成员子类永远不能直接访问。
// ============================================================================

class Engine
{
public:
    void start()
    {
        std::cout << "  Engine::start\n";
    }

    void stop()
    {
        std::cout << "  Engine::stop\n";
    }

protected:
    int rpm_ = 0;
};

// public 继承：外部可以通过 Car 对象调用 Engine 的 public 方法
class Car : public Engine
{
public:
    void drive()
    {
        start();     // 可以访问（public → public）
        rpm_ = 3000; // 可以访问（protected → protected）
        std::cout << "  Car::drive, rpm=" << rpm_ << "\n";
        stop();
    }
};

// private 继承：Engine 的 public/protected 成员在 Robot 中变成 private
// 外部不能通过 Robot 对象调用 start()/stop()
// 表达"Robot 用 Engine 实现，但 Robot 不是 Engine"
class Robot : private Engine
{
public:
    void operate()
    {
        // 类内部可以访问（private 继承来的成员在类内可用）
        start();
        rpm_ = 5000;
        std::cout << "  Robot::operate, rpm=" << rpm_ << "\n";
        stop();
    }

    // 如果想选择性暴露某个父类方法，可以用 using
    // using Engine::start;  // 取消注释可让外部调用 robot.start()
};

// protected 继承：Engine 的 public → protected
class Drone : protected Engine
{
public:
    void fly()
    {
        start();     // 类内可以访问（protected）
        rpm_ = 8000;
        std::cout << "  Drone::fly, rpm=" << rpm_ << "\n";
    }
};

// Drone 的子类仍然可以访问 Engine 的成员（因为是 protected）
class MilitaryDrone : public Drone
{
public:
    void attack()
    {
        start();  // 可以访问：Drone 是 protected 继承，孙子类仍能用
        std::cout << "  MilitaryDrone::attack\n";
        stop();
    }
};

void demo06_access_control()
{
    std::cout << "\n⑥ 继承中的访问控制\n";

    std::cout << "  --- public 继承 ---\n";
    Car car;
    car.drive();
    car.start();  // 外部可以调（public 继承保持 public）

    std::cout << "\n  --- private 继承 ---\n";
    Robot robot;
    robot.operate();
    // robot.start();  // 编译错误：start() 在 Robot 中是 private
    // robot.stop();   // 编译错误：stop() 在 Robot 中是 private
    std::cout << "  （外部不能直接调用 robot.start()，只能通过 operate()）\n";

    std::cout << "\n  --- protected 继承 ---\n";
    Drone drone;
    drone.fly();
    // drone.start();  // 编译错误：start() 在 Drone 中是 protected
    std::cout << "  （外部不能直接调用 drone.start()）\n";

    MilitaryDrone md;
    md.attack();  // 孙子类内部可以调用 protected 成员
    std::cout << "  （protected 继承：子类和孙子类内部能用，外部不能）\n";
}

// ============================================================================
// main
// ============================================================================

int main()
{
    std::cout << "=== 03_inheritance: 继承深入 ===";
    demo01_multiple_inheritance();
    demo02_diamond_problem();
    demo03_virtual_inheritance();
    demo04_final();
    demo05_hiding_vs_overriding();
    demo06_access_control();
    std::cout << "\n完成。\n";
    return 0;
}
