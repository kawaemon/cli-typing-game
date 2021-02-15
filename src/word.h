#pragma once
#include <stdint.h>

// 単語構造体
struct Word {
    // 文字数
    size_t char_count;
    // 文字
    const char *pointer;
};

// ランダムな単語を返す関数
const struct Word random_word(void);

// 収縮可能なWord構造体の配列
struct WordVector {
    // メモリ領域へのポインタ
    struct Word *pointer;
    // 現在の配列の長さ
    size_t length;
    // 現在確保されているメモリ領域の容量
    size_t capacity;
};

// 新しい WordVector を作成する関数
struct WordVector word_vector_new(void);

// self の index 番目の要素を取得する関数
struct Word word_vector_get(struct WordVector *self, size_t index);

// self の最後に value を追加する関数
void word_vector_push(struct WordVector *self, struct Word value);

// self の最後の要素を消去する関数
struct Word word_vector_pop(struct WordVector *self);

// self を開放する関数
void word_vector_free(struct WordVector *self);

// self に新たにsize領域確保する関数
const struct WordVector random_words(size_t count);
