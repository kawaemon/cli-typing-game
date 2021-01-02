#include <locale.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define failure(...)                                                           \
    fprintf(stderr, __VA_ARGS__);                                              \
    exit(1)

enum Color { RED, GREEN };

struct Terminal {
    HANDLE console_handle;
    CONSOLE_SCREEN_BUFFER_INFO origin_info;
};

struct Terminal get_terminal();
void set_term_fg(struct Terminal *terminal, enum Color color);
void set_term_bg(struct Terminal *terminal, enum Color color);
void term_reset(struct Terminal *terminal);
uint32_t string_len(const char *text);
const char *string_at(const char *src, size_t pos);

int main() {
    struct Terminal terminal = get_terminal();

    if (setlocale(LC_CTYPE, "") == NULL) {
        failure("failed to set locale");
    }

    const char *text = "‚ ‚¢‚¤‚¦‚¨";
    puts(text);
    printf("len: %u\n", string_len(text));

    for (int i = 0;; i++) {
        const char *fragment = string_at(text, i);

        if (fragment == NULL) {
            break;
        }

        printf("pos %d: %s\n", i, fragment);
        free((void *)fragment);
    }

    term_reset(&terminal);
    return 0;
}

struct Terminal get_terminal() {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO console_info;
    GetConsoleScreenBufferInfo(console_handle, &console_info);

    struct Terminal result = {console_handle, console_info};
    return result;
}

void set_term_fg(struct Terminal *terminal, enum Color color) {
    WORD win_color;

    switch (color) {
    case RED:
        win_color = FOREGROUND_RED;
        break;
    case GREEN:
        win_color = FOREGROUND_GREEN;
        break;
    }

    SetConsoleTextAttribute(terminal->console_handle, win_color);
}

void set_term_bg(struct Terminal *terminal, enum Color color) {
    WORD win_color;

    switch (color) {
    case RED:
        win_color = BACKGROUND_RED;
        break;
    case GREEN:
        win_color = BACKGROUND_GREEN;
        break;
    }

    SetConsoleTextAttribute(terminal->console_handle, win_color);
}

void term_reset(struct Terminal *terminal) {
    SetConsoleTextAttribute(terminal->console_handle,
                            terminal->origin_info.wAttributes);
}

uint32_t string_len(const char *text) {
    int32_t count = 0;
    size_t index = 0;

    while (text[index] != '\0') {
        int result = mblen(&text[index], MB_CUR_MAX);

        if (result < 0) {
            failure("mblen returned error: %d", result);
        }

        index += result;
        count += 1;
    }

    return count;
}

// requires to be freed!
const char *string_at(const char *src, size_t target_pos) {
    size_t index = 0;
    size_t pos = 0;

    while (src[index] != '\0') {
        int len = mblen(&src[index], MB_CUR_MAX);

        if (len < 0) {
            failure("mblen returned error: %d", len);
        }

        if (pos == target_pos) {
            char *output = malloc(len + 1);

            for (size_t i = 0; i < len; i++) {
                output[i] = src[index + i];
            }

            output[len] = '\0';
            return output;
        }

        index += len;
        pos += 1;
    }

    return NULL;
}
