#ifndef __SOURCE_H__
#define __SOURCE_H__

#include <stddef.h>

struct source
{
    const char *file;
    size_t size;
    char data[];
};

struct source *source_create(const char *file);
void source_destroy(struct source **source);

#endif /* __SOURCE_H__ */
