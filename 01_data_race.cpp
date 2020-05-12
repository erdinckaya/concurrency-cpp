#include <thread>
#include <iostream>
// Since both main and child thread want to reach same value at the same
// time this creates the data race and the result will be undefined behaviour.
int main() {
  int x = 0;
  constexpr int size = 100000;
  std::thread t([&x]() {
    for (int i = 0; i < size; ++i) {
      x++;
    }
  });

  for (int i = 0; i < size; ++i) {
    x++;
  }

  // Wait for thread end.
  t.join();
  std::cout << "Result x: " << x << std::endl;
  std::cout << "Expected x: " << size + size << std::endl;
  return 0;
}