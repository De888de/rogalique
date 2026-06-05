#include "BoxCollider.h"
#include "CircleCollider.h"
#include "../../Core/TransformComponent.h"
#include <cmath>

namespace engine {

void BoxCollider::GetAABB(sf::Vector2f& min, sf::Vector2f& max) const {
    sf::Vector2f center = GetCenter();
    min = sf::Vector2f(center.x - halfSize.x, center.y - halfSize.y);
    max = sf::Vector2f(center.x + halfSize.x, center.y + halfSize.y);
}

bool BoxCollider::CheckCollision(Collider* other, sf::Vector2f& normal, float& penetration) {
    switch (other->type) {
        case ColliderType::Box: {
            auto* box = static_cast<BoxCollider*>(other);
            sf::Vector2f min1, max1, min2, max2;
            GetAABB(min1, max1);
            box->GetAABB(min2, max2);
            
            sf::Vector2f delta = GetCenter() - box->GetCenter();
            sf::Vector2f overlap = sf::Vector2f(
                (halfSize.x + box->halfSize.x) - std::abs(delta.x),
                (halfSize.y + box->halfSize.y) - std::abs(delta.y)
            );
            
            if (overlap.x > 0 && overlap.y > 0) {
                if (overlap.x < overlap.y) {
                    normal = (delta.x > 0) ? sf::Vector2f(1, 0) : sf::Vector2f(-1, 0);
                    penetration = overlap.x;
                } else {
                    normal = (delta.y > 0) ? sf::Vector2f(0, 1) : sf::Vector2f(0, -1);
                    penetration = overlap.y;
                }
                return true;
            }
            return false;
        }
        
        case ColliderType::Circle: {
            auto* circle = static_cast<CircleCollider*>(other);
            return circle->CheckCollision(this, normal, penetration);
        }
        
        default:
            return false;
    }
}

} // namespace engine
