#pragma once

#include "vector.h"

typedef struct vec2_list {
    vec2* elements;
    int capacity;
    int length;
} vec2_list;

vec2_list vec2_list_init(int capacity);
void vec2_list_free(vec2_list* list);
void vec2_list_push(vec2_list* list, vec2 value);
vec2 vec2_list_get_at(vec2_list* list, int index);
void vec2_list_set_at(vec2_list* list, vec2 value, int index);
vec2 vec2_list_pop(vec2_list* list);
