#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>

struct Vector2 {
    float x, y;

    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
    Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
    Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
    Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }
    
    float Length() const { return std::sqrt(x * x + y * y); }
    void Normalize() {
        float len = Length();
        if (len > 0) { x /= len; y /= len; }
    }
    
    // Static constants
    static const Vector2 Zero;
    static const Vector2 One;
    static const Vector2 Up;
    static const Vector2 Down;
    static const Vector2 Left;
    static const Vector2 Right;
};

#endif // VECTOR2_H
