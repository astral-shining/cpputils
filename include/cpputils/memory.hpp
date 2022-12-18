#pragma once
#include "types.hpp"
#include "utility.hpp"


extern "C" {
    void* memcpy(void *, const void *, u64);
    void* malloc(u64);
    void* realloc(void *, u64);
    int snprintf(char *, u64, const char *, ...);
    int free(void *);
};

inline void* operator new  (u64 count, void* ptr ) {
    return ptr;
}
inline void* operator new[](u64 count, void* ptr ) {
    return ptr;
}

constexpr u32 cstrlen(const char* str) {
    u32 len {};
    while (str[len++]);
    return len-1;
}

template<typename T>
concept IsCopyable = __is_trivially_copyable(T);

template<typename T>
struct Allocator {
    T* m_data {};
    u32 m_capacity {};
    
    inline Allocator() {}

    inline Allocator(u32 capacity) {
        malloc(capacity);
    }

    inline Allocator(const Allocator& a) requires IsCopyable<T> {
        malloc(a.capacity());
        memcpy(a.data(), a.capacity());
    }

    inline Allocator(Allocator&& a) {
        m_data = a.m_data;
        m_capacity = a.m_capacity;
        a.m_data = 0;
        a.m_capacity = 0;
    }

    inline Allocator& operator=(const Allocator& a) requires IsCopyable<T> {
        free();
        malloc(a.capacity());
        memcpy(a.data(), a.capacity());
        return *this;
    }

    inline Allocator& operator=(Allocator&& a) {
        free();
        m_data = a.m_data;
        m_capacity = a.m_capacity;
        a.m_data = 0;
        a.m_capacity = 0;
        return *this;
    }

    inline T& operator[] (u32 i) {
        return *(m_data+i);
    }

    inline void malloc(u32 capacity) {
        m_capacity = capacity;
        m_data = (T*) ::malloc(capacity*sizeof(T));
    }

    inline void realloc(u32 new_cap) requires IsCopyable<T> {
        m_capacity = new_cap;
        m_data = (T*) ::realloc(m_data, new_cap*sizeof(T));
    }

    inline void free() {
        m_capacity = 0;
        m_data = 0;
        ::free(m_data);
    }

    template<typename... Ts>
    inline T& construct(u32 position, Ts&&... args) {
        return *(new (m_data+position) T (forward<Ts>(args)...));
    }

    inline void memcpy(const T* from, u32 size) requires IsCopyable<T> {
        ::memcpy((void*)m_data, (const void*)from, size);
    }

    inline void memcpy(u32 offset, const T* from, u32 size) requires IsCopyable<T> {
        ::memcpy((void*)((char*)m_data+offset), from, size);
    }

    inline T* data() const {
        return m_data;
    }

    inline u32 capacity() const {
        return m_capacity;
    }

    inline ~Allocator() {
        free();
    }
};

template<typename T, u32 S>
struct FixedAllocator {
    char m_data [S*sizeof(T)] {};

    constexpr FixedAllocator() {}

    template<u32 newS>
    constexpr FixedAllocator realloc() const requires IsCopyable<T> {
        FixedAllocator<T, newS> c;
        c.memcpy(m_data, newS < S ? newS : S);
        return c;
    }

    constexpr void memcpy(u32 offset, const char* from, u32 size) {

    }

    constexpr void memcpy(const char* from, u32 size) {
        for (u32 i{};i<size;i++) ((char*)m_data)[i] = from[i];
    }

    constexpr const char* data() const {
        return m_data;
    }
};
