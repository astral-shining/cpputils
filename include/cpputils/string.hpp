#pragma once
#include "types.hpp"
#include "iterator.hpp"
#include "memory.hpp"

template<u32 S>
struct FixedString {
    FixedAllocator<char, S> m_data;

    constexpr FixedString() {}

    constexpr FixedString(const char* str) {
        m_data.memcpy(str, S);
    }

    constexpr operator const char*() const {
        return m_data.data();
    }

    constexpr const char* data() const {
        return m_data.data();
    }

    template<u32 S2>
    constexpr auto operator+(const char (&str)[S2]) const {
        FixedString<S+S2-1> s;
        s.m_data.memcpy(m_data.data(), S-1);
        s.m_data.memcpy(S-1, str, S2);
        return s;
    }
};

template<u32 size> FixedString(const char(&)[size]) -> FixedString<size>;

struct String {
    Allocator<char> m_data;

    inline String() {
        
    }

    inline String(const char* str) : m_data(cstrlen(str)+1) {
        m_data.memcpy(str, m_data.capacity());
    }

    inline String& operator=(const char* str) {
        u32 size = cstrlen(str);
        m_data.realloc(size+1);
        m_data.memcpy(str, size+1);
        return *this;
    }

    template<typename T>
    inline String operator+(const T& t) const {
        String result {*this};
        result += t;
        return result;
    }

//    constexpr bool operator==(const String& s);
//    constexpr bool operator==(const char* str);
    
    inline String& operator+=(const char* str) {
        u32 size = cstrlen(str);
        u32 current_size = m_data.capacity()-1;
        m_data.realloc(current_size+size+1);
        m_data.memcpy(current_size, str, size+1);
        return *this;
    }

    template<IsIn<
        int,
        unsigned int,
        long int,
        unsigned long int,
        long long int,
        unsigned long long int,

        float,
        double
    > T>
    inline String& operator+=(T n) {
        char buff[32];
        const char* fmt = TypeSwitch<T,
            Case<int, IntegralConstant<FixedString("%d")>>,
            Case<unsigned int, IntegralConstant<FixedString("%u")>>,
            Case<long int, IntegralConstant<FixedString("%ld")>>,
            Case<unsigned long int, IntegralConstant<FixedString("%lu")>>,
            Case<long long int, IntegralConstant<FixedString("%lld")>>,
            Case<unsigned long long int, IntegralConstant<FixedString("%llu")>>,
            Case<float, IntegralConstant<FixedString("%f")>>,
            Case<double, IntegralConstant<FixedString("%lf")>>
        >::value;
        snprintf(buff, 32, fmt, n);
        (*this) += buff;
        return *this;
    }

    inline String& operator+=(const String& s) {
        u32 current_size = m_data.capacity();
        m_data.realloc(s.size()+1);
        m_data.memcpy(current_size, s.data(), s.size()+1);
        return *this;
    }

    inline operator const char*() const {
        return data();
    }

    inline const char* data() const {
        return m_data.data();
    }

    inline const char* sdata() const { 
        auto data = m_data.data();
        return data ? data : "";
    }

    inline void clear() {
        m_data.free();
    }

    inline char operator[] (u32 pos) const {
        return *(m_data.data()+pos);
    }

    inline u32 size() const {
        return m_data.capacity()-1;
    }

    inline BasicIterator<char> begin() {
        return m_data.data();
    }

    inline BasicIterator<char> end() {
        return m_data.data()+m_data.capacity();
    }
};

inline String operator""_str(const char* str, u64) {
    return str;
}
