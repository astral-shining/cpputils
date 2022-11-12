#pragma once
#include <type_traits>
#include <tuple>

#include "types.hpp"

template<typename T>
struct is_tuple : std::false_type {};

template<typename... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type {};

template<typename T>
struct is_tuple<const T> : is_tuple<T> {};

template<typename T>
inline constexpr bool is_tuple_v = is_tuple<T>::value;

template<typename T>
struct to_tuplechar {};

template<typename... Ts>
struct to_tuplechar<std::tuple<Ts...>> : std::type_identity<std::tuple<char[sizeof(Ts)]...>> {};

template<typename T>
using to_tuplechar_t = typename to_tuplechar<T>::type;

template<std::size_t I, typename T>
struct tuple_element_noerror : std::type_identity<void> {};

template<std::size_t I, typename T, typename... Ts>
requires (I == 0)
struct tuple_element_noerror<I, std::tuple<T, Ts...>> : std::type_identity<T> {};

template<std::size_t I, typename T, typename... Ts>
struct tuple_element_noerror<I, std::tuple<T, Ts...>> : tuple_element_noerror<I-1, std::tuple<Ts...>> {};

template<std::size_t I, typename T>
using tuple_element_noerror_t = typename tuple_element_noerror<I, T>::type;

template<typename T, typename Tupl>
std::size_t tuple_offset() {
    Tupl* tuple {};
    return (std::size_t) &std::get<T>(*tuple);
}

template<std::size_t n, typename Tupl>
std::size_t tuple_offset() {
    Tupl* tuple {};
    return (std::size_t) &std::get<n>(*tuple);
}

// Unpack tuples from tuple 
template<typename T, typename Result = std::tuple<>>
struct unpack_tuples : std::type_identity<Result> {};

template<typename T, typename... Ts1, typename... Ts2>
struct unpack_tuples<std::tuple<std::tuple<Ts1...>, Ts2...>, T> : unpack_tuples<std::tuple<Ts1..., Ts2...>, T> {};

template<typename T, typename... Ts1, typename... Ts2>
requires (!is_tuple_v<T>)
struct unpack_tuples<std::tuple<T, Ts1...>, std::tuple<Ts2...>> : unpack_tuples<std::tuple<Ts1...>, std::tuple<Ts2..., T>> {};

template<typename T>
using unpack_tuples_t = typename unpack_tuples<T>::type;

