#pragma once
#include <stdlib.h>

#define failure(msg, ...) failure_hook(__FILE__, __LINE__, msg, __VA_ARGS__);

#define unreachable() failure("expected to be unreachable code executed");

#define assert(condition, msg, ...)                                            \
    if (!(condition)) {                                                        \
        failure(msg, __VA_ARGS__);                                             \
    }

void failure_hook(const char *filename, int line, const char *msg, ...);
