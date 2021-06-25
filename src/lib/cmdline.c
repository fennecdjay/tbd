#include "lib/assertions.h"
#include "lib/cmdline.h"

// TODO: Command line arg parser

static const char *program_name = "tbdc";

void
set_program_name(const char *name)
{
    $assert_nonnull(name);

    program_name = name;
}

const char *
get_program_name(void)
{
    $assert_nonnull(program_name);

    return program_name;
}
