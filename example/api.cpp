#include "api.hpp"
#include "async_task.hpp"
#include <chrono>
#include <iostream>
#include <memory>

using SetTimeoutCallback = void (*)(uint64_t);
extern "C" uint64_t startTimeout(uint64_t timeout, SetTimeoutCallback callback,
                                 uint64_t context, uint32_t cyclical);
extern "C" void logInfo(const char *ptr, uint32_t len);

#ifndef TARGET_WASM

#include <thread>
uint64_t startTimeout(uint64_t timeout, SetTimeoutCallback callback,
                      uint64_t context, uint32_t cyclical) {
  std::thread([=] {
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
    callback(context);
  }).detach();
  return 0;
}
void logInfo(const char *ptr, uint32_t len) {
  std::cout << std::string{ptr, len} << "\n";
}

#endif // TARGET_WASM

namespace Api {

AsyncRuntime::Awaitable<void> sleepSync(uint64_t timeout) {
  auto result = std::make_shared<uint64_t>();
  return AsyncRuntime::Awaitable<void>{
      [timeout, result](std::coroutine_handle<> h) {
        startTimeout(
            timeout,
            [](uint64_t context) {
              std::coroutine_handle<>::from_address(
                  reinterpret_cast<void *>(context))
                  .resume();
            },
            reinterpret_cast<uint64_t>(h.address()), 0);
      },
      []() {}};
}

void log(std::string const &str) { logInfo(str.c_str(), str.length()); }

} // namespace Api
