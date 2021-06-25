#ifndef __SOURCE_ERROR_H__
#define __SOURCE_ERROR_H__

#include <stddef.h>

#include "tokens.h"
#include "lib/error.h"

void source_error_at(const char *file,
                     const size_t size,
                     const char data[size],
                     const struct Tokens *tokens,
                     const struct Loc *loc,
                     const char *fmt, ...);

#endif /* __SOURCE_ERROR_H__ */
