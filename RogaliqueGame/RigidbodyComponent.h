#pragma once
#include "Component.h"
#include <SFML/System/Vector2.hpp>

namespace rogalique {

class RigidbodyComponent : public Component {
public:
    RigidbodyComponent(RogaliqueGameObject* owner);
    ~RigidbodyComponent();
    
    // Управление скоростью
    void SetVelocity(const sf::Vector2f& v) { m_velocity = v; }
    void SetVelocity(float x, float y) { m_velocity = sf::Vector2f(x, y); }
    sf::Vector2f GetVelocity() const { return m_velocity; }
    
    void AddForce(const sf::Vector2f& force);
    void AddImpulse(const sf::Vector2f& impulse);
    
    // Настройки физики
    void SetMass(float m) { m_mass = m; m_invMass = (m > 0) ? 1.0f / m : 0; }
    void SetGravityScale(float s) { m_gravityScale = s; }
    void SetBounciness(float b) { m_bounciness = b; }
    void SetLinearDamping(float d) { m_linearDamping = d; }
    
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override {}  // Не рендерим
    
private:
    sf::Vector2f m_velocity = {0, 0};
    sf::Vector2f m_acceleration = {0, 0};
    float m_mass = 1.0f;
    float m_invMass = 1.0f;
    float m_gravityScale = 1.0f;
    float m_bounciness = 0.7f;
    float m_linearDamping = 0.98f;
    bool m_useGravity = true;
};

} // namespace rogalique
