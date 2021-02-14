#include "assert.h"
#include "terminal.h"
#include <stdarg.h>
#include <stdio.h>

extern struct Terminal TERMINAL;

void failure_hook(const char *filename, int line, const char *msg, ...) {
    term_reset(&TERMINAL);

    printf("at %s line %d: ", filename, line);

    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
}
