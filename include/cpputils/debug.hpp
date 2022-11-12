#pragma once
#ifndef NDEBUG
#include <cstdio>
#endif

template<typename... Ts>
inline void logDebug(const char* fmt, Ts... s) {
#ifndef NDEBUG
    std::printf(fmt, s...);
    std::printf("\n");
#endif
}
