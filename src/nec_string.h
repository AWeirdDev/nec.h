/*
    nec_string.h -- v0.0.1 -- tiny string library

    USAGE
    -----
    To add the implementations, first define `NEC_STRING_IMPLEMENTATION` and
    then the inclusion.

        #define NEC_STRING_IMPLEMENTATION
        #include "nec_string.h"

    ALLOCATION
    ----------
    You can setup your custom allocator defining `NEC_REALLOC`:

        #define NEC_REALLOC(ptr, size) my_realloc(ptr, size)

    This is useful if you're not with malloc().
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/////////////////////////
//                     //
//       HEADER        //
//                     //
/////////////////////////
#ifndef NEC_STRING_H_
#define NEC_STRING_H_

#ifndef NECSDEF
#define NECSDEF extern
#endif  // NECSDEF

typedef struct String {
    size_t len, capacity;
    char* items;
} String;

typedef struct StringView {
    size_t len;
    char* items;
} StringView;

NECSDEF String nec_string_new();
NECSDEF bool nec_string_push(String* str, const char item);
NECSDEF bool nec_string_push_cstr(String* str, const char* cstr);

NECSDEF String nec_string_from_cstr(const char* cstr);
NECSDEF StringView nec_string_slice(const String* str, size_t start,
                                    size_t end);
NECSDEF StringView nec_string_as_slice(const String* str);

#ifndef NEC_SV_FMT
#define NEC_SV_FMT "%.*s"
#endif  // NEC_SV_FMT

#ifndef NEC_SV_ARG
#define NEC_SV_ARG(sv) (int)(sv)->len, (sv)->items
#endif

#endif  // NEC_STRING_H_

/////////////////////////
//                     //
//        IMPL         //
//                     //
/////////////////////////

#ifdef NEC_STRING_IMPLEMENTATION

#ifndef NEC_REALLOC
#include <stdlib.h>
#define NEC_REALLOC(ptr, size) realloc(ptr, size)
#endif  // NEC_REALLOC

static bool nec_string__reserve(String* str, size_t expected_capacity) {
    if (str->capacity == 0) str->capacity = 128;

    while (expected_capacity > str->capacity) {
        str->capacity *= 2;
    }

    void* ptr = NEC_REALLOC(str->items, sizeof(char) * str->capacity);
    if (ptr == NULL) return false;

    str->items = (char*)ptr;
    return true;
}

static StringView nec_string__view(const String* str, size_t start,
                                   size_t end) {
    if (start < str->len && end < str->len && end >= start) {
        return (StringView){.len = end - start + 1,
                            .items = str->items + start};
    } else {
        return (StringView){.len = 0, .items = str->items};
    }
}

NECSDEF String nec_string_new() { return (String){0}; }

NECSDEF bool nec_string_push(String* str, const char item) {
    if (!nec_string__reserve(str, str->len + 1)) return false;

    str->items[str->len++] = item;
    return true;
}

NECSDEF bool nec_string_push_cstr(String* str, const char* cstr) {
    size_t len = strlen(cstr);

    // include null terminator for later copying
    if (!nec_string__reserve(str, str->len + len + 1)) return false;

    if (strcpy(str->items + str->len, cstr) == NULL) return false;

    str->len += len;
    return true;
}

NECSDEF String nec_string_from_cstr(const char* cstr) {
    String s = {0};

    // doesn't include the null terminator
    size_t len = strlen(cstr);

    if (!nec_string__reserve(&s, len + 1)) return (String){0};

    // man 3 strcpy:
    // The stpcpy() and strcpy() functions copy the string src to dst (including
    // the terminating '\0' character.)
    strcpy(s.items, cstr);
    s.len = len;  // treat it as not used

    return s;
}

NECSDEF StringView nec_string_slice(const String* str, size_t start,
                                    size_t end) {
    return nec_string__view(str, start, end);
}

NECSDEF StringView nec_string_as_slice(const String* str) {
    return (StringView){.len = str->len, .items = str->items};
}

#endif  // NEC_STRING_IMPLEMENTATION
