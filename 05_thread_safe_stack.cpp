#include <stack>
#include <mutex>
// Lets create a thread safe stack.

#define SECOND 0
#define THIRD 0

// So the main idea is we have to protect our data when other threads want to modify it.
// That is why we added the lock guard when data is pushed or popped.
// Lets analyze it. Look at the top function it returns the data so it will violate our data protection rule.
// Lets cover it by activating SECOND macro to 1
template<typename T>
struct Stack {
#if !THIRD
  T &top() const {
#if SECOND
    std::lock_guard<std::mutex> guard(m_mutex);
    // Now we are covering our data violation, but we have one more problem.
    // What if two threads wants to reach and one will get 0 as first element and second gets 0 as well.
    // Then two threads pop at the same time, so that we popped two elements and we processed these elements according to top
    // element which is zero and same element, but we should have processed these values respect to these order
    // 0th - 1th, then 2dh - 3rd. On the contrary we processed like this 0th - 1th, 0th - 2nd
    // Thus our design is bad we shouldn't use top function instead of we can return value by popping.
    // Please activate THIRD
#endif
    if (m_data.empty()) {
      throw std::runtime_error("Stack is empty!");
    }
    return m_data.top();
  }
#endif
#if THIRD
  // We have to overload our fucntion. In order to prevent unnecessary data copy we are taking T ref parameter.
  void pop(T &v) {
    std::lock_guard<std::mutex> guard(m_mutex);
    if (m_data.empty()) {
      throw std::runtime_error("Stack is empty!");
    }
    v = m_data.top();
    m_data.pop();
  }
#endif
  void pop() {
    std::lock_guard<std::mutex> guard(m_mutex);
    if (m_data.empty()) {
      throw std::runtime_error("Stack is empty!");
    }
    m_data.pop();
  }

  size_t size() const
  {
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_data.size();
  }

  bool empty() const
  {
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_data.empty();
  }

  void push(T &&t) {
    std::lock_guard<std::mutex> guard(m_mutex);
    m_data.emplace(std::forward(t));
  }

 private:
  std::stack<T> m_data;
  // Note that we have to define our mutex as mutable to break the constness of const functions.
  mutable std::mutex m_mutex;
};
