#include "worker.h"
#include <iostream>
#include <thread>
#include "logger.h"

namespace agvs{
Worker::Worker(const std::string &name)
    : name_{name}, state_{0}, content_{0}
{
}
Worker::Worker(const Worker &rhs)
    : name_{rhs.name_}, state_{rhs.state_}, content_{rhs.content_}, history_{rhs.history_}
{
}
Worker& Worker::operator=(const Worker &rhs) {
    Worker tmp{rhs};
    swap(tmp);
    return *this;
}
void Worker::swap(Worker &rhs) {
    std::swap(name_, rhs.name_);
    std::swap(state_, rhs.state_);
    std::swap(content_, rhs.content_);
    std::swap(history_, rhs.history_);
}
std::string Worker::getClassName() {
    return "Worker";
}
std::string Worker::className_ = "WORKER";

std::string Worker::getClassName2() {
    return className_;
}
std::string Worker::getName() const {
    return name_;
}

void Worker::doWork(size_t content) {
    static int callCount=0;
    content_ = content;
    history_.emplace_back(content);
    state_ = 1;
    std::cout << "start: " << content << std::endl;

    Logger::Instance() << "start: " << content << std::endl;
    for (size_t i=0; i<content_; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << i << std::endl;
    }
    state_ = 0;
    std::cout << "finished: " << callCount << "-th call" << std::endl;
    ++callCount;
}
size_t Worker::getLargestContent() const {
    if (history_.empty()) {
        throw std::runtime_error("history is empty");
    }
    auto largest = history_[0];
    for (auto &v : history_) {
        if (v > largest) {
            largest = v;
        }
    }
    return largest;
}
size_t Worker::operator[](size_t i) const {
    return history_[i];
}
size_t Worker::historySize() const {
    return history_.size();
}
void Worker::printHistory(std::ostream &out) const {
    out << "history of " << name_ << " is:";
    for (auto &h : history_) {
        out << " " << h;
    }
}

}   //namespace agvs
