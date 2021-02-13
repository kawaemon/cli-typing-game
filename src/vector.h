struct CharVector {
    char *pointer;
    size_t length;
    size_t capacity;
};

struct CharVector char_vector_new();
char char_vector_get(struct CharVector *self, size_t index);
void char_vector_push(struct CharVector *self, char value);
void char_vector_free(struct CharVector *self);
