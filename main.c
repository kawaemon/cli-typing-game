#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

enum Color { RED, GREEN };

struct Terminal {
    HANDLE console_handle;
    CONSOLE_SCREEN_BUFFER_INFO origin_info;
};

struct Terminal get_terminal();
void set_term_fg(struct Terminal *terminal, enum Color color);
void set_term_bg(struct Terminal *terminal, enum Color color);
void term_reset(struct Terminal *terminal);

int main() {
    struct Terminal terminal = get_terminal();

    set_term_fg(&terminal, GREEN);
    puts("‚±‚ñ‚É‚¿‚Í");
    term_reset(&terminal);

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
