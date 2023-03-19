cmake_minimum_required(VERSION 3.20)
project(worker)

set(CMAKE_CXX_STANDARD 14)

add_library(worker STATIC worker.cpp)
target_include_directories(worker PUBLIC
    ./../../include/worker
    ./../../include/util
)

