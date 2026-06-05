#pragma once
#include "Collider.h"

namespace engine {

    class BoxCollider : public Collider {
    public:
        Vector2 halfSize = Vector2(0.5f, 0.5f);

        BoxCollider(float w = 1.0f, float h = 1.0f)
            : Collider(ColliderType::Box), halfSize(w * 0.5f, h * 0.5f) {}

        BoxCollider(Vector2 half)
            : Collider(ColliderType::Box), halfSize(half) {}

        bool CheckCollision(Collider* other, Vector2& normal, float& penetration) override;

        void GetAABB(Vector2& min, Vector2& max) const;

        float GetMass() const {
            return material.density * (halfSize.x * 2) * (halfSize.y * 2);
        }
        float GetInertia() const {
            float w = halfSize.x * 2;
            float h = halfSize.y * 2;
            return (1.0f / 12.0f) * GetMass() * (w * w + h * h);
        }
    };

} // namespace engine