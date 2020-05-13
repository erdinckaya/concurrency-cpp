#include <thread>
#include <iostream>
#include <future>
#include <vector>

struct NetworkData {
  explicit NetworkData(const int *data = nullptr, int start = -1, int end = -1) : data(data), start(start), end(end) {}

  const int *data{nullptr};
  int start{-1};
  int end{-1};
};

long accumulate(std::future<NetworkData> &future) {
  // Thread will be waited until data comes.
  auto networkData = future.get();
  long res = 0;
  for (int i = networkData.start; i < networkData.end; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    res += networkData.data[i];
  }
  return res;
}

int main() {
  // Now we can have a situation like we need data first then we want to continue our calculation in other thread.
  // and get the result later. Lets use our last example.

  // Lets assume that our date creation is taking a a lot of time or we are fetching our date from some network.
  // Therefore we want to prepare our calculations and wait for the data.

  // Create data.
  constexpr size_t hugeDataSize = 1000;
  int data[hugeDataSize];
  for (int &i : data) {
    i = 1;
  }

  auto start = std::chrono::high_resolution_clock::now();

  constexpr int accumulatorSize = 5;
  constexpr int chunkSize = hugeDataSize / accumulatorSize;
  std::vector<std::future<long>> futures;
  std::vector<std::promise<NetworkData>> promises;
  std::vector<NetworkData> vNetworkData;
  for (int i = 0; i < accumulatorSize; ++i) {

    int begin = i * chunkSize;
    int end = (i + 1) * chunkSize;
    if (hugeDataSize - end < chunkSize) {
      end = hugeDataSize;
    }
    // Make a promise
    std::promise<NetworkData> promise;
    auto dataFuture = promise.get_future();
    // Store data to use later.
    vNetworkData.emplace_back(data, begin, end);
    std::cout << "Starting with range [" << begin << " " << end << "]" << std::endl;
    // Start async thread.
    auto future = std::async(accumulate, std::ref(dataFuture));
    // store it inside of vector to fetch them later.
    futures.push_back(std::move(future));
    promises.push_back(std::move(promise));
  }


  // Wait a bit.
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  // Feed the other threads with data and let the calculations begin.
  for (int i = 0; i < vNetworkData.size(); ++i) {
    promises[i].set_value(vNetworkData[i]);
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