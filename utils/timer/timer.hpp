#pragma once
#include <chrono>

namespace utils {

    // What this does:
    // A strict timer to ensure we never Exceed the 5-minute (300 seconds) limit.
    class Timer {
    private:
        std::chrono::time_point<std::chrono::steady_clock> start_time;
        double time_limit_seconds;

    public:
        // Start the clock!
        Timer(double limit) : time_limit_seconds(limit) {
            start_time = std::chrono::steady_clock::now();
        }

        // Returns true if we are out of time
        bool is_time_up() const {
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = now - start_time;
            return elapsed.count() >= time_limit_seconds;
        }

        double get_elapsed_seconds() const {
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = now - start_time;
            return elapsed.count();
        }
    };

} // namespace utils
