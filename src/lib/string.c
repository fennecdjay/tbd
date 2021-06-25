#include <stdlib.h>
#include <string.h>

#include "lib/assertions.h"
#include "lib/string.h"
#include "lib/util.h"
#include "lib/xmalloc.h"

void
string_new(struct String *self, const size_t length, const char str[length])
{
    $assert_nonnull(self);
    $assert_nonnull(str);

    const size_t capacity = length + (STRING_CAPACITY - (length % STRING_CAPACITY));
    self->capacity = capacity;
    self->length = length;

    $assert_lt(length, capacity);

    char *data;

    if (length < STRING_CAPACITY)
    {
        data = self->u.inlined;
    }
    else
    {
        self->u.data = xmalloc(capacity);
        data = self->u.data;
    }

    memcpy(data, str, length);
    data[length] = '\0';
}

void
string_destroy(struct String *self)
{
    $assert_nonnull(self);

    if (self->capacity != STRING_CAPACITY)
        free(self->u.data);
}

void
string_from_cstr(struct String *self, const char *cstr)
{
    $assert_nonnull(self);
    $assert_nonnull(cstr);

    string_new(self, strlen(cstr), cstr);
}

void
string_new_empty(struct String *string)
{
    $assert_nonnull(string);

    string->capacity = 0;
    string->length = 0;
}

size_t
string_length(const struct String *const string)
{
    $assert_nonnull(string);

    return string->length;
}

const char *
string_to_cstr(const struct String *self)
{
    $assert_nonnull(self);

    return self->capacity == STRING_CAPACITY
        ? self->u.inlined
        : self->u.data;
}

bool
string_compare(const struct String *s1, const struct String *s2)
{
    $assert_nonnull(s1);
    $assert_nonnull(s2);

    if (string_length(s1) != string_length(s2))
        return false;

    const char *cstr1 = string_to_cstr(s1);
    const char *cstr2 = string_to_cstr(s2);

    if (cstr1 == cstr2 && cstr1 != NULL)
        return true;

    return !strcmp(cstr1, cstr2);
}

bool
string_is_empty(const struct String *self)
{
    $assert_nonnull(self);

    return self->capacity == 0;
}
