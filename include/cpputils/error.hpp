#pragma once
#include "string.hpp"

template<FixedString a, typename T = const char*>
class Exception {
    T m_error;

public:
    Exception(T&& arg = "") : m_error(move(arg)) {}

    T& error() {
        return m_error;
    }
};


extern "C" int printf(const char*, ...);
extern "C" void exit(int);

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

inline void abort(const char* msg, int exit_code = 1) {
    printf("%sERROR: %s %s", COLOR_RED, msg, COLOR_RESET);
    exit(exit_code);
}
