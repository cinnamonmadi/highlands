#pragma once

#include "global.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct vec2 {
    int x;
    int y;
} vec2;

extern const vec2 VEC2_NULL;
extern const vec2 VEC2_ZERO;

vec2 vec2_sum(vec2 a, vec2 b);
vec2 vec2_sub(vec2 a, vec2 b);
vec2 vec2_scale(vec2 v, float s);
vec2 vec2_normalized(vec2 v);
vec2 vec2_direction(vec2 a, vec2 b);
float vec2_length(vec2 v);
float vec2_distance(vec2 a, vec2 b);
vec2 vec2_clamp(vec2 value, vec2 min, vec2 max);
bool vec2_equals(vec2 a, vec2 b);

int clamp(int value, int lower, int upper);
bool is_rect_collision(SDL_Rect a, SDL_Rect b);
bool is_rect_in_screen(SDL_Rect a);
bool is_point_in_rect(vec2 v, SDL_Rect r);
