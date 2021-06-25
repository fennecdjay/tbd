#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "source_error.h"
#include "lib/error.h"
#include "lib/term.h"

struct TableChars
{
    const char *column;
    const char *row;
    const char *left_corner;
};

static struct TableChars ascii_table = {
    .column = "|",
    .row = "-",
    .left_corner = "`"
};

static struct TableChars utf8_table = {
    .column = "│",
    .row = "─",
    .left_corner = "└"
};

static struct TableChars *
get_table_chars(void)
{
    if (term_supports_utf8())
        return &utf8_table;
    return &ascii_table;
}

static const char *
column_char(void)
{
    return get_table_chars()->column;
}

static const char *
left_corner_char(void)
{
    return get_table_chars()->left_corner;
}

static const char *
find_start_of_line(const size_t size, const char data[size], const size_t line)
{
    $assert_ge(line, 1);

    const char *curr = data;
    size_t curr_line = 1;
    size_t remaining = size;

    while (curr_line != line)
    {
        const char *next = memchr(curr, '\n', remaining);
        if (next == NULL)
            return curr;

        const size_t delta = next - curr + 1;
        curr += delta;
        remaining -= delta;

        curr_line++;
    }

    return curr;
}

void
source_error_at(const char *file,
                const size_t size,
                const char data[size],
                const struct Tokens *tokens,
                const struct Loc *loc,
                const char *fmt, ...)
{
    $assert_nonnull(file);
    $assert_nonnull(data);
    $assert_nonnull(tokens);
    $assert_nonnull(loc);
    $assert_nonnull(fmt);

    log_error("In %s:%zu:%zu:", file, loc->line, loc->start);

    fputc('\n', stderr);

    const char *start_of_line = find_start_of_line(size, data, loc->line);

    const char *end_of_line = memchr(start_of_line, '\n', (data + size) - start_of_line);
    if (end_of_line == NULL)
        end_of_line = data + size;

    log_padln("{bold}%s:{reset}\n", file);
    log_padln("%zu %s %.*s", loc->line, column_char(), (int)(end_of_line - start_of_line), start_of_line);

    const int line_len = snprintf(NULL, 0, "%zu", loc->line);

    log_pad("%*.s %s ", line_len, "", column_char());
    log_noneln("%*.s{magenta}%s{reset}", loc->start - 1, "", column_char());

    log_pad("%*.s %s ", line_len, "", column_char());
    log_none("%*.s{magenta}%s{reset} ", loc->start - 1, "", left_corner_char());

    va_list args;
    va_start(args, fmt);
    vlog(LOG_NONE, fmt, args, true);
    va_end(args);

    fputc('\n', stderr);
}

// TODO: Syntax highlighting on error messages, function names, levenshtein distance, handle binary data, invalid unicode (overlong, etc reporting)
