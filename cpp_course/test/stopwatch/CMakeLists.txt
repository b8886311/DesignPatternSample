cmake_minimum_required(VERSION 3.20)
project(test_stopwatch)

set(CMAKE_CXX_STANDARD 14)

add_executable(test_stopwatch test_stopwatch.cpp)
target_include_directories(test_stopwatch PUBLIC
    ./../../include/util
)

