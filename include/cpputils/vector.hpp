#pragma once
#include <initializer_list>

#include "memory.hpp"
#include "types.hpp"
#include "iterator.hpp"

template<typename T, bool swap_remove = false>
class Vector {
    u32 m_size {};
    Allocator<T> m_data;

public:

    inline Vector(std::initializer_list<T> l) {
        m_data.malloc(l.size());
        for (auto& el : l) {
            push_back(el);
        }
    }

    inline Vector() {}

    inline Vector(const Vector& other) {
        *this = other;
    }

    inline Vector& operator=(Vector&& other) {
        m_size = other.m_size;
        m_data = move(other.m_data);
        return *this;
    }

    inline Vector& operator=(Vector& other) {
        clear();
        //realloc();
        for (const auto& el : other) {
            push_back(el);
        }
        return *this;
    }

    inline BasicIterator<T> begin() {
        return m_data.data();
    }

    inline BasicIterator<T> end() {
        return m_data.data() + m_size;
    }

    inline T& back() {
        return *(--this->end());
    }

    inline const T* data() const {
        return m_data;
    }

    inline u32 capacity() const {
        return m_data.capacity();
    }

    inline u32 size() const {
        return m_size;
    }

    inline T& operator[] (u32 i) {
        return m_data[i];
    }

    inline void reserve(u32 capacity) {
        if constexpr (IsCopyable<T>) {
            m_data.realloc(capacity);
        } else {
            Allocator<T> new_data {capacity};
            for (u32 i{}; i<m_size; i++) {
                new_data.construct(i, move(m_data[i]));
                (m_data[i]).~T();
            }
            m_data = move(new_data);
        }
    }

    inline void shrink_to_fit() {
        reserve(m_size);
    }

    inline void grow() {
        reserve(capacity() ? capacity()*2 : 1);
    }

    inline T& push_back(T&& value) {
        return emplace_back(forward<T>(value));
    }

    inline T& push_back(const T& value) {
        return emplace_back(value);
    }

    inline void pop_back() {
        (*this)[--m_size].~T();
    }

    template<typename... Ts>
    inline T& emplace_back(Ts&&... args) {
        if (m_size == m_data.capacity()) {
            grow();
        }
        return m_data.construct(m_size++, forward<Ts>(args)...);
    }

    inline Vector copy() {
        Vector copy {*this};
        return copy;
    }

    inline void erase(BasicIterator<T> it) {
        if constexpr (swap_remove) {
            *it = move(*--this->end());
        } else {
            for (;it!=this->end()-1; it++) {
                *it = move(*(it+1));
            }
        }
        pop_back();
    }

    inline void clear() {
        for (u32 i{m_size}; i>0; i--) {
            pop_back();
        }
        m_data.free();
    }

    inline ~Vector() {
        clear();
    }
};
