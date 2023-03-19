//
// Created by purplearrow on 5/17/22.
//

#include "car.h"
#include <iostream>

Car::Car()
    : running_{false}, setAngle_{0}, sendAngle_{0}
{
    th_ = std::thread([this](){
        running_ = true;
        thread_body();
    });
}
Car::~Car() {
    stop();
}
void Car::stop() {
    setRunning(false);
    th_.join();
}

void Car::setAngle(int angle) {
    try{
        std::lock_guard<std::mutex> lock(mtx_);
        std::cout << "setAngle: " << angle << std::endl;
        //throw std::runtime_error("I am bad");
        setAngle_ = angle;
    }
    catch(std::exception &e) {
        std::cerr << "setAngle get exception: " << e.what() << std::endl;
    }
}
void Car::setRunning(bool r) {
    std::lock_guard<std::mutex> lock(mtx_);
    running_ = r;
}
bool Car::getRunning() {
    std::lock_guard<std::mutex> lock(mtx_);
    return running_;
}
void Car::thread_body() {
    while(getRunning()) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            std::cout << "send to driver: " << setAngle_ << std::endl;    //simulate send to driver
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "thread body is finished" << std::endl;
}
