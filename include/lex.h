#ifndef __LEX_H__
#define __LEX_H__

#include "tokens.h"

void lexer_new(const char *const file);
int lex(struct Tokens *tokens);
void lexer_destroy(void);

#endif /* __LEX_H__ */
