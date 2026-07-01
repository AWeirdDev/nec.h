/*

    nec.h
    Basic necessities I need lol

 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifndef NEC_H_
#define NEC_H_
#endif // NEC_H_

#ifndef NEC_EXPORT
#define NEC_EXPORT
#endif // NEC_EXPORT

#ifndef panic
#include <stdio.h>

#define panic(message)                                                         \
    do {                                                                       \
        fprintf(stderr, "program panicked: %s\n", message);                    \
        exit(1);                                                               \
    } while (0)

#endif // panic

//
// ========================= DYNAMIC ARRAYS =========================
//

/// The essential fields for a dynamic array.
#define DA_FIELDS(T)                                                           \
    size_t count;                                                              \
    size_t capacity;                                                           \
    T *items;

/// The essential fields for a dynamic array, with custom `items`. It must be a
/// pointer.
#define DA_FIELDS2(ITEMS)                                                      \
    size_t count;                                                              \
    size_t capacity;                                                           \
    ITEMS;

/// Defines a dynamic array struct.
#define DYNAMIC_ARRAY(T, Name)                                                 \
    typedef struct {                                                           \
        DA_FIELDS(T)                                                           \
    }(Name);

/// Defines a dynamic array struct.
#define DYNAMIC_ARRAY2(ITEMS, Name)                                            \
    typedef struct {                                                           \
        DA_FIELDS2(ITEMS)                                                      \
    }(Name);

/// Defines an annonymous dynamic array struct.
#define DYNAMIC_ARRAY_ANNON(T)                                                 \
    struct {                                                                   \
        DA_FIELDS(T)                                                           \
    }

/// Defines a dynamic array struct.
#define DYNAMIC_ARRAY2_ANNON(ITEMS)                                            \
    struct {                                                                   \
        DA_FIELDS2(ITEMS)                                                      \
    }

/// Reserves space for the dynamic array.
#define da_reserve(da, expected_capacity)                                      \
    do {                                                                       \
        if ((expected_capacity) > (da)->capacity) {                            \
            if ((da)->capacity == 0) {                                         \
                (da)->capacity = 128;                                          \
            }                                                                  \
            while ((expected_capacity) > (da)->capacity) {                     \
                (da)->capacity *= 2;                                           \
            }                                                                  \
                                                                               \
            void *ptr =                                                        \
                realloc((da)->items, sizeof(*(da)->items) * (da)->capacity);   \
            if (ptr == NULL)                                                   \
                panic("could not reallocate dynamic array");                   \
                                                                               \
            (da)->items = ptr;                                                 \
        }                                                                      \
    } while (0)

/// Pushes an item to the dynamic array.
#define da_push(da, item)                                                      \
    do {                                                                       \
        da_reserve((da), (da)->count + 1);                                     \
        (da)->items[(da)->count++] = item;                                     \
    } while (0)

/// Gets the length of the dynamic array.
#define da_len(da) ((da)->count)

/// Gets an item of index `idx` from the dynamic array.
#define da_get(da, idx) ((da)->items[idx])

/// Pops the last item from the dynamic array.
#define da_pop(da, item)                                                       \
    do {                                                                       \
        if ((da)->count == 0)                                                  \
            panic("pop from empty list");                                      \
        *(item) = ((da)->items[--(da)->count]);                                \
    } while (0)

/// Frees the dynamic array.
#define da_free(da) (free((da).items))

//
// ========================= STRINGS =========================
//

/// A string view.
DYNAMIC_ARRAY(char, StringView)

/// Checks if the string view ends with the specified character.
NEC_EXPORT bool sv_ends_with_char(StringView *view, char c);

/// Pushes a character to the string view.
NEC_EXPORT void sv_push_char(StringView *view, char c);

/// Pushes a C string to the string view.
NEC_EXPORT void sv_push_cstr(StringView *view, const char *src);

/// Temporarily turns this string view layout into that of a C string.
NEC_EXPORT const char *sv_temp_cstr(StringView *view);

/// Pops the last character from the string view.
NEC_EXPORT char sv_pop(StringView *view);

/// De-C-string this string view, turning the layout back to the original.
NEC_EXPORT void sv_de_cstr(StringView *view);

/// Creates a string view from a C string.
NEC_EXPORT StringView sv_from_cstr(const char *src);

/// Frees the string view.
NEC_EXPORT void sv_free(StringView view);

//
// ========================= QUEUES =========================
//

#define QUEUE_FIELDS(T)                                                        \
    DA_FIELDS(T)                                                               \
    size_t front;

/// Defines a queue struct.
#define QUEUE(T, Name)                                                         \
    typedef struct {                                                           \
        QUEUE_FIELDS(T)                                                        \
    }(Name);

/// Defines an annonymous queue struct.
#define QUEUE_ANNON(T)                                                         \
    struct {                                                                   \
        QUEUE_FIELDS(T)                                                        \
    }

#define queue_next_back_pos(queue)                                             \
    (((queue)->front + (queue)->count) % (queue)->capacity)

#define queue_push_back(queue, item)                                           \
    do {                                                                       \
        da_reserve(queue, (queue)->count + 1);                                 \
        (queue)->items[queue_next_back_pos((queue))] = (item);                 \
        (queue)->count++;                                                      \
    } while (0)

#define queue_next_front_pos(queue)                                            \
    ((queue)->front + (queue)->capacity - 1) % (queue)->capacity

#define queue_push_front(queue, item)                                          \
    do {                                                                       \
        da_reserve(queue, (queue)->count + 1);                                 \
        (queue)->front = queue_next_front_pos((queue));                        \
        (queue)->items[(queue)->front] = (item);                               \
        (queue)->count++;                                                      \
    } while (0)

#define queue_pop_back(queue, item)                                            \
    do {                                                                       \
        if ((queue)->count == 0)                                               \
            panic("pop back from empty queue");                                \
                                                                               \
        *(item) = ((queue)->items[((queue)->front + --(queue)->count) %        \
                                  (queue)->capacity]);                         \
    } while (0)

#define queue_pop_front(queue, item)                                           \
    do {                                                                       \
        if ((queue)->count == 0)                                               \
            panic("pop front from empty queue");                               \
                                                                               \
        *(item) = (queue)->items[(queue)->front];                              \
        (queue)->front = ((queue)->front + 1) % (queue)->capacity;             \
        (queue)->count--;                                                      \
    } while (0)

//
// ========================= IMPLEMENTATION =========================
//

#ifdef NEC_IMPLEMENTATION

NEC_EXPORT bool sv_ends_with_char(StringView *view, char c) {
    if (view->count == 0)
        return false;

    return view->items[view->count - 1] == c;
}

NEC_EXPORT void sv_push_char(StringView *view, char c) { da_push(view, c); }

NEC_EXPORT char sv_pop(StringView *view) {
    char x;
    da_pop(view, &x);
    return x;
}

NEC_EXPORT void sv_push_cstr(StringView *view, const char *src) {
    size_t len = strlen(src);
    da_reserve(view, da_len(view) + len + 1);
    strcpy((view->items) + (view->count), src);
    view->count += len;
}

NEC_EXPORT StringView sv_from_cstr(const char *src) {
    StringView view = {0};
    sv_push_cstr(&view, src);

    return view;
}

NEC_EXPORT const char *sv_temp_cstr(StringView *view) {
    if (sv_ends_with_char(view, '\0'))
        return view->items;

    sv_push_char(view, '\0');
    return view->items;
}

NEC_EXPORT void sv_de_cstr(StringView *view) {
    if (sv_ends_with_char(view, '\0'))
        sv_pop(view);
}

NEC_EXPORT void sv_free(StringView view) { da_free(view); }

#endif // NEC_IMPLEMENTATION
