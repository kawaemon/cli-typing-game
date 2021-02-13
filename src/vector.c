#include "vector.h"
#include "assert.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

const size_t INITIAL_CAPACITY = 2;

struct CharVector char_vector_new() {
    const struct CharVector result = {
        .pointer = malloc(sizeof(char) * INITIAL_CAPACITY),
        .length = 0,
        .capacity = INITIAL_CAPACITY};

    assert(result.pointer != NULL, "allocation failed");

    return result;
}

char char_vector_get(struct CharVector *self, size_t index) {
    assert(
        index < self->length,
        "attempted to get out of CharVector bounds(length: %d, requested: %d)",
        self->length, index);

    return self->pointer[index];
}

void char_vector_push(struct CharVector *self, char value) {
    assert(self != NULL, "passed NULL to char_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    if (self->capacity == self->length) {
        self->pointer = realloc(self->pointer, self->capacity * 2);
        self->capacity *= 2;
    }

    self->pointer[self->length] = value;
    self->length += 1;
}

void char_vector_free(struct CharVector *self) {
    assert(self != NULL, "passed NULL to char_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    free(self->pointer);
    self->pointer = NULL;
}
