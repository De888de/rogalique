#pragma once
#include <array>
#include <iostream>

namespace rogalique
{
    class TransformMatrix
    {
    public:
        TransformMatrix();
        TransformMatrix(const std::array<std::array<float, 3>, 3>& values);

        TransformMatrix operator*(const TransformMatrix& other) const;
        TransformMatrix& operator=(const TransformMatrix& other);

        void Transform(float& x, float& y) const;

        void SetTranslation(float tx, float ty);
        void SetRotation(float angleDeg);
        void SetScale(float sx, float sy);

        void Print() const;

    private:
        std::array<std::array<float, 3>, 3> m;
    };
}