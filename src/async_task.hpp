#pragma once

#include <coroutine>
#include <functional>
#include <type_traits>

namespace AsyncRuntime {

template <class T> class Awaitable {
public:
  class promise_type_void {
  public:
    Awaitable<T> get_return_object() {
      return Awaitable<T>{[this](std::coroutine_handle<> h) { h_ = h; }, [] {}};
    }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept {
      if (h_) {
        h_.resume();
      }
      return {};
    }
    void unhandled_exception() {}
    void return_void() {}

  private:
    std::coroutine_handle<> h_;
  };

  class promise_type_non_void {
  public:
    Awaitable<T> get_return_object() {
      return Awaitable<T>{[this](std::coroutine_handle<> h) { h_ = h; },
                          [this]() -> T { return value_; }};
    }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept {
      if (h_) {
        h_.resume();
      }
      return {};
    }
    void unhandled_exception() {}
    void return_value(T value) { value_ = value; }

  private:
    std::coroutine_handle<> h_;
    T value_;
  };

  using promise_type = std::conditional_t<std::is_void_v<T>, promise_type_void,
                                          promise_type_non_void>;

  explicit Awaitable(std::function<void(std::coroutine_handle<>)> suspendFunc,
                     std::function<T(void)> returnFunc)
      : suspendFunc_(std::move(suspendFunc)),
        returnFunc_(std::move(returnFunc)) {}
  bool await_ready() { return false; }
  void await_suspend(std::coroutine_handle<> h) { suspendFunc_(h); }
  T await_resume() { return returnFunc_(); }

private:
  std::function<void(std::coroutine_handle<>)> suspendFunc_;
  std::function<T(void)> returnFunc_;
};

} // namespace AsyncRuntime