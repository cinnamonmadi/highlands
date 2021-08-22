#pragma once

#include <cmath>

typedef struct vec2 {
    float x;
    float y;

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
} vec2;

extern const vec2 VEC2_NULL;
extern const vec2 VEC2_ZERO;
