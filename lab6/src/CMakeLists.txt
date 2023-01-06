cmake_minimum_required(VERSION 3.16)
project(oslab68)

set(CMAKE_CXX_STANDARD 14)

add_executable(terminal main.cpp)
add_executable(child child.cpp)

target_link_libraries(terminal zmq)
target_link_libraries(child zmq)