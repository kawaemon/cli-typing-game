#include "assert.h"
#include "char_vector.h"
#include "terminal.h"
#include "word.h"
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ESC_KEY 27

struct Terminal TERMINAL;

int main(void) {
    srand((unsigned)time(NULL));
    if (setlocale(LC_CTYPE, "") == NULL) {
        // cannot use failure macro because TERMINAL is not initialized.
        printf("failed to set locale");
        return 1;
    }

    TERMINAL = get_term();

    struct WordVector words = word_vector_new();
    for (int i = 0; i < 10; i++) {
        word_vector_push(&words, random_word());
    }

    while (true) {
        for (size_t i = 0; i < words.length; i++) {
            term_print(&TERMINAL, words.pointer[i].pointer);
            term_print(&TERMINAL, "\n");
        }

        const struct TerminalEvent event = term_poll_event(&TERMINAL);

        if (event.type == KEYBOARD_EVENT
            && event.keyboard_event.key_code == ESC_KEY) {
            break;
        }

        term_clear(&TERMINAL);
    }

    term_reset(&TERMINAL);
    return 0;
}
