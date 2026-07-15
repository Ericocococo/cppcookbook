// C++20
// 演示：枚举（enum/enum class）、命名空间、类型转换
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <string>

// ========== ① 枚举 ==========

// 传统枚举（C 风格）：枚举名直接暴露在外部命名空间，容易冲突
enum Season { SPRING, SUMMER, AUTUMN, WINTER };

// 枚举类（C++11，推荐）：枚举名被限定在枚举类内，安全不冲突
enum class Color { RED, GREEN, BLUE };
enum class Direction { UP, DOWN, LEFT, RIGHT };

// 可以指定底层整数类型（默认是 int）
enum class Status : uint8_t { OK = 0, ERR = 1, TIMEOUT = 2 };

void demo01_enum() {
    std::cout << "\n① 枚举\n";

    // 传统枚举：可以直接用枚举名，但容易和其他名字冲突
    Season s = SUMMER;
    std::cout << "  Season s = SUMMER = " << s << "（底层是整数1）\n";

    // 枚举类：必须写全名 Color::RED，不会和其他命名空间冲突
    Color c = Color::RED;
    // std::cout << c;  // 错误！枚举类不能隐式转为 int
    std::cout << "  Color::RED = " << static_cast<int>(c) << "（需要显式转int）\n";

    Direction d = Direction::UP;
    std::cout << "  Direction::UP = " << static_cast<int>(d) << "\n";

    Status st = Status::OK;
    std::cout << "  Status::OK = " << static_cast<int>(st) << "\n";

    // 枚举在 switch 中很常用
    switch (c) {
        case Color::RED:   std::cout << "  颜色是红色\n"; break;
        case Color::GREEN: std::cout << "  颜色是绿色\n"; break;
        case Color::BLUE:  std::cout << "  颜色是蓝色\n"; break;
    }
}

// ========== ② 命名空间 ==========
namespace math {
    const double PI = 3.14159265358979;

    int add(int a, int b) { return a + b; }
    double circleArea(double r) { return PI * r * r; }

    // 嵌套命名空间
    namespace trig {
        double sin_approx(double x) { return x - x*x*x/6; }  // 泰勒近似
    }
}

namespace io {
    void print(int n) {
        std::cout << "  io::print: " << n << "\n";
    }
    int add(int a, int b) { return a + b; }  // 和 math::add 同名，不冲突
}

void demo02_namespace() {
    std::cout << "\n② 命名空间\n";

    // 使用命名空间中的内容：命名空间::名字
    std::cout << "  math::PI = " << math::PI << "\n";
    std::cout << "  math::add(3, 4) = " << math::add(3, 4) << "\n";
    std::cout << "  math::circleArea(5) = " << math::circleArea(5) << "\n";

    // 嵌套命名空间
    std::cout << "  math::trig::sin_approx(0.1) ≈ " << math::trig::sin_approx(0.1) << "\n";

    io::print(42);
    std::cout << "  io::add(3,4)=" << io::add(3, 4)
              << "  math::add(3,4)=" << math::add(3, 4) << "（同名，不冲突）\n";

    // using：引入某个名字，避免每次都写全名
    using math::PI;
    std::cout << "  using math::PI 后，直接用 PI = " << PI << "\n";

    // using namespace：引入整个命名空间（头文件中不推荐，会污染全局）
    {
        using namespace math;
        std::cout << "  using namespace math 后，add(1,2)=" << add(1, 2) << "\n";
    }   // 离开块后失效
}

// ========== ③ 类型转换 ==========
void demo03_type_cast() {
    std::cout << "\n③ 类型转换\n";

    // --- C++ 风格转换（推荐）---

    // static_cast：编译期类型转换，最常用
    int i = 42;
    double d = static_cast<double>(i);   // int -> double
    int j = static_cast<int>(3.99);      // double -> int（截断，不四舍五入）
    std::cout << "  static_cast<double>(42)   = " << d << "\n";
    std::cout << "  static_cast<int>(3.99)    = " << j << "（截断小数）\n";

    // 枚举类转为整数
    Color c = Color::GREEN;
    int ci = static_cast<int>(c);
    std::cout << "  static_cast<int>(Color::GREEN) = " << ci << "\n";

    // const_cast：去掉 const（很少用，只在必要时用）
    const int FIXED = 100;
    const int* cp = &FIXED;
    int* p = const_cast<int*>(cp);   // 去掉 const 限定
    // 注意：去掉 const 后修改原本是 const 的变量是未定义行为！
    std::cout << "  const_cast 去掉const，*p=" << *p << "（只读，不要写入）\n";

    // reinterpret_cast：重新解释内存中的二进制（危险，慎用）
    int n = 65;
    char* ch = reinterpret_cast<char*>(&n);
    std::cout << "  reinterpret_cast<char*>(&65) -> '"
              << *ch << "'（按char解读int的首字节=ASCII 65='A'）\n";

    // --- C 风格转换（不推荐，原因：不安全，看不出意图）---
    double val = (double)i;   // C 风格
    std::cout << "  (double)42 = " << val << "（C风格，不推荐）\n";
}

int main() {
    std::cout << "=== 08_misc: 枚举、命名空间、类型转换 ===";
    demo01_enum();
    demo02_namespace();
    demo03_type_cast();
    std::cout << "\n完成。\n";
    return 0;
}
