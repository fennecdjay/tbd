#include <tbdc/frontend/parser.h>
#include <tbdc/lib/xmalloc.h>

struct parser *parser_create(void)
{
    struct parser *parser = xmalloc(sizeof(*parser));
    parser->index = 0;
    return parser;
}

void parser_destroy(struct parser **parser)
{
    xfree((void **)parser);
}

int parse(struct compile *ctx)
{
    ((void)ctx);
    return -1;
}
