#pragma once
#include "types.hpp"
#include "metafunctions.hpp"
#include "utility.hpp"

template<u32 i, typename... Ts>
struct TupleImpl {};

template<u32 I, typename T, typename... Ts>
struct TupleImpl<I, T, Ts...> : TupleImpl<I+1, Ts...> {
    T first;

    using Parent = TupleImpl<I+1, Ts...>;

    constexpr TupleImpl(T&& e, Ts&&... ee) : Parent(forward<Ts>(ee)...), first(move(e)) {}

    template<typename T2>
    constexpr T2& get() {
        if constexpr (IsSame<T, T2>) {
            return first;
        } else {
            return Parent::template get<T2>();
        } 
    }

    template<u32 i>
    constexpr auto& get() {
        if constexpr (i == I) {
            return first;
        } else {
            return Parent::template get<i>();
        } 
    }
};

template<typename... Ts>
using Tuple = TupleImpl<0, Ts...>;
