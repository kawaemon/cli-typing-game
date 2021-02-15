#pragma once
#include "char_vector.h"
#include <stdbool.h>
#include <stdint.h>

struct String {
    const char *text;
    size_t char_count;
};

// text �̕�������Ԃ��֐�
uint32_t string_len(const char *text);

// text �̃o�C�g����Ԃ��֐�
size_t string_bytes(const char *text);

// src �� target_pos �����ڂ�Ԃ��֐��B�}���`�o�C�g�����i���{��Ȃǁj�Ή��B
struct CharVector string_at(const char *src, size_t pos);

// a �� b �����������𔻒肷��֐�
bool string_eq(const char *a, const char *b);

// printf�֐����W���o�͂ɑ΂��Č��ʂ��������ނ̂ɑ΂��A
// ���̊֐��̓�������ɏ������݁A���̃������̈��Ԃ��֐��B
struct CharVector format(const char *format, ...);

// format�֐���va_list��
struct CharVector vformat(const char *format, va_list args);
