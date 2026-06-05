#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

struct Vector2  
{
    float x, y;

    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}
    Vector2(sf::Vector2f v) : x(v.x), y(v.y) {}           

    float Length() const
    {
        return std::sqrt(x * x + y * y);
    }

    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator*(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    // Добавь operator*=
    Vector2& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
};