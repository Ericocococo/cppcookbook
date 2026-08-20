// C++20 | 依赖：无
/* C++ 类的标准实现流程（三步走）：
   ① .h 头文件 — 声明类（有什么数据 + 能做什么）
   ② .cpp 文件 — 实现类（每个函数具体怎么写）
   ③ main.cpp  — 用类（创建对象、调方法）

   完整流程：
   stock.h    →  class Stock { ... };            // 声明：列出成员变量和成员函数
   stock.cpp  →  Stock::code() { return ...; }   // 实现：写每个函数的逻辑
   main.cpp   →  Stock s("600519.SH", 1500);     // 使用：创建对象，调方法

   类比：头文件 = 菜单（列出菜名），.cpp = 厨房（实际做菜）

   class  = 模具/蓝图，定义这类东西长什么样
   对象   = 按模具生产出来的具体东西（也叫"实例"）
   public:  = 外部也能访问
   private: = 只能在类内部访问（默认）
   实例化 = 用 class 创建一个对象：Stock s("600519.SH", 1500.0); */
#pragma once

#include <string>
#include <utility>  // pair

class Stock {
    // 成员变量（数据）：默认 private，外部不能直接访问
    std::string m_code;   // 股票代码
    double      m_close;  // 收盘价

public:
    // 以下都是成员函数（方法），外部可以调用

    // 构造函数：创建对象时自动调用（必须和类同名，语法规定）
    Stock(std::string code, double close);

    // 获取股票代码（末尾 const = 函数不修改成员变量）
    std::string code() const;

    // 获取收盘价
    double close_price() const;

    // 更新收盘价
    void set_close(double v);

    // 涨停/跌停价（简化 ±10%），返回 pair<涨停, 跌停>
    std::pair<double, double> limit_prices() const;
};
