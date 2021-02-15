#include "assert.h"
#include "char_vector.h"
#include "roma.h"
#include "string.h"
#include "terminal.h"
#include "word.h"
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ESC_KEY 27

// ターミナルの操作に使う構造体の実態
// プログラム中で1つしか作成しないので、failure_hook関数で使うためにグローバル変数にしてある。
struct Terminal TERMINAL;

// ゲームの情報を格納する構造体
struct Game {
    struct WordVector words;
    size_t char_index;
    size_t roma_index;
    bool previous_correct;
};

// ゲームの結果の情報を格納する構造体
struct GameResult {
    clock_t started_on;
    clock_t ended_on;
    int total_types;
    int miss_types;
};

// プログラムの状態全てを格納する構造体
struct Context {
    bool running;             // プログラムが実行されているか？
    bool gaming;              // タイピングゲーム中か？
    char last_key_code;       // 最後に入力されたキーコード
    struct Game game;         // ゲーム情報
    struct GameResult result; // ゲームの結果情報
};

// ゲーム画面を表示する関数のプロトタイプ宣言
void game_render(struct Context *context);
// ゲーム画面が表示されている時にキー入力されたときに実行される関数のプロトタイプ宣言
void game_on_key_type(struct Context *context, char input);

// リザルド画面を表示する関数のプロトタイプ宣言
void result_render(struct Context *context);
// リザルド画面が表示されている時にキー入力されたときに実行される関数のプロトタイプ宣言
void result_on_key_type(struct Context *context, char input);

int main(void) {
    srand((unsigned)time(NULL));

    // ロケールを設定する。mblen関数の挙動などに影響する
    if (setlocale(LC_CTYPE, "") == NULL) {
        // cannot use failure macro because TERMINAL is not initialized.
        fprintf(stderr, "failed to set locale");
        return 1;
    }

    // ターミナル構造体を初期化してグローバル変数に代入する
    TERMINAL = get_term();

    struct Context context = {
        .running = true,
        .gaming = true,
        .last_key_code = 0,
        .game = {
            .words = random_words(20),
            .char_index = 0,
            .roma_index = 0,
            .previous_correct = true,
        },
        .result = { 0 },
    };

    // カーソルを非表示する
    term_set_cursor_visible(&TERMINAL, false);

    do {
        // 適切な画面を表示する
        if (context.gaming) {
            game_render(&context);
        } else {
            result_render(&context);
        }

        // ターミナルで発生したイベントを取得する
        const struct TerminalEvent event = term_poll_event(&TERMINAL);

        switch (event.type) {
        // キーボードが操作されたとき
        case KEYBOARD_EVENT:
            const char keycode = event.keyboard_event.key_code;
            context.last_key_code = keycode;
            if (keycode == ESC_KEY) {
                // esc キーが押されたときはゲームを終了する
                context.running = false;
            }

            // 画面に応じた関数を呼び出す
            if (context.gaming) {
                game_on_key_type(&context, keycode);
            } else {
                result_on_key_type(&context, keycode);
            }

            break;

        case RESIZE_EVENT:
            break;
        }

        // ターミナルの画面をクリアする
        term_clear(&TERMINAL);
    } while (context.running);

    // ターミナルを元の状態に戻す
    term_reset(&TERMINAL);
    return 0;
}

// ゲーム画面を表示する関数
void game_render(struct Context *context) {
    if (context->game.words.length == 0) {
        // ゲーム終わり
        context->gaming = false;
        context->result.ended_on = clock();
        result_render(context);
        return;
    }

    // 現在の単語は、単語リストの一番最後の単語なので、それを取得する
    const struct Word current_word
        = context->game.words.pointer[context->game.words.length - 1];
    struct CharVector current_char
        = string_at(current_word.pointer, context->game.char_index);

    // 現在の文字を入力できるローマ字のパターンを取得する
    const struct StringSlice *romas = get_roma(current_char.pointer);

    term_set_fg(&TERMINAL, WHITE);
    term_set_bg(&TERMINAL, BLACK);

    term_print(&TERMINAL, "ESC: 終了 debug: %d %d %d %s\n",
               context->last_key_code, context->result.total_types,
               context->result.miss_types,
               context->game.previous_correct ? "true" : "false");
    term_print(&TERMINAL, "NEXT: ");

    {
        assert(romas->length > 0, "romas was empty");

        // ローマ字パターンの一番最初のものを代表として表示する
        const char *roma = romas->pointer[0];
        for (size_t i = 0; roma[i] != '\0'; i++) {
            if (i < context->game.roma_index) {
                // 既に入力済みの文字は緑色で表示する
                term_set_fg(&TERMINAL, GREEN);
            } else if (i == context->game.roma_index) {
                // 現在入力すべき文字は赤色でハイライトする
                term_set_fg(&TERMINAL, BLACK);
                term_set_bg(&TERMINAL, RED);
            }

            term_print(&TERMINAL, "%c", roma[i]);

            // 元の色に戻す
            term_set_fg(&TERMINAL, WHITE);
            term_set_bg(&TERMINAL, BLACK);
        }
    }

    term_print(&TERMINAL, "\n");

    {
        for (size_t i = 0; i < current_word.char_count; i++) {
            if (i < context->game.char_index) {
                // 既に入力済みの文字は緑色で表示する
                term_set_fg(&TERMINAL, GREEN);
            } else if (i == context->game.char_index) {
                // 現在入力すべき文字は赤色でハイライトする
                term_set_fg(&TERMINAL, BLACK);
                term_set_bg(&TERMINAL, RED);
            }

            struct CharVector flagment = string_at(current_word.pointer, i);
            term_print(&TERMINAL, "%s", flagment.pointer);
            char_vector_free(&flagment);

            term_set_fg(&TERMINAL, WHITE);
            term_set_bg(&TERMINAL, BLACK);
        }
    }

    term_print(&TERMINAL, "\n");

    // 逆順にリストを表示する。なお一番最後の要素は現在入力すべき単語であるから表示しない。
    for (int i = context->game.words.length - 2; i >= 0; i--) {
        term_print(&TERMINAL, "%s\n", context->game.words.pointer[i].pointer);
    }

    term_set_cursor_pos(&TERMINAL, 0, 1);
    char_vector_free(&current_char);
}

void game_on_key_type(struct Context *context, char input) {
    assert(context->game.words.length > 0, "game ended");
    const struct Word current_word
        = context->game.words.pointer[context->game.words.length - 1];
    struct CharVector current_char
        = string_at(current_word.pointer, context->game.char_index);
    const struct StringSlice *romas = get_roma(current_char.pointer);
    const size_t game_roma_index = context->game.roma_index;

    if (context->result.started_on == 0) {
        context->result.started_on = clock();
    }

    bool is_correct = false;

    for (size_t i = 0; i < romas->length; i++) {
        const char *roma = romas->pointer[i];
        const size_t roma_len = string_len(roma);

        if (roma_len > game_roma_index && roma[game_roma_index] == input) {
            // 正しい入力がされた
            context->result.total_types += 1;
            is_correct = true;

            if (roma_len - 1 == game_roma_index) {
                // 1ひらがな文字の最後

                context->game.roma_index = 0;
                context->game.char_index += 1;

                if (current_word.char_count == context->game.char_index) {
                    // 1単語の最後
                    // 単語リストの一番最後の要素を削除する
                    word_vector_pop(&context->game.words);
                    context->game.char_index = 0;
                }

                break;
            }

            context->game.roma_index += 1;
            break;
        }
    }

    if (!is_correct) {
        // 正しい入力されていない場合はミスカウントを増やす
        context->result.miss_types += 1;
    }
}

void result_render(struct Context *context) {
    const struct GameResult result = context->result;
    // ゲームが動作した時間を計算する。単位がクロックで格納されているので、CLOCKS_PER_SECで割って秒に変換する　
    const double gameDurationSec = (double)(result.ended_on - result.started_on)
                                   / (double)CLOCKS_PER_SEC;
    const double keys_per_sec = result.total_types / gameDurationSec;

    term_print(&TERMINAL, "終了: esc もう一度プレイ: space\n\n");

    term_print(&TERMINAL, "ゲーム結果\n");
    term_print(&TERMINAL, "かかった時間(秒): %02.02f\n", gameDurationSec);
    term_print(&TERMINAL, "総キータイプ数: %u\n", result.total_types);
    term_print(&TERMINAL, "ミスタイプ数: %u\n", result.miss_types);
    term_print(&TERMINAL, "秒間キータイプ数: %02.02f\n", keys_per_sec);
}

void result_on_key_type(struct Context *context, char input) {
    if (input == 32) {
        // spaceキーが押されたときに、ゲームを初期化してゲーム画面に戻す
        context->result = (struct GameResult){0};
        context->game.char_index = 0;
        context->game.roma_index = 0;
        context->game.words = random_words(20);
        context->gaming = true;
    }
}
