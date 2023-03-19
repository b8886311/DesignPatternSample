cmake_minimum_required(VERSION 3.20)
project(cppcourse VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

add_subdirectory(src/util)
add_subdirectory(src/worker)
add_subdirectory(test/stopwatch)
add_subdirectory(test/worker)
add_subdirectory(test/lambda)
add_subdirectory(test/thread)
add_subdirectory(test/asyncio)
add_subdirectory(test/smartptr)