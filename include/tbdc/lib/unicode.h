#ifndef __UNICODE_H__
#define __UNICODE_H__

#include <stddef.h>
#include <stdint.h>

typedef uint_fast32_t unicode_char;

#define UNICODE_REPLACEMENT ((unicode_char)0xFFFD)

size_t utf8_decode(const char *restrict buf, unicode_char *restrict c, int *restrict e);

#endif /* __UNICODE_H__ */
