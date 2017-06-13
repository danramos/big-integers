cmake_minimum_required(VERSION 2.8)

project(big-integers)

include_directories("./include")
set (CMAKE_CXX_FLAGS "--std=c++11 -Wall -Werror")

add_executable(big-integers
               big-integers.cpp)

