#ifndef MOCK_ARDUINO_H
#define MOCK_ARDUINO_H

#include <cstdint>
#include <cstring>
#include <chrono>
#include <thread>
#include <string>

#define F(x) x

typedef std::string String;

inline uint32_t millis() {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

inline void delay(uint32_t ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

inline void yield() {
    std::this_thread::yield();
}

#endif // MOCK_ARDUINO_H
