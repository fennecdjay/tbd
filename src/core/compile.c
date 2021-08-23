#include <stdio.h>

#include <tbdc/core/compile.h>
#include <tbdc/lib/error.h>
#include <tbdc/lib/log.h>
#include <tbdc/lib/xmalloc.h>

static void compile_file(const struct opts *opts, const char *file)
{
    ((void)opts);

    struct compile *ctx = xmalloc(sizeof(*ctx));
    ctx->source = source_create(file);
    ctx->lexer = lexer_create();
    ctx->tokens = tokens_create();
    ctx->parser = parser_create();
    ctx->ast = ast_create();

    if (lex(ctx))
    {
        compile_error(ctx->source->file, "Lexing failed");
    }

    tokens_dump(ctx->tokens);

    if (parse(ctx))
    {
        compile_error(ctx->source->file, "Parsing failed");
    }

    ast_destroy(&ctx->ast);
    parser_destroy(&ctx->parser);
    tokens_destroy(&ctx->tokens);
    lexer_destroy(&ctx->lexer);
    source_destroy(&ctx->source);
    xfree((void **)&ctx);
}

void compile(const struct opts *opts)
{
    for (size_t i = 0; i < opts->argc; i++)
    {
        DEBUG("%s", opts->argv[i]);
        compile_file(opts, opts->argv[i]);
    }
}
