#pragma once
#include <chrono>

struct Timer {
    using clock = std::chrono::high_resolution_clock;

    clock::time_point start = clock::now();

    // ms since last lap; resets the clock
    long long lap() {
        auto now = clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
        start = now;
        return ms;
    }

    // ms since construction without resetting
    long long elapsed() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            clock::now() - start).count();
    }
};
