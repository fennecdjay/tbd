#ifndef __AST_H__
#define __AST_H__

#include <stddef.h>

#include <tbdc/lib/string.h>

#define AST_ARRAY_SIZE (0x100000)

enum ast_node_type
{
    NODE_FUNC
};

struct ast_func
{
    struct string_view name;
};

struct ast_node
{
    enum ast_node_type tag;
    union
    {
        struct ast_func func;
    } u;
};

struct ast
{
    size_t size;
    struct ast_node nodes[AST_ARRAY_SIZE];
};

struct ast *ast_create(void);
void ast_destroy(struct ast **ast);

#endif /* __AST_H__ */
