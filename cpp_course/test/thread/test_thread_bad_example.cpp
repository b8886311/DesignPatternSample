#include <mutex>
#include <thread>
#include <iostream>
#include <queue>
#include <condition_variable>

void inner_func(std::mutex &mtx) {

    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "enter inner_func and get lock" << std::endl;
}
void test_recursive_lock(){
    std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "get lock" << std::endl;
    inner_func(mtx);
    std::cout << "going to leave test_recursive_lock" << std::endl;
}
void test_dead_lock() {
    std::mutex mtx1, mtx2;
    std::thread th1([&mtx1, &mtx2](){
        int i=0;
        while(true) {
            std::lock_guard<std::mutex> lock(mtx1);
            {
                std::lock_guard<std::mutex> lock(mtx2);
                ++i;
                std::cout << "th1 " << i << std::endl;
            }
        }
    });
    std::thread th2([&mtx1, &mtx2](){
        int i=0;
        while(true) {
            std::lock_guard<std::mutex> lock(mtx2);
            {
                std::lock_guard<std::mutex> lock(mtx1);
                ++i;
                std::cout << "      th2 " << i << std::endl;
            }
        }

    });
    pthread_setname_np(th1.native_handle(), "purple");
    pthread_setname_np(th2.native_handle(), "arrow");
    th1.join();
    th2.join();
}
void test_mutex_2() {
    std::mutex mtxWork, mtxDoneWorks;
    std::queue<int> works, doneWorks;
    std::thread workSourceTh([&mtxWork, &mtxDoneWorks, &works, &doneWorks](){
        for (int id=0; id<100; ++id) {
            {
                {
                    std::lock_guard<std::mutex> lock(mtxWork);
                    works.emplace(id);
                    std::cout << "source create: " << id << std::endl;
                }
                bool done=false;
                while(true) {
                    {
                        //wait until it is done
                        std::lock_guard<std::mutex> lock2(mtxDoneWorks);
                        if (doneWorks.front() == id) {
                            doneWorks.pop();
                            done=true;
                        }
                    }
                    if (done) {
                        std::cout << "source found " << id << " is done" << std::endl;
                        break;
                    } else {
                        std::this_thread::sleep_for(std::chrono::milliseconds (100));
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds (100));
        }
    });
    std::thread workingTh([&mtxWork, &mtxDoneWorks, &works, &doneWorks](){
        while(true) {
            int idWork = -1;
            {
                while(true) {
                    {
                        std::lock_guard<std::mutex> lock(mtxWork);
                        if (!works.empty()) {
                            idWork = works.front();
                            works.pop();
                            std::cout << "worker get work: " << idWork << std::endl;
                            break;
                        }
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds (100));
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));   //simulate do the job
            {
                std::lock_guard<std::mutex> lock2(mtxDoneWorks);
                doneWorks.emplace(idWork);
                std::cout << "worker finish work: " << idWork << std::endl;
            }
        }
    });
    pthread_setname_np(workSourceTh.native_handle(), "source");
    pthread_setname_np(workingTh.native_handle(), "worker");
    workingTh.join();
    workSourceTh.join();
}
void test_conditional_variable() {
    std::mutex mtxWork, mtxDoneWorks;
    std::queue<int> works, doneWorks;
    std::condition_variable condVarCreate, condVarDone;
    std::mutex mtxDone, mtxCreate;
    std::unique_lock<std::mutex> ulockDone(mtxDone), ulockCreate(mtxCreate);
    std::thread workSourceTh([&mtxWork, &mtxDoneWorks, &works, &doneWorks, &condVarCreate, &condVarDone, &ulockDone](){
        //question: why this?
        std::this_thread::sleep_for(std::chrono::seconds(1));
        for (int id=0; id<100; ++id) {
            {
                {
                    std::lock_guard<std::mutex> lock(mtxWork);
                    works.emplace(id);
                    std::cout << "source create: " << id << std::endl;
                    condVarCreate.notify_all();
                }
                bool done=false;
                while(true) {
                    {
                        //wait until it is done
                        condVarDone.wait(ulockDone);
                        std::lock_guard<std::mutex> lock2(mtxDoneWorks);
                        if (doneWorks.front() == id) {
                            doneWorks.pop();
                            done=true;
                        }
                    }
                    if (done) {
                        std::cout << "source found " << id << " is done" << std::endl;
                        break;
                    }
//                    else {
//                        std::this_thread::sleep_for(std::chrono::milliseconds (100));
//                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds (100));
        }
    });
    std::thread workingTh([&mtxWork, &mtxDoneWorks, &works, &doneWorks, &condVarCreate, &ulockCreate, &condVarDone](){
        while(true) {
            int idWork = -1;
            {
                while(true) {
                    {
                        condVarCreate.wait(ulockCreate);
                        std::lock_guard<std::mutex> lock(mtxWork);
                        if (!works.empty()) {
                            idWork = works.front();
                            works.pop();
                            std::cout << "worker get work: " << idWork << std::endl;
                            break;
                        }
                    }
                    //std::this_thread::sleep_for(std::chrono::milliseconds (100));
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));   //simulate do the job
            {
                std::lock_guard<std::mutex> lock2(mtxDoneWorks);
                doneWorks.emplace(idWork);
                std::cout << "worker finish work: " << idWork << std::endl;
                condVarDone.notify_all();
            }
        }
    });
    pthread_setname_np(workSourceTh.native_handle(), "source");
    pthread_setname_np(workingTh.native_handle(), "worker");
    workingTh.join();
    workSourceTh.join();
}
int main() {
    //test_recursive_lock();
    //test_mutex_2();
    //test_dead_lock2();
    test_conditional_variable();
    return 0;
}

//reference: https://docs.microsoft.com/en-us/cpp/parallel/concrt/asynchronous-agents-library?view=msvc-170
