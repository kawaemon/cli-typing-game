#include "char_vector.h"
#include "assert.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 2

// 新しい CharVector を作成する関数
struct CharVector char_vector_new(void) {
    const struct CharVector result = {
        .pointer = malloc(sizeof(char) * INITIAL_CAPACITY),
        .length = 0,
        .capacity = INITIAL_CAPACITY,
    };

    assert(result.pointer != NULL, "allocation failed");

    return result;
}

// self の index 番目の要素を取得する関数
char char_vector_get(struct CharVector *self, size_t index) {
    assert(
        index < self->length,
        "attempted to get out of CharVector bounds(length: %d, requested: %d)",
        self->length, index);

    return self->pointer[index];
}

// self の最後に value を追加する関数
void char_vector_push(struct CharVector *self, char value) {
    assert(self != NULL, "passed NULL to char_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    if (self->capacity == self->length) {
        // もうあまりの領域が無ければ、さらに容量を倍にして再確保する
        const size_t new_size_bytes = self->capacity * 2 * sizeof(char);

        self->pointer = realloc(self->pointer, new_size_bytes);
        self->capacity *= 2;
    }

    // 最後に追加して長さを1増やす
    self->pointer[self->length] = value;
    self->length += 1;
}

// self の最後に str を追加する関数
void char_vector_pushstr(struct CharVector *self, const char *str) {
    assert(self != NULL, "passed NULL to char_vector_push(self)");
    assert(str != NULL, "passed NULL to char_vector_pushstr(str)");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    // ヌル文字に当たるまでpushし続ける
    size_t index = 0;
    while (str[index] != '\0') {
        char_vector_push(self, str[index]);
        index += 1;
    }
}

// self の最後の要素を消去する関数
char char_vector_pop(struct CharVector *self) {
    assert(self != NULL, "passed NULL to char_vector_push(self)");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    char result = self->pointer[self->length - 1];

    self->pointer[self->length - 1] = 0;
    self->length -= 1;

    return result;
}

// self を開放する関数
void char_vector_free(struct CharVector *self) {
    assert(self != NULL, "passed NULL to char_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    // メモリ領域を解放する
    free(self->pointer);
    self->pointer = NULL;
}

// self に新たにsize領域確保する関数
// size is number of additional elements, not bytes.
void char_vector_reserve(struct CharVector *self, size_t size) {
    if (self->capacity - self->length < size) {
        const size_t new_capacity = self->capacity + size;
        const size_t new_size = new_capacity * sizeof(char);

        self->pointer = realloc(self->pointer, new_size);
        self->capacity = new_capacity;
    }
}
