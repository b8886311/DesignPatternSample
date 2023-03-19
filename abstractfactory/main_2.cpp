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

class Factory {
public:
    virtual std::shared_ptr<ITShirt> createTShirt(const std::string& brand) {
        if (brand == "Nike") {
            return std::make_shared<NikeTShirt>();
        } else if (brand == "Adidas") {
            return std::make_shared<AdidasTShirt>();
        } else {
            return nullptr;
        }
    };

    virtual std::shared_ptr<IShoes> createShoes(const std::string& brand) {
        if (brand == "Nike") {
            return std::make_shared<NikeShoes>();
        } else if (brand == "Adidas") {
            return std::make_shared<AdidasShoes>();
        } else {
            return nullptr;
        }
    };
};

//client code
class Application {
public:
    Application(std::shared_ptr<Factory> factory)
            : factory_(std::move(factory))
    {

    }

    void start(const std::string& brand) {
        do {
            std::cout << "How can I help you?\n";
            std::cout << "(1) Get T-shirt price (2) Get shoes price (3) Get package price (else) Quit\n";
            int cmd = 0;
            std::cin >> cmd;
            if (cmd == 1) {
                std::cout << "It costs " << factory_->createTShirt(brand)->getPrice() << " NT dollars.\n";
            } else if (cmd == 2) {
                std::cout << "It costs " << factory_->createShoes(brand)->getPrice() << " NT dollars.\n";
            } else if (cmd == 3) {
                std::cout << "Product A costs " << factory_->createTShirt(brand)->getPrice() << " NT dollars.\n";
                std::cout << "Product B costs " << factory_->createShoes(brand)->getPrice() << " NT dollars.\n";
                auto result = factory_->createShoes(brand)->getPackagePrice(factory_->createTShirt(brand));
                std::cout << "The package price is " << result << " NT dollars.\n";
            } else {
                std::cout << "Goodbye!\n";
                break;
            }
        } while (true);
    }

private:
    std::shared_ptr<Factory> factory_;
};

int main() {
    std::shared_ptr<Factory> factory = std::make_shared<Factory>();
    std::shared_ptr<Application> app;
    std::string brand;
    std::cout << "Which brand do you want to take a look?\n";
    std::cout << "(1) Nike (2) Adidas (else) Quit\n";
    int cmd = 0;
    std::cin >> cmd;
    if (cmd == 1) {
        brand = "Nike";
    } else if (cmd == 2) {
        brand = "Adidas";
    } else {
        std::cout << "Goodbye!\n";
        return 0;
    }
    app = std::make_shared<Application>(factory);
    app->start(brand);

    return 0;
}
