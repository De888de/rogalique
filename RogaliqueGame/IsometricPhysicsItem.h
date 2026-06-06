#pragma once
#include "RogaliqueGameObject.h"
#include <SFML/Graphics/CircleShape.hpp>

// Forward declarations
namespace rogalique {
    class TransformComponent;
    class CollisionComponent;
}

namespace rogalique {

class IsometricPhysicsItem : public RogaliqueGameObject {
public:
    IsometricPhysicsItem();
    void Spawn(float worldX, float worldZ);
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    void ApplyImpulse(float velX, float velZ, float velY);
    
    float GetRadius() const { return m_radius; }
    
private:
    sf::Vector2f m_worldPos;
    float m_height = 0.0f;
    sf::Vector2f m_velGround;
    float m_velY = 0.0f;
    
    float m_gravity = 800.0f;
    float m_bounciness = 0.6f;
    float m_damping = 0.98f;
    float m_radius = 12.0f;
    
    sf::CircleShape m_shape;
    sf::CircleShape m_shadow;
    bool m_active = true;
    
    TransformComponent* m_transform = nullptr;
    CollisionComponent* m_collision = nullptr;
    
    sf::Vector2f WorldToScreen(const sf::Vector2f& world, float height) const;
    sf::Vector2f ScreenToWorld(float screenX, float screenY) const;
    void HandleCollisions();
};

} // namespace rogalique
