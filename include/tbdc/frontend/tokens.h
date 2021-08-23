#ifndef __TOKENS_H__
#define __TOKENS_H__

#include <stddef.h>

#include <tbdc/core/compile.h>
#include <tbdc/lib/string.h>

#define TOKEN_ARRAY_SIZE (0x100000)

enum token_type
{
    TOKEN_UNKNOWN,

    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_MINUS,

    TOKEN_PLUS,

    TOKEN_FUNC,
    TOKEN_RETURN,

    TOKEN_IDENT,
    TOKEN_INT,

    TOKEN_EOF
};

struct loc
{
    size_t start;
    size_t end;
    size_t line;
};

struct token
{
    struct loc loc;
    enum token_type type;
    struct string_view lexeme;
};

struct tokens
{
    size_t size;
    struct token token[TOKEN_ARRAY_SIZE];
};

struct tokens *tokens_create(void);
void tokens_destroy(struct tokens **tokens);
void token_push(struct compile *ctx, const struct token *token);
void tokens_dump(const struct tokens *tokens);

#endif /* __TOKENS_H__ */
