#pragma once

template<typename T>
class BasicIterator {
    T* m_ptr;
public:

    constexpr T& operator*() const { return *m_ptr; }
    constexpr T* operator->() const { return m_ptr; }

    constexpr BasicIterator(T* ptr) : m_ptr(ptr) {}
    
    constexpr BasicIterator& operator++() { m_ptr++; return *this; }
    constexpr BasicIterator operator++(int) { auto i {*this}; ++(*this); return i; }

    constexpr BasicIterator& operator--() { m_ptr--; return *this; }
    constexpr BasicIterator operator--(int) { auto i {*this}; ++(*this); return i; }

    constexpr bool operator==(const BasicIterator& i) const { return i.m_ptr == m_ptr; }
    constexpr bool operator!=(const BasicIterator& i) const { return i.m_ptr != m_ptr; }
};
