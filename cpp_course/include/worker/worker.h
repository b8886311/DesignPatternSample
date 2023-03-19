#ifndef AGVS_INCLUDE_WORKER_WORKER_H
#define AGVS_INCLUDE_WORKER_WORKER_H
#include <string>
#include <vector>
#include <iostream>

namespace agvs {
class Worker {
public:
    explicit Worker(const std::string &name);
    Worker(const Worker &rhs);
    Worker& operator=(const Worker &rhs);
    ~Worker() = default;
    void swap(Worker &rhs);

    static std::string getClassName();
    static std::string getClassName2();
    std::string getName() const;
    void doWork(size_t content);

    size_t getLargestContent() const;
    size_t operator[](size_t i) const;
    size_t historySize() const;

    void printHistory(std::ostream &out) const;
private:
    static std::string className_;
    std::string name_;
    int state_;     //0: idle, 1: working
    size_t content_;
    std::vector<size_t> history_;
};

}  //namespace AGVS

#endif
