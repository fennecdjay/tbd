#include <tbdc/frontend/ast.h>
#include <tbdc/lib/xmalloc.h>

struct ast *ast_create(void)
{
    struct ast *ast = xmalloc(sizeof(*ast));
    ast->size = 0;
    return ast;
}

void ast_destroy(struct ast **ast)
{
    xfree((void **)ast);
}
