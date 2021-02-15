#pragma once
#include <Windows.h>
#include <stdbool.h>

// 背景色等の指定に使う色の列挙型
enum TerminalColor { RED, GREEN, WHITE, BLACK };

// ターミナルの制御に使う情報をまとめる構造体
struct Terminal {
    HANDLE console_handle;
    HANDLE stdin_handle;
    HANDLE game_buffer;
    CONSOLE_SCREEN_BUFFER_INFO origin_buffer_info;
    CONSOLE_CURSOR_INFO origin_cursor_info;
    WORD current_text_attributes;
};

// ターミナルで発生するイベントの種類の列挙型
enum TerminalEventType { KEYBOARD_EVENT, RESIZE_EVENT };

// キーボードイベント
struct TerminalKeyboardEvent {
    // 押されたキー
    char key_code;
};

// ターミナルで発生したイベントを表す構造体
struct TerminalEvent {
    // 起きたイベントの種類
    enum TerminalEventType type;

    // イベントがキーボードイベントの場合、この中に情報が格納される
    struct TerminalKeyboardEvent keyboard_event;
};

// Terminal 構造体を取得する関数
struct Terminal get_term(void);

// ターミナルを空白にする関数
void term_clear(struct Terminal *terminal);

// カーソルの可視状態を設定する関数
void term_set_cursor_visible(struct Terminal *terminal, bool visible);

// ターミナルの前景色を設定する関数
void term_set_fg(struct Terminal *terminal, enum TerminalColor color);

// ターミナルの背景色を設定する関数
void term_set_bg(struct Terminal *terminal, enum TerminalColor color);

// カーソルの場所を設定する関数
void term_set_cursor_pos(struct Terminal *terminal, size_t x, size_t y);

// Terminal 構造体を破壊して元の状態に戻す関数
void term_reset(struct Terminal *terminal);

// ターミナルから1文字入力を取得する関数。term_poll_eventに置き換えられたため使われていない。
char term_get_char(void);

// ターミナルに文字を表示する関数。printfに非常に似ている。
void term_print(struct Terminal *terminal, const char *format, ...);

// ターミナルから1イベント取得する関数。
struct TerminalEvent term_poll_event(struct Terminal *terminal);
