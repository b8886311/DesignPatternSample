cmake_minimum_required(VERSION 3.16)
project(simplest_server)

set(CMAKE_CXX_STANDARD 14)
find_package(Boost COMPONENTS filesystem REQUIRED)

add_executable(client2 terminal_client/client2.cpp terminal_server/session.cpp terminal_server/connector.cpp terminal_server/frame.cpp)
target_include_directories(client2 PUBLIC ./terminal_server)

add_executable(terminal_server terminal_server/server.cpp terminal_server/session.cpp terminal_server/frame.cpp)
target_include_directories(terminal_server PUBLIC ./terminal_server)
target_link_libraries(terminal_server ${Boost_FILESYSTEM_LIBRARY})
