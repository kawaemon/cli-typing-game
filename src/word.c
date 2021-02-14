#include "word.h"
#include <stdint.h>
#include <stdlib.h>

const struct Word *random_word() {
    // ../scripts/wordsGen.py
    const static struct Word WORDS[] = {
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

    const size_t WORDS_LEN = sizeof(WORDS) / sizeof(WORDS[0]);
    const size_t index = rand() % WORDS_LEN;

    return &WORDS[index];
}
