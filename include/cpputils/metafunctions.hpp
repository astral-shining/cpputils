#pragma once
#include <type_traits>
#include <tuple>

#include "types.hpp"

template<typename T>
struct IsTupleImpl : std::false_type {};

template<typename... Ts>
struct IsTupleImpl<std::tuple<Ts...>> : std::true_type {};

template<typename T>
struct IsTupleImpl<const T> : IsTupleImpl<T> {};

template<typename T>
concept IsTuple = IsTupleImpl<T>::value;

template<std::size_t I, typename T>
struct TupleElementNoerrorImpl : std::type_identity<void> {};

template<std::size_t I, typename T, typename... Ts>
requires (I == 0)
struct TupleElementNoerrorImpl<I, std::tuple<T, Ts...>> : std::type_identity<T> {};

template<std::size_t I, typename T, typename... Ts>
struct TupleElementNoerrorImpl<I, std::tuple<T, Ts...>> : TupleElementNoerrorImpl<I-1, std::tuple<Ts...>> {};

template<std::size_t I, typename T>
using TupleElementNoerror = typename TupleElementNoerrorImpl<I, T>::type;

template<typename T, typename Tupl>
constexpr std::size_t tupleOffset() {
    Tupl* tuple {};
    return (std::size_t) &std::get<T>(*tuple);
}

template<std::size_t n, typename Tupl>
constexpr std::size_t tupleOffset() {
    Tupl* tuple {};
    return (std::size_t) &std::get<n>(*tuple);
}

// Unpack tuples from tuple 
template<typename T, typename Result = std::tuple<>>
struct UnpackTuplesImpl : std::type_identity<Result> {};

template<typename T, typename... Ts1, typename... Ts2>
struct UnpackTuplesImpl<std::tuple<std::tuple<Ts1...>, Ts2...>, T> : UnpackTuplesImpl<std::tuple<Ts1..., Ts2...>, T> {};

template<IsTuple T, typename... Ts1, typename... Ts2>
struct UnpackTuplesImpl<std::tuple<T, Ts1...>, std::tuple<Ts2...>> : UnpackTuplesImpl<std::tuple<Ts1...>, std::tuple<Ts2..., T>> {};

template<typename T>
using UnpackTuples = typename UnpackTuplesImpl<T>::type;

// Is From Template
template<typename T, template<typename...> typename Tmpl>
struct IsFromTemplateImpl : std::false_type {};

template<typename... Ts, template<typename...> typename Tmpl>
struct IsFromTemplateImpl<Tmpl<Ts...>, Tmpl> : std::true_type {};

template<typename T, template<typename...> typename Tmpl>
concept IsFromTemplate = IsFromTemplateImpl<T, Tmpl>::value;

// Type switcher
template<typename T1, typename T2>
struct Case {
    using type_case = T1;
    using type_eq = T2;
};

template<typename T, typename... Cs>
struct TypeSwitchImpl {};

template<typename T, typename C, typename... Cs>
struct TypeSwitchImpl<T, C, Cs...> : 
    std::conditional_t<std::is_same_v<T, typename C::type_case>, 
        std::type_identity<typename C::type_eq>, 
        TypeSwitchImpl<T, Cs...>
    > {};

template<typename T, IsFromTemplate<Case>... Cs>
using TypeSwitch = typename TypeSwitchImpl<T, Cs...>::type;