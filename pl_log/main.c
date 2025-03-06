#include <stdio.h>

#define PL_LOG_IMPLEMENTATION
#include "pl_log.h"

static void warning_callback(void*)
{
    fprintf(stderr, "WARNING WARNING WARNING\n");
}

int main(void)
{
    pl_init();
    pl_set_warning_callback(warning_callback, NULL);

    PL_TRACE("Shows only if env PL_LEVEL=trace is set. Prints in faded gray.");
    PL_DEBUG("Shows only if env PL_LEVEL=debug or above is set. Prints in brown.");
    PL_INFO("Always shows. Prints in terminal color. All messages can use printf %s style tags.", "XXX");
    PL_WARNING("Always shows. Prints in magenta.");
    // PL_ERROR_RET(1, "Always shows. Prints in red. Returns from current function.");
    // PL_ABORT()

    pl_close();
    return 0;
}
