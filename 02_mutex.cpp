#include <thread>
#include <iostream>


int main() {
  // Use mutex to remove data race.
  // Encapsulate the critical section with m.lock() and m.unlock();
  std::mutex m;

  int x = 0;
  constexpr int size = 100000;
  std::thread t([&x, &m]() {
    for (int i = 0; i < size; ++i) {
      // we have to cover both section
      m.lock();
      x++;
      m.unlock();
    }
  });

  for (int i = 0; i < size; ++i) {
    // we have to cover both section
    m.lock();
    x++;
    m.unlock();
  }

  // Wait for thread end.
  t.join();
  std::cout << "Result x: " << x << std::endl;
  std::cout << "Expected x: " << size + size << std::endl;
  return 0;
}