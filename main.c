#include <stdlib.h>

// メッセージを表示してプログラムを即終了する　
#define failure(msg, ...) failure_hook(__FILE__, __LINE__, msg, __VA_ARGS__);

// プログラムのここには絶対に到達しないことを明示する
#define unreachable() failure("expected to be unreachable code executed");

// condition が false だった時に、メッセージを表示してプログラムを即終了する
#define assert(condition, msg, ...)                                            \
    if (!(condition)) {                                                        \
        failure(msg, __VA_ARGS__);                                             \
    }

// 終了するときに呼ばれる関数のプロトタイプ宣言
void failure_hook(const char *filename, int line, const char *msg, ...);

// 収縮可能な char の配列
struct CharVector {
    // メモリ領域へのポインタ
    char *pointer;
    // 現在の配列の長さ
    size_t length;
    // 現在確保されているメモリ領域の容量
    size_t capacity;
};

// 新しい CharVector を作成する関数
struct CharVector char_vector_new(void);

// self の index 番目の要素を取得する関数
char char_vector_get(struct CharVector *self, size_t index);

// self の最後に value を追加する関数
void char_vector_push(struct CharVector *self, char value);

// self の最後に str を追加する関数
void char_vector_pushstr(struct CharVector *self, const char *str);

// self の最後の要素を消去する関数
char char_vector_pop(struct CharVector *self);

// self を開放する関数
void char_vector_free(struct CharVector *self);

// self に新たにsize領域確保する関数
void char_vector_reserve(struct CharVector *self, size_t size);

#include <stdbool.h>
#include <stdint.h>

struct String {
    const char *text;
    size_t char_count;
};

// text の文字数を返す関数
uint32_t string_len(const char *text);

// text のバイト数を返す関数
size_t string_bytes(const char *text);

// src の target_pos 文字目を返す関数。マルチバイト文字（日本語など）対応。
struct CharVector string_at(const char *src, size_t pos);

// a と b が等しいかを判定する関数
bool string_eq(const char *a, const char *b);

// printf関数が標準出力に対して結果を書き込むのに対し、
// この関数はメモリ上に書き込み、そのメモリ領域を返す関数。
struct CharVector format(const char *format, ...);

// format関数のva_list版
struct CharVector vformat(const char *format, va_list args);

struct StringSlice {
    const size_t length;
    const char **pointer;
};

const struct StringSlice *get_roma(const char *hiragana);

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

#include <stdint.h>

// 単語構造体
struct Word {
    // 文字数
    size_t char_count;
    // 文字
    const char *pointer;
};

// ランダムな単語を返す関数
const struct Word random_word(void);

// 収縮可能なWord構造体の配列
struct WordVector {
    // メモリ領域へのポインタ
    struct Word *pointer;
    // 現在の配列の長さ
    size_t length;
    // 現在確保されているメモリ領域の容量
    size_t capacity;
};

// 新しい WordVector を作成する関数
struct WordVector word_vector_new(void);

// self の index 番目の要素を取得する関数
struct Word word_vector_get(struct WordVector *self, size_t index);

// self の最後に value を追加する関数
void word_vector_push(struct WordVector *self, struct Word value);

// self の最後の要素を消去する関数
struct Word word_vector_pop(struct WordVector *self);

// self を開放する関数
void word_vector_free(struct WordVector *self);

// self に新たにsize領域確保する関数
const struct WordVector random_words(size_t count);

#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ESC_KEY 27

// ターミナルの操作に使う構造体の実態
// プログラム中で1つしか作成しないので、failure_hook関数で使うためにグローバル変数にしてある。
struct Terminal TERMINAL;

// ゲームの情報を格納する構造体
struct Game {
    struct WordVector words;
    size_t char_index;
    size_t roma_index;
    bool previous_correct;
};

// ゲームの結果の情報を格納する構造体
struct GameResult {
    clock_t started_on;
    clock_t ended_on;
    int total_types;
    int miss_types;
};

// プログラムの状態全てを格納する構造体
struct Context {
    bool running;             // プログラムが実行されているか？
    bool gaming;              // タイピングゲーム中か？
    char last_key_code;       // 最後に入力されたキーコード
    struct Game game;         // ゲーム情報
    struct GameResult result; // ゲームの結果情報
};

// ゲーム画面を表示する関数のプロトタイプ宣言
void game_render(struct Context *context);
// ゲーム画面が表示されている時にキー入力されたときに実行される関数のプロトタイプ宣言
void game_on_key_type(struct Context *context, char input);

// リザルド画面を表示する関数のプロトタイプ宣言
void result_render(struct Context *context);
// リザルド画面が表示されている時にキー入力されたときに実行される関数のプロトタイプ宣言
void result_on_key_type(struct Context *context, char input);

int main(void) {
    srand((unsigned)time(NULL));

    // ロケールを設定する。mblen関数の挙動などに影響する
    if (setlocale(LC_CTYPE, "") == NULL) {
        // cannot use failure macro because TERMINAL is not initialized.
        fprintf(stderr, "failed to set locale");
        return 1;
    }

    // ターミナル構造体を初期化してグローバル変数に代入する
    TERMINAL = get_term();

    struct Context context = {
        .running = true,
        .gaming = true,
        .last_key_code = 0,
        .game = {
            .words = random_words(20),
            .char_index = 0,
            .roma_index = 0,
            .previous_correct = true,
        },
        .result = { 0 },
    };

    // カーソルを非表示する
    term_set_cursor_visible(&TERMINAL, false);

    do {
        // 適切な画面を表示する
        if (context.gaming) {
            game_render(&context);
        } else {
            result_render(&context);
        }

        // ターミナルで発生したイベントを取得する
        const struct TerminalEvent event = term_poll_event(&TERMINAL);

        switch (event.type) {
        // キーボードが操作されたとき
        case KEYBOARD_EVENT:
            const char keycode = event.keyboard_event.key_code;
            context.last_key_code = keycode;
            if (keycode == ESC_KEY) {
                // esc キーが押されたときはゲームを終了する
                context.running = false;
            }

            // 画面に応じた関数を呼び出す
            if (context.gaming) {
                game_on_key_type(&context, keycode);
            } else {
                result_on_key_type(&context, keycode);
            }

            break;

        case RESIZE_EVENT:
            break;
        }

        // ターミナルの画面をクリアする
        term_clear(&TERMINAL);
    } while (context.running);

    // ターミナルを元の状態に戻す
    term_reset(&TERMINAL);
    return 0;
}

// ゲーム画面を表示する関数
void game_render(struct Context *context) {
    if (context->game.words.length == 0) {
        // ゲーム終わり
        context->gaming = false;
        context->result.ended_on = clock();
        result_render(context);
        return;
    }

    // 現在の単語は、単語リストの一番最後の単語なので、それを取得する
    const struct Word current_word
        = context->game.words.pointer[context->game.words.length - 1];
    struct CharVector current_char
        = string_at(current_word.pointer, context->game.char_index);

    // 現在の文字を入力できるローマ字のパターンを取得する
    const struct StringSlice *romas = get_roma(current_char.pointer);

    term_set_fg(&TERMINAL, WHITE);
    term_set_bg(&TERMINAL, BLACK);

    term_print(&TERMINAL, "ESC: 終了 debug: %d %d %d %s\n",
               context->last_key_code, context->result.total_types,
               context->result.miss_types,
               context->game.previous_correct ? "true" : "false");
    term_print(&TERMINAL, "NEXT: ");

    {
        assert(romas->length > 0, "romas was empty");

        // ローマ字パターンの一番最初のものを代表として表示する
        const char *roma = romas->pointer[0];
        for (size_t i = 0; roma[i] != '\0'; i++) {
            if (i < context->game.roma_index) {
                // 既に入力済みの文字は緑色で表示する
                term_set_fg(&TERMINAL, GREEN);
            } else if (i == context->game.roma_index) {
                // 現在入力すべき文字は赤色でハイライトする
                term_set_fg(&TERMINAL, BLACK);
                term_set_bg(&TERMINAL, RED);
            }

            term_print(&TERMINAL, "%c", roma[i]);

            // 元の色に戻す
            term_set_fg(&TERMINAL, WHITE);
            term_set_bg(&TERMINAL, BLACK);
        }
    }

    term_print(&TERMINAL, "\n");

    {
        for (size_t i = 0; i < current_word.char_count; i++) {
            if (i < context->game.char_index) {
                // 既に入力済みの文字は緑色で表示する
                term_set_fg(&TERMINAL, GREEN);
            } else if (i == context->game.char_index) {
                // 現在入力すべき文字は赤色でハイライトする
                term_set_fg(&TERMINAL, BLACK);
                term_set_bg(&TERMINAL, RED);
            }

            struct CharVector flagment = string_at(current_word.pointer, i);
            term_print(&TERMINAL, "%s", flagment.pointer);
            char_vector_free(&flagment);

            term_set_fg(&TERMINAL, WHITE);
            term_set_bg(&TERMINAL, BLACK);
        }
    }

    term_print(&TERMINAL, "\n");

    // 逆順にリストを表示する。なお一番最後の要素は現在入力すべき単語であるから表示しない。
    for (int i = context->game.words.length - 2; i >= 0; i--) {
        term_print(&TERMINAL, "%s\n", context->game.words.pointer[i].pointer);
    }

    term_set_cursor_pos(&TERMINAL, 0, 1);
    char_vector_free(&current_char);
}

void game_on_key_type(struct Context *context, char input) {
    assert(context->game.words.length > 0, "game ended");
    const struct Word current_word
        = context->game.words.pointer[context->game.words.length - 1];
    struct CharVector current_char
        = string_at(current_word.pointer, context->game.char_index);
    const struct StringSlice *romas = get_roma(current_char.pointer);
    const size_t game_roma_index = context->game.roma_index;

    if (context->result.started_on == 0) {
        context->result.started_on = clock();
    }

    bool is_correct = false;

    for (size_t i = 0; i < romas->length; i++) {
        const char *roma = romas->pointer[i];
        const size_t roma_len = string_len(roma);

        if (roma_len > game_roma_index && roma[game_roma_index] == input) {
            // 正しい入力がされた
            context->result.total_types += 1;
            is_correct = true;

            if (roma_len - 1 == game_roma_index) {
                // 1ひらがな文字の最後

                context->game.roma_index = 0;
                context->game.char_index += 1;

                if (current_word.char_count == context->game.char_index) {
                    // 1単語の最後
                    // 単語リストの一番最後の要素を削除する
                    word_vector_pop(&context->game.words);
                    context->game.char_index = 0;
                }

                break;
            }

            context->game.roma_index += 1;
            break;
        }
    }

    if (!is_correct) {
        // 正しい入力されていない場合はミスカウントを増やす
        context->result.miss_types += 1;
    }
}

void result_render(struct Context *context) {
    const struct GameResult result = context->result;
    // ゲームが動作した時間を計算する。単位がクロックで格納されているので、CLOCKS_PER_SECで割って秒に変換する　
    const double gameDurationSec = (double)(result.ended_on - result.started_on)
                                   / (double)CLOCKS_PER_SEC;
    const double keys_per_sec = result.total_types / gameDurationSec;

    term_print(&TERMINAL, "終了: esc もう一度プレイ: space\n\n");

    term_print(&TERMINAL, "ゲーム結果\n");
    term_print(&TERMINAL, "かかった時間(秒): %02.02f\n", gameDurationSec);
    term_print(&TERMINAL, "総キータイプ数: %u\n", result.total_types);
    term_print(&TERMINAL, "ミスタイプ数: %u\n", result.miss_types);
    term_print(&TERMINAL, "秒間キータイプ数: %02.02f\n", keys_per_sec);
}

void result_on_key_type(struct Context *context, char input) {
    if (input == 32) {
        // spaceキーが押されたときに、ゲームを初期化してゲーム画面に戻す
        context->result = (struct GameResult){0};
        context->game.char_index = 0;
        context->game.roma_index = 0;
        context->game.words = random_words(20);
        context->gaming = true;
    }
}

#include <stdarg.h>
#include <stdio.h>

void failure_hook(const char *filename, int line, const char *msg, ...) {
    term_reset(&TERMINAL);

    fprintf(stderr, "at %s line %d: ", filename, line);

    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);

    fprintf(stderr, "\n");
    exit(1);
}

#define INITIAL_CAPACITY 2

// 新しい CharVector を作成する関数
struct CharVector char_vector_new(void) {
    const struct CharVector result = {
        .pointer = malloc(sizeof(char) * INITIAL_CAPACITY),
        .length = 0,
        .capacity = INITIAL_CAPACITY,
    };

    assert(result.pointer != NULL, "allocation failed");

    return result;
}

// self の index 番目の要素を取得する関数
char char_vector_get(struct CharVector *self, size_t index) {
    assert(
        index < self->length,
        "attempted to get out of CharVector bounds(length: %d, requested: %d)",
        self->length, index);

    return self->pointer[index];
}

// self の最後に value を追加する関数
void char_vector_push(struct CharVector *self, char value) {
    assert(self != NULL, "passed NULL to char_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    if (self->capacity == self->length) {
        // もうあまりの領域が無ければ、さらに容量を倍にして再確保する
        const size_t new_size_bytes = self->capacity * 2 * sizeof(char);

        self->pointer = realloc(self->pointer, new_size_bytes);
        self->capacity *= 2;
    }

    // 最後に追加して長さを1増やす
    self->pointer[self->length] = value;
    self->length += 1;
}

// self の最後に str を追加する関数
void char_vector_pushstr(struct CharVector *self, const char *str) {
    assert(self != NULL, "passed NULL to char_vector_push(self)");
    assert(str != NULL, "passed NULL to char_vector_pushstr(str)");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    // ヌル文字に当たるまでpushし続ける
    size_t index = 0;
    while (str[index] != '\0') {
        char_vector_push(self, str[index]);
        index += 1;
    }
}

// self の最後の要素を消去する関数
char char_vector_pop(struct CharVector *self) {
    assert(self != NULL, "passed NULL to char_vector_push(self)");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    char result = self->pointer[self->length - 1];

    self->pointer[self->length - 1] = 0;
    self->length -= 1;

    return result;
}

// self を開放する関数
void char_vector_free(struct CharVector *self) {
    assert(self != NULL, "passed NULL to char_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    // メモリ領域を解放する
    free(self->pointer);
    self->pointer = NULL;
}

// self に新たにsize領域確保する関数
// size is number of additional elements, not bytes.
void char_vector_reserve(struct CharVector *self, size_t size) {
    if (self->capacity - self->length < size) {
        const size_t new_capacity = self->capacity + size;
        const size_t new_size = new_capacity * sizeof(char);

        self->pointer = realloc(self->pointer, new_size);
        self->capacity = new_capacity;
    }
}

#define __GET_ROMA_IMPL(HIRAGANA, ROMAS_LEN, ...)                              \
    if (string_eq(hiragana, HIRAGANA)) {                                       \
        const static char *romas[] = {__VA_ARGS__};                            \
        const static struct StringSlice data                                   \
            = {.length = ROMAS_LEN, .pointer = romas};                         \
        return &data;                                                          \
    }

const struct StringSlice *get_roma(const char *hiragana) {
    assert(hiragana != NULL, "passed NULL to get_roma(hiragana)");

    // 入力されたひらがなに対して対応するローマ字の一覧を返却する。
    // 同じようなコードが並ぶため、マクロを使って省略する。
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
    __GET_ROMA_IMPL("ー", 1, "-")

    failure("couldn't find such character in get_roma: %s", hiragana);
}

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// text の文字数を返す関数
uint32_t string_len(const char *text) {
    assert(text != NULL, "passed NULL to string_len(text)");

    int32_t count = 0;
    size_t index = 0;

    // C言語の文字列は必ずヌル文字で終わる
    while (text[index] != '\0') {
        // mblen関数は指定された文字が何バイトで表されるのかを返す関数
        int result = mblen(&text[index], MB_CUR_MAX);

        if (result < 0) {
            failure("mblen returned error: %d", result);
        }

        index += result;
        count += 1;
    }

    return count;
}

// text のバイト数を返す関数
size_t string_bytes(const char *text) {
    assert(text != NULL, "passed NULL to string_bytes(text)");
    size_t result = 0;

    while (true) {
        // C言語の文字列は必ずヌル文字で終わる
        if (text[result] == '\0') {
            return result;
        }
        result += 1;
    }
}

// src の target_pos 文字目を返す関数。マルチバイト文字（日本語など）対応。
// requires to be freed!
struct CharVector string_at(const char *src, size_t target_pos) {
    assert(src != NULL, "passed NULL to string_at(src)");

    size_t index = 0;
    size_t pos = 0;

    // C言語の文字列は必ずヌル文字で終わる
    while (src[index] != '\0') {
        // mblen関数は指定された文字が何バイトで表されるのかを返す関数
        int len = mblen(&src[index], MB_CUR_MAX);

        if (len < 0) {
            failure("mblen returned error: %d", len);
        }

        if (pos == target_pos) {
            // output に1文字分のバイトを追加し、
            // 最後に文字列の終端を表すためにヌル文字を追加して返却する。
            struct CharVector output = char_vector_new();

            for (size_t i = 0; i < (size_t)len; i++) {
                char_vector_push(&output, src[index + i]);
            }

            char_vector_push(&output, '\0');
            return output;
        }

        index += len;
        pos += 1;
    }

    // ここに到達したということは、指定された場所には文字がなかったということなので、
    // エラーメッセージを表示しfailureマクロによってプログラムを即終了する。
    failure("there is no char on specific position. text: \"%s\", pos: \"%d\"",
            src, target_pos);
}

// a と b が等しいかを判定する関数
bool string_eq(const char *a, const char *b) {
    assert(a != NULL, "passed NULL to string_at(a)");
    assert(b != NULL, "passed NULL to string_at(b)");

    for (size_t index = 0;; index++) {
        // a と b が1要素でも等しくなかったら、
        // その時点で2つは違う文字列なのでfalseを返す。
        if (a[index] != b[index]) {
            return false;
        }

        // aとbがヌル文字の時、両方の文字列は同じ場所で終わったということなので、2つは等しい。
        // trueを返却する。
        if (a[index] == '\0') {
            return true;
        }
    }
}

// format関数のva_list版
struct CharVector vformat(const char *format, va_list args) {
    // 一度vsnprintfをNULLに対して呼び出し、結果の文字列を格納する
    // のに必要なバイト数を求める。
    // + 1 for \0
    const size_t need_bytes = vsnprintf(NULL, 0, format, args) + 1;

    struct CharVector result = char_vector_new();
    char_vector_reserve(&result, need_bytes);

    // 実際に書き込む。
    // we need to overwrite length manually because vsnprintf writes to memory
    // directly.
    result.length = vsnprintf(result.pointer, need_bytes, format, args);

    return result;
}

// printf関数が標準出力に対して結果を書き込むのに対し、
// この関数はメモリ上に書き込み、そのメモリ領域を返す関数。
struct CharVector format(const char *format, ...) {
    va_list args;
    va_start(args, format);

    const struct CharVector result = vformat(format, args);
    va_end(args);

    return result;
}

#include <conio.h>
#include <stdarg.h>
#include <stdio.h>

// Terminal 構造体を取得する関数
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

    const WORD text_attributes
        = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    SetConsoleTextAttribute(game_buffer, text_attributes);

    return (struct Terminal){
        .console_handle = console_handle,
        .stdin_handle = stdin_handle,
        .game_buffer = game_buffer,
        .origin_buffer_info = buffer_info,
        .origin_cursor_info = cursor_info,
        .current_text_attributes = text_attributes,
    };
}

// ターミナルを空白にする関数
void term_clear(struct Terminal *terminal) {
    DWORD size = terminal->origin_buffer_info.dwSize.X
                 * terminal->origin_buffer_info.dwSize.Y;
    COORD pos = {0, 0};
    DWORD written;
    FillConsoleOutputCharacter(terminal->game_buffer, ' ', size, pos, &written);
    FillConsoleOutputAttribute(terminal->game_buffer,
                               terminal->current_text_attributes, size, pos,
                               &written);
    SetConsoleCursorPosition(terminal->game_buffer, pos);
}

// カーソルの可視状態を設定する関数
void term_set_cursor_visible(struct Terminal *terminal, bool visible) {
    assert(terminal != NULL,
           "passed NULL to term_set_cursor_visible(terminal)");

    CONSOLE_CURSOR_INFO info = terminal->origin_cursor_info;

    info.bVisible = visible;

    SetConsoleCursorInfo(terminal->game_buffer, &info);
}

// ターミナルの前景色を設定する関数
void term_set_fg(struct Terminal *terminal, enum TerminalColor color) {
    assert(terminal != NULL, "passed NULL to term_set_fg(terminal)");

    WORD win_color = terminal->current_text_attributes & 0b11111000;

    switch (color) {
    case RED:
        win_color |= FOREGROUND_RED;
        break;
    case GREEN:
        win_color |= FOREGROUND_GREEN;
        break;
    case WHITE:
        win_color |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case BLACK:
        break;
    default:
        unreachable();
    }

    terminal->current_text_attributes = win_color;

    SetConsoleTextAttribute(terminal->game_buffer, win_color);
}

// ターミナルの背景色を設定する関数
void term_set_bg(struct Terminal *terminal, enum TerminalColor color) {
    assert(terminal != NULL, "passed NULL to term_set_bg(terminal)");

    WORD win_color = terminal->current_text_attributes & 0b10001111;

    switch (color) {
    case RED:
        win_color |= BACKGROUND_RED;
        break;
    case GREEN:
        win_color |= BACKGROUND_GREEN;
        break;
    case WHITE:
        win_color |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
        break;
    case BLACK:
        break;
    default:
        unreachable();
    }

    terminal->current_text_attributes = win_color;

    SetConsoleTextAttribute(terminal->game_buffer, win_color);
}

// カーソルの場所を設定する関数
void term_set_cursor_pos(struct Terminal *terminal, size_t x, size_t y) {
    assert(terminal != NULL, "passed NULL to term_set_cursor_pos(terminal)");

    COORD pos = {x, y};
    SetConsoleCursorPosition(terminal->game_buffer, pos);
}

// Terminal 構造体を破壊して元の状態に戻す関数
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

// ターミナルから1文字入力を取得する関数。term_poll_eventに置き換えられたため使われていない。
char term_get_char() {
    return _getch();
}

// ターミナルに文字を表示する関数。printfに非常に似ている。
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

// ターミナルから1イベント取得する関数。
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
        if (!event.Event.KeyEvent.bKeyDown) {
            return term_poll_event(terminal);
        }

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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// ランダムな単語を返す関数
const struct Word random_word() {
    // 単語一覧
    // ../scripts/wordsGen.py
    const struct Word WORDS[] = {
        {.char_count = 3, .pointer = "おかゆ"},
        {.char_count = 3, .pointer = "つうち"},
        {.char_count = 2, .pointer = "つめ"},
        {.char_count = 3, .pointer = "まうす"},
        {.char_count = 3, .pointer = "かいろ"},
        {.char_count = 4, .pointer = "のみもの"},
        {.char_count = 3, .pointer = "どうが"},
        {.char_count = 4, .pointer = "ねるまえ"},
        {.char_count = 4, .pointer = "せんさー"},
        {.char_count = 2, .pointer = "ねこ"},
        {.char_count = 3, .pointer = "わいん"},
        {.char_count = 3, .pointer = "こるく"},
        {.char_count = 5, .pointer = "はむすたー"},
        {.char_count = 2, .pointer = "かぎ"},
        {.char_count = 5, .pointer = "こんそーる"},
        {.char_count = 3, .pointer = "げーむ"},
        {.char_count = 3, .pointer = "うさぎ"},
        {.char_count = 4, .pointer = "ろーまじ"},
        {.char_count = 6, .pointer = "たいむらいん"},
        {.char_count = 3, .pointer = "せーぶ"},
        {.char_count = 4, .pointer = "かんすう"},
        {.char_count = 4, .pointer = "こうせん"},
    };

    // 単語リストの要素数
    const size_t WORDS_LEN = sizeof(WORDS) / sizeof(WORDS[0]);

    // ランダムな場所を選び返却する
    const size_t index = rand() % WORDS_LEN;
    const struct Word word = WORDS[index];

    return word;
}

// CharVectorとほぼ同じなので、詳しい説明は省略します。

#define INITIAL_CAPACITY 2

// 新しい WordVector を作成する関数
struct WordVector word_vector_new(void) {
    const struct WordVector result = {
        .pointer = malloc(sizeof(struct Word) * INITIAL_CAPACITY),
        .length = 0,
        .capacity = INITIAL_CAPACITY,
    };

    assert(result.pointer != NULL, "allocation failed");

    return result;
}

// self の index 番目の要素を取得する関数
struct Word word_vector_get(struct WordVector *self, size_t index) {
    assert(
        index < self->length,
        "attempted to get out of WordVector bounds(length: %d, requested: %d)",
        self->length, index);

    return self->pointer[index];
}

// self の最後に value を追加する関数
void word_vector_push(struct WordVector *self, struct Word value) {
    assert(self != NULL, "passed NULL to word_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid WordVector");

    if (self->capacity == self->length) {
        const size_t new_size_bytes = self->capacity * 2 * sizeof(struct Word);

        self->pointer = realloc(self->pointer, new_size_bytes);
        self->capacity *= 2;
    }

    self->pointer[self->length] = value;
    self->length += 1;
}

// self の最後の要素を消去する関数
struct Word word_vector_pop(struct WordVector *self) {
    assert(self != NULL, "passed NULL to word_vector_push(self)");
    assert(self->pointer != NULL, "passed freed or invalid WordVector");

    struct Word result = self->pointer[self->length - 1];

    self->length -= 1;

    return result;
}

// self を開放する関数
void word_vector_free(struct WordVector *self) {
    assert(self != NULL, "passed NULL to word_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid WordVector");

    free(self->pointer);
    self->pointer = NULL;
}

// self に新たにsize領域確保する関数
const struct WordVector random_words(size_t count) {
    struct WordVector result = word_vector_new();

    for (size_t i = 0; i < count; i++) {
        word_vector_push(&result, random_word());
    }

    return result;
}
