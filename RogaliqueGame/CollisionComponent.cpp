#include "CollisionComponent.h"
#include "TransformComponent.h"     // должен лежать рядом
#include <cmath>
#include <algorithm>

namespace rogalique {

    CollisionComponent::CollisionComponent(RogaliqueGameObject* owner, float radius)
        : Component(owner), m_radius(radius), m_isTrigger(false)
    {}

    void CollisionComponent::Update(float deltaTime)
    {
        (void)deltaTime;
    }

    void CollisionComponent::Render(sf::RenderWindow& window)
    {
#ifdef _DEBUG
        auto* transform = m_owner->GetComponent<TransformComponent>();
        if (transform)
        {
            sf::CircleShape circle(m_radius);
            circle.setOrigin(m_radius, m_radius);
            circle.setPosition(transform->GetPosition());   // используем SFML-версию
            circle.setFillColor(sf::Color::Transparent);
            circle.setOutlineColor(sf::Color::Green);
            circle.setOutlineThickness(2.0f);
            window.draw(circle);
        }
#endif
    }

    bool CollisionComponent::CheckCollision(CollisionComponent* other)
    {
        if (!other) return false;

        auto* ownerTransform = m_owner->GetComponent<TransformComponent>();
        auto* otherTransform = other->m_owner->GetComponent<TransformComponent>();

        if (!ownerTransform || !otherTransform) return false;

        sf::Vector2f ownerPos = ownerTransform->GetPosition();
        sf::Vector2f otherPos = otherTransform->GetPosition();

        float dx = ownerPos.x - otherPos.x;
        float dy = ownerPos.y - otherPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        return distance < (m_radius + other->m_radius);
    }

    sf::FloatRect CollisionComponent::GetBounds() const
    {
        auto* transform = m_owner->GetComponent<TransformComponent>();
        if (!transform) return sf::FloatRect(0, 0, 0, 0);

        sf::Vector2f pos = transform->GetPosition();
        return sf::FloatRect(pos.x - m_radius, pos.y - m_radius, m_radius * 2, m_radius * 2);
    }

    void CollisionComponent::OnCollisionEnter(CollisionComponent* other)
    {
        for (auto* obj : m_collidingObjects)
        {
            if (obj == other) return;
        }
        m_collidingObjects.push_back(other);
    }

    void CollisionComponent::OnCollisionExit(CollisionComponent* other)
    {
        auto it = std::find(m_collidingObjects.begin(), m_collidingObjects.end(), other);
        if (it != m_collidingObjects.end())
        {
            m_collidingObjects.erase(it);
        }
    }

} // namespace rogalique