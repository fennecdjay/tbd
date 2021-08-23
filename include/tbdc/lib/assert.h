#ifndef __TBDC_ASSERT_H__
#define __TBDC_ASSERT_H__

#ifndef NDEBUG

#include <stdbool.h>
#include <stddef.h>

#include <tbdc/lib/log.h>

#define assert(cond) do {               \
    if (!(cond))                        \
    {                                   \
        BUG("In file %s on line %d: "   \
            "Assertion '%s' failed!",   \
            __FILE__, __LINE__, #cond); \
    }                                   \
} while (0)

#define assert_bop(l, r, bop)                  \
    assert((l) bop (r))

/* Assertions for binary comparions */

#define assert_eq(l, r)                        \
    assert_bop(l, r, ==)

#define assert_ne(l, r)                        \
    assert_bop(l, r, !=)

#define assert_lt(l, r)                        \
    assert_bop(l, r, <)

#define assert_le(l, r)                        \
    assert_bop(l, r, <=)

#define assert_gt(l, r)                        \
    assert_bop(l, r, >)

#define assert_ge(l, r)                        \
    assert_bop(l, r, >=)

/* Assertions for pointers */

#define assert_nonnull(p)                      \
    assert_ne(p, NULL)

#define assert_null(p)                         \
    assert_eq(p, NULL)

/* Assertions for booleans */

#define assert_true(b)                         \
    assert_eq(b, true)

#define assert_false(b)                        \
    assert_eq(b, false)

#else

#define assert(test) ((void)0)

#define assert_bop(l, r, bop)

#define assert_eq(l, r)
#define assert_ne(l, r)
#define assert_lt(l, r)
#define assert_le(l, r)
#define assert_gt(l, r)
#define assert_ge(l, r)

#define assert_nonnull(p)
#define assert_null(p)

#define assert_true(b)
#define assert_false(b)

#endif /* NDEBUG */

#endif /* __TBDC_ASSERT_H__ */
