#pragma once
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
