#include <stdlib.h>

// ���b�Z�[�W��\�����ăv���O�����𑦏I������@
#define failure(msg, ...) failure_hook(__FILE__, __LINE__, msg, __VA_ARGS__);

// �v���O�����̂����ɂ͐�΂ɓ��B���Ȃ����Ƃ𖾎�����
#define unreachable() failure("expected to be unreachable code executed");

// condition �� false ���������ɁA���b�Z�[�W��\�����ăv���O�����𑦏I������
#define assert(condition, msg, ...)                                            \
    if (!(condition)) {                                                        \
        failure(msg, __VA_ARGS__);                                             \
    }

// �I������Ƃ��ɌĂ΂��֐��̃v���g�^�C�v�錾
void failure_hook(const char *filename, int line, const char *msg, ...);

// ���k�\�� char �̔z��
struct CharVector {
    // �������̈�ւ̃|�C���^
    char *pointer;
    // ���݂̔z��̒���
    size_t length;
    // ���݊m�ۂ���Ă��郁�����̈�̗e��
    size_t capacity;
};

// �V���� CharVector ���쐬����֐�
struct CharVector char_vector_new(void);

// self �� index �Ԗڂ̗v�f���擾����֐�
char char_vector_get(struct CharVector *self, size_t index);

// self �̍Ō�� value ��ǉ�����֐�
void char_vector_push(struct CharVector *self, char value);

// self �̍Ō�� str ��ǉ�����֐�
void char_vector_pushstr(struct CharVector *self, const char *str);

// self �̍Ō�̗v�f����������֐�
char char_vector_pop(struct CharVector *self);

// self ���J������֐�
void char_vector_free(struct CharVector *self);

// self �ɐV����size�̈�m�ۂ���֐�
void char_vector_reserve(struct CharVector *self, size_t size);

#include <stdbool.h>
#include <stdint.h>

struct String {
    const char *text;
    size_t char_count;
};

// text �̕�������Ԃ��֐�
uint32_t string_len(const char *text);

// text �̃o�C�g����Ԃ��֐�
size_t string_bytes(const char *text);

// src �� target_pos �����ڂ�Ԃ��֐��B�}���`�o�C�g�����i���{��Ȃǁj�Ή��B
struct CharVector string_at(const char *src, size_t pos);

// a �� b �����������𔻒肷��֐�
bool string_eq(const char *a, const char *b);

// printf�֐����W���o�͂ɑ΂��Č��ʂ��������ނ̂ɑ΂��A
// ���̊֐��̓�������ɏ������݁A���̃������̈��Ԃ��֐��B
struct CharVector format(const char *format, ...);

// format�֐���va_list��
struct CharVector vformat(const char *format, va_list args);

struct StringSlice {
    const size_t length;
    const char **pointer;
};

const struct StringSlice *get_roma(const char *hiragana);

#include <Windows.h>
#include <stdbool.h>

// �w�i�F���̎w��Ɏg���F�̗񋓌^
enum TerminalColor { RED, GREEN, WHITE, BLACK };

// �^�[�~�i���̐���Ɏg�������܂Ƃ߂�\����
struct Terminal {
    HANDLE console_handle;
    HANDLE stdin_handle;
    HANDLE game_buffer;
    CONSOLE_SCREEN_BUFFER_INFO origin_buffer_info;
    CONSOLE_CURSOR_INFO origin_cursor_info;
    WORD current_text_attributes;
};

// �^�[�~�i���Ŕ�������C�x���g�̎�ނ̗񋓌^
enum TerminalEventType { KEYBOARD_EVENT, RESIZE_EVENT };

// �L�[�{�[�h�C�x���g
struct TerminalKeyboardEvent {
    // �����ꂽ�L�[
    char key_code;
};

// �^�[�~�i���Ŕ��������C�x���g��\���\����
struct TerminalEvent {
    // �N�����C�x���g�̎��
    enum TerminalEventType type;

    // �C�x���g���L�[�{�[�h�C�x���g�̏ꍇ�A���̒��ɏ�񂪊i�[�����
    struct TerminalKeyboardEvent keyboard_event;
};

// Terminal �\���̂��擾����֐�
struct Terminal get_term(void);

// �^�[�~�i�����󔒂ɂ���֐�
void term_clear(struct Terminal *terminal);

// �J�[�\���̉���Ԃ�ݒ肷��֐�
void term_set_cursor_visible(struct Terminal *terminal, bool visible);

// �^�[�~�i���̑O�i�F��ݒ肷��֐�
void term_set_fg(struct Terminal *terminal, enum TerminalColor color);

// �^�[�~�i���̔w�i�F��ݒ肷��֐�
void term_set_bg(struct Terminal *terminal, enum TerminalColor color);

// �J�[�\���̏ꏊ��ݒ肷��֐�
void term_set_cursor_pos(struct Terminal *terminal, size_t x, size_t y);

// Terminal �\���̂�j�󂵂Č��̏�Ԃɖ߂��֐�
void term_reset(struct Terminal *terminal);

// �^�[�~�i������1�������͂��擾����֐��Bterm_poll_event�ɒu��������ꂽ���ߎg���Ă��Ȃ��B
char term_get_char(void);

// �^�[�~�i���ɕ�����\������֐��Bprintf�ɔ��Ɏ��Ă���B
void term_print(struct Terminal *terminal, const char *format, ...);

// �^�[�~�i������1�C�x���g�擾����֐��B
struct TerminalEvent term_poll_event(struct Terminal *terminal);

#include <stdint.h>

// �P��\����
struct Word {
    // ������
    size_t char_count;
    // ����
    const char *pointer;
};

// �����_���ȒP���Ԃ��֐�
const struct Word random_word(void);

// ���k�\��Word�\���̂̔z��
struct WordVector {
    // �������̈�ւ̃|�C���^
    struct Word *pointer;
    // ���݂̔z��̒���
    size_t length;
    // ���݊m�ۂ���Ă��郁�����̈�̗e��
    size_t capacity;
};

// �V���� WordVector ���쐬����֐�
struct WordVector word_vector_new(void);

// self �� index �Ԗڂ̗v�f���擾����֐�
struct Word word_vector_get(struct WordVector *self, size_t index);

// self �̍Ō�� value ��ǉ�����֐�
void word_vector_push(struct WordVector *self, struct Word value);

// self �̍Ō�̗v�f����������֐�
struct Word word_vector_pop(struct WordVector *self);

// self ���J������֐�
void word_vector_free(struct WordVector *self);

// self �ɐV����size�̈�m�ۂ���֐�
const struct WordVector random_words(size_t count);

#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ESC_KEY 27

// �^�[�~�i���̑���Ɏg���\���̂̎���
// �v���O��������1�����쐬���Ȃ��̂ŁAfailure_hook�֐��Ŏg�����߂ɃO���[�o���ϐ��ɂ��Ă���B
struct Terminal TERMINAL;

// �Q�[���̏����i�[����\����
struct Game {
    struct WordVector words;
    size_t char_index;
    size_t roma_index;
    bool previous_correct;
};

// �Q�[���̌��ʂ̏����i�[����\����
struct GameResult {
    clock_t started_on;
    clock_t ended_on;
    int total_types;
    int miss_types;
};

// �v���O�����̏�ԑS�Ă��i�[����\����
struct Context {
    bool running;             // �v���O���������s����Ă��邩�H
    bool gaming;              // �^�C�s���O�Q�[�������H
    char last_key_code;       // �Ō�ɓ��͂��ꂽ�L�[�R�[�h
    struct Game game;         // �Q�[�����
    struct GameResult result; // �Q�[���̌��ʏ��
};

// �Q�[����ʂ�\������֐��̃v���g�^�C�v�錾
void game_render(struct Context *context);
// �Q�[����ʂ��\������Ă��鎞�ɃL�[���͂��ꂽ�Ƃ��Ɏ��s�����֐��̃v���g�^�C�v�錾
void game_on_key_type(struct Context *context, char input);

// ���U���h��ʂ�\������֐��̃v���g�^�C�v�錾
void result_render(struct Context *context);
// ���U���h��ʂ��\������Ă��鎞�ɃL�[���͂��ꂽ�Ƃ��Ɏ��s�����֐��̃v���g�^�C�v�錾
void result_on_key_type(struct Context *context, char input);

int main(void) {
    srand((unsigned)time(NULL));

    // ���P�[����ݒ肷��Bmblen�֐��̋����Ȃǂɉe������
    if (setlocale(LC_CTYPE, "") == NULL) {
        // cannot use failure macro because TERMINAL is not initialized.
        fprintf(stderr, "failed to set locale");
        return 1;
    }

    // �^�[�~�i���\���̂����������ăO���[�o���ϐ��ɑ������
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

    // �J�[�\�����\������
    term_set_cursor_visible(&TERMINAL, false);

    do {
        // �K�؂ȉ�ʂ�\������
        if (context.gaming) {
            game_render(&context);
        } else {
            result_render(&context);
        }

        // �^�[�~�i���Ŕ��������C�x���g���擾����
        const struct TerminalEvent event = term_poll_event(&TERMINAL);

        switch (event.type) {
        // �L�[�{�[�h�����삳�ꂽ�Ƃ�
        case KEYBOARD_EVENT:
            const char keycode = event.keyboard_event.key_code;
            context.last_key_code = keycode;
            if (keycode == ESC_KEY) {
                // esc �L�[�������ꂽ�Ƃ��̓Q�[�����I������
                context.running = false;
            }

            // ��ʂɉ������֐����Ăяo��
            if (context.gaming) {
                game_on_key_type(&context, keycode);
            } else {
                result_on_key_type(&context, keycode);
            }

            break;

        case RESIZE_EVENT:
            break;
        }

        // �^�[�~�i���̉�ʂ��N���A����
        term_clear(&TERMINAL);
    } while (context.running);

    // �^�[�~�i�������̏�Ԃɖ߂�
    term_reset(&TERMINAL);
    return 0;
}

// �Q�[����ʂ�\������֐�
void game_render(struct Context *context) {
    if (context->game.words.length == 0) {
        // �Q�[���I���
        context->gaming = false;
        context->result.ended_on = clock();
        result_render(context);
        return;
    }

    // ���݂̒P��́A�P�ꃊ�X�g�̈�ԍŌ�̒P��Ȃ̂ŁA������擾����
    const struct Word current_word
        = context->game.words.pointer[context->game.words.length - 1];
    struct CharVector current_char
        = string_at(current_word.pointer, context->game.char_index);

    // ���݂̕�������͂ł��郍�[�}���̃p�^�[�����擾����
    const struct StringSlice *romas = get_roma(current_char.pointer);

    term_set_fg(&TERMINAL, WHITE);
    term_set_bg(&TERMINAL, BLACK);

    term_print(&TERMINAL, "ESC: �I�� debug: %d %d %d %s\n",
               context->last_key_code, context->result.total_types,
               context->result.miss_types,
               context->game.previous_correct ? "true" : "false");
    term_print(&TERMINAL, "NEXT: ");

    {
        assert(romas->length > 0, "romas was empty");

        // ���[�}���p�^�[���̈�ԍŏ��̂��̂��\�Ƃ��ĕ\������
        const char *roma = romas->pointer[0];
        for (size_t i = 0; roma[i] != '\0'; i++) {
            if (i < context->game.roma_index) {
                // ���ɓ��͍ς݂̕����͗ΐF�ŕ\������
                term_set_fg(&TERMINAL, GREEN);
            } else if (i == context->game.roma_index) {
                // ���ݓ��͂��ׂ������͐ԐF�Ńn�C���C�g����
                term_set_fg(&TERMINAL, BLACK);
                term_set_bg(&TERMINAL, RED);
            }

            term_print(&TERMINAL, "%c", roma[i]);

            // ���̐F�ɖ߂�
            term_set_fg(&TERMINAL, WHITE);
            term_set_bg(&TERMINAL, BLACK);
        }
    }

    term_print(&TERMINAL, "\n");

    {
        for (size_t i = 0; i < current_word.char_count; i++) {
            if (i < context->game.char_index) {
                // ���ɓ��͍ς݂̕����͗ΐF�ŕ\������
                term_set_fg(&TERMINAL, GREEN);
            } else if (i == context->game.char_index) {
                // ���ݓ��͂��ׂ������͐ԐF�Ńn�C���C�g����
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

    // �t���Ƀ��X�g��\������B�Ȃ���ԍŌ�̗v�f�͌��ݓ��͂��ׂ��P��ł��邩��\�����Ȃ��B
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
            // ���������͂����ꂽ
            context->result.total_types += 1;
            is_correct = true;

            if (roma_len - 1 == game_roma_index) {
                // 1�Ђ炪�ȕ����̍Ō�

                context->game.roma_index = 0;
                context->game.char_index += 1;

                if (current_word.char_count == context->game.char_index) {
                    // 1�P��̍Ō�
                    // �P�ꃊ�X�g�̈�ԍŌ�̗v�f���폜����
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
        // ���������͂���Ă��Ȃ��ꍇ�̓~�X�J�E���g�𑝂₷
        context->result.miss_types += 1;
    }
}

void result_render(struct Context *context) {
    const struct GameResult result = context->result;
    // �Q�[�������삵�����Ԃ��v�Z����B�P�ʂ��N���b�N�Ŋi�[����Ă���̂ŁACLOCKS_PER_SEC�Ŋ����ĕb�ɕϊ�����@
    const double gameDurationSec = (double)(result.ended_on - result.started_on)
                                   / (double)CLOCKS_PER_SEC;
    const double keys_per_sec = result.total_types / gameDurationSec;

    term_print(&TERMINAL, "�I��: esc ������x�v���C: space\n\n");

    term_print(&TERMINAL, "�Q�[������\n");
    term_print(&TERMINAL, "������������(�b): %02.02f\n", gameDurationSec);
    term_print(&TERMINAL, "���L�[�^�C�v��: %u\n", result.total_types);
    term_print(&TERMINAL, "�~�X�^�C�v��: %u\n", result.miss_types);
    term_print(&TERMINAL, "�b�ԃL�[�^�C�v��: %02.02f\n", keys_per_sec);
}

void result_on_key_type(struct Context *context, char input) {
    if (input == 32) {
        // space�L�[�������ꂽ�Ƃ��ɁA�Q�[�������������ăQ�[����ʂɖ߂�
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

// �V���� CharVector ���쐬����֐�
struct CharVector char_vector_new(void) {
    const struct CharVector result = {
        .pointer = malloc(sizeof(char) * INITIAL_CAPACITY),
        .length = 0,
        .capacity = INITIAL_CAPACITY,
    };

    assert(result.pointer != NULL, "allocation failed");

    return result;
}

// self �� index �Ԗڂ̗v�f���擾����֐�
char char_vector_get(struct CharVector *self, size_t index) {
    assert(
        index < self->length,
        "attempted to get out of CharVector bounds(length: %d, requested: %d)",
        self->length, index);

    return self->pointer[index];
}

// self �̍Ō�� value ��ǉ�����֐�
void char_vector_push(struct CharVector *self, char value) {
    assert(self != NULL, "passed NULL to char_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    if (self->capacity == self->length) {
        // �������܂�̗̈悪������΁A����ɗe�ʂ�{�ɂ��čĊm�ۂ���
        const size_t new_size_bytes = self->capacity * 2 * sizeof(char);

        self->pointer = realloc(self->pointer, new_size_bytes);
        self->capacity *= 2;
    }

    // �Ō�ɒǉ����Ē�����1���₷
    self->pointer[self->length] = value;
    self->length += 1;
}

// self �̍Ō�� str ��ǉ�����֐�
void char_vector_pushstr(struct CharVector *self, const char *str) {
    assert(self != NULL, "passed NULL to char_vector_push(self)");
    assert(str != NULL, "passed NULL to char_vector_pushstr(str)");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    // �k�������ɓ�����܂�push��������
    size_t index = 0;
    while (str[index] != '\0') {
        char_vector_push(self, str[index]);
        index += 1;
    }
}

// self �̍Ō�̗v�f����������֐�
char char_vector_pop(struct CharVector *self) {
    assert(self != NULL, "passed NULL to char_vector_push(self)");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    char result = self->pointer[self->length - 1];

    self->pointer[self->length - 1] = 0;
    self->length -= 1;

    return result;
}

// self ���J������֐�
void char_vector_free(struct CharVector *self) {
    assert(self != NULL, "passed NULL to char_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid CharVector");

    // �������̈���������
    free(self->pointer);
    self->pointer = NULL;
}

// self �ɐV����size�̈�m�ۂ���֐�
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

    // ���͂��ꂽ�Ђ炪�Ȃɑ΂��đΉ����郍�[�}���̈ꗗ��ԋp����B
    // �����悤�ȃR�[�h�����Ԃ��߁A�}�N�����g���ďȗ�����B
    __GET_ROMA_IMPL("��", 1, "a")
    __GET_ROMA_IMPL("��", 1, "i")
    __GET_ROMA_IMPL("��", 1, "u")
    __GET_ROMA_IMPL("��", 1, "e")
    __GET_ROMA_IMPL("��", 1, "o")
    __GET_ROMA_IMPL("��", 1, "ka")
    __GET_ROMA_IMPL("��", 1, "ki")
    __GET_ROMA_IMPL("��", 1, "ku")
    __GET_ROMA_IMPL("��", 1, "ke")
    __GET_ROMA_IMPL("��", 1, "ko")
    __GET_ROMA_IMPL("��", 1, "ga")
    __GET_ROMA_IMPL("��", 1, "gi")
    __GET_ROMA_IMPL("��", 1, "gu")
    __GET_ROMA_IMPL("��", 1, "ge")
    __GET_ROMA_IMPL("��", 1, "go")
    __GET_ROMA_IMPL("��", 1, "sa")
    __GET_ROMA_IMPL("��", 1, "si")
    __GET_ROMA_IMPL("��", 1, "su")
    __GET_ROMA_IMPL("��", 1, "se")
    __GET_ROMA_IMPL("��", 1, "so")
    __GET_ROMA_IMPL("��", 1, "za")
    __GET_ROMA_IMPL("��", 2, "zi", "ji")
    __GET_ROMA_IMPL("��", 1, "zu")
    __GET_ROMA_IMPL("��", 1, "ze")
    __GET_ROMA_IMPL("��", 1, "zo")
    __GET_ROMA_IMPL("��", 1, "ta")
    __GET_ROMA_IMPL("��", 2, "ti", "chi")
    __GET_ROMA_IMPL("��", 2, "tu", "tsu")
    __GET_ROMA_IMPL("��", 1, "te")
    __GET_ROMA_IMPL("��", 1, "to")
    __GET_ROMA_IMPL("��", 1, "da")
    __GET_ROMA_IMPL("��", 1, "di")
    __GET_ROMA_IMPL("��", 1, "du")
    __GET_ROMA_IMPL("��", 1, "de")
    __GET_ROMA_IMPL("��", 1, "do")
    __GET_ROMA_IMPL("��", 1, "na")
    __GET_ROMA_IMPL("��", 1, "ni")
    __GET_ROMA_IMPL("��", 1, "nu")
    __GET_ROMA_IMPL("��", 1, "ne")
    __GET_ROMA_IMPL("��", 1, "no")
    __GET_ROMA_IMPL("��", 1, "ha")
    __GET_ROMA_IMPL("��", 1, "hi")
    __GET_ROMA_IMPL("��", 2, "hu", "fu")
    __GET_ROMA_IMPL("��", 1, "he")
    __GET_ROMA_IMPL("��", 1, "ho")
    __GET_ROMA_IMPL("��", 1, "ba")
    __GET_ROMA_IMPL("��", 1, "bi")
    __GET_ROMA_IMPL("��", 1, "bu")
    __GET_ROMA_IMPL("��", 1, "be")
    __GET_ROMA_IMPL("��", 1, "bo")
    __GET_ROMA_IMPL("��", 1, "pa")
    __GET_ROMA_IMPL("��", 1, "pi")
    __GET_ROMA_IMPL("��", 1, "pu")
    __GET_ROMA_IMPL("��", 1, "pe")
    __GET_ROMA_IMPL("��", 1, "po")
    __GET_ROMA_IMPL("��", 1, "ma")
    __GET_ROMA_IMPL("��", 1, "mi")
    __GET_ROMA_IMPL("��", 1, "mu")
    __GET_ROMA_IMPL("��", 1, "me")
    __GET_ROMA_IMPL("��", 1, "mo")
    __GET_ROMA_IMPL("��", 1, "ya")
    __GET_ROMA_IMPL("��", 1, "yu")
    __GET_ROMA_IMPL("��", 1, "yo")
    __GET_ROMA_IMPL("��", 1, "ra")
    __GET_ROMA_IMPL("��", 1, "ri")
    __GET_ROMA_IMPL("��", 1, "ru")
    __GET_ROMA_IMPL("��", 1, "re")
    __GET_ROMA_IMPL("��", 1, "ro")
    __GET_ROMA_IMPL("��", 1, "wa")
    __GET_ROMA_IMPL("��", 1, "wo")
    __GET_ROMA_IMPL("��", 1, "nn")
    __GET_ROMA_IMPL("�[", 1, "-")

    failure("couldn't find such character in get_roma: %s", hiragana);
}

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// text �̕�������Ԃ��֐�
uint32_t string_len(const char *text) {
    assert(text != NULL, "passed NULL to string_len(text)");

    int32_t count = 0;
    size_t index = 0;

    // C����̕�����͕K���k�������ŏI���
    while (text[index] != '\0') {
        // mblen�֐��͎w�肳�ꂽ���������o�C�g�ŕ\�����̂���Ԃ��֐�
        int result = mblen(&text[index], MB_CUR_MAX);

        if (result < 0) {
            failure("mblen returned error: %d", result);
        }

        index += result;
        count += 1;
    }

    return count;
}

// text �̃o�C�g����Ԃ��֐�
size_t string_bytes(const char *text) {
    assert(text != NULL, "passed NULL to string_bytes(text)");
    size_t result = 0;

    while (true) {
        // C����̕�����͕K���k�������ŏI���
        if (text[result] == '\0') {
            return result;
        }
        result += 1;
    }
}

// src �� target_pos �����ڂ�Ԃ��֐��B�}���`�o�C�g�����i���{��Ȃǁj�Ή��B
// requires to be freed!
struct CharVector string_at(const char *src, size_t target_pos) {
    assert(src != NULL, "passed NULL to string_at(src)");

    size_t index = 0;
    size_t pos = 0;

    // C����̕�����͕K���k�������ŏI���
    while (src[index] != '\0') {
        // mblen�֐��͎w�肳�ꂽ���������o�C�g�ŕ\�����̂���Ԃ��֐�
        int len = mblen(&src[index], MB_CUR_MAX);

        if (len < 0) {
            failure("mblen returned error: %d", len);
        }

        if (pos == target_pos) {
            // output ��1�������̃o�C�g��ǉ����A
            // �Ō�ɕ�����̏I�[��\�����߂Ƀk��������ǉ����ĕԋp����B
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

    // �����ɓ��B�����Ƃ������Ƃ́A�w�肳�ꂽ�ꏊ�ɂ͕������Ȃ������Ƃ������ƂȂ̂ŁA
    // �G���[���b�Z�[�W��\����failure�}�N���ɂ���ăv���O�����𑦏I������B
    failure("there is no char on specific position. text: \"%s\", pos: \"%d\"",
            src, target_pos);
}

// a �� b �����������𔻒肷��֐�
bool string_eq(const char *a, const char *b) {
    assert(a != NULL, "passed NULL to string_at(a)");
    assert(b != NULL, "passed NULL to string_at(b)");

    for (size_t index = 0;; index++) {
        // a �� b ��1�v�f�ł��������Ȃ�������A
        // ���̎��_��2�͈Ⴄ������Ȃ̂�false��Ԃ��B
        if (a[index] != b[index]) {
            return false;
        }

        // a��b���k�������̎��A�����̕�����͓����ꏊ�ŏI������Ƃ������ƂȂ̂ŁA2�͓������B
        // true��ԋp����B
        if (a[index] == '\0') {
            return true;
        }
    }
}

// format�֐���va_list��
struct CharVector vformat(const char *format, va_list args) {
    // ��xvsnprintf��NULL�ɑ΂��ČĂяo���A���ʂ̕�������i�[����
    // �̂ɕK�v�ȃo�C�g�������߂�B
    // + 1 for \0
    const size_t need_bytes = vsnprintf(NULL, 0, format, args) + 1;

    struct CharVector result = char_vector_new();
    char_vector_reserve(&result, need_bytes);

    // ���ۂɏ������ށB
    // we need to overwrite length manually because vsnprintf writes to memory
    // directly.
    result.length = vsnprintf(result.pointer, need_bytes, format, args);

    return result;
}

// printf�֐����W���o�͂ɑ΂��Č��ʂ��������ނ̂ɑ΂��A
// ���̊֐��̓�������ɏ������݁A���̃������̈��Ԃ��֐��B
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

// Terminal �\���̂��擾����֐�
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

// �^�[�~�i�����󔒂ɂ���֐�
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

// �J�[�\���̉���Ԃ�ݒ肷��֐�
void term_set_cursor_visible(struct Terminal *terminal, bool visible) {
    assert(terminal != NULL,
           "passed NULL to term_set_cursor_visible(terminal)");

    CONSOLE_CURSOR_INFO info = terminal->origin_cursor_info;

    info.bVisible = visible;

    SetConsoleCursorInfo(terminal->game_buffer, &info);
}

// �^�[�~�i���̑O�i�F��ݒ肷��֐�
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

// �^�[�~�i���̔w�i�F��ݒ肷��֐�
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

// �J�[�\���̏ꏊ��ݒ肷��֐�
void term_set_cursor_pos(struct Terminal *terminal, size_t x, size_t y) {
    assert(terminal != NULL, "passed NULL to term_set_cursor_pos(terminal)");

    COORD pos = {x, y};
    SetConsoleCursorPosition(terminal->game_buffer, pos);
}

// Terminal �\���̂�j�󂵂Č��̏�Ԃɖ߂��֐�
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

// �^�[�~�i������1�������͂��擾����֐��Bterm_poll_event�ɒu��������ꂽ���ߎg���Ă��Ȃ��B
char term_get_char() {
    return _getch();
}

// �^�[�~�i���ɕ�����\������֐��Bprintf�ɔ��Ɏ��Ă���B
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

// �^�[�~�i������1�C�x���g�擾����֐��B
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

// �����_���ȒP���Ԃ��֐�
const struct Word random_word() {
    // �P��ꗗ
    // ../scripts/wordsGen.py
    const struct Word WORDS[] = {
        {.char_count = 3, .pointer = "������"},
        {.char_count = 3, .pointer = "����"},
        {.char_count = 2, .pointer = "��"},
        {.char_count = 3, .pointer = "�܂���"},
        {.char_count = 3, .pointer = "������"},
        {.char_count = 4, .pointer = "�݂̂���"},
        {.char_count = 3, .pointer = "�ǂ���"},
        {.char_count = 4, .pointer = "�˂�܂�"},
        {.char_count = 4, .pointer = "���񂳁["},
        {.char_count = 2, .pointer = "�˂�"},
        {.char_count = 3, .pointer = "�킢��"},
        {.char_count = 3, .pointer = "���邭"},
        {.char_count = 5, .pointer = "�͂ނ����["},
        {.char_count = 2, .pointer = "����"},
        {.char_count = 5, .pointer = "���񂻁[��"},
        {.char_count = 3, .pointer = "���[��"},
        {.char_count = 3, .pointer = "������"},
        {.char_count = 4, .pointer = "��[�܂�"},
        {.char_count = 6, .pointer = "�����ނ炢��"},
        {.char_count = 3, .pointer = "���[��"},
        {.char_count = 4, .pointer = "���񂷂�"},
        {.char_count = 4, .pointer = "��������"},
    };

    // �P�ꃊ�X�g�̗v�f��
    const size_t WORDS_LEN = sizeof(WORDS) / sizeof(WORDS[0]);

    // �����_���ȏꏊ��I�ѕԋp����
    const size_t index = rand() % WORDS_LEN;
    const struct Word word = WORDS[index];

    return word;
}

// CharVector�Ƃقړ����Ȃ̂ŁA�ڂ��������͏ȗ����܂��B

#define INITIAL_CAPACITY 2

// �V���� WordVector ���쐬����֐�
struct WordVector word_vector_new(void) {
    const struct WordVector result = {
        .pointer = malloc(sizeof(struct Word) * INITIAL_CAPACITY),
        .length = 0,
        .capacity = INITIAL_CAPACITY,
    };

    assert(result.pointer != NULL, "allocation failed");

    return result;
}

// self �� index �Ԗڂ̗v�f���擾����֐�
struct Word word_vector_get(struct WordVector *self, size_t index) {
    assert(
        index < self->length,
        "attempted to get out of WordVector bounds(length: %d, requested: %d)",
        self->length, index);

    return self->pointer[index];
}

// self �̍Ō�� value ��ǉ�����֐�
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

// self �̍Ō�̗v�f����������֐�
struct Word word_vector_pop(struct WordVector *self) {
    assert(self != NULL, "passed NULL to word_vector_push(self)");
    assert(self->pointer != NULL, "passed freed or invalid WordVector");

    struct Word result = self->pointer[self->length - 1];

    self->length -= 1;

    return result;
}

// self ���J������֐�
void word_vector_free(struct WordVector *self) {
    assert(self != NULL, "passed NULL to word_vector_push");
    assert(self->pointer != NULL, "passed freed or invalid WordVector");

    free(self->pointer);
    self->pointer = NULL;
}

// self �ɐV����size�̈�m�ۂ���֐�
const struct WordVector random_words(size_t count) {
    struct WordVector result = word_vector_new();

    for (size_t i = 0; i < count; i++) {
        word_vector_push(&result, random_word());
    }

    return result;
}
