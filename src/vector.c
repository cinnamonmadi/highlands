#include "vector.h"

#include <math.h>

const vec2 VEC2_ZERO = (vec2){ .x = 0, .y = 0 };

// Vector functions

vec2 vec2_sum(vec2 a, vec2 b) {
    return (vec2) { .x = a.x + b.x, .y = a.y + b.y };
}

vec2 vec2_sub(vec2 a, vec2 b) {
    return (vec2) { .x = a.x - b.x, .y = a.y - b.y };
}

vec2 vec2_scale(vec2 v, float s) {
    return (vec2) { .x = (int)(v.x * s), .y = (int)(v.y * s) };
}

vec2 vec2_normalized(vec2 v) {
    float length = vec2_length(v);
    return (vec2) { .x = v.x / length, .y = v.y / length };
}

float vec2_length(vec2 v) {
    return sqrt((v.x * v.x) + (v.y * v.y));
}

// Utility math

bool is_rect_collision(SDL_Rect a, SDL_Rect b) {
    return !(a.x + a.w <= b.x ||
             b.x + b.w <= a.x ||
             a.y + a.h <= b.y ||
             b.y + b.h <= a.y);
}

bool is_point_in_rect(vec2 v, SDL_Rect r) {
    return !(v.x < r.x ||
             v.x > r.x + r.w ||
             v.y < r.y ||
             v.y > r.y + r.h);
}
