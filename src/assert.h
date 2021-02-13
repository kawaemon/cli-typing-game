#include <stdlib.h>

#define failure(msg, ...)                                                      \
    failure_hook(__LINE__, msg, __VA_ARGS__);                                  \
    exit(1);

#define unreachable() failure("expected to be unreachable code executed");

#define assert(condition, msg, ...)                                            \
    if (!(condition)) {                                                        \
        failure(msg, __VA_ARGS__);                                             \
    }

void failure_hook(int line, const char *msg, ...);
