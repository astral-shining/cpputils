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

