#include "assert.h"
#include "terminal.h"
#include <stdarg.h>
#include <stdio.h>

extern struct Terminal TERMINAL;

void failure_hook(int line, const char *msg, ...) {
    term_reset(&TERMINAL);

    printf("at line %d: ", line);

    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
}