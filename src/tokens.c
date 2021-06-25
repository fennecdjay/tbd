#include <stdlib.h>

#include "tokens.h"
#include "lib/assertions.h"
#include "lib/string.h"

const char *stringify_type(const enum TokenType type);

void
tokens_new(struct Tokens *self)
{
    $assert_nonnull(self);

    new_vec(&self->tokens);
}

void
tokens_destroy(struct Tokens *self)
{
    $assert_nonnull(self);

    vec_destroy(&self->tokens);
}

void
token_push(struct Tokens *self,
           const enum TokenType type,
           const size_t line,
           const size_t start_col,
           const size_t end_col,
           const size_t size,
           const char lexeme[size])
{
    $assert_nonnull(self);
    $assert_nonnull(lexeme);

    struct Token token = {
        .loc = {
            .line = line,
            .start = start_col,
            .end = end_col
        },
        .type = type,
    };

    if (type != TOKEN_EOF)
        string_new(&token.lexeme, size, lexeme);
    else
        string_new_empty(&token.lexeme);

    vec_push(&self->tokens, token);
}

const char *
stringify_type(const enum TokenType type)
{
    #define ADD_CASE(type) case type: return #type;

    switch (type)
    {
        ADD_CASE(TOKEN_COLON)
        ADD_CASE(TOKEN_COMMA)
        ADD_CASE(TOKEN_SEMICOLON)
        ADD_CASE(TOKEN_LBRACE)
        ADD_CASE(TOKEN_RBRACE)
        ADD_CASE(TOKEN_MINUS)
        ADD_CASE(TOKEN_ARROW)
        ADD_CASE(TOKEN_ADD)

        ADD_CASE(TOKEN_PLUS)

        ADD_CASE(TOKEN_COLONCOLON)

        ADD_CASE(TOKEN_FUNC)
        ADD_CASE(TOKEN_RETURN)

        ADD_CASE(TOKEN_IDENT)
        ADD_CASE(TOKEN_INT)

        ADD_CASE(TOKEN_EOF)

        default:
            $assert_true(!"Received unknown token type!");
            abort();
    }

    #undef ADD_CASE
}

void
tokens_dump(const struct Tokens *self)
{
    $assert_nonnull(self);

    vec_foreach(&self->tokens)
    {
        const struct Token *token = &vec_get(&self->tokens, i);
        DEBUG("Type: %s, Line: %zu, Start: %zu, End: %zu, Lexeme: '%s'",
                stringify_type(token->type),
                token->loc.line,
                token->loc.start,
                token->loc.end,
                string_to_cstr(&token->lexeme));
    }
}
