#pragma once

// Overload
template<typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

#define validexpr(...) overloaded { \
        [] <typename = void> requires requires() { \
            __VA_ARGS__; \
        } () consteval { \
            return true; \
        }, \
        [] <typename = void> () consteval { \
            return false; \
        } \
    }() 


#define evalif_validexpr(EXPR, ...) if constexpr (validexpr(EXPR)) { \
    EXPR; \
} else { \
   __VA_ARGS__; \
}
