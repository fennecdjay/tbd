#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "source_error.h"
#include "lib/assertions.h"
#include "lex.h"
#include "lib/xmalloc.h"
#include "lib/error.h"
#include "lib/hashtable.h"
#include "lib/util.h"
#include "tokens.h"

struct FileCtx
{
    FILE *stream;
    const char *path;
    size_t size;
    char data[];
};

typedef enum TokenType TokenType;
decl_hashtable(TokenType);

enum LexStatus
{
    LEX_OK,
    LEX_ERR
};

struct Lexer
{
    struct FileCtx *file;
    Hashtable(TokenType) keywords;
    size_t index;
    size_t line;
    size_t col;
};

struct Lexer lexer;

static long
get_file_size(FILE *f)
{
    $assert_nonnull(f);

    if (fseek(f, 0, SEEK_END) == -1)
        return -1L;

    /* ftell(3) returns -1L on error, so no need to check for return value here as it propagates anyway */
    const long size = ftell(f);

    rewind(f);

    return size;
}

void
lexer_new(const char *const path)
{
    // TODO: Skip UTF-8 BOM

    $assert_nonnull(path);

    FILE *f = fopen(path, "r");
    if (f == NULL)
        log_fatal("Couldn't open input file '%s': %s.", path, strerror(errno));

    const long file_size = get_file_size(f);
    if (file_size == -1L)
        log_fatal("%s: %s", path, strerror(errno));

    const size_t buf_size = sizeof(struct FileCtx) + (sizeof(char) * (size_t)file_size);
    struct FileCtx *file = xmalloc(buf_size);
    lexer.file = file;

    lexer.file->stream = f;
    lexer.file->path = path;
    lexer.file->size = file_size;

    if (fread(lexer.file->data, file_size, 1, f) != 1)
        log_fatal("Couldn't read input file '%s': %s.", path, strerror(errno));

    lexer.index = 0;
    lexer.line = 1;
    lexer.col = 1;

    /* Skip past UTF-8 BOM */
    if (lexer.file->size >= 3 &&
        memcmp(lexer.file->data, "\xEF\xBB\xBF", 3) == 0)
    {
        lexer.index = 3;
    }

    static const struct
    {
        const char *key;
        const enum TokenType value;
    } keywords[] = {
        { "func", TOKEN_FUNC },
        { "return", TOKEN_RETURN }
    };

    hashtable_new(&lexer.keywords);

    for (size_t i = 0; i < lengthof(keywords); i++)
    {
        bool exists;
        hashtable_set(&lexer.keywords, &exists, keywords[i].key, keywords[i].value);
        $assert_false(exists);
    }
}

void
lexer_destroy(void)
{
    $assert_nonnull(lexer.file);

    hashtable_destroy(&lexer.keywords);
    fclose(lexer.file->stream);
    free(lexer.file);

    lexer.file = NULL;
}

static bool
at_eof(void)
{
    $assert_le(lexer.index, lexer.file->size);

    return lexer.index == lexer.file->size;
}

static char peek(void);

static bool
at_eol(void)
{
    return at_eof() || peek() == '\n';
}

static char
peek(void)
{
    $assert_false(at_eof());

    return lexer.file->data[lexer.index];
}

static char
consume(void)
{
    $assert_false(at_eof());

    lexer.col++;
    return lexer.file->data[lexer.index++];
}

static void
unconsume(void)
{
    lexer.col--;
    lexer.index--;
}

static bool
is_ident_start(const char c)
{
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c == '_');
}

static bool
is_ident_cont(const char c)
{
    return is_ident_start(c) || (c >= '0' && c <= '9');
}

static TokenType
scan_keyword_or_ident(void)
{
    const size_t start = lexer.index;

    while (!at_eof() && is_ident_cont(peek()))
        consume();

    const size_t end = lexer.index;

    const size_t size = end - start;
    char *key = xmalloc(size + 1);
    memcpy(key, &lexer.file->data[start], size);
    key[size] = '\0';

    bool exists;
    enum TokenType type;

    hashtable_get(&lexer.keywords, &exists, key, &type);

    free(key);

    return exists ? type : TOKEN_IDENT;
}

static enum LexStatus
scan_token(struct Tokens *const tokens)
{
    $assert_nonnull(tokens);

    const size_t start_col = lexer.col;
    const size_t start_index = lexer.index;
    const char c = consume();
    enum TokenType type;

    switch (c)
    {
        /* Single character tokens */
        case ';': type = TOKEN_SEMICOLON; break;
        case ',': type = TOKEN_COMMA;     break;
        case '{': type = TOKEN_LBRACE;    break;
        case '}': type = TOKEN_RBRACE;    break;
        case '+': type = TOKEN_ADD;       break;

        /* Single or double character tokens */
        case ':':
            if (peek() == ':')
            {
                type = TOKEN_COLONCOLON;
                consume();
            }
            else
            {
                type = TOKEN_COLON;
            }
            break;

        case '-':
            if (peek() == '>')
            {
                type = TOKEN_ARROW;
                consume();
            }
            else
            {
                type = TOKEN_MINUS;
            }
            break;

        /* Match insignificant whitespace */
        case '\t': /* FALLTHROUGH */
        case ' ':
            return LEX_OK;
    
        case '\n':
            lexer.line++;
            lexer.col = 1;
            return LEX_OK;
    
        default:
            if (is_ident_start(c))
            {
                unconsume(); /* Push back the stream so this character can be re-read */
                type = scan_keyword_or_ident();
            }
            else
            {
                type = TOKEN_UNKNOWN;
            }
    }

    if (type == TOKEN_UNKNOWN)
    {
        struct Loc loc = {
            .line = lexer.line,
            .start = start_col,
            .end = lexer.col
        };
        source_error_at(lexer.file->path,
                        lexer.file->size,
                        lexer.file->data,
                        tokens,
                        &loc,
                        "{bold}Unknown token '%c'.", c);
        return LEX_ERR;
    }

    const size_t size = lexer.index - start_index;
    token_push(tokens, type, lexer.line, start_col, lexer.col, size, &lexer.file->data[start_index]);

    return LEX_OK;
}

int
lex(struct Tokens *const tokens)
{
    $assert_nonnull(tokens);

    bool had_error = false;

    while (!at_eof())
    {
        if (scan_token(tokens) == LEX_ERR)
            had_error = true;
    }

    token_push(tokens, TOKEN_EOF, 0, 0, 0, 0, "");

    return !had_error ? EXIT_SUCCESS : EXIT_FAILURE;
}
