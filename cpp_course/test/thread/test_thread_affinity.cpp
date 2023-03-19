#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <fstream>

void case1_create_thread() {
    std::thread th([](){
        std::cout << "hello world" << std::endl;
    });
    th.join();
}
class Worker{
public:
    void Send(const std::string &msg) const{
        std::cout << "send: " << msg << std::endl;
    }
};
void case2_capture_by_ref() {
    Worker w;
    std::string msg("hello world");
    std::thread th([&w, &msg](){
        w.Send(msg);
    });
    //th.detach();
    th.join();
}
void case2_capture_by_value() {
    Worker w;
    std::string msg("hello world");
    std::thread th([w, msg](){
        w.Send(msg);
    });
    th.detach();
    //th.join();
}
void case3_exception() {
    std::thread th([](){
        try {
            throw std::runtime_error("bad");
        }
        catch (std::exception &e) {
            std::cerr << "in thread, got exception: " << e.what() << std::endl;
        }
    });
    th.join();
}
void case4_threadId() {
    unsigned numCPUs = std::thread::hardware_concurrency();
    std::cout << "Launching " << numCPUs << " threads\n";

    // A mutex ensures orderly access to std::cout from multiple threads.
    //std::mutex iomutex;
    std::vector<std::thread> threads(numCPUs);
    for (unsigned i = 0; i < numCPUs; ++i) {
        //threads[i] = std::thread([&iomutex, i] {
        threads[i] = std::thread([i] {
            {
                // Use a lexical scope and lock_guard to safely lock the mutex only for
                // the duration of std::cout usage.
                //std::lock_guard<std::mutex> iolock(iomutex);
                std::cout << "Thread #" << i << ": on CPU " << sched_getcpu() << ", thread id: " << std::this_thread::get_id() << std::endl;
            }

            // Simulate important work done by the tread by sleeping for a bit...
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        });
    }

    for (auto& t : threads) {
        t.join();
    }
}
void case5_thread_affinity() {
    unsigned numCPUs = std::thread::hardware_concurrency();
    std::cout << "Launching " << numCPUs << " threads\n";

    // A mutex ensures orderly access to std::cout from multiple threads.
    std::mutex iomutex;
    std::vector<std::thread> threads(numCPUs);
    for (unsigned i = 0; i < numCPUs; ++i) {
        threads[i] = std::thread([&iomutex, i] {
            while(true) {
                {
                    // Use a lexical scope and lock_guard to safely lock the mutex only for the duration of std::cout usage.
                    std::lock_guard<std::mutex> iolock(iomutex);
                    std::cout << "Thread #" << i << ": on CPU " << sched_getcpu() << ", thread id: " << std::this_thread::get_id() << std::endl;
                }

                // Simulate important work done by the tread by sleeping for a bit...
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        });
        // Create a cpu_set_t object representing a set of CPUs. Clear it and mark only CPU i as set.
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        int rc = pthread_setaffinity_np(threads[i].native_handle(),sizeof(cpu_set_t), &cpuset);
        if (rc != 0) {
            std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
        }
    }

    for (auto& t : threads) {
        t.join();
    }
}
void case5_mutex() {
    // A mutex ensures orderly access to std::cout from multiple threads.
    int sharedVariable = 0;
    std::vector<std::thread> threads(2);
    threads[0] = std::thread([&sharedVariable] {
        std::ofstream outFile("file0.txt");

        while(true) {
            ++sharedVariable;
            outFile << "thread 0: on CPU " << sched_getcpu() << " set sharedVariable to " << sharedVariable << std::endl;
            // Simulate important work done by the tread by sleeping for a bit...
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });
    {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(0, &cpuset);
        int rc = pthread_setaffinity_np(threads[0].native_handle(),sizeof(cpu_set_t), &cpuset);
        if (rc != 0) {
            std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
        }
    }
    threads[1] = std::thread([&sharedVariable] {
        std::ofstream outFile("file1.txt");

        while(true) {
            ++sharedVariable;
            outFile << "thread 1: on CPU " << sched_getcpu() << " set sharedVariable to " << sharedVariable << std::endl;
            // Simulate important work done by the tread by sleeping for a bit...
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });
    {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(0, &cpuset);
        int rc = pthread_setaffinity_np(threads[0].native_handle(),sizeof(cpu_set_t), &cpuset);
        if (rc != 0) {
            std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
        }
    }


    for (auto& t : threads) {
        t.join();
    }
}
int main() {
    case1_create_thread();
    //case2_capture_by_ref();
    //case2_capture_by_value();
//    try{
//        //throw std::runtime_error("eee");
//        case3_exception();
//    }
//    catch (std::exception &e) {
//        std::cerr << "get exception: " << e.what() << std::endl;
//    }
    //case4_threadId();
    //case5_thread_affinity();
    //case6_mutex();
    return 0;
}
