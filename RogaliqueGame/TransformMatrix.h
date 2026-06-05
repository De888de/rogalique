#pragma once
#include <array>
#include <iostream>
#include "Vector2.h"   // engine::Vector2

namespace rogalique {

    class TransformMatrix {
    public:
        TransformMatrix();

        TransformMatrix operator*(const TransformMatrix& other) const;
        TransformMatrix& operator=(const TransformMatrix& other);

        // Основные операции
        void SetIdentity();
        void SetTranslation(float tx, float ty);
        void SetRotation(float angleDeg);
        void SetScale(float sx, float sy);

        // Композитная трансформация (рекомендуется)
        void SetTransform(const engine::Vector2& position, float rotationDeg, const engine::Vector2& scale);

        void Transform(float& x, float& y) const;
        engine::Vector2 TransformPoint(const engine::Vector2& point) const;

        void Print() const;

    private:
        std::array<std::array<float, 3>, 3> m;
    };

} // namespace rogalique