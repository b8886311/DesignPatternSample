cmake_minimum_required(VERSION 3.20)
project(util)

set(CMAKE_CXX_STANDARD 14)

add_library(util STATIC logger.cpp)
target_include_directories(util PUBLIC
    ./../../include/util
)

