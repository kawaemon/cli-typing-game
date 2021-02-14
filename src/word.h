#pragma once
#include <stdint.h>

struct Word {
    size_t char_count;
    const char *pointer;
};

const struct Word random_word(void);

struct WordVector {
    struct Word *pointer;
    size_t length;
    size_t capacity;
};

struct WordVector word_vector_new(void);
struct Word word_vector_get(struct WordVector *self, size_t index);
void word_vector_push(struct WordVector *self, struct Word value);
struct Word word_vector_pop(struct WordVector *self);
void word_vector_free(struct WordVector *self);

const struct WordVector random_words(size_t count);
