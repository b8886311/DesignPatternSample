cmake_minimum_required(VERSION 3.20)
project(test_worker)

set(CMAKE_CXX_STANDARD 14)

add_executable(test_worker test_worker.cpp)
target_include_directories(test_worker PUBLIC
    ./../../include/worker
)
target_link_libraries(test_worker worker util)

