cmake_minimum_required(VERSION 3.15)
project(concurrency-cpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(thread 00_thread.cpp)
add_executable(data_race 01_data_race.cpp)
add_executable(mutex 02_mutex.cpp)
add_executable(dead_lock 03_dead_lock.cpp)
add_executable(lock_guard 04_lock_guard.cpp)
add_executable(thread_safe_stack 05_thread_safe_stack.cpp)
add_executable(condition_variable 06_condition_variable.cpp)
add_executable(blocking_queue 07_blocking_queue.cpp)
add_executable(async 08_async.cpp)
add_executable(future_promise 09_future_promise.cpp)
