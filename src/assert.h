#pragma once
#include <stdlib.h>

// メッセージを表示してプログラムを即終了する　
#define failure(msg, ...) failure_hook(__FILE__, __LINE__, msg, __VA_ARGS__);

// プログラムのここには絶対に到達しないことを明示する
#define unreachable() failure("expected to be unreachable code executed");

// condition が false だった時に、メッセージを表示してプログラムを即終了する
#define assert(condition, msg, ...)                                            \
    if (!(condition)) {                                                        \
        failure(msg, __VA_ARGS__);                                             \
    }

// 終了するときに呼ばれる関数のプロトタイプ宣言
void failure_hook(const char *filename, int line, const char *msg, ...);
