#ifndef __PARSER_H__
#define __PARSER_H__

#include <tbdc/core/compile.h>

struct parser
{
    size_t index;
};

struct parser *parser_create(void);
void parser_destroy(struct parser **parser);
int parse(struct compile *ctx);

#endif /* __PARSER_H__ */
