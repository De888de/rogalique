#pragma once
#include "RogaliqueGameObject.h"
#include <SFML/Graphics/CircleShape.hpp>

// Forward declarations
namespace rogalique {
    class TransformComponent;
    class CollisionComponent;
    class Player;
}

namespace rogalique {

    class IsometricPhysicsItem : public RogaliqueGameObject {
    public:
        IsometricPhysicsItem();

        void Spawn(float worldX, float worldZ);
        void SpawnAtWorld(float worldX, float worldZ);
        void SpawnInFrontOfPlayer(float distance = 60.0f);

        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;
        void ApplyImpulse(float velX, float velZ, float velY);

        float GetRadius() const { return m_radius; }
        sf::Vector2f GetWorldPosition() const { return m_worldPos; }

    private:
        sf::Vector2f m_worldPos{ 0.f, 0.f };
        float m_height = 0.0f;
        sf::Vector2f m_velGround{ 0.f, 0.f };
        float m_velY = 0.0f;

        float m_gravity = 1500.0f;
        float m_bounciness = 0.2f;
        float m_damping = 0.94f;
        float m_radius = 14.0f;
    float m_lifeTime = 3.0f;

        sf::CircleShape m_shape;
        sf::CircleShape m_shadow;
        bool m_active = false;

        TransformComponent* m_transform = nullptr;
        CollisionComponent* m_collision = nullptr;

        sf::Vector2f WorldToScreen(const sf::Vector2f& world, float height) const;
        sf::Vector2f ScreenToWorld(float screenX, float screenY) const;
        void HandleCollisions();
    };

} // namespace rogalique