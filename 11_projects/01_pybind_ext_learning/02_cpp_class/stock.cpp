// C++20 | 依赖：无
// Stock 类实现 — 纯 C++，这里写"怎么做"。
#include "stock.h"

/* 构造函数：创建对象时自动调用，函数名必须和类名完全一致（包括大小写）

   :: 作用域运算符 —— 告诉编译器"去哪找"
   ┌─────────────────────────┬──────────────────────────────┐
   │ 写法                    │ 含义                         │
   ├─────────────────────────┼──────────────────────────────┤
   │ Stock::Stock(...)       │ Stock 类的构造函数           │
   │ Stock::code()           │ Stock 类的 code 成员函数     │
   │ std::cout               │ std 命名空间的 cout 对象     │
   │ std::string             │ std 命名空间的 string 类     │
   │ Finance::Stock::Stock() │ Finance 命名空间→Stock 类→构造函数 │
   └─────────────────────────┴──────────────────────────────┘
   :: 左边是类名 → 表示"这个函数属于这个类"
   :: 左边是命名空间 → 表示"这个东西在这个命名空间里"

   场景 ① 类作用域（本文件用的就是这种）：
     class Stock { Stock(...); };
     Stock::Stock(...) {}              // 在类外实现构造函数

   场景 ② 命名空间作用域：
     namespace Finance { void foo(); }
     void Finance::foo() {}            // 在命名空间外实现函数

   场景 ③ 命名空间 + 类叠加：
     namespace Finance { class Stock { Stock(...); }; }
     Finance::Stock::Stock(...) {}     // 命名空间→类→构造函数

   : m_code(...), m_close(...)  → 成员初始化列表，冒号开头，逗号分隔
     直接用值构造成员，比函数体内赋值少一次默认构造
   std::move(code)              → 把字符串"搬"给 m_code，避免拷贝
     基本类型（int/double）不需要 move，拷贝和搬没区别
   {}                           → 函数体为空，所有工作已在初始化列表完成 */
Stock::Stock(std::string code, double close)
    : m_code(std::move(code)), m_close(close)
{
}


/* 成员函数末尾 const — 只有成员函数能用，普通函数没有
   每个成员函数都有一个隐藏参数 this（指向调用它的对象）：

     Stock s("600519.SH", 1500.0);
     s.code();
     // 编译器实际做的：Stock::code(&s)
     //                              └── 把 s 的地址传给 this

     m_code  →  实际是 this->m_code 的简写

   末尾 const 的作用就落在这个隐藏的 this 上：
     code()        → this 类型是 Stock*        → 可以改成员变量
     code() const  → this 类型是 const Stock*  → 不能改成员变量

   你写的：                           编译器实际理解为：
     std::string Stock::code() const    std::string Stock::code(const Stock* this)
     {                                  {
         return m_code;                     return this->m_code;
     }                                  } */
std::string Stock::code() const
{
    return m_code;
}

// 返回收盘价
double Stock::close_price() const
{
    return m_close;
}

// 更新收盘价
void Stock::set_close(double v)
{
    m_close = v;
}

// 涨停/跌停价（简化 ±10%），返回 pair<涨停, 跌停>
std::pair<double, double> Stock::limit_prices() const
{
    return {m_close * 1.1, m_close * 0.9};
}
