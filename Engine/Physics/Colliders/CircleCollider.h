#pragma once
#include "Collider.h"

namespace engine {

    class CircleCollider : public Collider {
    public:
        float radius = 0.5f;

        CircleCollider(float r = 0.5f) : Collider(ColliderType::Circle), radius(r) {}

        bool CheckCollision(Collider* other, Vector2& normal, float& penetration) override;

        float GetMass() const { return material.density * 3.14159f * radius * radius; }
        float GetInertia() const { return 0.5f * GetMass() * radius * radius; }
    };

} // namespace engine
