#include "assert.h"
#include "char_vector.h"
#include "roma.h"
#include "string.h"
#include "terminal.h"
#include "word.h"
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
