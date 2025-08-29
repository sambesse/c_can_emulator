/* function to create a vector */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

int vec_init(vec_t *vec, uint32_t element_size, uint32_t init_cap, uint32_t max_increment) {
    vec->data = malloc(element_size * init_cap);
    if (!vec->data) {
        return -1;
    }
    vec->elem_size = element_size;
    vec->capacity = init_cap;
    vec->size = 0;
    vec->max_inc = max_increment;
    return 0;
}

int append(vec_t *vec, void *data) {
    if (vec->size >= vec->capacity) {
        uint32_t new_size = (vec->size*2) > (vec->size + vec->max_inc) ? (vec->size + vec->max_inc) : (vec->size*2);
        vec->data = realloc(vec->data, new_size * vec->elem_size);
        if (!vec->data)
            return -1;
    }
    memcpy(vec->data + vec->elem_size * (vec->size-1), data, vec->elem_size);
    vec->size++;
    return 0;
}

int pop(vec_t *vec, void *elem) {
    if (vec->size == 0)
        return -1;
    memcpy(vec->data + (vec->size-1) * vec->elem_size, elem, vec->elem_size);
    vec->size--;
    return 0;
}

int get(vec_t *vec, uint32_t index, void *element) {
    if (index >= vec->size) {
        return -1;
    }
    memcpy(element, vec->data + index * vec->elem_size, vec->elem_size);
    return 0;
}

int set(vec_t *vec, uint32_t index, void *element) {
    if (index >= vec->size) {
        return -1;
    }
    memcpy(vec->data + index * vec->elem_size, element, vec->elem_size);
    return 0;
}
