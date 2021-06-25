#include <errno.h>
#include <string.h>

#include "lib/assertions.h"
#include "lib/xmalloc.h"

void *
xmalloc(size_t size)
{
    if (size == 0)
        size = 1;
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        log_fatal("malloc experienced an out-of-memory condition.");
    }

    $assert_nonnull(ptr);
    return ptr;
}

void *
xrealloc(void *old_ptr, size_t new_size)
{
    if (new_size == 0)
        new_size = 1;
    void *new_ptr = realloc(old_ptr, new_size);
    if (new_ptr == NULL)
    {
        log_fatal("realloc experienced an out-of-memory condition.");
    }

    $assert_nonnull(new_ptr);
    return new_ptr;
}
