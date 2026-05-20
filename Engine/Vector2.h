#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>

struct Vector2 {
    float x, y;

    // Constructors
    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}

    // Бинарные операторы (сложение, вычитание, умножение, деление)
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }
    Vector2 operator/(float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }

    // Операторы присваивания с операцией
    Vector2& operator+=(const Vector2& other) {
        x += other.x; y += other.y; return *this;
    }
    Vector2& operator-=(const Vector2& other) {
        x -= other.x; y -= other.y; return *this;
    }
    Vector2& operator*=(float scalar) {
        x *= scalar; y *= scalar; return *this;
    }
    Vector2& operator/=(float scalar) {
        x /= scalar; y /= scalar; return *this;
    }

    // Операторы сравнения (равенство и неравенство)
    bool operator==(const Vector2& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Vector2& other) const {
        return !(*this == other);
    }

    // Длина вектора
    float Length() const {
        return std::sqrt(x * x + y * y);
    }
    float LengthSquared() const {
        return x * x + y * y;
    }

    // Нормализация
    void Normalize() {
        float len = Length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
    }

    Vector2 Normalized() const {
        float len = Length();
        if (len > 0) return Vector2(x / len, y / len);
        return Vector2(0, 0);
    }

    // Скалярное произведение с другим вектором
    float Dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    // Векторное произведение (2D: возвращает скаляр - z-компоненту)
    float Cross(const Vector2& other) const {
        return x * other.y - y * other.x;
    }

    // Расстояние до другого вектора
    float Distance(const Vector2& other) const {
        return (*this - other).Length();
    }
    float DistanceSquared(const Vector2& other) const {
        return (*this - other).LengthSquared();
    }

    // Линейная интерполяция
    Vector2 Lerp(const Vector2& target, float t) const {
        t = std::fmax(0, std::fmin(1, t));
        return Vector2(x + (target.x - x) * t, y + (target.y - y) * t);
    }

    // Угол вектора в радианах
    float Angle() const {
        return std::atan2(y, x);
    }

    // Поворот вектора на угол (радианы)
    Vector2 Rotate(float angle) const {
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);
        return Vector2(x * cosA - y * sinA, x * sinA + y * cosA);
    }

    // Статические константы
    static const Vector2 Zero;
    static const Vector2 One;
    static const Vector2 Up;
    static const Vector2 Down;
    static const Vector2 Left;
    static const Vector2 Right;
};

// Дружественные операторы (скаляр * вектор)
inline Vector2 operator*(float scalar, const Vector2& vec) {
    return Vector2(vec.x * scalar, vec.y * scalar);
}

#endif // VECTOR2_H