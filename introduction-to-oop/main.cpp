#include <iostream>
#include <string>

namespace object {
    class Cat {
    public:
        std::string name;
        std::string sex;
        int age;
        int weight;
        std::string color;
        std::string texture;

        void breathe() {
            std::cout << name << " is breathing." << std::endl;
        }

        void eat(std::string food) {
            std::cout << name << " is eating " << food << "." << std::endl;
        }

        void run(std::string destination) {
            std::cout << name << " is running to " << destination << std::endl;
            for (size_t i = 0; i < 3; i++) {
                std::cout << "." << std::endl;
            }
            std::cout << name << " arrives " << std::endl;
        }

        void sleep(int hour) {
            std::cout << name << " is sleeping." << std::endl;
            for (size_t i = 0; i < hour; i++) {
                std::cout << "." << std::endl;
            }
            std::cout << name << " wakes up." << std::endl;
        }

        void meow() {
            std::cout << "meow" << std::endl;
        }
    };
}

namespace inheritance {
    class Animal {
    public:
//        virtual void eat() = 0;

        virtual void eat() {
            std::cout << "this animal is eating food." << std::endl;
        }
    };

    class Cat : public Animal {
    public:
        void eat() override {
            std::cout << "cat is eating fish." << std::endl;
        }

        void meow() {
            std::cout << "meow" << std::endl;
        }
    };

    class Dog : public Animal {
    public:
        void eat() override {
            std::cout << "dog is eating bone." << std::endl;
        }

        void bark() {
            std::cout << "bark" << std::endl;
        }
    };
}

namespace interface {
    class FlyingTransport {
    public:
        virtual void fly(std::string origin, std::string destination, int passengers) = 0;
    };

    class Airplane : public FlyingTransport {
    public:
        void fly(std::string origin, std::string destination, int passengers) override {
            std::cout << "airplane is flying." << std::endl;
        }
    };

    class Helicopter : public FlyingTransport {
    public:
        void fly(std::string origin, std::string destination, int passengers) override {
            std::cout << "helicopter is flying." << std::endl;
        }
    };

    class DomesticatedGryphon : public FlyingTransport {
        void fly(std::string origin, std::string destination, int passengers) override {
            std::cout << "domesticated gryphon is flying." << std::endl;
        }
    };

    class Airport {
    public:
        void acceptP(FlyingTransport* flyingTransport) {
            std::cout << flyingTransport << std::endl;
            flyingTransport->fly("here", "there", 100);
        }

        void acceptR(FlyingTransport& flyingTransport) {
            std::cout << &flyingTransport << std::endl;
            flyingTransport.fly("here", "there", 100);
        }
    };
}

namespace relations {
    class A {
    public:
        A() {
            std::cout << "class A constructor\n";
        }

        ~A() {
            std::cout << "class A destructor\n";
        }

        void greet() {
            std::cout << "hello\n";
        }
    };

    class B {
    public:
        B() {
            std::cout << "class B constructor\n";
        }

        ~B() {
            std::cout << "class B destructor\n";
        }

        void greet() {
            std::cout << "hello\n";
        }
    };

    class C {
    public:
        C() {
            std::cout << "class C constructor\n";
        }

        ~C() {
            std::cout << "class C destructor\n";
        }

        void greetA(A* a) {
            a->greet();
        }

        void greetB() {
            b_.greet();
        }
    private:
        B b_;
    };
}

int main() {
//    object::Cat cat1;
//    cat1.name = "Oscar";
//    cat1.sex = "male";
//    cat1.age = 3;
//    cat1.weight = 7;
//    cat1.color = "brown";
//    cat1.texture = "striped";
//    cat1.breathe();
//    cat1.eat("fish");
//    cat1.run("home");
//    cat1.sleep(3);
//    cat1.meow();
//
//    std::cout << "/********************/\n";

//    inheritance::Cat cat2;
//    inheritance::Dog dog;
//    cat2.eat();
//    cat2.meow();
//    dog.eat();
//    dog.bark();
//
    std::cout << "/********************/\n";

    interface::Airplane airplane1;
    interface::Helicopter helicopter1;
    interface::DomesticatedGryphon domesticatedGryphon1;
    interface::Airport airport;
    airport.acceptP(&airplane1);
    airport.acceptR(airplane1);
    airport.acceptP(&helicopter1);
    airport.acceptR(helicopter1);
    airport.acceptP(&domesticatedGryphon1);
    airport.acceptR(domesticatedGryphon1);
//
//    std::cout << "/********************/\n";

//    relations::A a;
//    relations::C c;
//    c.greetA(&a);
//    c.greetB();

    return 0;
}
