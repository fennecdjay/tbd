#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>

#include "lib/assertions.h"
#include "lib/xmalloc.h"

#define Vec(T) struct Vec_##T

#define decl_vec(T)     \
Vec(T)                  \
{                       \
    size_t capacity;    \
    size_t size;        \
    T *data;            \
}

#define new_vec(self) do {  \
    (self)->capacity = 0;   \
    (self)->size = 0;       \
    (self)->data = NULL;    \
} while (0)

#define vec_destroy(self) do { \
    free((self)->data);        \
} while (0)

#define vec_push(self, item) do {                                   \
    $assert_le((self)->size, (self)->capacity);                     \
    if ((self)->size == (self)->capacity)                           \
    {                                                               \
        const size_t capacity = (self)->capacity + 16;              \
        (self)->data = xrealloc((self)->data,                       \
                             capacity * sizeof(*((self)->data)));   \
        (self)->capacity = capacity;                                \
    }                                                               \
    (self)->data[(self)->size++] = item;                            \
} while (0)

#define vec_size(self)  \
    ((self)->size)

#define vec_get(self, i)    \
    ((self)->data[(i)])

// TODO: Improve this API
#define vec_foreach(self)   \
    for (size_t i = 0; i < vec_size((self)); i++)

#endif /* __VECTOR_H__ */
