cmake_minimum_required(VERSION 3.15)
project(advanced-templates)

set(CMAKE_CXX_STANDARD 14)

add_executable(thread 00_thread.cpp)
add_executable(data_race 01_data_race.cpp)
