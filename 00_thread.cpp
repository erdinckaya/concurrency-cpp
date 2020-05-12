#include <iostream>
#include <thread>

void func() {}
void func1(int &x, int &y) {}
void func2(int x, int y) {}

struct Foo {
  void print_sum(int n1, int n2) {
    std::cout << n1 + n2 << '\n';
  }
  int data = 10;
};

struct Functor {
  int num{0};
  // This operator overloading enables calling
  // operator function () on objects of increment
  int operator()(int arr_num) const {
    return num + arr_num;
  }
};

int main() {
  // Basic void func
  std::thread t(func);

  // With parameter. you have to pass parameter with std::ref for reference passing.
  int x = 10;
  int y = 15;
  std::thread t1(func1, std::ref(x), std::ref(y));

  // With parameter.
  std::thread t2(func2, x, y);

  // std::bind
  // It has overhead so that it is not suggested.
  Foo foo;
  std::thread t3(std::bind(&Foo::print_sum, &foo, x, y));

  // Functor
  Functor functor;
  std::thread t4(functor, x);

  // IMO best and easiest one lambda
  std::thread t5([](){});


  // Waits the current thread to finish t.
  t.join();
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();

  return 0;
}