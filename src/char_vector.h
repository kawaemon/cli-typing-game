#pragma once

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
