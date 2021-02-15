#include "string.h"
#include "assert.h"
#include "char_vector.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// text �̕�������Ԃ��֐�
uint32_t string_len(const char *text) {
    assert(text != NULL, "passed NULL to string_len(text)");

    int32_t count = 0;
    size_t index = 0;

    // C����̕�����͕K���k�������ŏI���
    while (text[index] != '\0') {
        // mblen�֐��͎w�肳�ꂽ���������o�C�g�ŕ\�����̂���Ԃ��֐�
        int result = mblen(&text[index], MB_CUR_MAX);

        if (result < 0) {
            failure("mblen returned error: %d", result);
        }

        index += result;
        count += 1;
    }

    return count;
}

// text �̃o�C�g����Ԃ��֐�
size_t string_bytes(const char *text) {
    assert(text != NULL, "passed NULL to string_bytes(text)");
    size_t result = 0;

    while (true) {
        // C����̕�����͕K���k�������ŏI���
        if (text[result] == '\0') {
            return result;
        }
        result += 1;
    }
}

// src �� target_pos �����ڂ�Ԃ��֐��B�}���`�o�C�g�����i���{��Ȃǁj�Ή��B
// requires to be freed!
struct CharVector string_at(const char *src, size_t target_pos) {
    assert(src != NULL, "passed NULL to string_at(src)");

    size_t index = 0;
    size_t pos = 0;

    // C����̕�����͕K���k�������ŏI���
    while (src[index] != '\0') {
        // mblen�֐��͎w�肳�ꂽ���������o�C�g�ŕ\�����̂���Ԃ��֐�
        int len = mblen(&src[index], MB_CUR_MAX);

        if (len < 0) {
            failure("mblen returned error: %d", len);
        }

        if (pos == target_pos) {
            // output ��1�������̃o�C�g��ǉ����A
            // �Ō�ɕ�����̏I�[��\�����߂Ƀk��������ǉ����ĕԋp����B
            struct CharVector output = char_vector_new();

            for (size_t i = 0; i < (size_t)len; i++) {
                char_vector_push(&output, src[index + i]);
            }

            char_vector_push(&output, '\0');
            return output;
        }

        index += len;
        pos += 1;
    }

    // �����ɓ��B�����Ƃ������Ƃ́A�w�肳�ꂽ�ꏊ�ɂ͕������Ȃ������Ƃ������ƂȂ̂ŁA
    // �G���[���b�Z�[�W��\����failure�}�N���ɂ���ăv���O�����𑦏I������B
    failure("there is no char on specific position. text: \"%s\", pos: \"%d\"",
            src, target_pos);
}

// a �� b �����������𔻒肷��֐�
bool string_eq(const char *a, const char *b) {
    assert(a != NULL, "passed NULL to string_at(a)");
    assert(b != NULL, "passed NULL to string_at(b)");

    for (size_t index = 0;; index++) {
        // a �� b ��1�v�f�ł��������Ȃ�������A
        // ���̎��_��2�͈Ⴄ������Ȃ̂�false��Ԃ��B
        if (a[index] != b[index]) {
            return false;
        }

        // a��b���k�������̎��A�����̕�����͓����ꏊ�ŏI������Ƃ������ƂȂ̂ŁA2�͓������B
        // true��ԋp����B
        if (a[index] == '\0') {
            return true;
        }
    }
}

// format�֐���va_list��
struct CharVector vformat(const char *format, va_list args) {
    // ��xvsnprintf��NULL�ɑ΂��ČĂяo���A���ʂ̕�������i�[����
    // �̂ɕK�v�ȃo�C�g�������߂�B
    // + 1 for \0
    const size_t need_bytes = vsnprintf(NULL, 0, format, args) + 1;

    struct CharVector result = char_vector_new();
    char_vector_reserve(&result, need_bytes);

    // ���ۂɏ������ށB
    // we need to overwrite length manually because vsnprintf writes to memory
    // directly.
    result.length = vsnprintf(result.pointer, need_bytes, format, args);

    return result;
}

// printf�֐����W���o�͂ɑ΂��Č��ʂ��������ނ̂ɑ΂��A
// ���̊֐��̓�������ɏ������݁A���̃������̈��Ԃ��֐��B
struct CharVector format(const char *format, ...) {
    va_list args;
    va_start(args, format);

    const struct CharVector result = vformat(format, args);
    va_end(args);

    return result;
}
