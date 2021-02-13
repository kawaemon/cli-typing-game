#pragma once
#include <Windows.h>
#include <stdbool.h>

enum TerminalColor { RED, GREEN, WHITE };

struct Terminal {
    HANDLE console_handle;
    HANDLE game_buffer;
    CONSOLE_SCREEN_BUFFER_INFO origin_buffer_info;
    CONSOLE_CURSOR_INFO origin_cursor_info;
};

struct Terminal get_term(void);
void term_clear(struct Terminal *terminal);
void set_term_cursor_visible(struct Terminal *terminal, bool visible);
void set_term_fg(struct Terminal *terminal, enum TerminalColor color);
void set_term_bg(struct Terminal *terminal, enum TerminalColor color);
void term_reset(struct Terminal *terminal);
char term_get_char(void);
