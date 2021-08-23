#ifndef __COMPILER_H__
#define __COMPILER_H__

struct compile; /* Forward declare this here as it's needed by the includes */

#include <tbdc/frontend/ast.h>
#include <tbdc/frontend/lexer.h>
#include <tbdc/frontend/parser.h>
#include <tbdc/frontend/source.h>
#include <tbdc/frontend/tokens.h>
#include <tbdc/lib/opts.h>

struct compile
{
    struct source *source;
    struct lexer *lexer;
    struct tokens *tokens;
    struct parser *parser;
    struct ast *ast;
};

void compile(const struct opts *opts);

#endif /* __COMPILER_H__ */
