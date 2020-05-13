#include <thread>
#include <iostream>
#include <queue>
#include <condition_variable>

template<typename T>
struct BlockingQueue {
  void push(const T &t) {
    std::lock_guard<std::mutex> guard(m_mutex);
    m_data.push(t);
    m_cond.notify_all();
  }

  void pop(T &t) {
    std::unique_lock<std::mutex> locker(m_mutex);
    m_cond.wait(locker, [this]() { return !m_data.empty(); });
    t = m_data.front();
    m_data.pop();
  }

  T front() const {
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_data.front();
  }

 private:
  mutable std::mutex m_mutex;
  std::condition_variable m_cond;
  std::queue<T> m_data;
};

int main() {
  BlockingQueue<int> queue;
  std::thread producer([&queue]() {
    for (int i = 50; i > 0; --i) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      queue.push(i);
    }
  });

  std::vector<std::thread> consumers;
  constexpr int size = 5;
  for (int i = 0; i < size; ++i) {
    std::thread consumer([&queue, i]() {
      int data = 0;
      while (data != 1) {
        queue.pop(data);
        std::cout << "Consumer " << i << " data received " << data << std::endl;
      }
    });
    consumers.push_back(std::move(consumer));
  }

  producer.join();
  for (std::thread &th : consumers) {
    if (th.joinable())
      th.join();
  }

  return 0;
}