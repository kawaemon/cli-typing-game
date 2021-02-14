#include "assert.h"
#include "char_vector.h"
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
};

int main(void) {
    srand((unsigned)time(NULL));
    if (setlocale(LC_CTYPE, "") == NULL) {
        // cannot use failure macro because TERMINAL is not initialized.
        printf("failed to set locale");
        return 1;
    }

    TERMINAL = get_term();

    const struct Game game = {
        .words = random_words(20),
        .char_index = 0,
        .roma_index = 0,
    };

    bool running = true;
    char last_key_code = 0;

    do {
        term_print(&TERMINAL, "ESC: I—¹ debug: %d\n", last_key_code);

        for (int i = game.words.length - 1; i >= 0; i--) {
            term_print(&TERMINAL, "%s\n", game.words.pointer[i].pointer);
        }

        term_set_cursor_pos(&TERMINAL, 0, 1);

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
    } while (running);

    term_reset(&TERMINAL);
    return 0;
}
