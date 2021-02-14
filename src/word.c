#include "word.h"
#include "assert.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// ../scripts/wordsGen.py
const struct Word WORDS[] = {
    {.char_count = 3, .pointer = "������"},
    {.char_count = 3, .pointer = "����"},
    {.char_count = 2, .pointer = "��"},
    {.char_count = 3, .pointer = "�܂���"},
    {.char_count = 3, .pointer = "������"},
    {.char_count = 4, .pointer = "�݂̂���"},
    {.char_count = 3, .pointer = "�ǂ���"},
    {.char_count = 4, .pointer = "�˂�܂�"},
    {.char_count = 4, .pointer = "���񂳁["},
    {.char_count = 2, .pointer = "�˂�"},
    {.char_count = 3, .pointer = "�킢��"},
    {.char_count = 3, .pointer = "���邭"},
    {.char_count = 5, .pointer = "�͂ނ����["},
    {.char_count = 2, .pointer = "����"},
    {.char_count = 5, .pointer = "���񂻁[��"},
    {.char_count = 3, .pointer = "���[��"},
    {.char_count = 3, .pointer = "������"},
    {.char_count = 4, .pointer = "��[�܂�"},
    {.char_count = 6, .pointer = "�����ނ炢��"},
    {.char_count = 3, .pointer = "���[��"},
    {.char_count = 4, .pointer = "���񂷂�"},
    {.char_count = 4, .pointer = "��������"},
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
