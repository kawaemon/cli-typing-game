#include "assert.h"
#include "terminal.h"
#include "vector.h"
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
    const struct Word *word = random_word();

    while (true) {
        term_print(&TERMINAL, word->pointer);
        const key = term_get_char();

        if (key == ESC_KEY) {
            break;
        }

        term_clear(&TERMINAL);
    }

    term_reset(&TERMINAL);
    return 0;
}
