#ifndef __TOKENS_H__
#define __TOKENS_H__

#include "lib/vector.h"
#include "lib/string.h"

struct Loc
{
    size_t line;
    size_t start;
    size_t end;
};

enum TokenType
{
    TOKEN_UNKNOWN,

    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_MINUS,
    TOKEN_ARROW,
    TOKEN_ADD,

    TOKEN_PLUS,

    TOKEN_COLONCOLON,

    TOKEN_FUNC,
    TOKEN_RETURN,

    TOKEN_IDENT,
    TOKEN_INT,

    TOKEN_EOF
};

struct Token
{
    struct Loc loc;
    enum TokenType type;
    struct String lexeme;
};

typedef struct Token Token;
decl_vec(Token);

struct Tokens
{
    // const char *const file;
    Vec(Token) tokens;
};

void tokens_new(struct Tokens *self);
void tokens_destroy(struct Tokens *self);
void token_push(struct Tokens *self,
                const enum TokenType type,
                const size_t line,
                const size_t start_col,
                const size_t end_col,
                const size_t size,
                const char lexeme[size]);
void tokens_dump(const struct Tokens *self);

#endif /* __TOKENS_H__ */
