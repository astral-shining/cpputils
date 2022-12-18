#pragma once
#ifndef NDEBUG
extern "C" int printf(const char*, ...);
#endif

template<typename... Ts>
inline void logDebug(const char* fmt, Ts... s) {
#ifndef NDEBUG
    printf(fmt, s...);
    printf("\n");
#endif
}
