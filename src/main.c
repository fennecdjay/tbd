#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/assertions.h"
#include "lex.h"
#include "lib/error.h"

static int
compile(const size_t count, const char *infile[count])
{
    $assert_ne(count, 0);
    $assert_nonnull(infile);

    for (size_t i = 0; i < count; i++) // TODO: Spawn min(count, nproc) threads to parallelise compilation
    {
        lexer_new(infile[i]);

        struct Tokens tokens;
        tokens_new(&tokens);

        if (lex(&tokens) == EXIT_FAILURE)
        {
            log_fatal("Failed to compile input file '%s'.", infile[i]);
        }

        tokens_dump(&tokens);

        tokens_destroy(&tokens);
        lexer_destroy();
    }

    return EXIT_SUCCESS;
}

int
main(int argc, char *argv[argc + 1])
{
    argv++;
    argc--;

    return compile(argc, (const char **)argv);
}
