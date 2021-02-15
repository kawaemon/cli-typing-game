#pragma once
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
