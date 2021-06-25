#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stddef.h>
#include <stdlib.h>

#include "lib/assertions.h"
#include "lib/string.h"

#define HASHTABLE_MAX_LOAD (0.75)

#define HashtableEntry(T) struct HashtableEntry_##T
#define Hashtable(T) struct Hashtable_##T

struct HashtableEntry
{
    struct String key;
};

struct Hashtable
{
    size_t capacity;
    size_t size;
    struct HashtableEntry *data;
};

#define decl_hashtable(T)       \
HashtableEntry(T)               \
{                               \
    struct String key;          \
    T value;                    \
};                              \
Hashtable(T)                    \
{                               \
    size_t capacity;            \
    size_t size;                \
    HashtableEntry(T) *data;    \
}

#define hashtable_new(self) do {    \
    (self)->capacity = 0;           \
    (self)->size = 0;               \
    (self)->data = NULL;            \
} while (0)

#define hashtable_destroy(self) do {    \
    $assert_nonnull((self));            \
    free((self)->data);                 \
} while (0)

#define hashtable_set(self, exists, cstr, _value) do {                              \
    $assert_nonnull((self));                                                        \
    $assert_nonnull((exists));                                                      \
    $assert_nonnull((cstr));                                                        \
                                                                                    \
    if ((self)->size + 1 >= (self)->capacity * HASHTABLE_MAX_LOAD)                  \
    {                                                                               \
        const size_t capacity = hashtable_grow_capacity((self)->capacity);          \
        hashtable_adjust_capacity((struct Hashtable *)(self),                       \
                                  capacity,                                         \
                                  sizeof(*((self)->data)));                         \
    }                                                                               \
                                                                                    \
    struct String key;                                                              \
    string_from_cstr(&key, (cstr));                                                 \
                                                                                    \
    const size_t index = hashtable_find_entry((const char *)(self)->data,           \
                                              (self)->capacity,                     \
                                              &key,                                 \
                                              sizeof(*((self)->data)));             \
                                                                                    \
    *(exists) = !string_is_empty(&(self)->data[index].key);                         \
    if (*(exists) == false)                                                         \
    {                                                                               \
        (self)->size++;                                                             \
    }                                                                               \
                                                                                    \
    (self)->data[index].key = key;                                                  \
    (self)->data[index].value = (_value);                                           \
} while (0)

#define hashtable_get(self, exists, cstr, _value) do {                              \
    $assert_nonnull((self));                                                        \
    $assert_nonnull((exists));                                                      \
    $assert_nonnull((cstr));                                                        \
    $assert_nonnull((_value));                                                      \
                                                                                    \
    *(exists) = false;                                                              \
    if ((self)->size != 0)                                                          \
    {                                                                               \
        struct String _key;                                                         \
        string_from_cstr(&_key, (cstr));                                            \
                                                                                    \
        const size_t index = hashtable_find_entry((const char *)(self)->data,       \
                                                  (self)->capacity,                 \
                                                  &_key,                            \
                                                  sizeof(*((self)->data)));         \
        if (!string_is_empty(&(self)->data[index].key))                             \
        {                                                                           \
            *(exists) = true;                                                       \
            *(_value) = (self)->data[index].value;                                  \
        }                                                                           \
        string_destroy(&_key);                                                      \
    }                                                                               \
} while (0)

size_t hashtable_grow_capacity(const size_t capacity);
void hashtable_adjust_capacity(struct Hashtable *self,
                               const size_t new_capacity,
                               const size_t entry_size);
size_t hashtable_find_entry(const char *entries,
                            const size_t capacity,
                            const struct String *key,
                            const size_t entry_size);

// new_hashtable, destroy_hashtable, hashtable_set, hashtable_get

#endif /* __HASHTABLE_H__ */
