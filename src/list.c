#include "list.h"

#include <stdlib.h>
#include <stdio.h>

vec2_list vec2_list_init(int capacity) {
    return (vec2_list) {
        .elements = malloc(capacity * sizeof(vec2*)),
        .capacity = capacity,
        .length = 0,
    };
}

void vec2_list_free(vec2_list* list) {
    free(list->elements);
}

void vec2_list_push(vec2_list* list, vec2 value) {
    if(list->length == list->capacity) {
        list->capacity *= 2;
        list->elements = realloc(list->elements, list->capacity * sizeof(vec2*));
    }

    list->elements[list->length] = value;
    list->length++;
}

vec2 vec2_list_get_at(vec2_list* list, int index) {
    if(index >= list->length) {
        printf("vec2_list index of %i out of bounds!\n", index);
        return VEC2_NULL;
    }

    return list->elements[index];
}

void vec2_list_set_at(vec2_list* list, vec2 value, int index) {
    if(index >= list->length) {
        printf("vec2_list index of %i out of bounds!\n", index);
        return;
    }

    list->elements[index] = value;
}

vec2 vec2_list_pop(vec2_list* list) {
    vec2 value = vec2_list_get_at(list, 0);

    list->length--;
    for(int i = 0; i < list->length; i++) {
        list->elements[i] = list->elements[i + 1];
    }

    return value;
}
