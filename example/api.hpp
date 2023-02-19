#pragma once

#include "async_task.hpp"
#include <cstdint>

namespace Api {

AsyncRuntime::Awaitable<void> sleepSync(uint64_t timeout);
void log(std::string const &str);

} // namespace Api
