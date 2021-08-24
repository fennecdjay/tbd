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

    /* The size needs to have at most 3 bytes of padding in order for the UTF-8 decoder to work correctly.
       The padding is silently "forgotten" by the rest of the codebase but as it is still present
       in the allocation, the UTF-8 decoder is able to read it without invoking UB.*/
    const long padded_size = size + 3;

    struct source *source = xmalloc(sizeof(*source) + (sizeof(*source->data) * padded_size));
    source->size = size;

    if (fread(source->data, size, 1, fp) != 1)
        log_fatal("Failed to read file '%s': %s.", file, strerror(errno));

    /* Zero the padding bytes */
    memset(&source->data[size], 0, padded_size - size);

    fclose(fp);

    source->file = file;

    return source;
}

void source_destroy(struct source **source)
{
    assert_nonnull(source);

    xfree((void **)source);
}
