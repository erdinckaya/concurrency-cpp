#include <thread>
#include <iostream>

int main() {
  // By using lock_guard we have bound the lock and unlock mechanism to lock_guard object's
  // life time. With the help of the RAII technique in every situation critical section will be unlocked.
  std::mutex m;

  int x = 0;
  constexpr int size = 100000;
  std::thread t([&x, &m]() {
    using namespace std::chrono_literals;
    for (int i = 0; i < size; ++i) {
      std::lock_guard<std::mutex> guard(m);
      x++;
      std::this_thread::sleep_for(1ns);
    }
  });

  using namespace std::chrono_literals;
  for (int i = 0; i < size; ++i) {
    std::lock_guard<std::mutex> guard(m);
    x++;
    std::this_thread::sleep_for(1ns);
  }

  // Wait for thread end.
  t.join();
  std::cout << "Result x: " << x << std::endl;
  std::cout << "Expected x: " << size + size << std::endl;
  return 0;
}