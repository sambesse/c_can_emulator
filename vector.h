#include <stdint.h>

typedef struct vector {
    void *data;
    uint32_t elem_size;
    uint32_t size;
    uint32_t capacity;
    uint32_t max_inc;
} vec_t;


int vec_init(vec_t *vec, uint32_t element_size, uint32_t init_cap, uint32_t max_increment);
int append(vec_t *vec, void *data);
int pop(vec_t *vec, void *elem);
int get(vec_t *vec, uint32_t index, void *element);
int set(vec_t *vec, uint32_t index, void *element);
