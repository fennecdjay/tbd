#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "lib/hashtable.h"
#include "lib/xmalloc.h"

static uint32_t
hash_string(const struct String *string)
{
    uint32_t hash = 2166136261U;

    const char *cstr = string_to_cstr(string);
    const size_t length = string_length(string);

    for (size_t i = 0; i < length; i++)
    {
        hash ^= cstr[i];
        hash *= 16777619;
    }

    return hash;
}

size_t
hashtable_grow_capacity(const size_t capacity)
{
    return ((capacity) < 8 ? 8 : (capacity) * 2);
}

void
hashtable_adjust_capacity(struct Hashtable *self, const size_t new_capacity, const size_t entry_size)
{
    $assert_nonnull(self);
    $assert_ne(new_capacity, 0);
    $assert_gt(entry_size, sizeof(struct HashtableEntry));

    char *entries = xmalloc(entry_size * new_capacity);

    for (size_t i = 0; i < new_capacity; i++)
    {
        struct HashtableEntry *entry = (struct HashtableEntry *)(entries + (i * entry_size));
        string_new_empty(&entry->key);
    }

    self->size = 0;
    for (size_t i = 0; i < self->capacity; i++)
    {
        const struct HashtableEntry *src = (struct HashtableEntry *)(self->data + (i * entry_size));
        if (string_is_empty(&src->key))
            continue;

        const size_t index = hashtable_find_entry(entries, new_capacity, &src->key, entry_size);
        struct HashtableEntry *dst = (struct HashtableEntry *)(entries + (index * entry_size));
        memcpy(dst, src, entry_size);
        self->size++;
    }

    free(self->data);

    self->data = (struct HashtableEntry *)entries;
    self->capacity = new_capacity;
}

size_t
hashtable_find_entry(const char *entries,
                     const size_t capacity,
                     const struct String *key,
                     const size_t entry_size)
{
    $assert_nonnull(entries);
    $assert_ne(capacity, 0);
    $assert_nonnull(key);
    $assert_gt(entry_size, sizeof(struct HashtableEntry));

    size_t index = hash_string(key) % capacity;

    while (true)
    {
        struct HashtableEntry *entry = (struct HashtableEntry *)(entries + (index * entry_size));

        if (string_compare(&entry->key, key) || string_is_empty(&entry->key))
            return index;

        index = (index + 1) % capacity;
    }
}
