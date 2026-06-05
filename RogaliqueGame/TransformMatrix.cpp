#include "TransformMatrix.h"
#include <cmath>

namespace rogalique {

    TransformMatrix::TransformMatrix() {
        SetIdentity();
    }

    void TransformMatrix::SetIdentity() {
        m = { {
            {{1.0f, 0.0f, 0.0f}},
            {{0.0f, 1.0f, 0.0f}},
            {{0.0f, 0.0f, 1.0f}}
        } };
    }

    TransformMatrix TransformMatrix::operator*(const TransformMatrix& other) const {
        TransformMatrix result;
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                result.m[row][col] = 0;
                for (int k = 0; k < 3; k++) {
                    result.m[row][col] += m[row][k] * other.m[k][col];
                }
            }
        }
        return result;
    }

    TransformMatrix& TransformMatrix::operator=(const TransformMatrix& other) {
        if (this != &other) {
            m = other.m;
        }
        return *this;
    }

    void TransformMatrix::SetTranslation(float tx, float ty) {
        m[0][2] = tx;
        m[1][2] = ty;
    }

    void TransformMatrix::SetRotation(float angleDeg) {
        float angleRad = angleDeg * 3.14159f / 180.0f;
        float c = std::cos(angleRad);
        float s = std::sin(angleRad);

        m[0][0] = c;   m[0][1] = -s;
        m[1][0] = s;   m[1][1] = c;
    }

    void TransformMatrix::SetScale(float sx, float sy) {
        m[0][0] = sx;
        m[1][1] = sy;
    }

    void TransformMatrix::SetTransform(const engine::Vector2& position, float rotationDeg, const engine::Vector2& scale) {
        SetIdentity();

        // Порядок: Scale * Rotation * Translation
        TransformMatrix scaleMat;
        scaleMat.SetScale(scale.x, scale.y);

        TransformMatrix rotMat;
        rotMat.SetRotation(rotationDeg);

        TransformMatrix transMat;
        transMat.SetTranslation(position.x, position.y);

        *this = transMat * rotMat * scaleMat;
    }

    void TransformMatrix::Transform(float& x, float& y) const {
        float newX = m[0][0] * x + m[0][1] * y + m[0][2];
        float newY = m[1][0] * x + m[1][1] * y + m[1][2];
        x = newX;
        y = newY;
    }

    engine::Vector2 TransformMatrix::TransformPoint(const engine::Vector2& point) const {
        float x = point.x;
        float y = point.y;
        Transform(x, y);
        return engine::Vector2(x, y);
    }

    void TransformMatrix::Print() const {
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                std::cout << m[row][col] << " ";
            }
            std::cout << "\n";
        }
    }

} // namespace rogalique