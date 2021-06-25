#ifndef __XMALLOC_H__
#define __XMALLOC_H__

#include <stdlib.h>

void *xmalloc(size_t size);
void *xrealloc(void *old_ptr, size_t new_size);

#endif /* __XMALLOC_H__ */
