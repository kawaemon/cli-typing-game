#pragma once
#include "char_vector.h"
#include <stdbool.h>
#include <stdint.h>

struct String {
    const char *text;
    size_t char_count;
};

// text の文字数を返す関数
uint32_t string_len(const char *text);

// text のバイト数を返す関数
size_t string_bytes(const char *text);

// src の target_pos 文字目を返す関数。マルチバイト文字（日本語など）対応。
struct CharVector string_at(const char *src, size_t pos);

// a と b が等しいかを判定する関数
bool string_eq(const char *a, const char *b);

// printf関数が標準出力に対して結果を書き込むのに対し、
// この関数はメモリ上に書き込み、そのメモリ領域を返す関数。
struct CharVector format(const char *format, ...);

// format関数のva_list版
struct CharVector vformat(const char *format, va_list args);
