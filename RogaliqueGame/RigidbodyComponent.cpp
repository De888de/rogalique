#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "GameWorld.h"

namespace rogalique {

RigidbodyComponent::RigidbodyComponent(RogaliqueGameObject* owner)
    : Component(owner) {
    SetMass(1.0f);
}

RigidbodyComponent::~RigidbodyComponent() {}

void RigidbodyComponent::AddForce(const sf::Vector2f& force) {
    m_acceleration += force * m_invMass;
}

void RigidbodyComponent::AddImpulse(const sf::Vector2f& impulse) {
    m_velocity += impulse * m_invMass;
}

void RigidbodyComponent::Update(float deltaTime) {
    if (!m_owner) return;
    
    // Гравитация
    if (m_useGravity) {
        m_velocity.y += 500.0f * m_gravityScale * deltaTime;
    }
    
    // Применяем ускорение
    m_velocity += m_acceleration * deltaTime;
    
    // Демпфирование
    m_velocity *= m_linearDamping;
    
    // Обновляем позицию
    auto transform = m_owner->GetComponent<TransformComponent>();
    if (transform) {
        sf::Vector2f pos = transform->GetPosition();
        pos += m_velocity * deltaTime;
        transform->SetPosition(pos);
    }
    
    // Сбрасываем ускорение
    m_acceleration = {0, 0};
}

} // namespace rogalique
