#include <iostream>
#include <string>
#include <memory>

class ITShirt {
public:
    virtual int getPrice() = 0;
};

class NikeTShirt : public ITShirt {
public:
    int getPrice() override {
        return 100;
    }
};

class AdidasTShirt : public ITShirt {
public:
    int getPrice() override {
        return 120;
    }
};

class IShoes {
public:
    virtual int getPrice() = 0;

    virtual int getPackagePrice(std::shared_ptr<ITShirt> itShirt) = 0;
};

class NikeShoes : public IShoes {
public:
    int getPrice() override {
        return 1000;
    }

    int getPackagePrice(std::shared_ptr<ITShirt> itShirt) override {
        std::cout << "You will get 20% off on T-shirt if you take with pair of shoes.\n";
        auto result = itShirt->getPrice() * 0.8 + getPrice();
        return static_cast<int>(result);
    }
};

class AdidasShoes : public IShoes {
public:
    int getPrice() override {
        return 990;
    }

    int getPackagePrice(std::shared_ptr<ITShirt> itShirt) override {
        std::cout << "You will get 15% off on T-shirt if you take with pair of shoes.\n";
        auto result = itShirt->getPrice() * 0.85 + getPrice();
        return static_cast<int>(result);
    }
};

class IFactory {
public:
    virtual std::shared_ptr<ITShirt> createTShirt() = 0;

    virtual std::shared_ptr<IShoes> createShoes() = 0;
};

class NikeFactory : public IFactory {
public:
    std::shared_ptr<ITShirt> createTShirt() override {
        return std::make_shared<NikeTShirt>();
    }

    std::shared_ptr<IShoes> createShoes() override {
        return std::make_shared<NikeShoes>();
    }
};

class AdidasFactory : public IFactory {
public:
    std::shared_ptr<ITShirt> createTShirt() override {
        return std::make_shared<AdidasTShirt>();
    }

    std::shared_ptr<IShoes> createShoes() override {
        return std::make_shared<AdidasShoes>();
    }
};

//client code
class Application {
public:
    Application(std::shared_ptr<IFactory> iFactory)
        : iFactory_(std::move(iFactory))
    {

    }

    void start() {
        do {
            std::cout << "How can I help you?\n";
            std::cout << "(1) Get T-shirt price (2) Get shoes price (3) Get package price (else) Quit\n";
            int cmd = 0;
            std::cin >> cmd;
            if (cmd == 1) {
                std::cout << "It costs " << iFactory_->createTShirt()->getPrice() << " NT dollars.\n";
            } else if (cmd == 2) {
                std::cout << "It costs " << iFactory_->createShoes()->getPrice() << " NT dollars.\n";
            } else if (cmd == 3) {
                std::cout << "Product A costs " << iFactory_->createTShirt()->getPrice() << " NT dollars.\n";
                std::cout << "Product B costs " << iFactory_->createShoes()->getPrice() << " NT dollars.\n";
                auto result = iFactory_->createShoes()->getPackagePrice(iFactory_->createTShirt());
                std::cout << "The package price is " << result << " NT dollars.\n";
            } else {
                std::cout << "Goodbye!\n";
                break;
            }
        } while (true);
    }

private:
    std::shared_ptr<IFactory> iFactory_;
};

int main() {
    std::shared_ptr<IFactory> iFactory;
    std::shared_ptr<Application> app;
    std::cout << "Which brand do you want to take a look?\n";
    std::cout << "(1) Nike (2) Adidas (else) Quit\n";
    int cmd = 0;
    std::cin >> cmd;
    if (cmd == 1) {
        iFactory = std::make_shared<NikeFactory>();
    } else if (cmd == 2) {
        iFactory = std::make_shared<AdidasFactory>();
    } else {
        std::cout << "Goodbye!\n";
        return 0;
    }
    app = std::make_shared<Application>(iFactory);
    app->start();

    return 0;
}
