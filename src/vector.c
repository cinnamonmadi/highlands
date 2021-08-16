#include "vector.h"

#include <math.h>

const vec2 VEC2_NULL = (vec2){ .x = -1, .y = -1 };
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

vec2 vec2_direction(vec2 a, vec2 b) {
    return vec2_normalized(vec2_sub(b, a));
}

float vec2_length(vec2 v) {
    return sqrt((v.x * v.x) + (v.y * v.y));
}

float vec2_distance(vec2 a, vec2 b){
    return vec2_length(vec2_sub(a, b));
}

vec2 vec2_clamp(vec2 value, vec2 min, vec2 max) {
    return (vec2) { .x = clamp(value.x, min.x, max.x), .y = clamp(value.y, min.y, max.y) };
}

bool vec2_equals(vec2 a, vec2 b) {
    return a.x == b.x && a.y == b.y;
}

// Rectangle functions

SDL_Rect to_rect(vec2 a, vec2 b) {
    return (SDL_Rect) {
        .x = min(a.x, b.x),
        .y = min(a.y, b.y),
        .w = abs(a.x - b.x),
        .h = abs(a.y - b.y)
    };
}

bool is_rect_collision(SDL_Rect a, SDL_Rect b) {
    return !(a.x + a.w <= b.x ||
             b.x + b.w <= a.x ||
             a.y + a.h <= b.y ||
             b.y + b.h <= a.y);
}

bool is_rect_in_screen(SDL_Rect a) {
    return !(a.x + a.w <= 0 ||
             SCREEN_WIDTH <= a.x ||
             a.y + a.h <= 0 ||
             SCREEN_HEIGHT <= a.y);
}

bool is_point_in_rect(vec2 v, SDL_Rect r) {
    return !(v.x < r.x ||
             v.x > r.x + r.w ||
             v.y < r.y ||
             v.y > r.y + r.h);
}

// Utility math

int min(int a, int b) {
    if(a <= b){
        return a;
    } else {
        return b;
    }
}

int max(int a, int b) {
    if(a >= b) {
        return a;
    } else {
        return b;
    }
}

int clamp(int value, int lower, int upper) {
    if(value < lower) {
        return lower;
    } else if(value > upper) {
        return upper;
    }
    return value;
}
