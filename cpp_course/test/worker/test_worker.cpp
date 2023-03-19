#include "worker.h"
#include <iostream>

int main() {
    std::cout << "getClassName: " << agvs::Worker::getClassName() << std::endl;
    std::cout << "getClassName2: " << agvs::Worker::getClassName2() << std::endl;

    agvs::Worker worker("purplearrow");
    agvs::Worker anotherWorker(worker);
    std::cout << "name is " << anotherWorker.getName() << std::endl;

    std::cout << "name is " << worker.getName() << std::endl;
    worker.doWork(4);
    worker.doWork(1);
    worker.doWork(3);

    std::cout << "size of " << worker.getName() << " is " << worker.historySize() << std::endl;
    std::cout << "size of " << anotherWorker.getName() << " is " << anotherWorker.historySize() << std::endl;

    std::cout << "largest work of " << worker.getName() << " is " << worker.getLargestContent() << std::endl;

    worker.printHistory(std::cout);
    std::cout << std::endl;

    return 0;
}
