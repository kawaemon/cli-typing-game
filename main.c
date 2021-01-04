#include <conio.h>
#include <locale.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define ESC_KEY 27

#define failure(...)                                                           \
    fprintf(stderr, __VA_ARGS__);                                              \
    exit(1)

#define fmt_bool(b) b ? "true" : "false"

enum Color { RED, GREEN };

struct Terminal {
    HANDLE console_handle;
    CONSOLE_SCREEN_BUFFER_INFO origin_buffer_info;
    CONSOLE_CURSOR_INFO origin_cursor_info;
};

struct StringSlice {
    const size_t length;
    const char **pointer;
};

// https://docs.microsoft.com/en-us/windows/console/console-functions
struct Terminal get_term();
void set_term_cursor_visible(struct Terminal *terminal, bool visible);
void set_term_fg(struct Terminal *terminal, enum Color color);
void set_term_bg(struct Terminal *terminal, enum Color color);
void term_reset(struct Terminal *terminal);
uint32_t string_len(const char *text);
const char *string_at(const char *src, size_t pos);
bool string_eq(const char *a, const char *b);
const struct StringSlice *get_roma(const char *hiragana);

int main() {
    if (setlocale(LC_CTYPE, "") == NULL) {
        failure("failed to set locale");
    }

    struct Terminal terminal = get_term();

    const char *word = "‚ ‚¢‚·‚ª‚½‚×‚½‚¢‚È";

    const uint32_t word_len = string_len(word);
    size_t char_index = 0;
    size_t char_inner_index = 0;

    while (true) {
        printf("\r");
        for (size_t index = 0; index < word_len; index++) {
            const char *character = string_at(word, index);
            bool is_typed = index < char_index;

            if (is_typed) {
                set_term_fg(&terminal, GREEN);
            }

            printf("%s", character);

            if (is_typed) {
                term_reset(&terminal);
            }
        }

        set_term_cursor_visible(&terminal, false);
        const char input = _getch();

        if (input == ESC_KEY) {
            break;
        }

        const char *current_char = string_at(word, char_index);
        const struct StringSlice *romas = get_roma(current_char);
        free((void *)current_char);

        for (size_t roma_index = 0; roma_index < romas->length; roma_index++) {
            const char *roma = romas->pointer[roma_index];

            // this is valid because roma contains only single byte characters.
            if (input == roma[char_inner_index]) {
                char_inner_index += 1;
                if (char_inner_index == string_len(roma)) {
                    // TODO: check word bound
                    char_index += 1;
                    char_inner_index = 0;
                }
            }
        }
    }

    term_reset(&terminal);
    return 0;
}

struct Terminal get_term() {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO buffer_info;
    GetConsoleScreenBufferInfo(console_handle, &buffer_info);

    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(console_handle, &cursor_info);

    struct Terminal result = {console_handle, buffer_info, cursor_info};
    return result;
}

void set_term_cursor_visible(struct Terminal *terminal, bool visible) {
    CONSOLE_CURSOR_INFO info = terminal->origin_cursor_info;

    info.bVisible = visible;

    SetConsoleCursorInfo(terminal->console_handle, &info);
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
                            terminal->origin_buffer_info.wAttributes);
    SetConsoleCursorInfo(terminal->console_handle,
                         &terminal->origin_cursor_info);
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

bool string_eq(const char *a, const char *b) {
    for (size_t index = 0;; index++) {
        if (a[index] != b[index]) {
            return false;
        }

        if (a[index] == '\0') {
            return true;
        }
    }
}

#define __GET_ROMA_IMPL(HIRAGANA, ROMAS_LEN, ...)                              \
    if (string_eq(hiragana, HIRAGANA)) {                                       \
        const static char *romas[] = {__VA_ARGS__};                            \
        const static struct StringSlice data = {ROMAS_LEN, romas};             \
        return &data;                                                          \
    }

const struct StringSlice *get_roma(const char *hiragana) {
    __GET_ROMA_IMPL("‚ ", 1, "a")
    __GET_ROMA_IMPL("‚¢", 1, "i")
    __GET_ROMA_IMPL("‚¤", 1, "u")
    __GET_ROMA_IMPL("‚¦", 1, "e")
    __GET_ROMA_IMPL("‚¨", 1, "o")
    __GET_ROMA_IMPL("‚©", 1, "ka")
    __GET_ROMA_IMPL("‚«", 1, "ki")
    __GET_ROMA_IMPL("‚­", 1, "ku")
    __GET_ROMA_IMPL("‚¯", 1, "ke")
    __GET_ROMA_IMPL("‚±", 1, "ko")
    __GET_ROMA_IMPL("‚ª", 1, "ga")
    __GET_ROMA_IMPL("‚¬", 1, "gi")
    __GET_ROMA_IMPL("‚®", 1, "gu")
    __GET_ROMA_IMPL("‚°", 1, "ge")
    __GET_ROMA_IMPL("‚²", 1, "go")
    __GET_ROMA_IMPL("‚³", 1, "sa")
    __GET_ROMA_IMPL("‚µ", 1, "si")
    __GET_ROMA_IMPL("‚·", 1, "su")
    __GET_ROMA_IMPL("‚¹", 1, "se")
    __GET_ROMA_IMPL("‚»", 1, "so")
    __GET_ROMA_IMPL("‚´", 1, "za")
    __GET_ROMA_IMPL("‚¶", 2, "zi", "ji")
    __GET_ROMA_IMPL("‚¸", 1, "zu")
    __GET_ROMA_IMPL("‚º", 1, "ze")
    __GET_ROMA_IMPL("‚¼", 1, "zo")
    __GET_ROMA_IMPL("‚½", 1, "ta")
    __GET_ROMA_IMPL("‚¿", 2, "ti", "chi")
    __GET_ROMA_IMPL("‚Â", 2, "tu", "tsu")
    __GET_ROMA_IMPL("‚Ä", 1, "te")
    __GET_ROMA_IMPL("‚Æ", 1, "to")
    __GET_ROMA_IMPL("‚¾", 1, "da")
    __GET_ROMA_IMPL("‚À", 1, "di")
    __GET_ROMA_IMPL("‚Ã", 1, "du")
    __GET_ROMA_IMPL("‚Å", 1, "de")
    __GET_ROMA_IMPL("‚Ç", 1, "do")
    __GET_ROMA_IMPL("‚È", 1, "na")
    __GET_ROMA_IMPL("‚É", 1, "ni")
    __GET_ROMA_IMPL("‚Ê", 1, "nu")
    __GET_ROMA_IMPL("‚Ë", 1, "ne")
    __GET_ROMA_IMPL("‚Ì", 1, "no")
    __GET_ROMA_IMPL("‚Í", 1, "ha")
    __GET_ROMA_IMPL("‚Ð", 1, "hi")
    __GET_ROMA_IMPL("‚Ó", 2, "hu", "fu")
    __GET_ROMA_IMPL("‚Ö", 1, "he")
    __GET_ROMA_IMPL("‚Ù", 1, "ho")
    __GET_ROMA_IMPL("‚Î", 1, "ba")
    __GET_ROMA_IMPL("‚Ñ", 1, "bi")
    __GET_ROMA_IMPL("‚Ô", 1, "bu")
    __GET_ROMA_IMPL("‚×", 1, "be")
    __GET_ROMA_IMPL("‚Ú", 1, "bo")
    __GET_ROMA_IMPL("‚Ï", 1, "pa")
    __GET_ROMA_IMPL("‚Ò", 1, "pi")
    __GET_ROMA_IMPL("‚Õ", 1, "pu")
    __GET_ROMA_IMPL("‚Ø", 1, "pe")
    __GET_ROMA_IMPL("‚Û", 1, "po")
    __GET_ROMA_IMPL("‚Ü", 1, "ma")
    __GET_ROMA_IMPL("‚Ý", 1, "mi")
    __GET_ROMA_IMPL("‚Þ", 1, "mu")
    __GET_ROMA_IMPL("‚ß", 1, "me")
    __GET_ROMA_IMPL("‚à", 1, "mo")
    __GET_ROMA_IMPL("‚â", 1, "ya")
    __GET_ROMA_IMPL("‚ä", 1, "yu")
    __GET_ROMA_IMPL("‚æ", 1, "yo")
    __GET_ROMA_IMPL("‚ç", 1, "ra")
    __GET_ROMA_IMPL("‚è", 1, "ri")
    __GET_ROMA_IMPL("‚é", 1, "ru")
    __GET_ROMA_IMPL("‚ê", 1, "re")
    __GET_ROMA_IMPL("‚ë", 1, "ro")
    __GET_ROMA_IMPL("‚í", 1, "wa")
    __GET_ROMA_IMPL("‚ð", 1, "wo")
    __GET_ROMA_IMPL("‚ñ", 1, "nn")
    __GET_ROMA_IMPL("‚Ÿ", 2, "xa", "la")
    __GET_ROMA_IMPL("‚¡", 2, "xi", "li")
    __GET_ROMA_IMPL("‚£", 2, "xu", "lu")
    __GET_ROMA_IMPL("‚¥", 2, "xe", "le")
    __GET_ROMA_IMPL("‚§", 2, "xo", "lo")

    return NULL;
}
