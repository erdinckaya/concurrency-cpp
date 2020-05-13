#include <thread>
#include <iostream>
#include <future>
#include <vector>

long accumulate(int start, int end, const int *data) {
  long res = 0;
  for (int i = start; i < end; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    res += data[i];
  }
  return res;
}

int main() {
  // Lets say we need ton of data in an array and we wanted to get sum of these data.
  // But the problem is while we are doing this we are getting timeout because data is so big to handle
  // O(N) time, so lets solve this problem by using async.

  // Create the data.
  constexpr size_t hugeDataSize = 1000;
  int data[hugeDataSize];
  for (int &i : data) {
    i = 1;
  }

  // We are gonna use the async. Basically std::async is creates and thread and handles the data sharing processes.
  // The return value of the async is future, and when we call future.get  it blocks our program till async returns a value
  // Lets see with an example.

  // Lets start with one accumulator
  auto start = std::chrono::high_resolution_clock::now();

  constexpr int accumulatorSize = 5;
  constexpr int chunkSize = hugeDataSize / accumulatorSize;
  std::vector<std::future<long>> futures;
  for (int i = 0; i < accumulatorSize; ++i) {
    // Arrange the chunks to be calculated.
    int begin = i * chunkSize;
    int end = (i + 1) * chunkSize;
    if (hugeDataSize - end < chunkSize) {
      end = hugeDataSize;
    }
    std::cout << "Starting with range [" << begin << " " << end << "]" << std::endl;
    // Start async thread.
    auto future = std::async(accumulate, begin, end, data);
    // store it inside of vector to fetch them later.
    futures.push_back(std::move(future));
  }

  // Wait for the futures.
  long result = 0;
  for (auto &future : futures) {
    result += future.get();
  }
  // Result is ready
  // You can try new values with changing `accumulatorSize` variable.
  std::cout << "Result is " << result << std::endl;
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  std::cout << "Elapsed time: " << elapsed.count() << " s\n";

  return 0;
}