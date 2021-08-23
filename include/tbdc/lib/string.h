#ifndef __TBDC_STRING_H__
#define __TBDC_STRING_H__

#include <stdbool.h>
#include <stddef.h>

#define SV_FMT "%.*s"
#define SV_ARG(v) v.size, v.str

struct string_view
{
    size_t size;
    const char *str;
};

bool str_equal(const char *s1, const char *s2);
bool strn_equal(const char *s1, const char *s2, const size_t size);

#endif /* __TBDC_STRING_H__ */
