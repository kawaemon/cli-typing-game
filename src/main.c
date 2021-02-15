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

struct Terminal TERMINAL;

struct Game {
    struct WordVector words;
    size_t char_index;
    size_t roma_index;
    bool previous_collect;
};

struct GameResult {
    clock_t started_on;
    clock_t ended_on;
    uint64_t total_types;
    double miss_types;
};

struct Context {
    bool running;
    bool gaming;
    char last_key_code;
    struct Game game;
    struct GameResult result;
};

void game_render(struct Context *context);
void game_on_key_type(struct Context *context, char input);

void result_render(struct Context *context);
void result_on_key_type(struct Context *context, char input);

int main(void) {
    srand((unsigned)time(NULL));
    if (setlocale(LC_CTYPE, "") == NULL) {
        // cannot use failure macro because TERMINAL is not initialized.
        fprintf(stderr, "failed to set locale");
        return 1;
    }

    TERMINAL = get_term();

    struct Context context = {
        .running = true,
        .gaming = true,
        .last_key_code = 0,
        .game = {
            .words = random_words(20),
            .char_index = 0,
            .roma_index = 0,
            .previous_collect = true,
        },
        .result = { 0 },
    };

    term_set_cursor_visible(&TERMINAL, false);

    do {
        if (context.gaming) {
            game_render(&context);
        } else {
            result_render(&context);
        }

        const struct TerminalEvent event = term_poll_event(&TERMINAL);

        switch (event.type) {
        case KEYBOARD_EVENT:
            const char keycode = event.keyboard_event.key_code;
            context.last_key_code = keycode;
            if (keycode == ESC_KEY) {
                context.running = false;
            }

            if (context.gaming) {
                game_on_key_type(&context, keycode);
            } else {
                result_on_key_type(&context, keycode);
            }

            break;

        case RESIZE_EVENT:
            break;
        }

        term_clear(&TERMINAL);
    } while (context.running);

    term_reset(&TERMINAL);
    return 0;
}

void game_render(struct Context *context) {
    if (context->game.words.length == 0) {
        // ゲーム終わり
        context->gaming = false;
        context->result.ended_on = clock();
        return;
    }

    const struct Word current_word
        = context->game.words.pointer[context->game.words.length - 1];
    struct CharVector current_char
        = string_at(current_word.pointer, context->game.char_index);
    const struct StringSlice *romas = get_roma(current_char.pointer);

    term_set_fg(&TERMINAL, WHITE);
    term_set_bg(&TERMINAL, BLACK);

    term_print(&TERMINAL, "ESC: 終了 debug: %d %d %d %s\n",
               context->last_key_code, context->result.total_types,
               context->result.miss_types,
               context->game.previous_collect ? "true" : "false");
    term_print(&TERMINAL, "NEXT: ");

    {
        assert(romas->length > 0, "romas was empty");

        const char *roma = romas->pointer[0];
        for (size_t i = 0; roma[i] != '\0'; i++) {
            if (i < context->game.roma_index) {
                term_set_fg(&TERMINAL, GREEN);
            } else if (i == context->game.roma_index) {
                term_set_fg(&TERMINAL, BLACK);
                term_set_bg(&TERMINAL, RED);
            }

            term_print(&TERMINAL, "%c", roma[i]);

            term_set_fg(&TERMINAL, WHITE);
            term_set_bg(&TERMINAL, BLACK);
        }
    }

    term_print(&TERMINAL, "\n");

    {
        for (size_t i = 0; i < current_word.char_count; i++) {
            if (i < context->game.char_index) {
                term_set_fg(&TERMINAL, GREEN);
            } else if (i == context->game.char_index) {
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
        context->result.miss_types += 1;
    }

    context->game.previous_collect = is_correct;
}

void result_render(struct Context *context) {
    const struct GameResult result = context->result;
    const double gameDurationSec = (double)(result.ended_on - result.started_on)
                                   / (double)CLOCKS_PER_SEC;
    const double keys_per_sec = result.total_types / gameDurationSec;

    term_print(&TERMINAL, "終了: esc もう一度プレイ: space\n\n");

    term_print(&TERMINAL, "ゲーム結果\n");
    term_print(&TERMINAL, "かかった時間(秒): %02.02f\n", gameDurationSec);
    term_print(&TERMINAL, "総キータイプ数: %d\n", result.total_types);
    term_print(&TERMINAL, "ミスタイプ数: %d\n", result.miss_types);
    term_print(&TERMINAL, "秒間キータイプ数: %02.02f\n", keys_per_sec);
}

void result_on_key_type(struct Context *context, char input) {
    // todo
}
