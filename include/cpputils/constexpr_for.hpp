#pragma once

#define constexpr_for(ini, cmp, inc, ...) [&] () constexpr {    \
    const auto f = [&] <ini> (const auto& fref) constexpr {     \
        if constexpr (cmp) {                                    \
            __VA_ARGS__;                                        \
            return fref.template operator()<inc>(fref);         \
        }                                                       \
    };                                                          \
    return f.template operator()(f);                            \
}();

#define AND ,

