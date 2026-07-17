// C++20
// 演示：引用、指针、nullptr、const指针、指针算术、void*、new/delete、智能指针
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <memory>    // unique_ptr / shared_ptr / weak_ptr

// ① 引用：变量的别名
void demo01_reference() {
    std::cout << "\n① 引用\n";

    int a = 10;
    int& ref = a;   // 引用必须初始化，之后不能改变绑定

    std::cout << "  a=" << a << "  地址=" << &a << "\n";
    std::cout << "  ref=" << ref << "  地址=" << &ref << "（同一地址！）\n";

    ref = 999;
    std::cout << "  ref=999 后 a=" << a << "（修改引用 = 修改原变量）\n";

    // 引用 vs 指针
    std::cout << "  引用特点：必须初始化，不能为空，不能改变绑定\n";
    std::cout << "  指针特点：可以不初始化，可以为空，可以改变指向\n";

    // const 引用：可绑定临时值
    const int& cref = 42;
    std::cout << "  const int& cref=42: " << cref << "\n";

    // 右值引用（C++11）：绑定临时值，用于移动语义
    int&& rref = 10 + 20;
    std::cout << "  int&& rref=10+20: " << rref << "（右值引用）\n";
}

// ② 指针：存储地址
void demo02_pointer() {
    std::cout << "\n② 指针\n";

    int a = 10;
    int* p = &a;    // & 取地址；* 声明指针
    std::cout << "  a=" << a << "  &a=" << &a << "\n";
    std::cout << "  p=" << p << "（存的是 a 的地址）\n";
    std::cout << "  *p=" << *p << "（解引用，得到 a 的值）\n";

    *p = 999;       // 通过指针修改值
    std::cout << "  *p=999 后 a=" << a << "\n";

    // 未初始化指针：危险！值不确定
    // int* bad;    // 危险：bad 指向随机地址，解引用崩溃
    // *bad = 1;    // 未定义行为

    // nullptr（C++11）：空指针，比 NULL 和 0 更安全
    int* np = nullptr;
    std::cout << "  nullptr=" << np << "\n";
    if (np == nullptr) std::cout << "  指针为空，跳过访问\n";

    // 指针的指针
    int** pp = &p;
    std::cout << "  int** pp=&p: *pp=" << *pp << "（p的值）  **pp=" << **pp << "（a的值）\n";
}

// ③ const 指针四种形式（从右往左读）
void demo03_const_pointer() {
    std::cout << "\n③ const 指针（从右往左读）\n";

    int a = 10, b = 20;

    // const int* p：指向 const 的指针——不能改值，能改指向
    const int* p1 = &a;
    // *p1 = 99;   // 错误
    p1 = &b;       // 可以
    std::cout << "  const int* p1（不能改值，能改指向）: *p1=" << *p1 << "\n";

    // int* const p：const 指针——能改值，不能改指向
    int* const p2 = &a;
    *p2 = 99;      // 可以
    // p2 = &b;    // 错误
    std::cout << "  int* const p2（能改值，不能改指向）: *p2=" << *p2 << "\n";

    // const int* const p：都不能改
    const int* const p3 = &a;
    (void)p3;
    std::cout << "  const int* const p3（都不能改）\n";

    // 助记：从右往左读：
    // const int*  → *p 是 const int → 值不可改
    // int* const  → p 是 const 指针 → 指向不可改
}

// ④ 指针算术：+1 移动一个元素大小
void demo04_pointer_arithmetic() {
    std::cout << "\n④ 指针算术\n";

    int arr[] = {10, 20, 30, 40, 50};
    int* p = arr;   // 数组名 = 首元素地址

    std::cout << "  *p=" << *p << "（arr[0]）\n";
    ++p;
    std::cout << "  ++p 后 *p=" << *p << "（arr[1]，移动了 " << sizeof(int) << " 字节）\n";

    // 遍历
    std::cout << "  指针遍历: ";
    for (int* q = arr; q < arr + 5; ++q)
        std::cout << *q << " ";
    std::cout << "\n";

    // 指针差：两指针相减得到元素个数（不是字节数）
    int* first = arr;
    int* last  = arr + 4;
    std::cout << "  last - first = " << (last - first)
              << "（元素个数，不是字节数）\n";

    // 注意：指针算术只在同一数组内有意义，越界是未定义行为
}

// ⑤ void* 指针：万能指针
void demo05_void_pointer() {
    std::cout << "\n⑤ void* 指针（万能指针）\n";

    int    n = 42;
    double d = 3.14;

    void* vp = &n;   // void* 可以指向任何类型
    std::cout << "  void* vp=&n: vp=" << vp << "\n";
    // *vp = 1;  // 错误：void* 不能解引用
    int* ip = static_cast<int*>(vp);   // 需要强转才能解引用
    std::cout << "  static_cast<int*>(vp): *ip=" << *ip << "\n";

    vp = &d;   // 改为指向 double
    std::cout << "  void* 可以指向任何类型，但解引用前必须转为正确类型\n";
    std::cout << "  （C 的 malloc/memcpy 大量用 void*，C++ 推荐用模板替代）\n";
}

// ⑥ new / delete：动态内存
void demo06_dynamic_memory() {
    std::cout << "\n⑥ new / delete\n";

    // 分配单个对象
    int* p = new int(42);
    std::cout << "  new int(42): *p=" << *p << "\n";
    delete p;      // 必须释放！
    p = nullptr;   // 释放后置空，防悬空指针

    // 分配数组
    int* arr = new int[5]{1,2,3,4,5};
    std::cout << "  new int[5]: ";
    for (int i = 0; i < 5; ++i) std::cout << arr[i] << " ";
    std::cout << "\n";
    delete[] arr;  // 数组用 delete[]，不能用 delete
    arr = nullptr;

    // 常见错误
    std::cout << "  常见错误：\n";
    std::cout << "    1. 忘记 delete → 内存泄漏\n";
    std::cout << "    2. delete 后继续用 → 悬空指针（未定义行为）\n";
    std::cout << "    3. double delete → 崩溃\n";
    std::cout << "    4. new[] 配 delete（非 delete[]）→ 未定义行为\n";
    std::cout << "  → 推荐用智能指针代替裸 new/delete\n";
}

// ⑦ 智能指针（C++11）：自动释放内存
void demo07_smart_pointers() {
    std::cout << "\n⑦ 智能指针\n";

    // unique_ptr：独占所有权，离开作用域自动释放
    {
        auto up = std::make_unique<int>(10);
        std::cout << "  unique_ptr: *up=" << *up << "\n";
        // 不能复制，只能移动
        auto up2 = std::move(up);   // up 变为 nullptr
        std::cout << "  move 后 up=" << up.get()
                  << "  *up2=" << *up2 << "\n";
    }   // up2 离开作用域，自动 delete
    std::cout << "  unique_ptr 已自动释放\n";

    // shared_ptr：共享所有权，引用计数
    auto sp1 = std::make_shared<int>(99);
    std::cout << "  shared_ptr: *sp1=" << *sp1
              << "  引用数=" << sp1.use_count() << "\n";
    {
        auto sp2 = sp1;   // 共享，引用数+1
        std::cout << "  sp2=sp1 后引用数=" << sp1.use_count() << "\n";
    }   // sp2 离开，引用数-1
    std::cout << "  sp2 离开后引用数=" << sp1.use_count() << "\n";

    // weak_ptr：弱引用，不增加引用计数，用于解决循环引用
    std::weak_ptr<int> wp = sp1;
    std::cout << "  weak_ptr: expired=" << wp.expired()
              << "（false=指向的对象还活着）\n";
    if (auto locked = wp.lock()) {   // lock() 尝试转为 shared_ptr
        std::cout << "  lock() 成功: *locked=" << *locked << "\n";
    }

    std::cout << "  选择原则：\n";
    std::cout << "    独占所有权 → unique_ptr（首选）\n";
    std::cout << "    共享所有权 → shared_ptr\n";
    std::cout << "    观察不拥有 → weak_ptr（解决循环引用）\n";
}

int main() {
    std::cout << "=== 06_pointers_refs: 指针与引用 ===";
    demo01_reference();
    demo02_pointer();
    demo03_const_pointer();
    demo04_pointer_arithmetic();
    demo05_void_pointer();
    demo06_dynamic_memory();
    demo07_smart_pointers();
    std::cout << "\n完成。\n";
    return 0;
}
