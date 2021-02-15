#pragma once

// 収縮可能な char の配列
struct CharVector {
    // メモリ領域へのポインタ
    char *pointer;
    // 現在の配列の長さ
    size_t length;
    // 現在確保されているメモリ領域の容量
    size_t capacity;
};

// 新しい CharVector を作成する関数
struct CharVector char_vector_new(void);

// self の index 番目の要素を取得する関数
char char_vector_get(struct CharVector *self, size_t index);

// self の最後に value を追加する関数
void char_vector_push(struct CharVector *self, char value);

// self の最後に str を追加する関数
void char_vector_pushstr(struct CharVector *self, const char *str);

// self の最後の要素を消去する関数
char char_vector_pop(struct CharVector *self);

// self を開放する関数
void char_vector_free(struct CharVector *self);

// self に新たにsize領域確保する関数
void char_vector_reserve(struct CharVector *self, size_t size);
