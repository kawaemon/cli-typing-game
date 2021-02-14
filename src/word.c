#include "word.h"
#include "assert.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// ../scripts/wordsGen.py
const struct Word WORDS[] = {
    {.char_count = 3, .pointer = "おかゆ"},
    {.char_count = 3, .pointer = "つうち"},
    {.char_count = 2, .pointer = "つめ"},
    {.char_count = 3, .pointer = "まうす"},
    {.char_count = 3, .pointer = "かいろ"},
    {.char_count = 4, .pointer = "のみもの"},
    {.char_count = 3, .pointer = "どうが"},
    {.char_count = 4, .pointer = "ねるまえ"},
    {.char_count = 4, .pointer = "せんさー"},
    {.char_count = 2, .pointer = "ねこ"},
    {.char_count = 3, .pointer = "わいん"},
    {.char_count = 3, .pointer = "こるく"},
    {.char_count = 5, .pointer = "はむすたー"},
    {.char_count = 2, .pointer = "かぎ"},
    {.char_count = 5, .pointer = "こんそーる"},
    {.char_count = 3, .pointer = "げーむ"},
    {.char_count = 3, .pointer = "うさぎ"},
    {.char_count = 4, .pointer = "ろーまじ"},
    {.char_count = 6, .pointer = "たいむらいん"},
    {.char_count = 3, .pointer = "せーぶ"},
    {.char_count = 4, .pointer = "かんすう"},
    {.char_count = 4, .pointer = "こうせん"},
};

const struct Word random_word() {
    const size_t WORDS_LEN = sizeof(WORDS) / sizeof(WORDS[0]);
    const size_t index = rand() % WORDS_LEN;
    const struct Word word = WORDS[index];

    return word;
}

#define INITIAL_CAPACITY 2

struct WordVector word_vector_new(void) {
    const struct WordVector result = {
        .pointer = malloc(sizeof(struct Word) * INITIAL_CAPACITY),
        .length = 0,
        .capacity = INITIAL_CAPACITY,
    };

    assert(result.pointer != NULL, "allocation failed");

    return result;
}

struct Word word_vector_get(struct WordVector *self, size_t index) {
    assert(
        index < self->length,
        "attempted to get out of WordVector bounds(length: %d, requested: %d)",
        self->length, index);

    return self->pointer[index];
}

void word_vector_push(struct WordVector *self, struct Word value) {
    assert(self != NULL, "passed NULL to word_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid WordVector");

    if (self->capacity == self->length) {
        const size_t new_size_bytes = self->capacity * 2 * sizeof(struct Word);

        self->pointer = realloc(self->pointer, new_size_bytes);
        self->capacity *= 2;
    }

    self->pointer[self->length] = value;
    self->length += 1;
}

struct Word word_vector_pop(struct WordVector *self) {
    assert(self != NULL, "passed NULL to word_vector_push(self)");
    assert(self->pointer != NULL, "passed freed or invalid WordVector");

    struct Word result = self->pointer[self->length - 1];

    self->length -= 1;

    return result;
}

void word_vector_free(struct WordVector *self) {
    assert(self != NULL, "passed NULL to word_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid WordVector");

    free(self->pointer);
    self->pointer = NULL;
}
