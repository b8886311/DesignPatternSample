#include "stop_watch.h"
#include <vector>
#include <iostream>
#include <thread>

void test_stopwatch() {
    agvs::StopWatch w;
    std::cout << "at beginning, it is not running: " << w.isRunning() << std::endl;
    w.start();
    std::cout << "now, it is running: " << w.isRunning() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    w.stop();
    std::cout << "now, it is stopped: " << w.isRunning() << std::endl;
    std::cout << "get time, should be 1000: " << w.getTime() << ", ave, should be 1000: " << w.getAverageTime() << std::endl;

    w.start();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    w.stop();
    std::cout << "get time, should be 3000: " << w.getTime() << ", ave, should be 1500: " << w.getAverageTime() << std::endl;

    w.reset();
    std::cout << "reset, should be 0: " << w.getTime() << ", ave, should be 0: " << w.getAverageTime() << std::endl;
}
int main() {
    test_stopwatch();
    return 0;
}