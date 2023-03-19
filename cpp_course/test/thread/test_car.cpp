//
// Created by purplearrow on 5/17/22.
//
#include <iostream>
#include "car.h"

int main() {
    std::cout << "ver6" << std::endl;
    Car car;


    while(true) {
        std::cout << "please input angle: " << std::endl;
        int a;
        std::cin >> a;
        if (a < -100) {
            break;
        }
        car.setAngle(a);
    }

    //car.stop();
    return 0;
}