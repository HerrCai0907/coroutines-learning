#include "api.hpp"
#include "async_task.hpp"
#include <string>

AsyncRuntime::Awaitable<int> asyncStartFoo(int v) {
  Api::log("asyncStartFoo " + std::to_string(v) + "-1");
  co_await Api::sleepSync(100);
  Api::log("asyncStartFoo " + std::to_string(v) + "-2");
  co_await Api::sleepSync(200);
  Api::log("asyncStartFoo " + std::to_string(v) + "-3");
  co_return v;
}

AsyncRuntime::Awaitable<void> asyncStart() {
  Api::log("asyncStart 1");
  Api::log(std::to_string(co_await asyncStartFoo(1)));
  Api::log("asyncStart 2");
  Api::log(std::to_string(co_await asyncStartFoo(2)));
  Api::log("asyncStart 3");
  Api::log(std::to_string(co_await asyncStartFoo(3)));
  Api::log("asyncStart 4");
}

#ifdef TARGET_WASM

__attribute__((export_name("_start"))) void _start() { asyncStart(); }

#else // TARGET_WASM

int main() {
  asyncStart();
  for (;;) {
  }
}

#endif // TARGET_WASM
