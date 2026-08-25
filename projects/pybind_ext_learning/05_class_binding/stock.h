// C++20 | 依赖：无
/* .h 头文件的作用：
   声明"有什么"，不写"怎么做"。
   不写 .h、全部塞一个 .cpp 也能编译运行，但有了 .h 后：

   ① 别的 .cpp 也能用 → include 一下就能用这个类（比如 bindings.cpp）
   ② 改实现不用重编译全部 → 只改 .cpp 时，其他文件不用重新编译
   ③ 接口和实现分离 → 看 .h 就知道这个类有什么数据和能做什么

   类比：头文件 = 菜单（列出菜名），.cpp = 厨房（实际做菜）

   class  = 模具/蓝图，定义这类东西有什么数据（成员变量）+ 能做什么（成员函数）
   对象   = 按模具生产出来的具体东西（也叫"实例"）
   public:   = 外部也能访问
   private:  = 只能在类内部访问（默认）
   实例化   = 用 class 创建一个对象：Stock s("600519.SH", 1500.0); */
#pragma once

#include <string>
#include <utility>

/* class：把数据 + 操作数据的方法打包在一起。上面是成员变量（数据），下面是成员函数（方法）。

   用法示例：
     Stock s("600519.SH", 1500.0);   // 创建一个对象（实例化）
     s.code();                        // "600519.SH" — 获取股票代码
     s.close_price();                 // 1500.0      — 获取收盘价
     s.set_close(1600.0);             // 修改收盘价
     s.limit_prices();                // {1760.0, 1440.0} — 涨跌停价
     s.m_code;                        // ✗ 编译错误：private 成员不能从外部访问 */
class Stock {
    // 成员变量（数据），默认 private，外部不能直接访问
    std::string m_code;   // 股票代码
    double      m_close;  // 收盘价

public:
    // 以下都是成员函数（方法），外部可以调用

    // 构造函数：创建对象时自动调用，初始化成员变量（必须和类同名，语法规定）
    Stock(std::string code, double close);

    // 获取股票代码
    /* 末尾的 const = 承诺这个函数不会修改对象的成员变量（m_code、m_close）
       这是给调用者的保证："放心调，不会改你的数据"
       编译器会检查：如果函数里写了 m_code = "xxx"，直接编译报错*/
    std::string code() const;

    // 获取收盘价
    double close_price() const;

    // 更新收盘价
    void set_close(double v);

    // 涨停/跌停价（简化 ±10%），返回 pair<涨停, 跌停>
    std::pair<double, double> limit_prices() const;

    /* 对象的字符串表示（Python 的 __repr__）
       pybind11 绑定到 __repr__ 后，Python 侧 print(s) 或 repr(s) 自动调用这个函数
       C++ 写好字符串拼接 → pybind11 桥接 → Python 展现实例信息 */
    std::string repr() const;
};
