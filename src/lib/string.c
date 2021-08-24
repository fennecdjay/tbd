#include <stddef.h>
#include <string.h>

#include <tbdc/lib/string.h>

bool str_equal(const char *s1, const char *s2)
{
    return s1 == s2 || !strcmp(s1, s2);
}

bool strn_equal(const char *s1, const char *s2, const size_t size)
{
    return s1 == s2 || !strncmp(s1, s2, size);
}
