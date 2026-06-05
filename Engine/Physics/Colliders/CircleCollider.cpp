#include "CircleCollider.h"
#include "BoxCollider.h"
#include "../../Core/TransformComponent.h"
#include "Vector2.h"
#include <cmath>

namespace engine {

    Vector2 Collider::GetCenter() const {
        auto* transform = GetTransform();
        return transform ? transform->GetPosition() : Vector2::Zero;  
    }

    TransformComponent* Collider::GetTransform() const {
        return gameObject ? gameObject->GetComponent<TransformComponent>() : nullptr;
    }

    bool CircleCollider::CheckCollision(Collider* other, Vector2& normal, float& penetration) {
        switch (other->type) {
        case ColliderType::Circle: {
            auto* circle = static_cast<CircleCollider*>(other);
            Vector2 delta = GetCenter() - circle->GetCenter();
            float distance = delta.Length();
            float radiusSum = radius + circle->radius;

            if (distance < radiusSum) {
                if (distance > 0.0001f) {
                    normal = delta * (1.0f / distance);
                }
                else {
                    normal = Vector2(1, 0);
                }
                penetration = radiusSum - distance;
                return true;
            }
            return false;
        }

        case ColliderType::Box: {
            auto* box = static_cast<BoxCollider*>(other);
            Vector2 center = GetCenter();
            Vector2 min, max;
            box->GetAABB(min, max);

            Vector2 closestPoint(
                std::max(min.x, std::min(center.x, max.x)),
                std::max(min.y, std::min(center.y, max.y))
            );

            Vector2 delta = center - closestPoint;
            float distance = delta.Length();

            if (distance < radius) {
                if (distance > 0.0001f) {
                    normal = delta * (1.0f / distance);
                }
                else {
                    normal = Vector2(1, 0);
                }
                penetration = radius - distance;
                return true;
            }
            return false;
        }

        default:
            return false;
        }
    }

} 