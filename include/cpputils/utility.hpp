#pragma once
#include "metafunctions.hpp"


constexpr bool isInCompileTime() {
    return __builtin_is_constant_evaluated();
}

template<typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;
};

template<typename T>
constexpr T&& forward(RemoveReference<T>& t) {
    return static_cast<T&&>(t);
}

template<typename T>
constexpr T&& forward(RemoveReference<T>&& t) {
    return static_cast<T&&>(t);
}

template<typename T>
constexpr auto&& move(T&& t) {
    return static_cast<RemoveReference<T>&&>(t);
}
