// C++20
// 演示：运算符重载全形式 — 算术/比较/流/下标/函数调用/自增/太空船

#include <iostream>
#include <string>
#include <compare>    // std::strong_ordering, operator<=>
#include <vector>
#include <algorithm>  // std::sort, std::transform
#include <sstream>    // std::istringstream

// ═══════════════════════════════════════════════════════════════════
// ① 算术运算符（+, -, *, /）
// ═══════════════════════════════════════════════════════════════════

// 二维向量类，演示算术运算符的两种写法
class Vec2
{
public:
    double x, y;

    Vec2(double x_val, double y_val)
        : x(x_val), y(y_val)
    {
    }

    // ---- 成员函数写法：operator+= 修改自身，返回 *this ----
    // 返回引用：允许链式调用 a += b += c
    // 修改自身而非创建新对象，语义和内置 += 一致
    Vec2& operator+=(const Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    // ---- 标量乘法：成员函数写法 ----
    // vec * scalar 可以用成员函数
    // 但 scalar * vec 左操作数不是 Vec2，必须用非成员函数（见下方友元）
    Vec2& operator*=(double scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // ---- 友元声明：让非成员函数访问私有成员 ----
    // 这里 x, y 是 public 的，友元非必需，但演示标准写法

    // operator+ 用非成员函数的原因：
    // 如果写成成员函数 Vec2 operator+(const Vec2&)，
    // 那么 3.0 + vec 无法编译（左操作数不是 Vec2）
    // 非成员函数两个参数地位对等，支持任意一侧做隐式转换
    friend Vec2 operator+(Vec2 lhs, const Vec2& rhs);
    friend Vec2 operator-(Vec2 lhs, const Vec2& rhs);

    // scalar * vec：左操作数是 double，只能用非成员函数
    friend Vec2 operator*(double scalar, Vec2 v);
    friend Vec2 operator*(Vec2 v, double scalar);

    // 流运算符也需要友元（③ 中详细讲）
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v);
    friend std::istream& operator>>(std::istream& is, Vec2& v);
};

// ---- 非成员函数实现 ----
// 参数 lhs 按值传入（拷贝），然后复用 += 实现
// 这是经典惯用法：operator+ 基于 operator+= 实现，避免代码重复
Vec2 operator+(Vec2 lhs, const Vec2& rhs)
{
    lhs += rhs;
    return lhs;
}

Vec2 operator-(Vec2 lhs, const Vec2& rhs)
{
    lhs -= rhs;
    return lhs;
}

// 标量 * 向量
Vec2 operator*(double scalar, Vec2 v)
{
    v *= scalar;
    return v;
}

// 向量 * 标量：委托给上面的版本
Vec2 operator*(Vec2 v, double scalar)
{
    return scalar * v;
}

void demo01_arithmetic()
{
    std::cout << "\n① 算术运算符（+, -, *, /）\n";

    Vec2 a(1.0, 2.0);
    Vec2 b(3.0, 4.0);

    Vec2 c = a + b;
    std::cout << "  a + b = " << c << "\n";

    Vec2 d = b - a;
    std::cout << "  b - a = " << d << "\n";

    // += 修改自身
    a += b;
    std::cout << "  a += b 后 a = " << a << "\n";

    // 标量乘法：两种写法都可以
    Vec2 e = 2.0 * b;
    Vec2 f = b * 3.0;
    std::cout << "  2.0 * b = " << e << "\n";
    std::cout << "  b * 3.0 = " << f << "\n";
}

// ═══════════════════════════════════════════════════════════════════
// ② 比较运算符（==, <=>）
// ═══════════════════════════════════════════════════════════════════

// 版本号类，演示 C++20 太空船运算符
// major.minor.patch 三级版本号
class Version
{
public:
    int major;
    int minor;
    int patch;

    Version(int maj, int min, int pat)
        : major(maj), minor(min), patch(pat)
    {
    }

    // ---- C++20 太空船运算符 <=> ----
    // = default 让编译器按成员声明顺序逐个比较
    // 一行代码自动生成 ==, !=, <, >, <=, >= 全部六个运算符
    //
    // 返回类型 std::strong_ordering：
    //   strong_ordering::less      表示 a < b
    //   strong_ordering::equal     表示 a == b
    //   strong_ordering::greater   表示 a > b
    //
    // 如果不用 = default，可以手写自定义比较逻辑：
    //   auto operator<=>(const Version& rhs) const {
    //       if (auto cmp = major <=> rhs.major; cmp != 0) return cmp;
    //       if (auto cmp = minor <=> rhs.minor; cmp != 0) return cmp;
    //       return patch <=> rhs.patch;
    //   }
    auto operator<=>(const Version&) const = default;

    friend std::ostream& operator<<(std::ostream& os, const Version& v)
    {
        os << v.major << "." << v.minor << "." << v.patch;
        return os;
    }
};

void demo02_comparison()
{
    std::cout << "\n② 比较运算符（==, <=>）\n";

    Version v1(1, 0, 0);
    Version v2(1, 2, 3);
    Version v3(1, 2, 3);
    Version v4(2, 0, 0);

    std::cout << "  v1 = " << v1 << "\n";
    std::cout << "  v2 = " << v2 << "\n";
    std::cout << "  v3 = " << v3 << "\n";
    std::cout << "  v4 = " << v4 << "\n";

    // == 和 != 自动生成
    std::cout << "  v2 == v3 ? " << (v2 == v3 ? "true" : "false") << "\n";
    std::cout << "  v1 != v2 ? " << (v1 != v2 ? "true" : "false") << "\n";

    // <, >, <=, >= 自动生成
    std::cout << "  v1 < v2  ? " << (v1 < v2 ? "true" : "false") << "\n";
    std::cout << "  v4 > v2  ? " << (v4 > v2 ? "true" : "false") << "\n";
    std::cout << "  v2 <= v3 ? " << (v2 <= v3 ? "true" : "false") << "\n";

    // 太空船运算符返回 ordering
    auto result = v1 <=> v4;
    if (result < 0)
    {
        std::cout << "  v1 <=> v4 : v1 更小\n";
    }
    else if (result > 0)
    {
        std::cout << "  v1 <=> v4 : v1 更大\n";
    }
    else
    {
        std::cout << "  v1 <=> v4 : 相等\n";
    }

    // 排序：有了 < 运算符，可以直接排序
    std::vector<Version> versions = {v4, v1, v3, v2};
    std::sort(versions.begin(), versions.end());
    std::cout << "  排序后：";
    for (const auto& v : versions)
    {
        std::cout << v << "  ";
    }
    std::cout << "\n";
}

// ═══════════════════════════════════════════════════════════════════
// ③ 流运算符（<<, >>）
// ═══════════════════════════════════════════════════════════════════

// operator<< 和 operator>> 必须用友元函数（非成员函数）
// 原因：左操作数是 std::ostream / std::istream，不是我们的类
// 如果写成成员函数，调用方式变成 v << cout，语义完全反了

// operator<< 的实现（Vec2 的友元，声明在上面的类内）
std::ostream& operator<<(std::ostream& os, const Vec2& v)
{
    // 返回 ostream& 支持链式输出：cout << a << b << c
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

// operator>> 的实现
// 格式：输入两个数字，分别赋给 x 和 y
std::istream& operator>>(std::istream& is, Vec2& v)
{
    // 注意参数 v 不是 const，因为要修改它
    is >> v.x >> v.y;
    return is;
}

void demo03_stream()
{
    std::cout << "\n③ 流运算符（<<, >>）\n";

    Vec2 v(3.14, 2.72);

    // operator<< 支持链式输出
    std::cout << "  v = " << v << "\n";

    // operator>> 从字符串流读取（演示用，避免等待键盘输入）
    std::istringstream input("10.5 20.5");
    Vec2 v2(0, 0);
    input >> v2;
    std::cout << "  从字符串流读取: " << v2 << "\n";
}

// ═══════════════════════════════════════════════════════════════════
// ④ 下标运算符（[]）
// ═══════════════════════════════════════════════════════════════════

// 简单的整数数组封装类
class IntArray
{
    int* data_;
    size_t size_;

    // 越界时的错误值，返回其引用可以避免未定义行为
    static int error_val_;

public:
    explicit IntArray(size_t n)
        : data_(new int[n]{}), size_(n)
    {
    }

    ~IntArray()
    {
        delete[] data_;
    }

    // 禁止拷贝（简化示例，专注 operator[]）
    IntArray(const IntArray&) = delete;
    IntArray& operator=(const IntArray&) = delete;

    size_t size() const
    {
        return size_;
    }

    // ---- 非 const 版本：返回引用，允许读写 ----
    // arr[i] = 42;  调用此版本
    int& operator[](size_t index)
    {
        if (index >= size_)
        {
            std::cout << "  [错误] 下标越界: index=" << index
                      << ", size=" << size_ << "\n";
            return error_val_;
        }
        return data_[index];
    }

    // ---- const 版本：返回 const 引用，只允许读取 ----
    // 当对象本身是 const 时调用此版本
    // 不提供 const 版本的话，const IntArray 对象无法使用 [] 运算符
    const int& operator[](size_t index) const
    {
        if (index >= size_)
        {
            std::cout << "  [错误] 下标越界: index=" << index
                      << ", size=" << size_ << "\n";
            return error_val_;
        }
        return data_[index];
    }
};

int IntArray::error_val_ = 0;

void demo04_subscript()
{
    std::cout << "\n④ 下标运算符（[]）\n";

    IntArray arr(5);

    // 通过 [] 写入
    for (size_t i = 0; i < arr.size(); ++i)
    {
        arr[i] = static_cast<int>(i * 10);
    }

    // 通过 [] 读取
    std::cout << "  arr: ";
    for (size_t i = 0; i < arr.size(); ++i)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // 越界访问：打印错误而非崩溃
    arr[99] = 42;

    // const 对象使用 const 版本的 []
    const IntArray& carr = arr;
    std::cout << "  const arr[2] = " << carr[2] << "\n";
}

// ═══════════════════════════════════════════════════════════════════
// ⑤ 函数调用运算符（()）— 仿函数
// ═══════════════════════════════════════════════════════════════════

// 仿函数（Functor）：重载 operator() 让对象像函数一样调用
// lambda 的本质：编译器会把 lambda 翻译成一个匿名的仿函数类
//   auto mul = [factor](int n) { return n * factor; };
//   等价于编译器生成：
//   struct __lambda {
//       int factor;
//       int operator()(int n) const { return n * factor; }
//   };

// 乘法器：构造时存倍数，调用时返回 n * 倍数
class Multiplier
{
    int factor_;

public:
    explicit Multiplier(int factor)
        : factor_(factor)
    {
    }

    // operator() 可以接受任意参数，没有限制
    int operator()(int n) const
    {
        return n * factor_;
    }
};

// 自定义比较器：按绝对值升序排序
struct AbsLess
{
    bool operator()(int a, int b) const
    {
        // std::abs 返回绝对值
        return std::abs(a) < std::abs(b);
    }
};

void demo05_functor()
{
    std::cout << "\n⑤ 函数调用运算符 ()  — 仿函数\n";

    // 创建一个 "乘以3" 的仿函数
    Multiplier triple(3);
    std::cout << "  triple(7)  = " << triple(7) << "\n";
    std::cout << "  triple(10) = " << triple(10) << "\n";

    // 创建一个 "乘以5" 的仿函数
    Multiplier times5(5);
    std::cout << "  times5(4)  = " << times5(4) << "\n";

    // 在 STL 算法中使用仿函数
    std::vector<int> nums = {-3, 1, -4, 1, 5, -9};
    std::sort(nums.begin(), nums.end(), AbsLess());
    std::cout << "  按绝对值排序: ";
    for (int n : nums)
    {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // 等价的 lambda 写法
    std::sort(nums.begin(), nums.end(), [](int a, int b) {
        return std::abs(a) < std::abs(b);
    });
    std::cout << "  lambda 等价: ";
    for (int n : nums)
    {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // std::transform 配合仿函数
    std::vector<int> src = {1, 2, 3, 4, 5};
    std::vector<int> dst(src.size());
    std::transform(src.begin(), src.end(), dst.begin(), Multiplier(10));
    std::cout << "  transform * 10: ";
    for (int n : dst)
    {
        std::cout << n << " ";
    }
    std::cout << "\n";
}

// ═══════════════════════════════════════════════════════════════════
// ⑥ 自增 / 自减运算符（++, --）
// ═══════════════════════════════════════════════════════════════════

class Counter
{
    int value_;

public:
    explicit Counter(int v = 0)
        : value_(v)
    {
    }

    int value() const
    {
        return value_;
    }

    // ---- 前缀 ++x：先加再返回 ----
    // 返回引用：修改自身后返回，调用方拿到的是加完之后的值
    // 无参数
    Counter& operator++()
    {
        ++value_;
        return *this;
    }

    // ---- 后缀 x++：先返回旧值再加 ----
    // int 是哑参数（dummy parameter），仅用于区分前缀和后缀，不使用
    // 返回值而非引用：因为返回的是修改前的旧值（临时对象）
    // 后缀比前缀多一次拷贝，性能稍差，优先用前缀
    Counter operator++(int)
    {
        Counter old = *this;
        ++value_;
        return old;
    }

    // ---- 前缀 --x ----
    Counter& operator--()
    {
        --value_;
        return *this;
    }

    // ---- 后缀 x-- ----
    Counter operator--(int)
    {
        Counter old = *this;
        --value_;
        return old;
    }

    friend std::ostream& operator<<(std::ostream& os, const Counter& c)
    {
        os << c.value_;
        return os;
    }
};

void demo06_increment()
{
    std::cout << "\n⑥ 自增 / 自减运算符（++, --）\n";

    Counter c(10);
    std::cout << "  初始值: " << c << "\n";

    // 前缀 ++：返回加完之后的值
    Counter& ref = ++c;
    std::cout << "  ++c = " << ref << "  （c 也是 " << c << "）\n";

    // 后缀 ++：返回加之前的旧值
    Counter old = c++;
    std::cout << "  c++ 返回旧值 " << old << "  （c 变为 " << c << "）\n";

    // 前缀 --
    --c;
    std::cout << "  --c 后 c = " << c << "\n";

    // 后缀 --
    Counter old2 = c--;
    std::cout << "  c-- 返回旧值 " << old2 << "  （c 变为 " << c << "）\n";
}

// ═══════════════════════════════════════════════════════════════════
// ⑦ 类型转换运算符
// ═══════════════════════════════════════════════════════════════════

// 可选整数：有值或无值
// 演示 explicit operator bool() 的用法
class OptionalInt
{
    int value_;
    bool has_value_;

public:
    // 无参构造：表示无值
    OptionalInt()
        : value_(0), has_value_(false)
    {
    }

    // 有值构造
    explicit OptionalInt(int v)
        : value_(v), has_value_(true)
    {
    }

    int value() const
    {
        return value_;
    }

    bool has_value() const
    {
        return has_value_;
    }

    // ---- explicit operator bool() ----
    // 让对象在 if / while / 逻辑运算中当布尔值使用
    //
    // 为什么加 explicit：
    //   不加 explicit 时，OptionalInt 可以隐式转成 bool，
    //   然后 bool 又能隐式转成 int，导致 opt + 1 这种无意义的代码能编译通过
    //   加了 explicit 后，只有在条件表达式（if/while/&&/||/!/?:）中才会自动转换
    //   其他地方必须显式 static_cast<bool>(opt)
    explicit operator bool() const
    {
        return has_value_;
    }

    friend std::ostream& operator<<(std::ostream& os, const OptionalInt& opt)
    {
        if (opt.has_value_)
        {
            os << opt.value_;
        }
        else
        {
            os << "(empty)";
        }
        return os;
    }
};

void demo07_conversion()
{
    std::cout << "\n⑦ 类型转换运算符\n";

    OptionalInt a(42);
    OptionalInt b;  // 无值

    std::cout << "  a = " << a << "\n";
    std::cout << "  b = " << b << "\n";

    // 在 if 条件中使用 operator bool()
    if (a)
    {
        std::cout << "  a 有值: " << a.value() << "\n";
    }

    if (!b)
    {
        std::cout << "  b 无值\n";
    }

    // 逻辑运算中也可以使用
    std::cout << "  a && !b = " << (a && !b ? "true" : "false") << "\n";

    // 下面这行会编译错误（explicit 阻止隐式转换）：
    // int x = a;          // 错误：不能隐式转为 bool 再转为 int
    // int y = a + 1;      // 错误：同上

    // 如果确实需要，必须显式转换
    bool flag = static_cast<bool>(a);
    std::cout << "  static_cast<bool>(a) = " << (flag ? "true" : "false") << "\n";
}

// ═══════════════════════════════════════════════════════════════════

int main()
{
    std::cout << "=== 05_operator_overload: 运算符重载 ===";
    demo01_arithmetic();
    demo02_comparison();
    demo03_stream();
    demo04_subscript();
    demo05_functor();
    demo06_increment();
    demo07_conversion();
    std::cout << "\n完成。\n";
    return 0;
}
