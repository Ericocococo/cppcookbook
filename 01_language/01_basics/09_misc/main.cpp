// C++20
// 演示：enum/enum class、namespace（命名空间）、using、四种 C++ 类型转换
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <string>
#include <cstdint>   // uint8_t

// ① 枚举：enum（传统）vs enum class（推荐）
void demo01_enum()
{
    std::cout << "\n① 枚举\n";

    // 知识点 1.1：传统 enum
    // 枚举名暴露到外部命名空间，容易冲突
    enum Direction { UP, DOWN, LEFT, RIGHT };
    Direction d = UP;
    int n = UP; // 可以隐式转为 int（可能意外）
    std::cout << "  enum Direction: UP=" << UP << "  (int)UP=" << n << "\n";
    std::cout << "  传统 enum 问题：枚举名污染外部命名空间，可以隐式转为 int\n";

    // 知识点 1.2：enum class（C++11，推荐）
    // 限定在枚举类内，类型安全
    enum class Color { RED, GREEN, BLUE };
    enum class Status : uint8_t { OK = 0, ERR = 1, TIMEOUT = 2 };

    Color c = Color::RED; // 必须写全名
    // int bad = c;          // 错误：不能隐式转 int
    int ci = static_cast<int>(c); // 需要显式转
    std::cout << "  enum class Color::RED = " << ci << "\n";

    Status s = Status::OK;
    std::cout << "  Status::OK = " << (int)s << "\n";
    std::cout << "  Status 底层类型 uint8_t，大小=" << sizeof(s) << " 字节\n";

    // 知识点 1.3：switch 配合 enum class
    switch (c)
    {
    case Color::RED: std::cout << "  颜色: 红\n";
        break;
    case Color::GREEN: std::cout << "  颜色: 绿\n";
        break;
    case Color::BLUE: std::cout << "  颜色: 蓝\n";
        break;
    }
    (void)d;
}

// ② 命名空间：避免名字冲突
namespace math
{
    const double PI = 3.14159265358979;
    int add(int a, int b) { return a + b; }
    double circleArea(double r) { return PI * r * r; }

    namespace trig
    {
        // 知识点 2.1：嵌套命名空间
        double sinApprox(double x) { return x - x * x * x / 6.0; } // 泰勒近似
    }
}

namespace io
{
    int add(int a, int b) { return a + b + 100; } // 同名但不冲突
    void print(int n) { std::cout << "  io::print: " << n << "\n"; }
}

// 知识点 2.2：C++17 嵌套命名空间简写
namespace project::utils
{
    void hello() { std::cout << "  project::utils::hello()\n"; }
}

void demo02_namespace()
{
    std::cout << "\n② 命名空间\n";

    std::cout << "  math::PI=" << math::PI << "\n";
    std::cout << "  math::add(3,4)=" << math::add(3, 4) << "\n";
    std::cout << "  io::add(3,4)=" << io::add(3, 4) << "（同名不冲突）\n";
    std::cout << "  math::trig::sinApprox(0.1)=" << math::trig::sinApprox(0.1) << "\n";

    // 知识点 2.3：using 声明
    // 引入单个名字
    using math::PI;
    std::cout << "  using math::PI; PI=" << PI << "\n";

    // 知识点 2.4：using namespace
    // 引入所有（不推荐在头文件用，会污染）
    {
        using namespace math;
        std::cout << "  using namespace math; circleArea(2)=" << circleArea(2) << "\n";
    } // 出块后失效

    // 知识点 2.5：C++17 嵌套命名空间
    project::utils::hello();
    io::print(42);
}

// ③ 类型转换
void demo03_type_cast()
{
    std::cout << "\n③ 类型转换\n";

    // 知识点 3.1：static_cast
    // 最常用，编译期类型转换
    int i = 42;
    double d = static_cast<double>(i); // int → double
    int j = static_cast<int>(3.99); // double → int（截断）
    std::cout << "  static_cast<double>(42)=" << d << "\n";
    std::cout << "  static_cast<int>(3.99)=" << j << "（截断，不四舍五入）\n";

    // 知识点 3.2：enum class 转 int
    enum class Dir { UP = 0, DOWN = 1 };
    int di = static_cast<int>(Dir::DOWN);
    std::cout << "  static_cast<int>(Dir::DOWN)=" << di << "\n";

    // 知识点 3.3：dynamic_cast
    // 运行时多态向下转型
    class Base
    {
    public:
        virtual ~Base()
        {
        }

        virtual std::string name() { return "Base"; }
    };
    class Derived : public Base
    {
    public:
        std::string name() override { return "Derived"; }
    };

    Base* bp = new Derived();
    Derived* dp = dynamic_cast<Derived*>(bp); // 向下转型（运行时检查）
    if (dp)
    {
        std::cout << "  dynamic_cast 成功: " << dp->name() << "\n";
    }
    Base* bp2 = new Base();
    Derived* dp2 = dynamic_cast<Derived*>(bp2); // 失败返回 nullptr
    std::cout << "  dynamic_cast 失败: dp2=" << dp2 << "（nullptr）\n";
    delete bp;
    delete bp2;

    // 知识点 3.4：const_cast
    // 去掉/加上 const
    const int ci = 100;
    const int* cp = &ci;
    int* p = const_cast<int*>(cp); // 去掉 const 限定
    std::cout << "  const_cast 去 const: *p=" << *p << "\n";
    // 注意：修改真正 const 变量是未定义行为！const_cast 主要用于和老 API 交互

    // 知识点 3.5：reinterpret_cast
    // 内存重新解释（危险！）
    int ni = 65;
    char* ch = reinterpret_cast<char*>(&ni);
    std::cout << "  reinterpret_cast<char*>(&65): '" << *ch
        << "'（ASCII 65='A'，把 int 的内存解释为 char）\n";

    // 知识点 3.6：C 风格转换（不推荐）
    // 看不出意图，不安全
    double cd = (double)i; // C 风格
    std::cout << "  C风格 (double)42=" << cd << "（不推荐，改用 static_cast）\n";

    std::cout << "\n  四种转换对比:\n";
    std::cout << "  static_cast:      编译期，安全，最常用\n";
    std::cout << "  dynamic_cast:     运行期，多态向下转型，失败返回 nullptr\n";
    std::cout << "  const_cast:       去除/添加 const，慎用\n";
    std::cout << "  reinterpret_cast: 内存重解释，危险，几乎不用\n";
}

int main()
{
    std::cout << "=== 09_misc: 枚举、命名空间、类型转换 ===";
    demo01_enum();
    demo02_namespace();
    demo03_type_cast();
    std::cout << "\n完成。\n";
    return 0;
}
