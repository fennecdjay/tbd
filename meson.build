project('tbd', 'c',
    default_options : [
        'c_std=c99',
        'b_sanitize=address,undefined',
        'werror=false',
        'warning_level=3'
    ]
)

# -Db_fsanitize=address,undefined
# -Db_coverage=true

munit = subproject('munit')

srcs = [
    'src/lex.c',
    'src/main.c',
    'src/parse.c',
    'src/source_error.c',
    'src/tokens.c',

    'src/lib/cmdline.c',
    'src/lib/error.c',
    'src/lib/hashtable.c',
    'src/lib/string.c',
    'src/lib/xmalloc.c',
    'src/lib/term.c'
]

includes = include_directories('include')

executable('tbdc', srcs, include_directories : includes)
