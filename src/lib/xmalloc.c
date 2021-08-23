#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static void oom(void)
{
    fprintf(stderr, "Exiting due to OOM condition!\n");
    exit(EXIT_FAILURE);
}

void *xmalloc(size_t size)
{
    if (size == 0)
        size = 1;
    void *ptr = malloc(size);
    if (ptr == NULL)
        oom();
    return ptr;
}

void *xrealloc(void *old_ptr, size_t new_size)
{
    if (new_size == 0)
        new_size = 1;
    void *new_ptr = realloc(old_ptr, new_size);
    if (new_ptr == NULL)
        oom();
    return new_ptr;
}

void xfree(void **ptr)
{
    if (ptr != NULL)
    {
        free(*ptr);
        *ptr = NULL;
    }
}
