#ifndef __XMALLOC_H__
#define __XMALLOC_H__

#include <stddef.h>

void *xmalloc(size_t size);
void *xrealloc(void *old_ptr, size_t new_size);
void xfree(void **ptr);

#endif /* __XMALLOC_H__ */
