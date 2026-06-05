#pragma once

namespace sf {
    template<typename T>
    struct Vector2 {
        T x, y;
        Vector2() : x(0), y(0) {}
        Vector2(T x, T y) : x(x), y(y) {}
        
        Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
        Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
        Vector2 operator*(T scalar) const { return Vector2(x * scalar, y * scalar); }
        Vector2 operator/(T scalar) const { return Vector2(x / scalar, y / scalar); }
        
        Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
        Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; return *this; }
        
        float Length() const { return std::sqrt(x * x + y * y); }
    };
    
    using Vector2f = Vector2<float>;
}
