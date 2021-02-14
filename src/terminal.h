#pragma once
#include <Windows.h>
#include <stdbool.h>

enum TerminalColor { RED, GREEN, WHITE, BLACK };

struct Terminal {
    HANDLE console_handle;
    HANDLE stdin_handle;
    HANDLE game_buffer;
    CONSOLE_SCREEN_BUFFER_INFO origin_buffer_info;
    CONSOLE_CURSOR_INFO origin_cursor_info;
};

enum TerminalEventType { KEYBOARD_EVENT, RESIZE_EVENT };

struct TerminalKeyboardEvent {
    char key_code;
};

struct TerminalEvent {
    enum TerminalEventType type;
    struct TerminalKeyboardEvent keyboard_event;
};

struct Terminal get_term(void);
void term_clear(struct Terminal *terminal);
void term_set_cursor_visible(struct Terminal *terminal, bool visible);
void term_set_fg(struct Terminal *terminal, enum TerminalColor color);
void term_set_bg(struct Terminal *terminal, enum TerminalColor color);
void term_set_cursor_pos(struct Terminal *terminal, size_t x, size_t y);
void term_reset(struct Terminal *terminal);
char term_get_char(void);
void term_print(struct Terminal *terminal, const char *format, ...);
struct TerminalEvent term_poll_event(struct Terminal *terminal);
