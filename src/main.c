#include "assert.h"
#include "char_vector.h"
#include "string.h"
#include "terminal.h"
#include "word.h"
#include "roma.h"
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
};

struct Context {
    bool running;
    char last_key_code;
    struct Game game;
};

void render(struct Context *context);

int main(void) {
    srand((unsigned)time(NULL));
    if (setlocale(LC_CTYPE, "") == NULL) {
        // cannot use failure macro because TERMINAL is not initialized.
        printf("failed to set locale");
        return 1;
    }

    TERMINAL = get_term();

    const struct Context context = {
        .running = true,
        .last_key_code = 0,
        .game = {
            .words = random_words(20),
            .char_index = 0,
            .roma_index = 0,
        }
    };

    do {
        const struct TerminalEvent event = term_poll_event(&TERMINAL);

        switch (event.type) {
        case KEYBOARD_EVENT:
            const char keycode = last_key_code = event.keyboard_event.key_code;
            if (keycode == ESC_KEY) {
                running = false;
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

void render(struct Context *context) {
    const struct Word current_word = context->game.words.pointer[0];
    struct CharVector current_char = string_at(current_word.pointer, context->game.char_index);
    const struct StringSlice *romas = get_roma(current_char.pointer);

    term_set_fg(&TERMINAL, WHITE);
    term_set_bg(&TERMINAL, BLACK);

    term_print(&TERMINAL, "ESC: I—¹ debug: %d\n", context->last_key_code);
    term_print(&TERMINAL, "NEXT: ");

    {
        assert(romas.length > 0, "romas was empty");

        const char* roma = romas->pointer[0];
        for(size_t i = 0; roma[i] != '\0'; i++) {
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
        for(size_t i = 0; i < current_word.char_count; i++) {
            if (i < context->game.roma_index) {
                term_set_fg(&TERMINAL, GREEN);
            } else if (i == context->game.roma_index) {
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
