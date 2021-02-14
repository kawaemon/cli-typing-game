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

int main(void) {
    srand((unsigned)time(NULL));
    if (setlocale(LC_CTYPE, "") == NULL) {
        // cannot use failure macro because TERMINAL is not initialized.
        printf("failed to set locale");
        return 1;
    }

    printf("%s\n",
           format("ÇƒÇ∑Ç∆ÇƒÇ∑Ç∆Ç†Å[Ç†Å[Ç†Å[ %d Ç†Ç¢Ç§Ç¶Ç® %d", 2, 3).pointer);
    return 0;

    TERMINAL = get_term();

    struct WordVector words = word_vector_new();
    for (int i = 0; i < 10; i++) {
        word_vector_push(&words, random_word());
    }

    bool running = true;
    uint32_t resize_count = 0;

    do {
        for (size_t i = 0; i < words.length; i++) {
            // term_print(&TERMINAL, );
            term_print(&TERMINAL, "\n");
        }

        const struct TerminalEvent event = term_poll_event(&TERMINAL);

        switch (event.type) {
        case KEYBOARD_EVENT:
            if (event.keyboard_event.key_code == ESC_KEY) {
                running = false;
            }
            break;
        }

        term_clear(&TERMINAL);
    } while (running);

    term_reset(&TERMINAL);
    return 0;
}
