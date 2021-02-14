#include <conio.h>
#include <stdarg.h>
#include <stdio.h>

#include "assert.h"
#include "string.h"
#include "terminal.h"

struct Terminal get_term() {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE stdin_handle = GetStdHandle(STD_INPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO buffer_info;
    GetConsoleScreenBufferInfo(console_handle, &buffer_info);

    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(console_handle, &cursor_info);

    HANDLE game_buffer = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        CONSOLE_TEXTMODE_BUFFER, NULL);

    SetConsoleActiveScreenBuffer(game_buffer);

    return (struct Terminal){
        .console_handle = console_handle,
        .stdin_handle = stdin_handle,
        .game_buffer = game_buffer,
        .origin_buffer_info = buffer_info,
        .origin_cursor_info = cursor_info,
    };
}

void term_clear(struct Terminal *terminal) {
    DWORD size = terminal->origin_buffer_info.dwSize.X
                 * terminal->origin_buffer_info.dwSize.Y;
    COORD pos = {0, 0};
    DWORD written;
    FillConsoleOutputCharacter(terminal->game_buffer, ' ', size, pos, &written);
    SetConsoleCursorPosition(terminal->game_buffer, pos);
}

void term_set_cursor_visible(struct Terminal *terminal, bool visible) {
    assert(terminal != NULL,
           "passed NULL to term_set_cursor_visible(terminal)");

    CONSOLE_CURSOR_INFO info = terminal->origin_cursor_info;

    info.bVisible = visible;

    SetConsoleCursorInfo(terminal->game_buffer, &info);
}

void term_set_fg(struct Terminal *terminal, enum TerminalColor color) {
    assert(terminal != NULL, "passed NULL to term_set_fg(terminal)");

    WORD win_color = 0;

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
    case BLACK:
        break;
    default:
        unreachable();
    }

    SetConsoleTextAttribute(terminal->game_buffer, win_color);
}

void term_set_bg(struct Terminal *terminal, enum TerminalColor color) {
    assert(terminal != NULL, "passed NULL to term_set_bg(terminal)");

    WORD win_color = 0;

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
    case BLACK:
        break;
    default:
        unreachable();
    }

    SetConsoleTextAttribute(terminal->game_buffer, win_color);
}

void term_set_cursor_pos(struct Terminal *terminal, size_t x, size_t y) {
    assert(terminal != NULL, "passed NULL to term_set_cursor_pos(terminal)");

    COORD pos = {x, y};
    SetConsoleCursorPosition(terminal->game_buffer, pos);
}

// destroys terminal object
void term_reset(struct Terminal *terminal) {
    assert(terminal != NULL, "passed NULL to term_reset(terminal)");

    SetConsoleTextAttribute(terminal->console_handle,
                            terminal->origin_buffer_info.wAttributes);
    SetConsoleCursorInfo(terminal->console_handle,
                         &terminal->origin_cursor_info);
    SetConsoleActiveScreenBuffer(terminal->console_handle);
    CloseHandle(terminal->game_buffer);
    CloseHandle(terminal->stdin_handle);
    CloseHandle(terminal->console_handle);
}

char term_get_char() {
    return _getch();
}

void term_print(struct Terminal *terminal, const char *format, ...) {
    va_list args;

    va_start(args, format);
    struct CharVector text = vformat(format, args);
    va_end(args);

    if (!WriteConsole(terminal->game_buffer, text.pointer, text.length, NULL,
                      NULL)) {
        failure("failed to write to console: %d", GetLastError());
    }

    char_vector_free(&text);
}

struct TerminalEvent term_poll_event(struct Terminal *terminal) {
    INPUT_RECORD buffer[1];
    DWORD read;

    if (!ReadConsoleInput(terminal->stdin_handle, buffer, 1, &read)) {
        failure("failed to read console input: %d", GetLastError());
    }

    if (read == 0) {
        failure("failed to poll event");
    }

    const INPUT_RECORD event = buffer[0];

    switch (event.EventType) {
    case KEY_EVENT:
        return (struct TerminalEvent) {
            .type = KEYBOARD_EVENT,
            .keyboard_event = {
                .key_code = event.Event.KeyEvent.uChar.AsciiChar,
            },
        };

    case WINDOW_BUFFER_SIZE_EVENT:
        return (struct TerminalEvent){
            .type = RESIZE_EVENT,
        };

    default:
        // ignore other events and repoll
        return term_poll_event(terminal);
    }
}
