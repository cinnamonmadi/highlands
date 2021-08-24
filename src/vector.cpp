#include "vector.hpp"

#include "global.hpp"

const vec2 VEC2_ZERO = vec2(0.0f, 0.0f);
const vec2 VEC2_NULL = vec2(-1.0f, -1.0f);

SDL_Rect to_rect(const vec2& a, const vec2& b) {
    return (SDL_Rect) {
        .x = (int)fmin(a.x, b.x),
        .y = (int)fmin(a.y, b.y),
        .w = abs((int)(a.x - b.x)),
        .h = abs((int)(a.y - b.y))
    };
}

bool is_rect_collision(const SDL_Rect& a, const SDL_Rect& b) {
    return !(a.x + a.w <= b.x ||
             b.x + b.w <= a.x ||
             a.y + a.h <= b.y ||
             b.y + b.h <= a.y);
}

bool is_rect_in_screen(const SDL_Rect& rect) {
    return !(rect.x + rect.w <= 0 ||
             SCREEN_WIDTH <= rect.x ||
             rect.y + rect.h <= 0 ||
             SCREEN_HEIGHT <= rect.y);
}

bool is_point_in_rect(const vec2& point, const SDL_Rect& rect) {
    return !(point.x < rect.x ||
             point.x > rect.x + rect.w ||
             point.y < rect.y ||
             point.y > rect.y + rect.h);
}

