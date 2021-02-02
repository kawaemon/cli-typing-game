#include <conio.h>
#include <locale.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define ESC_KEY 27

#define printfln(text, ...) printf(text "\n", __VA_ARGS__)
#define fprintfln(stream, text, ...) fprintf(stream, text "\n", __VA_ARGS__)

#define failure(msg, ...)                                                      \
    failure_hook();                                                            \
    fprintfln(stderr, "at L%d: " msg, __LINE__, __VA_ARGS__);                  \
    exit(1);

#define unreachable() failure("expected to be unreachable code executed");

#define assert(condition, msg, ...)                                            \
    if (!(condition)) {                                                        \
        failure(msg, __VA_ARGS__);                                             \
    }

#define safe_free(pointer)                                                     \
    if (pointer == NULL) {                                                     \
        failure("at line %d, attempted to free NULL pointer.\n", __LINE__);    \
    }                                                                          \
    free((void *)pointer);

#define fmt_bool(b) b ? "true" : "false"

enum Color { RED, GREEN, WHITE };

struct Terminal {
    HANDLE console_handle;
    HANDLE game_buffer;
    CONSOLE_SCREEN_BUFFER_INFO origin_buffer_info;
    CONSOLE_CURSOR_INFO origin_cursor_info;
};

struct String {
    const char *text;
    size_t char_count;
};

struct Sentence {
    struct String hiragana;
    struct String origin;
};

struct SentenceSlice {
    const size_t length;
    struct Sentence *pointer;
};

struct StringSlice {
    const size_t length;
    const char **pointer;
};

// https://docs.microsoft.com/en-us/windows/console/console-functions
void failure_hook(void);
struct Terminal get_term(void);
void term_clear(struct Terminal *terminal);
void set_term_cursor_visible(struct Terminal *terminal, bool visible);
void set_term_fg(struct Terminal *terminal, enum Color color);
void set_term_bg(struct Terminal *terminal, enum Color color);
void term_reset(struct Terminal *terminal);
uint32_t string_len(const char *text);
size_t string_bytes(const char *text);
const char *string_at(const char *src, size_t pos);
bool string_eq(const char *a, const char *b);
const struct SentenceSlice gen_random_words(size_t len);
const struct Sentence random_word(void);
const struct StringSlice *get_roma(const char *hiragana);

// GLOBAL VARIABLE DEFINITION
struct Terminal TERMINAL;

int main(void) {
    srand((unsigned)time(NULL));
    if (setlocale(LC_CTYPE, "") == NULL) {
        // cannot use failure macro because TERMINAL is not initialized.
        fprintfln(stderr, "failed to set locale");
        return 1;
    }

    TERMINAL = get_term();

    const struct Sentence word = random_word();
    size_t char_index = 0;
    size_t char_inner_index = 0;

    while (true) {
        term_clear(&TERMINAL);

        // render
        {
            size_t loop_char_index = 0;
            for (size_t index = 0; index < word.origin.char_count; index++) {
                const char *character = string_at(word.origin.text, index);

                if (string_eq(character, "|")) {
                    loop_char_index += 1;
                    continue;
                }

                bool is_typed = loop_char_index < char_index;

                if (is_typed) {
                    set_term_fg(&TERMINAL, GREEN);
                }

                DWORD data;
                WriteConsole(TERMINAL.game_buffer, character,
                             string_bytes(character), &data, NULL);

                if (is_typed) {
                    set_term_fg(&TERMINAL, WHITE);
                }

                safe_free(character);
            }
        }

        set_term_cursor_visible(&TERMINAL, false);
        const int input = _getch();

        if (input == ESC_KEY) {
            break;
        }

        const char *current_char = string_at(word, char_index);
        const struct StringSlice *romas = get_roma(current_char);
        safe_free(current_char);

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

    term_reset(&TERMINAL);
    return 0;
}

void failure_hook() { term_reset(&TERMINAL); }

struct Terminal get_term() {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO buffer_info;
    GetConsoleScreenBufferInfo(console_handle, &buffer_info);

    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(console_handle, &cursor_info);

    HANDLE game_buffer = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        CONSOLE_TEXTMODE_BUFFER, NULL);

    SetConsoleActiveScreenBuffer(game_buffer);

    struct Terminal result = {.console_handle = console_handle,
                              .game_buffer = game_buffer,
                              .origin_buffer_info = buffer_info,
                              .origin_cursor_info = cursor_info};

    return result;
}

void term_clear(struct Terminal *terminal) {
    DWORD size = terminal->origin_buffer_info.dwSize.X *
                 terminal->origin_buffer_info.dwSize.Y;
    COORD pos = {0, 0};
    DWORD written;
    FillConsoleOutputCharacter(terminal->game_buffer, ' ', size, pos, &written);
    SetConsoleCursorPosition(terminal->game_buffer, pos);
}

void set_term_cursor_visible(struct Terminal *terminal, bool visible) {
    assert(terminal != NULL,
           "passed NULL to set_term_cursor_visible(terminal)");

    CONSOLE_CURSOR_INFO info = terminal->origin_cursor_info;

    info.bVisible = visible;

    SetConsoleCursorInfo(terminal->game_buffer, &info);
}

void set_term_fg(struct Terminal *terminal, enum Color color) {
    assert(terminal != NULL, "passed NULL to set_term_fg(terminal)");

    WORD win_color;

    switch (color) {
    case RED:
        win_color = FOREGROUND_RED;
        break;
    case GREEN:
        win_color = FOREGROUND_GREEN;
        break;
    case WHITE:
        win_color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    default:
        unreachable();
    }

    SetConsoleTextAttribute(terminal->game_buffer, win_color);
}

void set_term_bg(struct Terminal *terminal, enum Color color) {
    assert(terminal != NULL, "passed NULL to set_term_bg(terminal)");

    WORD win_color;

    switch (color) {
    case RED:
        win_color = BACKGROUND_RED;
        break;
    case GREEN:
        win_color = BACKGROUND_GREEN;
        break;
    case WHITE:
        win_color = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
        break;
    default:
        unreachable();
    }

    SetConsoleTextAttribute(terminal->game_buffer, win_color);
}

void term_reset(struct Terminal *terminal) {
    assert(terminal != NULL, "passed NULL to term_reset(terminal)");

    SetConsoleTextAttribute(terminal->console_handle,
                            terminal->origin_buffer_info.wAttributes);
    SetConsoleCursorInfo(terminal->console_handle,
                         &terminal->origin_cursor_info);
    SetConsoleActiveScreenBuffer(terminal->console_handle);
}

uint32_t string_len(const char *text) {
    assert(text != NULL, "passed NULL to string_len(text)");

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

size_t string_bytes(const char *text) {
    assert(text != NULL, "passed NULL to string_bytes(text)");
    size_t result = 0;

    while (true) {
        if (text[result] == '\0') {
            return result;
        }
        result += 1;
    }
}

// requires to be freed!
const char *string_at(const char *src, size_t target_pos) {
    assert(src != NULL, "passed NULL to string_at(src)");

    size_t index = 0;
    size_t pos = 0;

    while (src[index] != '\0') {
        int len = mblen(&src[index], MB_CUR_MAX);

        if (len < 0) {
            failure("mblen returned error: %d", len);
        }

        if (pos == target_pos) {
            char *output = malloc(len + 1);

            for (size_t i = 0; i < (size_t)len; i++) {
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
    assert(a != NULL, "passed NULL to string_at(a)");
    assert(b != NULL, "passed NULL to string_at(b)");

    for (size_t index = 0;; index++) {
        if (a[index] != b[index]) {
            return false;
        }

        if (a[index] == '\0') {
            return true;
        }
    }
}

// requires to be freed!
const struct SentenceSlice gen_random_words(size_t len) {
    struct Sentence *result_mem = malloc(sizeof(struct Sentence) * len);

    for (size_t i = 0; i < len; i++) {
        result_mem[i] = random_word();
    }

    const struct SentenceSlice result = {.length = len, .pointer = result_mem};
    return result;
}

#define STRING_DEFINITION(TEXT)                                                \
    { .text = TEXT, .char_count = string_len(TEXT) }

const struct Sentence random_word() {
    const struct Sentence word1 = {
        .hiragana =
            STRING_DEFINITION("わたし|は|ぷ|ろ|ぐ|ら|み|ん|ぐ|が|す|き|で|す"),
        .origin =
            STRING_DEFINITION("私|は|プ|ロ|グ|ラ|ミ|ン|グ|が|好|き|で|す"),
    };

    // TODO: multiple sentence
    return word1;
}

#define __GET_ROMA_IMPL(HIRAGANA, ROMAS_LEN, ...)                              \
    if (string_eq(hiragana, HIRAGANA)) {                                       \
        const static char *romas[] = {__VA_ARGS__};                            \
        const static struct StringSlice data = {.length = ROMAS_LEN,           \
                                                .pointer = romas};             \
        return &data;                                                          \
    }

const struct StringSlice *get_roma(const char *hiragana) {
    assert(hiragana != NULL, "passed NULL to get_roma(hiragana)");

    __GET_ROMA_IMPL("あ", 1, "a")
    __GET_ROMA_IMPL("い", 1, "i")
    __GET_ROMA_IMPL("う", 1, "u")
    __GET_ROMA_IMPL("え", 1, "e")
    __GET_ROMA_IMPL("お", 1, "o")
    __GET_ROMA_IMPL("か", 1, "ka")
    __GET_ROMA_IMPL("き", 1, "ki")
    __GET_ROMA_IMPL("く", 1, "ku")
    __GET_ROMA_IMPL("け", 1, "ke")
    __GET_ROMA_IMPL("こ", 1, "ko")
    __GET_ROMA_IMPL("が", 1, "ga")
    __GET_ROMA_IMPL("ぎ", 1, "gi")
    __GET_ROMA_IMPL("ぐ", 1, "gu")
    __GET_ROMA_IMPL("げ", 1, "ge")
    __GET_ROMA_IMPL("ご", 1, "go")
    __GET_ROMA_IMPL("さ", 1, "sa")
    __GET_ROMA_IMPL("し", 1, "si")
    __GET_ROMA_IMPL("す", 1, "su")
    __GET_ROMA_IMPL("せ", 1, "se")
    __GET_ROMA_IMPL("そ", 1, "so")
    __GET_ROMA_IMPL("ざ", 1, "za")
    __GET_ROMA_IMPL("じ", 2, "zi", "ji")
    __GET_ROMA_IMPL("ず", 1, "zu")
    __GET_ROMA_IMPL("ぜ", 1, "ze")
    __GET_ROMA_IMPL("ぞ", 1, "zo")
    __GET_ROMA_IMPL("た", 1, "ta")
    __GET_ROMA_IMPL("ち", 2, "ti", "chi")
    __GET_ROMA_IMPL("つ", 2, "tu", "tsu")
    __GET_ROMA_IMPL("て", 1, "te")
    __GET_ROMA_IMPL("と", 1, "to")
    __GET_ROMA_IMPL("だ", 1, "da")
    __GET_ROMA_IMPL("ぢ", 1, "di")
    __GET_ROMA_IMPL("づ", 1, "du")
    __GET_ROMA_IMPL("で", 1, "de")
    __GET_ROMA_IMPL("ど", 1, "do")
    __GET_ROMA_IMPL("な", 1, "na")
    __GET_ROMA_IMPL("に", 1, "ni")
    __GET_ROMA_IMPL("ぬ", 1, "nu")
    __GET_ROMA_IMPL("ね", 1, "ne")
    __GET_ROMA_IMPL("の", 1, "no")
    __GET_ROMA_IMPL("は", 1, "ha")
    __GET_ROMA_IMPL("ひ", 1, "hi")
    __GET_ROMA_IMPL("ふ", 2, "hu", "fu")
    __GET_ROMA_IMPL("へ", 1, "he")
    __GET_ROMA_IMPL("ほ", 1, "ho")
    __GET_ROMA_IMPL("ば", 1, "ba")
    __GET_ROMA_IMPL("び", 1, "bi")
    __GET_ROMA_IMPL("ぶ", 1, "bu")
    __GET_ROMA_IMPL("べ", 1, "be")
    __GET_ROMA_IMPL("ぼ", 1, "bo")
    __GET_ROMA_IMPL("ぱ", 1, "pa")
    __GET_ROMA_IMPL("ぴ", 1, "pi")
    __GET_ROMA_IMPL("ぷ", 1, "pu")
    __GET_ROMA_IMPL("ぺ", 1, "pe")
    __GET_ROMA_IMPL("ぽ", 1, "po")
    __GET_ROMA_IMPL("ま", 1, "ma")
    __GET_ROMA_IMPL("み", 1, "mi")
    __GET_ROMA_IMPL("む", 1, "mu")
    __GET_ROMA_IMPL("め", 1, "me")
    __GET_ROMA_IMPL("も", 1, "mo")
    __GET_ROMA_IMPL("や", 1, "ya")
    __GET_ROMA_IMPL("ゆ", 1, "yu")
    __GET_ROMA_IMPL("よ", 1, "yo")
    __GET_ROMA_IMPL("ら", 1, "ra")
    __GET_ROMA_IMPL("り", 1, "ri")
    __GET_ROMA_IMPL("る", 1, "ru")
    __GET_ROMA_IMPL("れ", 1, "re")
    __GET_ROMA_IMPL("ろ", 1, "ro")
    __GET_ROMA_IMPL("わ", 1, "wa")
    __GET_ROMA_IMPL("を", 1, "wo")
    __GET_ROMA_IMPL("ん", 1, "nn")
    __GET_ROMA_IMPL("ぁ", 2, "xa", "la")
    __GET_ROMA_IMPL("ぃ", 2, "xi", "li")
    __GET_ROMA_IMPL("ぅ", 2, "xu", "lu")
    __GET_ROMA_IMPL("ぇ", 2, "xe", "le")
    __GET_ROMA_IMPL("ぉ", 2, "xo", "lo")
    __GET_ROMA_IMPL("っ", 2, "xtu", "ltu")

    return NULL;
}
