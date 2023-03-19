//
// Created by purplearrow on 5/17/22.
//

#ifndef CPPCOURSE_TEST_THREAD_CAR_H
#define CPPCOURSE_TEST_THREAD_CAR_H

#include <thread>
#include <mutex>

class Car {
public:
    Car();
    ~Car();

    void setAngle(int angle);
    void stop();
private:
    std::mutex mtx_;
    bool running_;
    int setAngle_;
    int sendAngle_;
    std::thread th_;

    void thread_body();
    void setRunning(bool r);
    bool getRunning();
};


#endif //CPPCOURSE_TEST_THREAD_CAR_H
