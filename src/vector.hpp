#pragma once

#include <SDL2/SDL.h>
#include <cmath>

typedef struct vec2 {
    float x;
    float y;

    vec2() {
        this->x = 0.0f;
        this->y = 0.0f;
    }
    vec2(int x, int y) {
        this->x = x;
        this->y = y;
    }
    inline vec2 operator+(const vec2& other) const {
        return vec2(x + other.x, y + other.y);
    }
    inline vec2 operator-(const vec2& other) const {
        return vec2(x - other.x, y - other.y);
    }
    inline vec2 operator*(const float& scaler) const {
        return vec2(x * scaler, y * scaler);
    }
    inline bool operator==(const vec2& other) const {
        return x == other.x && y == other.y;
    }
    inline bool operator!=(const vec2& other) const {
        return !(*this == other);
    }
    inline float length() const {
        return sqrt((x * x) + (y * y));
    }
    inline float distance_to(const vec2& other) const {
        return (other - *this).length();
    }
    inline vec2 normalized() const {
        float length = (*this).length();
        return vec2(x / length, y / length);
    }
    inline vec2 direction_to(const vec2& other) const {
        return (other - *this).normalized();
    }
    inline vec2 scaled(const float& scale) const {
        return (*this).normalized() * scale;
    }
    inline vec2 clamp(const vec2& lower, const vec2& upper) const {
        return vec2(fmin(fmax(x, lower.x), upper.x), fmin(fmax(y, lower.y), upper.y));
    }
} vec2;

extern const vec2 VEC2_NULL;
extern const vec2 VEC2_ZERO;

SDL_Rect to_rect(const vec2& a, const vec2& b);
bool is_rect_collision(const SDL_Rect& a, const SDL_Rect& b);
bool is_rect_in_screen(const SDL_Rect& rect);
bool is_point_in_rect(const vec2& point, const SDL_Rect& rect);
