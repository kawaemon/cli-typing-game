#include "string.h"
#include "assert.h"
#include "char_vector.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t string_len(const char *text) {
    assert(text != NULL, "passed NULL to string_len(text)");

    int32_t count = 0;
    size_t index = 0;

    while (text[index] != '\0') {
        int result = mblen(&text[index], MB_CUR_MAX);

        if (result < 0) {
            failure("mblen returned error: %d", result);
        }

        index += result;
        count += 1;
    }

    return count;
}

size_t string_bytes(const char *text) {
    assert(text != NULL, "passed NULL to string_bytes(text)");
    size_t result = 0;

    while (true) {
        if (text[result] == '\0') {
            return result;
        }
        result += 1;
    }
}

// requires to be freed!
struct CharVector string_at(const char *src, size_t target_pos) {
    assert(src != NULL, "passed NULL to string_at(src)");

    size_t index = 0;
    size_t pos = 0;

    while (src[index] != '\0') {
        int len = mblen(&src[index], MB_CUR_MAX);

        if (len < 0) {
            failure("mblen returned error: %d", len);
        }

        if (pos == target_pos) {
            struct CharVector output = char_vector_new();

            for (size_t i = 0; i < (size_t)len; i++) {
                char_vector_push(&output, src[index + i]);
            }

            char_vector_push(&output, '\0');
            return output;
        }

        index += len;
        pos += 1;
    }

    failure("there is no char on specific position. text: \"%s\", pos: \"%d\"",
            src, target_pos);
}

bool string_eq(const char *a, const char *b) {
    assert(a != NULL, "passed NULL to string_at(a)");
    assert(b != NULL, "passed NULL to string_at(b)");

    for (size_t index = 0;; index++) {
        if (a[index] != b[index]) {
            return false;
        }

        if (a[index] == '\0') {
            return true;
        }
    }
}

struct CharVector vformat(const char *format, va_list args) {
    // + 1 for \0
    const size_t need_bytes = vsnprintf(NULL, 0, format, args) + 1;

    struct CharVector result = char_vector_new();
    char_vector_reserve(&result, need_bytes);

    // we need to overwrite length manually because vsnprintf writes to memory
    // directly.
    result.length = vsnprintf(result.pointer, need_bytes, format, args);

    return result;
}

struct CharVector format(const char *format, ...) {
    va_list args;
    va_start(args, format);

    const struct CharVector result = vformat(format, args);
    va_end(args);

    return result;
}
