#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <tbdc/frontend/source.h>
#include <tbdc/lib/assert.h>
#include <tbdc/lib/log.h>
#include <tbdc/lib/xmalloc.h>

static long filesize(FILE *fp)
{
    assert_nonnull(fp);

    if (fseek(fp, 0, SEEK_END) == -1)
        return -1L;

    const long size = ftell(fp);
    if (size == -1L)
        return -1L;

    if (fseek(fp, 0, SEEK_SET) == -1)
        return -1L;

    assert_eq(ferror(fp), 0);

    return size;
}

struct source *source_create(const char *file)
{
    assert_nonnull(file);

    FILE *fp = fopen(file, "rb");
    if (fp == NULL)
        log_fatal("Failed to open file '%s': %s.", file, strerror(errno));

    const long size = filesize(fp);
    if (size == -1L)
        log_fatal("Failed to get size of file '%s': %s.", file, strerror(errno));

    const long aligned_size = size + 3;

    struct source *source = xmalloc(sizeof(*source) + (sizeof(*source->data) * aligned_size));
    source->size = size;

    if (fread(source->data, size, 1, fp) != 1)
        log_fatal("Failed to read file '%s': %s.", file, strerror(errno));

    memset(&source->data[size], 0, aligned_size - size);

    fclose(fp);

    source->file = file;

    return source;
}

void source_destroy(struct source **source)
{
    assert_nonnull(source);

    xfree((void **)source);
}
