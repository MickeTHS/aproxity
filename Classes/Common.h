#pragma once

#include <stdio.h>
#include <chrono>

static inline std::uint64_t milliseconds_now() {
    return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}
