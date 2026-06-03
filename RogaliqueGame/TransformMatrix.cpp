#include "TransformMatrix.h"
#include <cmath>

namespace rogalique
{
    TransformMatrix::TransformMatrix()
    {
        // Единичная матрица
        m = { {
            {{1, 0, 0}},
            {{0, 1, 0}},
            {{0, 0, 1}}
        } };
    }

    TransformMatrix::TransformMatrix(const std::array<std::array<float, 3>, 3>& values)
        : m(values) {}

    TransformMatrix& TransformMatrix::operator=(const TransformMatrix& other)
    {
        if (this != &other)
        {
            m = other.m;
        }
        return *this;
    }

    void TransformMatrix::Transform(float& x, float& y) const
    {
        float newX = m[0][0] * x + m[0][1] * y + m[0][2];
        float newY = m[1][0] * x + m[1][1] * y + m[1][2];
        x = newX;
        y = newY;
    }

    void TransformMatrix::SetTranslation(float tx, float ty)
    {
        m[0][2] = tx;
        m[1][2] = ty;
    }

    void TransformMatrix::SetRotation(float angleDeg)
    {
        float angleRad = angleDeg * 3.14159f / 180.0f;
        float cosA = std::cos(angleRad);
        float sinA = std::sin(angleRad);
        m[0][0] = cosA; m[0][1] = -sinA;
        m[1][0] = sinA; m[1][1] = cosA;
    }

    void TransformMatrix::SetScale(float sx, float sy)
    {
        m[0][0] = sx;
        m[1][1] = sy;
    }

    void TransformMatrix::Print() const
    {
        for (int row = 0; row < 3; row++)
        {
            std::cout << "| ";
            for (int col = 0; col < 3; col++)
            {
                std::cout << m[row][col] << " | ";
            }
            std::cout << "\n";
        }
    }
}