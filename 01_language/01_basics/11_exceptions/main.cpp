// C++20
// 演示：try/catch/throw、标准异常层次、noexcept、异常安全、RAII
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <stdexcept>   // runtime_error / logic_error / out_of_range 等
#include <string>
#include <vector>
#include <memory>
#include <utility>     // std::move
#include <type_traits> // std::is_nothrow_move_constructible_v

// ① 基本的 try / catch / throw
double safeDivide(double a, double b)
{
    if (b == 0.0)
        throw std::runtime_error("除数不能为零"); // throw：抛出异常，函数立即终止
    return a / b;
}

void demo01_basic()
{
    std::cout << "\n① try / catch / throw\n";

    // 正常情况
    try
    {
        double r = safeDivide(10.0, 3.0);
        std::cout << "  10/3=" << r << "\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "  异常: " << e.what() << "\n";
    }

    // 触发异常
    try
    {
        double r = safeDivide(10.0, 0.0); // throw
        (void)r;
        std::cout << "  这行不会执行\n";
    }
    catch (const std::runtime_error& e)
    {
        // 捕获具体类型（放在 exception 前面！）
        std::cout << "  runtime_error: " << e.what() << "\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "  exception: " << e.what() << "\n";
    }
    catch (...)
    {
        // 捕获任何类型（兜底）
        std::cout << "  未知异常\n";
    }
}

// ② 标准异常层次
void demo02_exception_hierarchy()
{
    std::cout << "\n② 标准异常层次\n";
    std::cout << "  std::exception（基类）\n";
    std::cout << "  ├── std::logic_error（程序逻辑错误）\n";
    std::cout << "  │   ├── invalid_argument（无效参数）\n";
    std::cout << "  │   ├── out_of_range（越界）\n";
    std::cout << "  │   ├── length_error（长度超限）\n";
    std::cout << "  │   └── domain_error（定义域错误）\n";
    std::cout << "  └── std::runtime_error（运行时错误）\n";
    std::cout << "      ├── overflow_error（上溢）\n";
    std::cout << "      ├── underflow_error（下溢）\n";
    std::cout << "      └── range_error（范围错误）\n";

    // 演示各种标准异常
    std::vector<int> v = {1, 2, 3};
    try
    {
        v.at(10); // at() 在越界时抛 out_of_range
    }
    catch (const std::out_of_range& e)
    {
        std::cout << "  out_of_range: " << e.what() << "\n";
    }

    try
    {
        throw std::invalid_argument("参数不合法");
    }
    catch (const std::logic_error& e)
    {
        std::cout << "  logic_error(invalid_arg): " << e.what() << "\n";
    }
}

// ③ 自定义异常类
class DatabaseError : public std::runtime_error
{
    int code_;

public:
    DatabaseError(int code, const std::string& msg)
        : std::runtime_error(msg), code_(code)
    {
    }

    int code() const { return code_; }
};

void demo03_custom_exception()
{
    std::cout << "\n③ 自定义异常\n";
    try
    {
        throw DatabaseError(404, "表不存在: users");
    }
    catch (const DatabaseError& e)
    {
        std::cout << "  DatabaseError[" << e.code() << "]: " << e.what() << "\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "  其他异常: " << e.what() << "\n";
    }
}

// ④ noexcept：承诺函数不抛出异常
int safeAdd(int a, int b) noexcept
{
    return a + b; // 确实不会抛出
}

// noexcept 的条件形式
template <typename T>
void swapSafe(T& a, T& b) noexcept(std::is_nothrow_move_constructible_v<T>)
{
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

void demo04_noexcept()
{
    std::cout << "\n④ noexcept\n";
    int r = safeAdd(3, 4);
    std::cout << "  safeAdd(3,4)=" << r << "（noexcept，编译器可优化）\n";

    int a = 10, b = 20;
    swapSafe(a, b);
    std::cout << "  swapSafe 后: a=" << a << " b=" << b << "\n";

    // noexcept 的作用：
    // 1. 移动构造函数标 noexcept，vector 扩容时才会用移动（否则用拷贝）
    // 2. 让编译器去掉异常处理代码，生成更小更快的代码
    std::cout << "  noexcept 函数如果真的抛出异常：std::terminate() 直接终止程序\n";
}

// ⑤ 异常安全与 RAII
class File
{
    std::string name_;
    bool open_;

public:
    explicit File(const std::string& name) : name_(name), open_(true)
    {
        std::cout << "  [File 打开] " << name_ << "\n";
    }

    ~File()
    {
        if (open_)
        {
            std::cout << "  [File 关闭] " << name_ << "（RAII 保证关闭）\n";
            open_ = false;
        }
    }
};

void demo05_raii_exception()
{
    std::cout << "\n⑤ RAII 与异常安全\n";

    // RAII：即使抛出异常，析构函数也会被调用
    try
    {
        File f("data.txt"); // 构造时打开
        std::cout << "  处理文件中...\n";
        throw std::runtime_error("处理失败"); // 抛出异常
        std::cout << "  这行不会执行\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "  捕获异常: " << e.what() << "\n";
        std::cout << "  （文件已被 RAII 关闭，即使抛出了异常）\n";
    }

    // 不用 RAII 的危险写法
    std::cout << "  危险写法：手动 delete 在异常路径中会被跳过！\n";
    std::cout << "  解决：用 unique_ptr / shared_ptr 代替裸指针\n";
}

// ⑥ 多层 catch 的顺序
void demo06_catch_order()
{
    std::cout << "\n⑥ catch 顺序：具体到通用\n";
    std::cout << "  规则：先捕获具体类型，再捕获基类（否则基类 catch 会先匹配）\n";

    for (int i = 0; i < 3; ++i)
    {
        try
        {
            if (i == 0) throw std::out_of_range("越界");
            if (i == 1) throw std::runtime_error("运行时错误");
            if (i == 2) throw std::string("非标准异常");
        }
        catch (const std::out_of_range& e)
        {
            std::cout << "  [" << i << "] out_of_range: " << e.what() << "\n";
        }
        catch (const std::exception& e)
        {
            std::cout << "  [" << i << "] exception: " << e.what() << "\n";
        }
        catch (...)
        {
            std::cout << "  [" << i << "] 未知类型异常（非 std::exception）\n";
        }
    }
}

int main()
{
    std::cout << "=== 11_exceptions: 异常处理 ===";
    demo01_basic();
    demo02_exception_hierarchy();
    demo03_custom_exception();
    demo04_noexcept();
    demo05_raii_exception();
    demo06_catch_order();
    std::cout << "\n完成。\n";
    return 0;
}
