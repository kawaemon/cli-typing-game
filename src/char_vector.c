#include "char_vector.h"
#include "assert.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 2

// �V���� CharVector ���쐬����֐�
struct CharVector char_vector_new(void) {
    const struct CharVector result = {
        .pointer = malloc(sizeof(char) * INITIAL_CAPACITY),
        .length = 0,
        .capacity = INITIAL_CAPACITY,
    };

    assert(result.pointer != NULL, "allocation failed");

    return result;
}

// self �� index �Ԗڂ̗v�f���擾����֐�
char char_vector_get(struct CharVector *self, size_t index) {
    assert(
        index < self->length,
        "attempted to get out of CharVector bounds(length: %d, requested: %d)",
        self->length, index);

    return self->pointer[index];
}

// self �̍Ō�� value ��ǉ�����֐�
void char_vector_push(struct CharVector *self, char value) {
    assert(self != NULL, "passed NULL to char_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    if (self->capacity == self->length) {
        // �������܂�̗̈悪������΁A����ɗe�ʂ�{�ɂ��čĊm�ۂ���
        const size_t new_size_bytes = self->capacity * 2 * sizeof(char);

        self->pointer = realloc(self->pointer, new_size_bytes);
        self->capacity *= 2;
    }

    // �Ō�ɒǉ����Ē�����1���₷
    self->pointer[self->length] = value;
    self->length += 1;
}

// self �̍Ō�� str ��ǉ�����֐�
void char_vector_pushstr(struct CharVector *self, const char *str) {
    assert(self != NULL, "passed NULL to char_vector_push(self)");
    assert(str != NULL, "passed NULL to char_vector_pushstr(str)");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    // �k�������ɓ�����܂�push��������
    size_t index = 0;
    while (str[index] != '\0') {
        char_vector_push(self, str[index]);
        index += 1;
    }
}

// self �̍Ō�̗v�f����������֐�
char char_vector_pop(struct CharVector *self) {
    assert(self != NULL, "passed NULL to char_vector_push(self)");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    char result = self->pointer[self->length - 1];

    self->pointer[self->length - 1] = 0;
    self->length -= 1;

    return result;
}

// self ���J������֐�
void char_vector_free(struct CharVector *self) {
    assert(self != NULL, "passed NULL to char_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    // �������̈���������
    free(self->pointer);
    self->pointer = NULL;
}

// self �ɐV����size�̈�m�ۂ���֐�
// size is number of additional elements, not bytes.
void char_vector_reserve(struct CharVector *self, size_t size) {
    if (self->capacity - self->length < size) {
        const size_t new_capacity = self->capacity + size;
        const size_t new_size = new_capacity * sizeof(char);

        self->pointer = realloc(self->pointer, new_size);
        self->capacity = new_capacity;
    }
}
