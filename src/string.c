#include "string.h"
#include "assert.h"
#include "char_vector.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// text の文字数を返す関数
uint32_t string_len(const char *text) {
    assert(text != NULL, "passed NULL to string_len(text)");

    int32_t count = 0;
    size_t index = 0;

    // C言語の文字列は必ずヌル文字で終わる
    while (text[index] != '\0') {
        // mblen関数は指定された文字が何バイトで表されるのかを返す関数
        int result = mblen(&text[index], MB_CUR_MAX);

        if (result < 0) {
            failure("mblen returned error: %d", result);
        }

        index += result;
        count += 1;
    }

    return count;
}

// text のバイト数を返す関数
size_t string_bytes(const char *text) {
    assert(text != NULL, "passed NULL to string_bytes(text)");
    size_t result = 0;

    while (true) {
        // C言語の文字列は必ずヌル文字で終わる
        if (text[result] == '\0') {
            return result;
        }
        result += 1;
    }
}

// src の target_pos 文字目を返す関数。マルチバイト文字（日本語など）対応。
// requires to be freed!
struct CharVector string_at(const char *src, size_t target_pos) {
    assert(src != NULL, "passed NULL to string_at(src)");

    size_t index = 0;
    size_t pos = 0;

    // C言語の文字列は必ずヌル文字で終わる
    while (src[index] != '\0') {
        // mblen関数は指定された文字が何バイトで表されるのかを返す関数
        int len = mblen(&src[index], MB_CUR_MAX);

        if (len < 0) {
            failure("mblen returned error: %d", len);
        }

        if (pos == target_pos) {
            // output に1文字分のバイトを追加し、
            // 最後に文字列の終端を表すためにヌル文字を追加して返却する。
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

    // ここに到達したということは、指定された場所には文字がなかったということなので、
    // エラーメッセージを表示しfailureマクロによってプログラムを即終了する。
    failure("there is no char on specific position. text: \"%s\", pos: \"%d\"",
            src, target_pos);
}

// a と b が等しいかを判定する関数
bool string_eq(const char *a, const char *b) {
    assert(a != NULL, "passed NULL to string_at(a)");
    assert(b != NULL, "passed NULL to string_at(b)");

    for (size_t index = 0;; index++) {
        // a と b が1要素でも等しくなかったら、
        // その時点で2つは違う文字列なのでfalseを返す。
        if (a[index] != b[index]) {
            return false;
        }

        // aとbがヌル文字の時、両方の文字列は同じ場所で終わったということなので、2つは等しい。
        // trueを返却する。
        if (a[index] == '\0') {
            return true;
        }
    }
}

// format関数のva_list版
struct CharVector vformat(const char *format, va_list args) {
    // 一度vsnprintfをNULLに対して呼び出し、結果の文字列を格納する
    // のに必要なバイト数を求める。
    // + 1 for \0
    const size_t need_bytes = vsnprintf(NULL, 0, format, args) + 1;

    struct CharVector result = char_vector_new();
    char_vector_reserve(&result, need_bytes);

    // 実際に書き込む。
    // we need to overwrite length manually because vsnprintf writes to memory
    // directly.
    result.length = vsnprintf(result.pointer, need_bytes, format, args);

    return result;
}

// printf関数が標準出力に対して結果を書き込むのに対し、
// この関数はメモリ上に書き込み、そのメモリ領域を返す関数。
struct CharVector format(const char *format, ...) {
    va_list args;
    va_start(args, format);

    const struct CharVector result = vformat(format, args);
    va_end(args);

    return result;
}
