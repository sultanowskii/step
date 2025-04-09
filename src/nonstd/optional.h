#pragma once

#include <stdbool.h>
#include <stddef.h>

#define DECLARE_OPTIONAL_STRUCT(type, name) \
    typedef struct {                        \
        type val;                           \
        bool exists;                        \
    } optional_##name;

#define DECLARE_OPTIONAL(type, name)                       \
    DECLARE_OPTIONAL_STRUCT(type, name)                    \
                                                           \
    static inline optional_##name name##_some(type val) {  \
        return (optional_##name){                          \
            .val = val,                                    \
            .exists = true,                                \
        };                                                 \
    }                                                      \
                                                           \
    static inline optional_##name name##_none(void) {      \
        return (optional_##name){                          \
            .exists = false,                               \
        };                                                 \
    }                                                      \
                                                           \
    static inline bool name##_is_some(optional_##name o) { \
        return o.exists;                                   \
    }                                                      \
                                                           \
    static inline bool name##_is_none(optional_##name o) { \
        return !o.exists;                                  \
    }                                                      \
                                                           \
    static inline type name##_get_val(optional_##name o) { \
        return o.val;                                      \
    }
