cmake_minimum_required(VERSION 3.10)

project(OS_Lab1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(parent parent.cpp)
add_executable(child child.cpp)