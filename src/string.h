#include <stdbool.h>
#include <stdint.h>

struct String {
    const char *text;
    size_t char_count;
};

uint32_t string_len(const char *text);
size_t string_bytes(const char *text);
const char *string_at(const char *src, size_t pos);
bool string_eq(const char *a, const char *b);
