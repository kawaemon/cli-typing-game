#include "word.h"
#include "assert.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// �����_���ȒP���Ԃ��֐�
const struct Word random_word() {
    // �P��ꗗ
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

    // �P�ꃊ�X�g�̗v�f��
    const size_t WORDS_LEN = sizeof(WORDS) / sizeof(WORDS[0]);

    // �����_���ȏꏊ��I�ѕԋp����
    const size_t index = rand() % WORDS_LEN;
    const struct Word word = WORDS[index];

    return word;
}

// CharVector�Ƃقړ����Ȃ̂ŁA�ڂ��������͏ȗ����܂��B

#define INITIAL_CAPACITY 2

// �V���� WordVector ���쐬����֐�
struct WordVector word_vector_new(void) {
    const struct WordVector result = {
        .pointer = malloc(sizeof(struct Word) * INITIAL_CAPACITY),
        .length = 0,
        .capacity = INITIAL_CAPACITY,
    };

    assert(result.pointer != NULL, "allocation failed");

    return result;
}

// self �� index �Ԗڂ̗v�f���擾����֐�
struct Word word_vector_get(struct WordVector *self, size_t index) {
    assert(
        index < self->length,
        "attempted to get out of WordVector bounds(length: %d, requested: %d)",
        self->length, index);

    return self->pointer[index];
}

// self �̍Ō�� value ��ǉ�����֐�
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

// self �̍Ō�̗v�f����������֐�
struct Word word_vector_pop(struct WordVector *self) {
    assert(self != NULL, "passed NULL to word_vector_push(self)");
    assert(self->pointer != NULL, "passed freed or invalid WordVector");

    struct Word result = self->pointer[self->length - 1];

    self->length -= 1;

    return result;
}

// self ���J������֐�
void word_vector_free(struct WordVector *self) {
    assert(self != NULL, "passed NULL to word_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid WordVector");

    free(self->pointer);
    self->pointer = NULL;
}

// self �ɐV����size�̈�m�ۂ���֐�
const struct WordVector random_words(size_t count) {
    struct WordVector result = word_vector_new();

    for (size_t i = 0; i < count; i++) {
        word_vector_push(&result, random_word());
    }

    return result;
}
