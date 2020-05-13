#include <iostream>
#include <thread>
#include <mutex>
#include <deque>

#define FIRST   0
#define SECOND  0

std::deque<int> q;
std::mutex mu;
std::condition_variable cond;

void func1() {
  int count = 10;
  while (count > 0) {
    std::unique_lock<std::mutex> locker(mu);
    q.push_front(count);
    locker.unlock();
#if FIRST
    cond.notify_one();
#endif
    std::this_thread::sleep_for(std::chrono::seconds(1));
    count--;
  }
}

void func2() {
  int data = 0;
  while (data != 1) {
    std::unique_lock<std::mutex> locker(mu);
#if FIRST
    // Every thing seems ok but there is a minor problem can cause huge problems.
    // In this wait this thread can awake itself and this will cause different problems.
    // So that we have to check the condition in different way. Please activate SECOND
#if SECOND
    // See our program executing very well and ends gracefully.
    cond.wait(locker, []() { return !q.empty(); });
#else
    cond.wait(locker);
#endif
    data = q.back();
    q.pop_back();
    std::cout << "func2 got value from func1 " << data << std::endl;
#else
    // In this situation we are wasting our cpu cycles we have to come up with more elegant way.
    // We can use conditional_variables update FIRST with 1
    if (!q.empty()) {
      data = q.back();
      q.pop_back();
      locker.unlock();
      std::cout << "func2 got value from func1 " << data << std::endl;
    } else {
      locker.unlock();
    }
#endif
  }
}

int main() {
  std::thread t1(func1);
  std::thread t2(func2);

  t1.join();
  t2.join();
  return 0;
}