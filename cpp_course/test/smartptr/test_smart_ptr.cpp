#include <iostream>
#include <memory>

class Memory: public std::enable_shared_from_this<Memory>{
public:
    explicit Memory(size_t size) {std::cout << "default constructor" << std::endl;}
    ~Memory() {std::cout << "destructor" << std::endl;}
    Memory(const Memory &rhs) {std::cout << "copy constructor" << std::endl;}
    Memory& operator=(const Memory &rhs) {std::cout << "copy assignment" << std::endl; return *this;}
    Memory(Memory &&rhs) noexcept {std::cout << "move constructor" << std::endl;}
    Memory& operator=(Memory &&rhs) noexcept {std::cout << "move assignment" << std::endl; return *this;}

    virtual void run() {std::cout << "this is memory" << std::endl; }

    void nono() {std::cout << "nono: this is memory" << std::endl;}
    void print() {std::cout << "print";}
    std::shared_ptr<Memory> dont_do_this() {
        return std::shared_ptr<Memory>(this);
    }
    std::shared_ptr<Memory> do_this() {
        return shared_from_this();
    }
};
struct Tmp{
    Memory *p_;
    int count_;
};

class MemoryHolder {
public:
    explicit MemoryHolder(Memory* p)
    {
        tmp_ = new Tmp;
        tmp_->p_ = p;
        tmp_->count_ = 1;
    }
    MemoryHolder(const MemoryHolder &rhs) : tmp_{rhs.tmp_} {
        tmp_->count_ += 1;
    }
    MemoryHolder& operator=(const MemoryHolder &rhs) {
        tmp_ = rhs.tmp_;
        tmp_->count_ += 1;
//        tmp_->p_ = rhs.tmp_->p_;
//        ++tmp_->count_;
        return *this;
    }
    ~MemoryHolder() {
        tmp_->count_ -= 1;
        if (tmp_->count_<=0) {
            delete tmp_->p_;
        }
        delete tmp_;
    }
    Memory* operator->() {
        return tmp_->p_;
    }
    operator bool() {return tmp_->count_ > 0;}
    double operator()(int a, int b);
    int operator[](int a);

private:
    Tmp *tmp_;
};
MemoryHolder make_memory_holder(size_t size) {
    Memory *p = new Memory(size);
    return MemoryHolder(p);
}

class DerivedMemory : public Memory{
public:
    DerivedMemory() : Memory(3) {}
    virtual ~DerivedMemory() = default;

    virtual void run() {std::cout << "this is DerivedMemory" << std::endl; }
    void nono() {std::cout << "nono: this is DerivedMemory" << std::endl;}
};
void test_func(Memory m) {
    Memory a(std::move(m));
    //a.print();
}
void fff() {
    std::shared_ptr<Memory> obj = std::make_shared<Memory>(22);
//    std::shared_ptr<Memory> a(&obj);
    //auto aaa = obj.dont_do_this();
    auto aaa = obj->do_this();
}
int main() {
//    Memory m(111);
//    Memory m2(m);
//    m2 = m;
//    Memory m3 = m;
    //Memory *p = new Memory(22);
    //delete p;

    //std::shared_ptr<Memory> aaa = std::make_shared<Memory>(22);
    MemoryHolder mh = make_memory_holder(22);
    mh->run();
    if (mh) {
        std::cout << "point to something" << std::endl;
    }

    return 0;
}
int main2() {
    std::unique_ptr<Memory> uu = std::make_unique<Memory>(22);
    auto u2 = std::make_unique<Memory>(22);
    if (uu) {
        std::cout << "uu point to something" << std::endl;
    }
    std::unique_ptr<Memory> u3 = std::move(uu);
    if (uu) {
        std::cout << "uu point to something" << std::endl;
    }else {
        std::cout << "uu point to nothing" << std::endl;
    }
    if (u3) {
        std::cout << "u3 point to something" << std::endl;
    }else {
        std::cout << "u3 point to nothing" << std::endl;
    }
//    Memory a(10);
//    Memory b(a);
//    b = a;
//    Memory c(std::move(b));
//    c = std::move(b);
    //Memory a(10);
    //test_func(Memory(10));
    //Memory *p = new Memory(22);
    //std::shared_ptr<Memory> a = std::make_shared<Memory>(22);
    //auto a = std::make_shared<Memory>(22);

    //std::shared_ptr<Memory> a (new Memory(22));
    fff();

    //Memory *p = some_function();
    //std::shared_ptr<Memory> a (new Memory(22));

//    if (a) {
//        std::cout << " a is point to something" << std::endl;
//    }else {
//        std::cout << "a is null" << std::endl;
//    }
//
//    //Memory *p = new Memory();
//    //Memory *p = new DerivedMemory();
//
//    a = std::make_shared<DerivedMemory>();
//    a->run();
//    a->nono();
    return 0;
}