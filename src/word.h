#include <stdint.h>

struct Word {
    size_t char_count;
    const char *pointer;
};

const struct Word *random_word();
