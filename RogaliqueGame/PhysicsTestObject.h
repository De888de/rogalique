#pragma once
#include "RogaliqueGameObject.h"
#include <SFML/Graphics/CircleShape.hpp>

namespace rogalique {

class PhysicsTestObject : public RogaliqueGameObject {
public:
    PhysicsTestObject();
    void Spawn(float x, float y);
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    
    void ApplyForce(float x, float y);
    void ApplyImpulse(float x, float y);
    
private:
    class RigidbodyComponent* m_rigidbody = nullptr;
    class TransformComponent* m_transform = nullptr;
    class SpriteComponent* m_sprite = nullptr;
    sf::CircleShape m_shape;  // fallback
    bool m_useShape = true;
};

} // namespace rogalique
