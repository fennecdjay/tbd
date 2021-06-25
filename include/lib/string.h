#ifndef __STRING_H__
#define __STRING_H__

#include <stdbool.h>
#include <stddef.h>

#define STRING_CAPACITY 16

struct String
{
    size_t capacity;
    size_t length;
    union
    {
        char inlined[STRING_CAPACITY];
        char *data;
    } u;
};

void string_new(struct String *self, const size_t size, const char str[size]);
void string_from_cstr(struct String *self, const char *cstr);
void string_new_empty(struct String *self);
void string_destroy(struct String *self);

size_t string_length(const struct String *const self);
const char *string_to_cstr(const struct String *const self);

bool string_compare(const struct String *s1, const struct String *s2);
bool string_is_empty(const struct String *self);

#endif /* __STRING_H__ */
