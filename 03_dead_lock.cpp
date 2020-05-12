#include <thread>
#include <iostream>


int main() {
  // Use mutex to remove data race.
  // Encapsulate the critical section with m.lock() and m.unlock();
  // Please notice the order, since it is important.
  std::mutex m;

  int x = 0;
  constexpr int size = 100000;
  std::thread t([&x, &m]() {
    using namespace std::chrono_literals;
    for (int i = 0; i < size; ++i) {
      // we have to cover both section
      m.lock();
      x++;
      std::this_thread::sleep_for(1ns);
      m.unlock();
    }
  });

  using namespace std::chrono_literals;
  for (int i = 0; i < size; ++i) {
    // we have to cover both section
    // Look at the order we have to cover it  with same order.!
    m.unlock();
    x++;
    std::this_thread::sleep_for(1ns);
    m.lock();
  }

  // Wait for thread end.
  t.join();
  std::cout << "Result x: " << x << std::endl;
  std::cout << "Expected x: " << size + size << std::endl;
  return 0;
}