#pragma once
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
