#include "SeekerComponent.h"
#include "TransformComponent.h"
#include "GameWorld.h"
#include "Player.h"
#include <iostream>

namespace rogalique
{
    SeekerComponent::SeekerComponent(RogaliqueGameObject* owner, float speed)
        : Component(owner), m_speed(speed)
    {
        auto& world = GameWorld::GetInstance();
        for (auto* obj : world.GetAllGameObjects())
        {
            if (dynamic_cast<Player*>(obj))
            {
                m_target = obj;
                std::cout << "[SeekerComponent] Target acquired: Player" << std::endl;
                break;
            }
        }
    }

    void SeekerComponent::Update(float deltaTime)
    {
        if (!m_target) return;

        auto* ownerTransform = m_owner->GetComponent<TransformComponent>();
        auto* targetTransform = m_target->GetComponent<TransformComponent>();

        if (!ownerTransform || !targetTransform) return;

        sf::Vector2f ownerPos = ownerTransform->GetPosition();
        sf::Vector2f targetPos = targetTransform->GetPosition();

        sf::Vector2f direction = targetPos - ownerPos;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length > 0.01f)
            direction /= length;

        sf::Vector2f newPos = ownerPos + direction * m_speed * deltaTime;
        ownerTransform->SetPosition(newPos);
    }

    void SeekerComponent::Render(sf::RenderWindow&) {}

    void SeekerComponent::SetTarget(RogaliqueGameObject* target)
    {
        m_target = target;
    }
}
