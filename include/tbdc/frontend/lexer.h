#ifndef __LEXER_H__
#define __LEXER_H__

#include <stddef.h>

#include <tbdc/core/compile.h>

struct lexer
{
    size_t index;
    size_t line;
    size_t col;
};

struct lexer *lexer_create(void);
void lexer_destroy(struct lexer **lexer);
int lex(struct compile *ctx);

#endif /* __LEXER_H__ */
