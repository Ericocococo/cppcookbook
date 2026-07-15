// C++20
// 演示：引用、指针、nullptr、const 指针、指针算术、动态内存、智能指针
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <memory>   // 智能指针 unique_ptr / shared_ptr

// ① 引用：变量的别名，比指针更安全更简单
void demo01_reference() {
    std::cout << "\n① 引用（变量的别名）\n";

    int a = 10;
    int& ref = a;    // ref 是 a 的别名，它们指向同一块内存

    std::cout << "  a   = " << a   << "  地址=" << &a   << "\n";
    std::cout << "  ref = " << ref << "  地址=" << &ref << "（地址相同！）\n";

    ref = 999;   // 通过 ref 修改，a 也变
    std::cout << "  ref=999 后，a = " << a << "\n";

    // 引用必须在声明时绑定，之后不能改变绑定的变量
    // int& bad;     // 错误：引用必须初始化
    // ref = b;      // 这不是改变绑定，是把 b 的值赋给 a

    // 常量引用：可以绑定临时值，不能修改
    const int& cref = 42;   // 42 是临时值，普通引用不能绑定
    std::cout << "  const int& cref = 42 -> cref=" << cref << "\n";
}

// ② 指针：存储变量地址的变量
void demo02_pointer() {
    std::cout << "\n② 指针（存储地址）\n";

    int a = 10;
    int* p = &a;    // & 取地址，p 存的是 a 的地址

    std::cout << "  a 的值: " << a  << "\n";
    std::cout << "  a 的地址: " << &a << "\n";
    std::cout << "  p 的值（存的是地址）: " << p  << "\n";
    std::cout << "  *p（解引用，得到a的值）: " << *p << "\n";

    // 通过指针修改值
    *p = 999;
    std::cout << "  *p=999 后，a = " << a << "\n";

    // 空指针：不指向任何地址
    int* null_p = nullptr;   // C++11，推荐用 nullptr，不用 NULL 或 0
    std::cout << "  nullptr 指针 = " << null_p << "\n";

    // 使用前必须检查指针是否为空！
    if (null_p != nullptr) {
        *null_p = 1;   // 安全
    } else {
        std::cout << "  指针为空，跳过访问\n";
    }
}

// ③ const 指针的四种形式
void demo03_const_pointer() {
    std::cout << "\n③ const 指针（读法：从右往左读）\n";

    int a = 10, b = 20;

    // 1. 指向 const 的指针：不能通过指针修改值，但指针本身可以指向别处
    const int* p1 = &a;
    // *p1 = 99;   // 错误：不能修改 a 的值
    p1 = &b;       // 可以：指向 b
    std::cout << "  const int* p1（指向const）: *p1=" << *p1 << "（不能改值，能改指向）\n";

    // 2. const 指针：指针本身不可变（必须初始化），但可以修改指向的值
    int* const p2 = &a;
    *p2 = 99;      // 可以：修改 a 的值
    // p2 = &b;    // 错误：p2 本身不可变
    std::cout << "  int* const p2（const指针）: *p2=" << *p2 << "（能改值，不能改指向）\n";

    // 3. 指向 const 的 const 指针：都不能改
    const int* const p3 = &a;
    std::cout << "  const int* const p3（都const）: *p3=" << *p3 << "（两者都不能改）\n";
}

// ④ 指针算术：指针 +1 移动一个元素的大小
void demo04_pointer_arithmetic() {
    std::cout << "\n④ 指针算术（配合数组）\n";

    int arr[] = {10, 20, 30, 40, 50};
    int* p = arr;   // 数组名本身就是首元素地址

    std::cout << "  arr[0]=" << arr[0] << "  p指向=" << *p << "\n";

    ++p;   // 指针前移一个 int（4字节），指向 arr[1]
    std::cout << "  p++ 后，*p=" << *p << "（arr[1]）\n";

    std::cout << "  用指针遍历数组: ";
    for (int* q = arr; q < arr + 5; ++q) {
        std::cout << *q << " ";
    }
    std::cout << "\n";
}

// ⑤ 动态内存：在运行时分配内存（堆上分配）
void demo05_dynamic_memory() {
    std::cout << "\n⑤ 动态内存（new/delete）\n";

    // new：在堆上分配内存，返回指针
    int* p = new int(42);   // 分配一个 int，初始化为 42
    std::cout << "  new int(42): *p=" << *p << "\n";

    *p = 100;
    std::cout << "  修改后 *p=" << *p << "\n";

    delete p;    // 必须手动释放！不释放 = 内存泄漏
    p = nullptr; // 释放后置空，防止悬空指针

    // 分配数组
    int* arr = new int[3]{1, 2, 3};
    std::cout << "  new int[3]: ";
    for (int i = 0; i < 3; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
    delete[] arr;   // 数组用 delete[]，不能用 delete
    arr = nullptr;

    std::cout << "  （裸指针需要手动 delete，推荐用智能指针）\n";
}

// ⑥ 智能指针（C++11）：自动管理内存，不需要手动 delete
void demo06_smart_pointer() {
    std::cout << "\n⑥ 智能指针（自动释放）\n";

    // unique_ptr：独占所有权，离开作用域自动释放
    {
        std::unique_ptr<int> up = std::make_unique<int>(10);
        std::cout << "  unique_ptr *up=" << *up << "\n";
        // 不需要 delete，离开 {} 后自动释放
    }
    std::cout << "  离开作用域后 unique_ptr 自动释放\n";

    // shared_ptr：共享所有权，引用计数归零时自动释放
    std::shared_ptr<int> sp1 = std::make_shared<int>(99);
    std::shared_ptr<int> sp2 = sp1;   // sp1 和 sp2 共享同一块内存
    std::cout << "  shared_ptr *sp1=" << *sp1
              << "  引用计数=" << sp1.use_count() << "\n";
    sp2.reset();  // sp2 释放，引用计数减1
    std::cout << "  sp2.reset() 后引用计数=" << sp1.use_count() << "\n";
    // sp1 离开作用域后，引用计数归零，内存释放
}

int main() {
    std::cout << "=== 05_pointers_refs: 指针与引用 ===";
    demo01_reference();
    demo02_pointer();
    demo03_const_pointer();
    demo04_pointer_arithmetic();
    demo05_dynamic_memory();
    demo06_smart_pointer();
    std::cout << "\n完成。\n";
    return 0;
}
