#pragma once
#include "string.h"

struct StringSlice {
    const size_t length;
    const char **pointer;
};

const struct StringSlice *get_roma(const char *hiragana);
