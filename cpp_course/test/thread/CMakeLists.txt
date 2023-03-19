cmake_minimum_required(VERSION 3.20)
project(test_thread_affinity)

set(CMAKE_CXX_STANDARD 14)

add_executable(test_thread_affinity test_thread_affinity.cpp)
#target_include_directories(test_worker PUBLIC
#    ./../../include/worker
#)
#target_link_libraries(test_thread_affinity worker util)

add_executable(test_car car.cpp test_car.cpp)

add_executable(test_thread_bad_example test_thread_bad_example.cpp)
