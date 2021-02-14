#include "word.h"
#include <stdint.h>
#include <stdlib.h>

const struct Word *random_word() {
    // ../scripts/wordsGen.py
    const static struct Word WORDS[] = {
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

    const size_t WORDS_LEN = sizeof(WORDS) / sizeof(WORDS[0]);
    const size_t index = rand() % WORDS_LEN;

    return &WORDS[index];
}
